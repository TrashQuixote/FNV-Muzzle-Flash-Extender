#include "nvse/PluginAPI.h"
#include "internal/class_vtbls.h"
#include "nvse/GameObjects.h"
#include "nvse/SafeWrite.h"
#include <filesystem>
#include <array>


#define MuzzleFlashDebug 0

IDebugLog		gLog("MuzzleFlashExtender.log");
PluginHandle	g_pluginHandle = kPluginHandle_Invalid;
NVSEMessagingInterface* g_messagingInterface{};
NVSEEventManagerInterface* g_eventInterface{};
NVSEInterface* g_nvseInterface{};


bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
{
	_MESSAGE("query");

	// fill out the info structure
	info->infoVersion = PluginInfo::kInfoVersion;
	info->name = "MuzzleFlashExtender";
	info->version = 1.0;

	// version checks
	if (nvse->nvseVersion < PACKED_NVSE_VERSION)
	{
		_ERROR("NVSE version too old (got %08X expected at least %08X)", nvse->nvseVersion, PACKED_NVSE_VERSION);
		return false;
	}

	if (!nvse->isEditor)
	{
		if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525)
		{
			_ERROR("incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525);
			return false;
		}

		if (nvse->isNogore)
		{
			_ERROR("NoGore is not supported");
			return false;
		}
	}
	else
	{
		if (nvse->editorVersion < CS_VERSION_1_4_0_518)
		{
			_ERROR("incorrect editor version (got %08X need at least %08X)", nvse->editorVersion, CS_VERSION_1_4_0_518);
			return false;
		}
	}

	// version checks pass
	// any version compatibility checks should be done here
	return true;
}


namespace HookMuzzleFlashLoaded {
	static UInt32 SavedLoadModelFunc = GetRelJumpAddr(0x009BAF94);
	static ModelLoader** g_modelLoader = (ModelLoader**)0x011C3B3C;

/* ¡ý Copied From JIP ¡ý  */
#define ADDR_ReturnTrue	0x8D0360
#define IS_ACTOR(form) ((*(UInt32**)form)[0x100 >> 2] == ADDR_ReturnTrue)
/* ¡ü Copied From JIP ¡ü */

/* ¡ý Copied From XNVSE ¡ý  */
	class MatchBySlot : public FormMatcher
	{
		UInt32 m_slotMask;
	public:
		MatchBySlot(UInt32 slot) : m_slotMask(TESBipedModelForm::MaskForSlot(slot)) {}
		bool Matches(TESForm* pForm) const {
			UInt32 formMask = 0;
			if (pForm) {
				if (pForm->IsWeapon()) {
					formMask = TESBipedModelForm::eSlot_Weapon;
				}
				else {
					TESBipedModelForm* pBip = DYNAMIC_CAST(pForm, TESForm, TESBipedModelForm);
					if (pBip) {
						formMask = pBip->partMask;
					}
				}
			}
			return (formMask & m_slotMask) != 0;
		}
	};

	static EquipData FindEquipped(TESObjectREFR* thisObj, FormMatcher& matcher) {
		ExtraContainerChanges* pContainerChanges = static_cast<ExtraContainerChanges*>(thisObj->extraDataList.GetByType(kExtraData_ContainerChanges));
		return (pContainerChanges) ? pContainerChanges->FindEquipped(matcher) : EquipData();
	}

	static UInt8 GetEquippedWeaponModFlags(TESObjectREFR* thisObj)
	{
		MatchBySlot matcher(5);
		EquipData equipD = FindEquipped(thisObj, matcher);

		if (!equipD.pForm) return UInt8(0);
		if (!equipD.pExtraData) return UInt8(0);

		ExtraWeaponModFlags* pXWeaponModFlags = (ExtraWeaponModFlags*)equipD.pExtraData->GetByType(kExtraData_WeaponModFlags);
		if (pXWeaponModFlags)return pXWeaponModFlags->flags;
		else return UInt8(0);
	}
/* ¡ü Copied From XNVSE ¡ü */

//debug
/*	
	void __forceinline PrintMuzzleFlash(MuzzleFlash* _mf)
	{
		auto* wpSrc = _mf->sourceWeap;
		auto* mfSrc = _mf->sourceRefr;
		auto* mfBPJ = _mf->baseProj;
		if (wpSrc) {
			gLog.FormattedMessage("wpSrc id is %x, editorid is %s", wpSrc->refID, wpSrc->GetEditorID());
		}
		else gLog.Message("wpSrc is none");
		if (mfSrc) {
			gLog.FormattedMessage("mfSrc id is %x, editorid is %s", mfSrc->refID, mfSrc->GetEditorID());
		}
		else gLog.Message("mfSrc is none");
		if (wpSrc) {
			gLog.FormattedMessage("mfBPJ id is %x, editorid is %s", mfBPJ->refID, mfBPJ->GetEditorID());
		}
		else gLog.Message("wpSrc is none");
	}
*/

	namespace fs = std::filesystem;

	constexpr UInt32 WpModTypeNums = 16;

	enum HaveMod : UInt8
	{
		HaveMod_1 = 1,	//1
		HaveMod_2 = 2,	//2
		HaveMod_3 = 3	//4
	};
	
	enum SuppAndSil : UInt8
	{
		ModEffect_Suppressor = 11,
		ModEffect_Silence = 16
	};

	enum ConstructReturn : UInt8
	{
		No_Extend = 0,
		Sil_Extend = 1,
		Supp_Extend = 2,
		Have_Extend = 3
	};

/* Just Check File Exist */
	// check giving effect index
	static bool __forceinline CheckFileExistsWithSuffix( std::string_view path_with_suffix )	
	{
		return fs::exists(path_with_suffix);
	}

	// check effects array and findout the first matched
	static bool __forceinline CheckFileExistsWithSuffix(const std::array<UInt8, 3>& effects, 
														const std::string& s_path,
														std::string& res,
														bool CheckForWeap)
	{
		res.assign("");
		if (CheckForWeap) {
			for (std::size_t ar_idx = 0; ar_idx < effects.size(); ar_idx++) {
				if (effects.at(ar_idx) == 0) continue;
				res = std::to_string((int)(ar_idx + 1));
				return CheckFileExistsWithSuffix(s_path + "_s" + res + ".nif");
			}
		}
		else {
			for (std::size_t ar_idx = 0; ar_idx < effects.size(); ar_idx++) {
				if (effects.at(ar_idx) == 0) continue;
				res = std::to_string((int)( effects.at(ar_idx) ));
				return CheckFileExistsWithSuffix(s_path + "_m" + res + ".nif");
			}
		}
		return false;
	}

	// check single effect like supp and sil
	static bool __forceinline CheckFileExistsWithSuffix( const std::array<UInt8, 3>& effects,
														 const UInt8& mod_effect,
														 std::string_view path_with_suffix)
	{
		std::size_t l_eft_exist = std::count_if(effects.begin(), effects.end(), [&](const UInt8& elem) { return elem == mod_effect; });
		if (l_eft_exist !=0){
			return fs::exists(path_with_suffix);
		}
		return false;
	}
/* Just Check File Exist */

/*
	Muzzle Flash Path Format For Specific Weapon 
	- WeaponEditorID_Sil : Have Sil Mod
	- WeaponEditorID_Supp : Have Supp Mod
	
	- WeaponEditorID_s0	: No Mods In Any Slots
	- WeaponEditorID_s1	: Mod In first Slots
	- WeaponEditorID_s2	: Mod In Second Slots
	- WeaponEditorID_s3	: Mod In Third Slots

	Return 'No_Extend' When Dont Have Any Path Matched 
*/
	static UInt8 __forceinline ConstructPathSuffixForWeap(std::string& s_path,
														  const std::array<UInt8, 3>& effects,
														  const UInt8& cur_mod_flag = 0) {
		
#ifdef MuzzleFlashDebug
		gLog.Message("ConstructPathSuffixForWeap");
#endif
		if (cur_mod_flag == 0) {	// Weap No Any Mod
			if (CheckFileExistsWithSuffix(s_path + "_s0.nif")) {
				s_path += "_s0.nif";
				return Have_Extend;
			}
			return No_Extend;
		}
		
		if (CheckFileExistsWithSuffix(effects, ModEffect_Silence, s_path + "_Sil.nif")) {	// Weap Have Sil
			
			s_path += "_Sil.nif";
			return Sil_Extend;
		}

		if (CheckFileExistsWithSuffix(effects, ModEffect_Suppressor, s_path + "_Supp.nif")) { // Weap Have Supp
			s_path += "_Supp.nif";
			return Supp_Extend;
		}

		// Weap Dont Have Sil And Supp.	Find first one to match
		std::string suffix{};
		if (CheckFileExistsWithSuffix(effects, s_path, suffix, /* For Weap */ true)) {
			s_path + "_s" + suffix + ".nif";
			return Have_Extend;
		}
		// Don't have any muzzle extend for this moded weapon
		return No_Extend;
	}

/*
	Muzzle Flash Path Format For Specific Proj And Current Weapon Effect Mod
	- BaseProjEditorID_m0	: No Mods In Any Slots
	- BaseProjEditorID_m11	: Weapon Have Sil Mod Which Effect ID == 11
	- BaseProjEditorID_m16	: Weapon Have Supp Which Effect ID == 16
	- BaseProjEditorID_m%id	: Weapon Have Mod Which Effect ID == %id

	Return 'No_Extend' When Dont Have Any Path Matched
*/
	static UInt8 __forceinline ConstructPathSuffixForProjAndUniversal(std::string& s_path,
														  const std::array<UInt8, 3>& effects,
														  const UInt8& cur_mod_flag = 0) {
		
#ifdef MuzzleFlashDebug
		gLog.Message("ConstructPathSuffixForProjAndUniversal");
#endif
		if (cur_mod_flag == 0) {	// Weap No Any Mod
			if (CheckFileExistsWithSuffix(s_path + "_m0.nif")) {
				s_path += "_m0.nif";
				return Have_Extend;
			}
			return No_Extend;
		}
		
		if ( CheckFileExistsWithSuffix(effects,ModEffect_Silence,s_path + "_m11.nif")) {	// Weap Have Sil
			s_path += "_m11.nif";
			return Sil_Extend;
		}

		if ( CheckFileExistsWithSuffix(effects, ModEffect_Suppressor, s_path + "_m16.nif")) { // Weap Have Supp
			s_path += "_m16.nif";
			return Supp_Extend;
		}

		// Weap Dont Have Sil And Supp.	Find first one to match
		std::string suffix{};
		if (CheckFileExistsWithSuffix(effects, s_path, suffix,/* For Weap */false)) {
			s_path += "_m" + suffix + ".nif";
			return Have_Extend;
		}
		// Don't have any muzzle extend for this moded weapon
		return No_Extend;
	}


	static UInt8 __forceinline LookupMuzzleFlash(std::string& dir,
												 std::string& new_path,
												 const UInt8& cur_mod_flag,
												 std::string_view prefix_comparsion,
												 const std::array<UInt8, 3>& effects,
												 bool LookupForWeap)
	{
		std::string l_filename{};
		std::size_t l_split_pos = std::string::npos;
		if (!fs::exists(dir)) return No_Extend;
		for (auto const& dir_entry : std::filesystem::directory_iterator{ dir }) {
			if (dir_entry.path().extension() != ".nif") continue;			// *.nif
			l_filename = dir_entry.path().stem().string();
			l_split_pos = l_filename.find_first_of('_');
			if (l_split_pos == std::string::npos) continue;		// *_m%d.nif

			auto l_prefix = l_filename.substr(0, l_split_pos);
#ifdef MuzzleFlashDebug
			gLog.FormattedMessage("Find A Nif %s,And _ is in pos %u", l_filename.c_str(), l_split_pos);
			gLog.FormattedMessage("Nif Prefix Is %s", l_prefix.c_str());
#endif // MuzzleFlashDebug
			
			if (l_prefix.compare(prefix_comparsion) != 0) continue;		// editorid_m%d.nif

#ifdef MuzzleFlashDebug
			gLog.FormattedMessage("Nif Prefix Matched");
#endif // MuzzleFlashDebug

			// Can find a nif file which prefix is proj editorid
			new_path += prefix_comparsion;
			if (LookupForWeap)return ConstructPathSuffixForWeap(new_path, effects, cur_mod_flag);
			return ConstructPathSuffixForProjAndUniversal(new_path, effects, cur_mod_flag);
		}
		return No_Extend;
	}


	static bool __forceinline LookupNewMuzzleFlashPath(MuzzleFlash* _muzzleflash,std::string& ext_muzzleflash_path ) {
		if (!_muzzleflash) return false;
		auto* _mf_src_actor = static_cast<Actor*>(_muzzleflash->sourceRefr);
		if ( !_mf_src_actor || !(IS_ACTOR(_mf_src_actor)) ) return false;
		
		auto* _mf_src_weap = _muzzleflash->sourceWeap;
		if (!_mf_src_weap) return false;

		auto curwp_mod_flag = GetEquippedWeaponModFlags(static_cast<TESObjectREFR*>(_mf_src_actor));

		UInt8 effect_idx_1 = (curwp_mod_flag) & 1 ? _mf_src_weap->GetItemModEffect(HaveMod_1) : 0;
		UInt8 effect_idx_2 = (curwp_mod_flag >> 1) & 1 ? _mf_src_weap->GetItemModEffect(HaveMod_2) : 0;
		UInt8 effect_idx_3 = (curwp_mod_flag >> 2) & 1 ? _mf_src_weap->GetItemModEffect(HaveMod_3) : 0;
		std::array<UInt8, 3> effects{ effect_idx_1, effect_idx_2, effect_idx_3 };
		std::string root_path = fs::current_path().string();	// X:\SteamLibrary\steamapps\common\Fallout New Vegas
#ifdef MuzzleFlashDebug
		gLog.FormattedMessage("cur weapon mod flag %u", curwp_mod_flag);
		gLog.FormattedMessage("cur_mod_flag_1,2,3 is %u,%u,%u", curwp_mod_flag, ((curwp_mod_flag >> 1) & 1), ((curwp_mod_flag >> 2) & 1));
		gLog.FormattedMessage("effect_idx_1,2,3 is %u,%u,%u", effect_idx_1, effect_idx_2, effect_idx_3);
#endif // MuzzleFlashDebug

/* Deal with weapon */
		std::string s_weap_path = root_path + R"(\Data\Meshes\Effects\MuzzleFlashes\MuzzleExtender\Weapon\)";
		if (LookupMuzzleFlash(s_weap_path,  s_weap_path,curwp_mod_flag,( _mf_src_weap->GetEditorID() ),effects,true) != No_Extend ) {
			ext_muzzleflash_path = s_weap_path.substr(s_weap_path.find("Effects"));	// Path Must Relative To 'Meshes'
			return true;
		}
/* Deal with weapon */

/* Deal with projectile */
		auto* _mf_base_proj = _muzzleflash->baseProj;
		if (!_mf_base_proj) return false;
		std::string s_proj_path = root_path + R"(\Data\Meshes\Effects\MuzzleFlashes\MuzzleExtender\Projectile\)";
		gLog.FormattedMessage("Projectile path %s", s_proj_path.c_str());
		
		if (LookupMuzzleFlash(s_proj_path, s_proj_path, curwp_mod_flag,( _mf_base_proj->GetEditorID() ),effects,false) != No_Extend) {
			ext_muzzleflash_path = s_proj_path.substr(s_proj_path.find("Effects"));	// Path Must Relative To 'Meshes'
			return true;
		}
/* Deal with projectile */

/* Deal with universal */
		std::string s_universal_path = root_path + R"(\Data\Meshes\Effects\MuzzleFlashes\MuzzleExtender\Universal\)";
		if (LookupMuzzleFlash(s_universal_path, s_universal_path, curwp_mod_flag, "universal", effects, false) != No_Extend) {
			ext_muzzleflash_path = s_universal_path.substr(s_universal_path.find("Effects"));	// Path Must Relative To 'Meshes'
			return true;
		}
/* Deal with universal */

#ifdef MuzzleFlashDebug
		gLog.FormattedMessage("Failed To Lookup new muzzleflash ");
#endif // MuzzleFlashDebug

		return false;
	}

	static NiNode* __fastcall CheckMuzzleFlashBeforeLoadModel_009BAF94(ModelLoader* _this, MuzzleFlash* _muzzleflash, const char* nifpath, UInt32 baseClass , bool unkArg0 , bool unkArg1, bool unkArg2 , bool unkArg3 )
	{
		std::string s_new_muzzleflash_path { nifpath };

		bool success = LookupNewMuzzleFlashPath(_muzzleflash,s_new_muzzleflash_path);

		
		//Should be => ThisStdCall<NiNode*>( SavedLoadModelFunc,_this, nifpath, 0, 1, 0, 0, 0 );
		NiNode* ret = ThisStdCall<NiNode*>(SavedLoadModelFunc, *g_modelLoader, (success? s_new_muzzleflash_path.c_str() : nifpath), baseClass, unkArg0, unkArg1, unkArg2, unkArg3);
		return ret;
	}

	static __declspec(naked) void Caller_009BAF94() {
		__asm {
			mov edx, [ebp - 0x18]			// MuzzleFlash *
			jmp CheckMuzzleFlashBeforeLoadModel_009BAF94
		}
	}

	static inline void InstallHook()
	{
		static UInt32 SavedLoadModelFunc = GetRelJumpAddr(0x009BAF94);
		static ModelLoader** g_modelLoader = (ModelLoader**)0x011C3B3C;
		WriteRelCall(0x009BAF94, UInt32(Caller_009BAF94));
	} 
}



// This is a message handler for nvse events
// With this, plugins can listen to messages such as whenever the game loads
void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	switch (msg->type)
	{
	case NVSEMessagingInterface::kMessage_DeferredInit:
		HookMuzzleFlashLoaded::InstallHook();
		
		break;
	}
}

void OnActorEquip_EraseMuzzleFlash(TESObjectREFR* ref, void* args)
{
	auto** refArgs = static_cast<TESForm**>(args);
	auto* equipper = refArgs[0];
	auto* equipped = refArgs[1];
	//|| !IS_ACTOR(equipper)
	if (!equipper || !equipped || !IS_ACTOR(equipper) ) return;
	//if (!IS_TYPE(equipped, TESObjectWEAP) && !IS_TYPE(equipped, TESAmmo) ) return;
	if (!IS_TYPE(equipped, TESObjectWEAP) ) return;
	
	auto* eq_actor = static_cast<Actor*>(equipper);
	HighProcess* hgProc = (HighProcess*)eq_actor->baseProcess;
	if (!hgProc || hgProc->processLevel) return;
	//if (!hgProc->muzzleFlash) return;	

	ThisStdCall(0x902230,hgProc);	// will check muzzle flash exist
}

bool NVSEPlugin_Load(NVSEInterface* nvse)
{
	_MESSAGE("MuzzleFlash load");
	g_pluginHandle = nvse->GetPluginHandle();

	// save the NVSE interface in case we need it later
	g_nvseInterface = nvse;

	// register to receive messages from NVSE
	
	if (!nvse->isEditor)
	{
		g_messagingInterface = static_cast<NVSEMessagingInterface*>(nvse->QueryInterface(kInterface_Messaging));
		g_messagingInterface->RegisterListener(g_pluginHandle, "NVSE", MessageHandler);
		g_eventInterface = static_cast<NVSEEventManagerInterface*>(nvse->QueryInterface(kInterface_EventManager));
		g_eventInterface->SetNativeEventHandler("OnActorEquip", OnActorEquip_EraseMuzzleFlash);
	}
	return true;
}
