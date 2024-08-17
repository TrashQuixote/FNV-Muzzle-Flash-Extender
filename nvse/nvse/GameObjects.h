#pragma once

#include "GameForms.h"
#include "GameBSExtraData.h"
#include "GameExtraData.h"
#include "GameProcess.h"
#include "NiPoint.h"

class TESObjectCELL;
struct ScriptEventList;
class ActiveEffect;
class NiNode;
class Animation;

typedef ExtraContainerChanges::EntryData ContChangesEntry;
typedef ExtraContainerChanges::EntryDataList ContChangesEntryList;
typedef ExtraContainerChanges::ExtendDataList ContChangesExtraList;

// 008
class TESChildCell
{
public:
	TESChildCell();
	~TESChildCell();

	// no virtual destructor
	virtual TESObjectCELL *	GetPersistentCell(void);		// 000

//	void	** vtbl;	// 00
};

// 68
class TESObjectREFR : public TESForm
{
public:
	MEMBER_FN_PREFIX(TESObjectREFR);

	TESObjectREFR();
	~TESObjectREFR();

	virtual void		Unk_4E(void);	// GetStartingPosition(Position, Rotation, WorldOrCell)
	virtual void		Unk_4F(void);
	virtual void		Unk_50(void);
	virtual void		Unk_51(void);
	virtual bool		CastShadows(void);
	virtual void		Unk_53(void);
	virtual void		Unk_54(void);
	virtual void		Unk_55(void);
	virtual void		Unk_56(void);
	virtual void		Unk_57(void);
	virtual void		Unk_58(void);
	virtual void		Unk_59(void);
	virtual void		Unk_5A(void);
	virtual void		Unk_5B(void);
	virtual void		Unk_5C(void);
	virtual void		Unk_5D(void);
	virtual void		Unk_5E(void);
	virtual TESObjectREFR*		RemoveItem(TESForm* toRemove, BaseExtraList* extraList, UInt32 count, UInt32 unk3, UInt32 unk4, TESObjectREFR* destRef, 
		UInt32 unk6, UInt32 unk7, UInt32 unk8, UInt8 unk9);	// 40 unk2 quantity? Returns the reference assigned to the removed item.
	virtual void		Unk_60(void);
	virtual void		Unk_61(void);	// Linked to AddItem, (item, count, ExtraDataList), func0042 in OBSE
	virtual void		Unk_62(void);	// Linked to Unequip (and or equip maybe)
	virtual void		Unk_63(void);
	virtual void		AddItem(TESForm* item, ExtraDataList* xDataList, UInt32 Quantity);	// Needs confirmation
	virtual void		Unk_65(void);
	virtual void		Unk_66(void);
	virtual void		Unk_67(void);					// Actor: GetMagicEffectList
	virtual bool		GetIsChildSize(bool checkHeight);		// 068 Actor: GetIsChildSize
	virtual UInt32		GetActorUnk0148(void);			// result can be interchanged with baseForm, so TESForm* ?
	virtual void		SetActorUnk0148(UInt32 arg0);
	virtual void		Unk_6B(void);
	virtual void		Unk_6C(void);	// REFR: GetBSFaceGenNiNodeSkinned
	virtual void		Unk_6D(void);	// REFR: calls 006C
	virtual void		Unk_6E(void);	// MobileActor: calls 006D then NiNode::Func0040
	virtual void		Unk_6F(void);
	virtual void		Unk_70(void);
	virtual void		AnimateNiNode(void);					// same in FOSE ! identical to Func0052 in OBSE which says (inits animation-related data, and more)
	virtual void		GenerateNiNode(bool arg0);				// same in FOSE !
	virtual void		Set3D(NiNode* niNode, bool unloadArt);	// same in FOSE !
	virtual NiNode *	GetNiNode(void);						// same in FOSE !
	virtual void		Unk_75(void);
	virtual void		Unk_76(void);
	virtual void		Unk_77(void);
	virtual void		Unk_78(void);
	virtual AnimData*	GetAnimData(void);			// 0079
	virtual ValidBip01Names * GetValidBip01Names(void);	// 007A	Character only
	virtual ValidBip01Names * CallGetValidBip01Names(void);
	virtual void		SetValidBip01Names(ValidBip01Names validBip01Names);
	virtual NiPoint3*	GetPos(void) const;				// GetPos or GetDistance
	virtual void		Unk_7E(UInt32 arg0);
	virtual void		Unk_7F(void);
	virtual void		Unk_80(UInt32 arg0);
	virtual void		Unk_81(UInt32 arg0);
	virtual void		Unk_82(void);
	virtual UInt32		Unk_83(void);
	virtual void		Unk_84(UInt32 arg0);
	virtual UInt32		Unk_85(void);			// 0 or GetActor::Unk01AC
	virtual bool		IsCharacter(void);			// return false for Projectile, Actor and Creature, true for character and PlayerCharacter
	virtual bool		Unk_87(void);			// seems to always return 0
	virtual bool		Unk_88(void);			// seems to always return 0
	virtual void		Unk_89(void);
	virtual void		Unk_8A(void);			// SetParentCell (Interior only ?)
	virtual void		Unk_8B(void);			// IsDead = HasNoHealth (baseForm health <= 0 or Flags bit23 set)
	virtual bool		Unk_8C(void);
	virtual bool		Unk_8D(void);
	virtual void		Unk_8E(void);
	virtual void		Unk_8F(void);

	enum {
		kFlags_Unk00000002	= 0x00000002,
		kFlags_Deleted		= 0x00000020,		// refr removed from .esp or savegame
		kFlags_Persistent	= 0x00000400,		//shared bit with kFormFlags_QuestItem
		kFlags_Temporary	= 0x00004000,
		kFlags_Taken		= kFlags_Deleted | kFlags_Unk00000002,

		kChanged_Inventory	= 0x08000000,
	};

	struct RenderState
	{
		UInt32	unk00;
		UInt32	unk04;
		float	unk08;		// waterLevel
		float	unk0C;
		UInt32	unk10;		// flags most likely
		NiNode	* niNode;	// same in FOSE
		// possibly more, need to find alloc
	};

	struct EditorData {
		UInt32	unk00;	// 00
	};
	// 0C

#ifdef EDITOR
	EditorData	editorData;			// +04
#endif

	TESChildCell	childCell;				// 018

	UInt32			unk1C;					// 01C

	TESForm			* baseForm;				// 020
	
	float			rotX, rotY, rotZ;		// 024 - either public or accessed via simple inline accessor common to all child classes
	float			posX, posY, posZ;		// 030 - seems to be private
	float			scale;					// 03C 

	TESObjectCELL	* parentCell;			// 040
	ExtraDataList	extraDataList;			// 044
	RenderState		* renderState;			// 064	- (05C in FOSE)

	ExtraScript* GetExtraScript() const;
	ScriptEventList *	GetEventList() const;

	bool IsTaken() const { return (flags & kFlags_Taken) != 0; }
	bool IsPersistent() const { return (flags & kFlags_Persistent) != 0; }
	bool IsTemporary() { return (flags & kFlags_Temporary) ? true : false; }
	bool IsDeleted() const { return (flags & kFlags_Deleted) ? true : false; }

	bool Update3D();
	bool Update3D_v1c();	// Less worse version as used by some modders
	TESContainer* GetContainer();
	bool IsMapMarker();
	bool GetInventoryItems(InventoryItemsMap &invItems);
	ExtraDroppedItemList* GetDroppedItems();

	double GetHeadingAngle(const TESObjectREFR* to) const;
	bool __fastcall GetInSameCellOrWorld(TESObjectREFR* target) const;
	float __vectorcall GetDistance(TESObjectREFR* target) const;

	static TESObjectREFR* Create(bool bTemp = false);

	MEMBER_FN_PREFIX(TESObjectREFR);
#if RUNTIME
	DEFINE_MEMBER_FN(Activate, bool, 0x00573170, TESObjectREFR*, UInt32, UInt32, UInt32);	// Usage Activate(actionRef, 0, 0, 1); found inside Cmd_Activate_Execute as the last call (190 bytes)
	DEFINE_MEMBER_FN(Set3D, void, 0x0094EB40, NiNode*, bool);	// Usage Set3D(niNode, true); virtual func 0073
#endif
};

TESForm* GetPermanentBaseForm(TESObjectREFR* thisObj);	// For LevelledForm, find real baseForm, not temporary one.

// Taken from JIP LN NVSE.
float __vectorcall GetDistance3D(const TESObjectREFR* ref1, const TESObjectREFR* ref2);

STATIC_ASSERT(offsetof(TESObjectREFR, baseForm) == 0x020);
STATIC_ASSERT(offsetof(TESObjectREFR, extraDataList) == 0x044);
STATIC_ASSERT(sizeof(TESObjectREFR) == 0x068);

class BaseProcess;

// 088
class MobileObject : public TESObjectREFR
{
public:
	MobileObject();
	~MobileObject();

	virtual void		StartHighProcess(void);		// 090
	virtual void		Unk_91(void);
	virtual void		Unk_92(void);
	virtual void		Unk_93(void);
	virtual void		Unk_94(void);
	virtual void		Unk_95(void);
	virtual void		Unk_96(void);
	virtual void		Unk_97(void);
	virtual void		Unk_98(void);
	virtual void		Unk_99(void);
	virtual void		Unk_9A(void);
	virtual void		Unk_9B(void);
	virtual void		Unk_9C(void);
	virtual void		Unk_9D(void);
	virtual void		Unk_9E(void);
	virtual void		Unk_9F(void);
	virtual void		Unk_A0(void);	// StartConversation(targetActor, subjectLocationData, targetLocationData, headTrack, allowMovement, arg, topicID, arg, arg
	virtual void		Unk_A1(void);
	virtual void		Unk_A2(void);
	virtual void		Unk_A3(void);
	virtual void		Unk_A4(void);
	virtual void		Unk_A5(void);
	virtual void		Unk_A6(void);
	virtual void		Unk_A7(void);
	virtual void		Unk_A8(void);
	virtual void		Unk_A9(void);
	virtual void		Unk_AA(void);
	virtual void		Unk_AB(void);
	virtual void		Unk_AC(void);
	virtual void		Unk_AD(void);
	virtual void		Unk_AE(void);
	virtual float		AdjustRot(UInt32 arg1);
	virtual void		Unk_B0(void);
	virtual void		Unk_B1(void);
	virtual void		Unk_B2(void);
	virtual void		Unk_B3(void);
	virtual void		Unk_B4(void);
	virtual void		Unk_B5(void);
	virtual void		Unk_B6(void);
	virtual void		Unk_B7(void);
	virtual void		Unk_B8(void);
	virtual void		Unk_B9(void);
	virtual void		Unk_BA(void);
	virtual void		Unk_BB(void);
	virtual void		Unk_BC(void);
	virtual void		Unk_BD(void);
	virtual void		Unk_BE(void);
	virtual void		Unk_BF(void);
	virtual void		Unk_C0(void);
	
	BaseProcess	* baseProcess;	// 068
	UInt32		unk06C;			// 06C - loaded	set to the talking actor ref when initialising ExtraTalkingActor
	UInt32		unk070;			// 070 - loaded
	UInt32		unk074;			// 074 - loaded
	UInt32		unk078;			// 078 - loaded
	UInt8		unk07C;			// 07C - loaded
	UInt8		unk07D;			// 07D - loaded
	UInt8		unk07E;			// 07E - loaded
	UInt8		unk07F;			// 07F - loaded
	UInt8		unk080;			// 080 - loaded
	UInt8		unk081;			// 081 - loaded
	UInt8		unk082;			// 082 - cleared when starting combat on player
	UInt8		unk083;			// 083 - loaded
	UInt8		unk084;			// 084 - loaded
	UInt8		unk085;			// 085 - loaded
	UInt8		unk086;			// 086 - loaded
	UInt8		unk087;			// 087	Init'd to the inverse of NoLowLevelProcessing
};
STATIC_ASSERT(offsetof(MobileObject, baseProcess) == 0x068);
STATIC_ASSERT(sizeof(MobileObject) == 0x088);

// 00C
class MagicCaster
{
public:
	MagicCaster();
	~MagicCaster();

	UInt32 vtabl;
	UInt32 unk004;	// 004
	UInt32 unk008;	// 008
};

STATIC_ASSERT(sizeof(MagicCaster) == 0x00C);

typedef tList<ActiveEffect> EffectNode;
// 010
class MagicTarget
{
public:
	MagicTarget();
	~MagicTarget();

	virtual void	Destructor(void);
	virtual TESObjectREFR *	GetParent(void);
	virtual EffectNode *	GetEffectList(void);

	UInt8			byt004;		// 004 
	UInt8			byt005;		// 005 
	UInt8			byt006[2];	// 006-7
	tList<void*>	lst008;		// 008

	void RemoveEffect(EffectItem *effItem);

	void StopEffect(void *arg0, bool arg1)
	{
		ThisStdCall(0x8248E0, this, arg0, arg1);
	}
};

STATIC_ASSERT(sizeof(MagicTarget) == 0x010);

class hkaRaycastInterface
{
public:
	hkaRaycastInterface();
	~hkaRaycastInterface();
	virtual hkaRaycastInterface*	Destroy(bool doFree);
	virtual void					Unk_01(void* arg0);
	virtual void					Unk_02(void);

	// Assumed to be 0x010 bytes due to context where the vftable is used
	UInt32	unk000[(0x010 - 0x004) >> 2];	// 0000
};
STATIC_ASSERT(sizeof(hkaRaycastInterface) == 0x010);

class bhkRagdollController : public hkaRaycastInterface
{
public:
	bhkRagdollController();
	~bhkRagdollController();

	UInt32	unk000[(0x021C - 0x010) >> 2];	// 0010
	UInt8	fill021C[3];					// 021C
	bool	bool021F;						// 021F	when null assume FIK status is false
	bool	fikStatus;						// 0220
	UInt8	fill0221[3];					// 0221
};
STATIC_ASSERT(sizeof(bhkRagdollController) == 0x0224);

class bhkRagdollPenetrationUtil;
class ActorMover;
class PlayerMover;
class ImageSpaceModifierInstanceDOF;
class ImageSpaceModifierInstanceDRB;

class ActorMover	// I need to call Func008
{
public:
	virtual void		Unk_00(void);
	virtual void		Unk_01(void);
	virtual void		Unk_02(void);
	virtual void		Unk_03(void);
	virtual void		Unk_04(void);
	virtual void		Unk_05(void);
	virtual void		Unk_06(void);
	virtual void		Unk_07(void);
	virtual UInt32		Unk_08(void);	// for PlayerMover, it is GetMovementFlags
		// bit 11 = swimming 
		// bit 9 = sneaking
		// bit 8 = run
		// bit 7 = walk
		// bit 0 = keep moving (Q)
};

typedef std::vector<TESForm*> EquippedItemsList;

struct ActorValueMod
{
	UInt8		avCode;
	UInt8		pad01[3];
	float		mod;
};

class Actor : public MobileObject
{
public:
	/*304*/virtual UInt8	IsGuard();
	/*308*/virtual void		SetGuard(UInt8 setTo);
	/*30C*/virtual void		Unk_C3(void);
	/*310*/virtual void		Unk_C4(void);
	/*314*/virtual void		Unk_C5(void);
	/*318*/virtual void		Unk_C6(void);
	/*31C*/virtual void		SetIgnoreCrime(bool ignoreCrime);
	/*320*/virtual bool		GetIgnoreCrime();
	/*324*/virtual void		Resurrect(UInt8 arg1, UInt8 arg2, UInt8 arg3);
	/*328*/virtual void		Unk_CA(void);
	/*32C*/virtual void		Unk_CB(void);
	/*330*/virtual void		Unk_CC(void);
	/*334*/virtual void		Unk_CD(void);
	/*338*/virtual void		DamageHealthAndFatigue(float healthDmg, float fatigueDmg, Actor* source);
	/*33C*/virtual void		DamageActionPoints(float amount); // checks GetIsGodMode before decreasing
	/*340*/virtual void		Unk_D0(void);
	/*344*/virtual int		CalculateDisposition(Actor* target, void* arg2);
	/*348*/virtual void		UpdateMovement(float arg1, UInt32 arg2);
	/*34C*/virtual void		Unk_D3(void);
	/*350*/virtual void		Unk_D4(void);
	/*354*/virtual float	GetDefaultTurningSpeed();
	/*358*/virtual bool		IsOverencumbered();
	/*35C*/virtual void		Unk_D7(void);
	/*360*/virtual bool		IsPlayerRef();
	/*364*/virtual void		Unk_D9(void);
	/*368*/virtual void		Unk_DA(void);
	/*36C*/virtual void		Unk_DB(void);
	/*370*/virtual void		Unk_DC(void);
	/*374*/virtual void		Unk_DD(void);
	/*378*/virtual void		Unk_DE(void);
	/*37C*/virtual TESRace* GetRace();
	/*380*/virtual float	GetHandReachTimesCombatDistance();
	/*384*/virtual void		Unk_E1(void);
	/*388*/virtual void		Unk_E2(void);
	/*38C*/virtual bool		IsPushable();
	/*390*/virtual UInt32	GetActorType();	// Creature = 0, Character = 1, PlayerCharacter = 2
	/*394*/virtual void		SetActorValue(UInt32 avCode, float value);
	/*398*/virtual void		SetActorValueInt(UInt32 avCode, UInt32 value);
	/*39C*/virtual void		ModActorValue(UInt32 avCode, float modifier, Actor* attacker);
	/*3A0*/virtual void		Unk_E8(void);
	/*3A4*/virtual void		ForceActorValue(UInt32 avCode, float modifier, UInt32 arg3);
	/*3A8*/virtual void		ModActorValueInt(UInt32 avCode, int modifier, UInt32 arg3);
	/*3AC*/virtual void		DamageActorValue(UInt32 avCode, float damage, Actor* attacker);
	/*3B0*/virtual void		Unk_EC(void);
	/*3B4*/virtual void		Unk_ED(void);
	/*3B8*/virtual void		Unk_EE(void);
	/*3BC*/virtual void* GetPreferedWeapon(UInt32 unk);	// void* == ContChangesEntry
	/*3C0*/virtual void		Unk_F0(void);
	/*3C4*/virtual void		ResetArmorDRDT();
	/*3C8*/virtual bool		DamageItem(void* itemEntry, float damage, int unused); // void* == ContChangesEntry
	/*3CC*/virtual void		DropItem(TESForm* itemForm, ExtraDataList* xDataList, SInt32 count, NiVector3* pos, int arg5);
	/*3D0*/virtual void		DoActivate(TESObjectREFR* activatedRef, UInt32 count, bool arg3);
	/*3D4*/virtual void		Unk_F5(void);
	/*3D8*/virtual void		Unk_F6(void);
	/*3DC*/virtual void		Unk_F7(void);
	/*3E0*/virtual bool		AddActorEffect(SpellItem* actorEffect);
	/*3E4*/virtual bool		RemoveActorEffect(SpellItem* actorEffect);
	/*3E8*/virtual void		Reload(TESObjectWEAP* weapon, UInt32 animType, UInt8 hasExtendedClip);
	/*3EC*/virtual void		Reload2(TESObjectWEAP* weapon, UInt32 animType, UInt8 hasExtendedClip, UInt8 isInstantSwapHotkey);
	/*3F0*/virtual void		DecreaseAmmo(int amount);
	/*3F4*/virtual void		Unk_FD(void);
	/*3F8*/virtual CombatActors* GetCombatGroup();
	/*3FC*/virtual void		SetCombatGroup(CombatActors* combatActors);
	/*400*/virtual void		Unk_100(void);
	/*404*/virtual void		Unk_101(void);
	/*408*/virtual void		Unk_102(void);
	/*40C*/virtual void		Unk_103(void);
	/*410*/virtual void		Unk_104(void);
	/*414*/virtual void		Unk_105(void);
	/*418*/virtual void		InitGetUpPackage();
	/*41C*/virtual void		SetAlpha(float alpha);
	/*420*/virtual float	GetAlpha();
	/*424*/virtual void		ForceAttackActor(Actor* target, CombatActors* combatGroup, UInt8 arg3, UInt32 arg4, UInt8 arg5, UInt32 arg6, UInt32 arg7, UInt32 arg8);
	/*428*/virtual CombatController* GetCombatController();
	/*42C*/virtual Actor* GetCombatTarget();
	/*430*/virtual void		UpdateCombat();
	/*434*/virtual void		StopCombat(Actor* target);
	/*438*/virtual void		Unk_10E(void);
	/*43C*/virtual float	GetTotalArmorDR();
	/*440*/virtual float	GetTotalArmorDT();
	/*444*/virtual UInt32	Unk_111(void);
	/*448*/virtual bool		IsTrespassing();
	/*44C*/virtual void		Unk_113(void);
	/*450*/virtual void		Unk_114(void);
	/*454*/virtual void		Unk_115(void);
	/*458*/virtual float	CalculateWalkSpeed();
	/*45C*/virtual float	CalculateRunSpeed();
	/*460*/virtual void		ModDisposition(Actor* target, float value);
	/*464*/virtual float	GetDisposition(Actor* target);
	/*468*/virtual void		ClearDisposition(Actor* target);
	/*46C*/virtual void		SetStartingPosition();
	/*470*/virtual bool		GetAttacked();
	/*474*/virtual void		Unk_11D(void);
	/*478*/virtual void		Unk_11E(void);
	/*47C*/virtual void		Unk_11F(void);
	/*480*/virtual void		Unk_120(void);
	/*484*/virtual void		Unk_121(void);
	/*488*/virtual void		RewardXP(UInt32 amount);
	/*48C*/virtual void		Unk_123(void);
	/*490*/virtual void		Unk_124(void);
	/*494*/virtual void		Unk_125(void);
	/*498*/virtual void		SetPerkRank(BGSPerk* perk, UInt8 rank, bool alt);
	/*49C*/virtual void		RemovePerk(BGSPerk* perk, bool alt);
	/*4A0*/virtual UInt8	GetPerkRank(BGSPerk* perk, bool alt);
	/*4A4*/virtual void		AddPerkEntryPoint(BGSEntryPointPerkEntry* perkEntry, bool alt);
	/*4A8*/virtual void		RemovePerkEntryPoint(BGSEntryPointPerkEntry* perkEntry, bool alt);
	/*4AC*/virtual void* GetPerkEntryPointList(UInt8 entryPointID, bool alt);	// void == PerkEntryPointList
	/*4B0*/virtual void		Unk_12C(void);
	/*4B4*/virtual bool		GetIsImmobileCreature();
	/*4B8*/virtual void		DoHealthDamage(Actor* attacker, float damage);
	/*4BC*/virtual void		Unk_12F(void);
	/*4C0*/virtual void		Unk_130(void);
	/*4C4*/virtual float	Unk_131(void);
	/*4C8*/virtual void		HandleHeadTracking(float arg1);
	/*4CC*/virtual void		UpdateHeadTrackingEmotions(UInt32 arg1);
	/*4D0*/virtual void		Unk_134(void);
	/*4D4*/virtual void		Unk_135(void);
	/*4D8*/virtual NiVector3* GetAnticipatedLocation(NiVector3* resPos, float time);

	enum LifeStates
	{
		kLifeState_Alive = 0,
		kLifeState_Dying = 1,
		kLifeState_Dead = 2,
		kLifeState_Unconscious = 3,
		kLifeState_Reanimate = 4,
		kLifeState_Restrained = 5
	};

	struct Disposition
	{
		float	value;
		Actor* target;
	};

	MagicCaster			magicCaster;			// 088
	MagicTarget			magicTarget;			// 094
	ActorValueOwner		avOwner;				// 0A4
	CachedValuesOwner	cvOwner;				// 0A8

	bhkRagdollController* ragDollController;			// 0AC
	bhkRagdollPenetrationUtil* ragDollPentrationUtil;		// 0B0
	UInt32								unk0B4;						// 0B4-
	float								flt0B8;						// 0B8
	bool								bAIState;					// 0BC-
	UInt8								byte0BD;					// 0BD
	UInt8								byte0BE;					// 0BE
	UInt8								byte0BF;					// 0BF
	Actor* killer;					// 0C0
	UInt8								byte0C4;					// 0C4-
	UInt8								byte0C5;					// 0C5
	UInt8								byte0C6;					// 0C6
	UInt8								byte0C7;					// 0C7
	float								flt0C8;						// 0C8
	float								flt0CC;						// 0CC
	tList<ActorValueMod>				forceAVList;				// 0D0
	UInt8								byte0D8;					// 0D8
	UInt8								byte0D9;					// 0D9
	UInt8								byte0DA;					// 0DA
	UInt8								byte0DB;					// 0DB
	UInt32								unk0DC;						// 0DC
	tList<ActorValueMod>				setAVList;					// 0E0
	UInt8								byte0E8;					// 0E8	const 1
	UInt8								byte0E9;					// 0E9
	UInt8								byte0EA;					// 0EA
	UInt8								byte0EB;					// 0EB
	UInt32								unk0EC;						// 0EC
	UInt8								byte0F0;					// 0F0-
	UInt8								byte0F1;					// 0F1-
	UInt8								byte0F2;					// 0F2
	UInt8								byte0F3;					// 0F3
	tList<void>							list0F4;					// 0F4
	tList<Disposition>					dispositionList;			// 0FC
	bool								isInCombat;					// 104
	UInt8								jipActorFlags1;				// 105
	UInt8								jipActorFlags2;				// 106
	UInt8								jipActorFlags3;				// 107
	UInt32								lifeState;					// 108	saved as byte HasHealth
	UInt32								criticalStage;				// 10C
	UInt32								unk110;						// 110-
	float								painSoundTimer;				// 114
	UInt8								byte118;					// 118-
	UInt8								byte119;					// 119+
	UInt8								byte11A;					// 11A
	UInt8								byte11B;					// 11B
	UInt32								unk11C;						// 11C-
	UInt32								resetTime;					// 120-
	bool								forceRun;					// 124
	bool								forceSneak;					// 125
	UInt8								byte126;					// 126-
	bool								searchingForEnemies;		// 127
	Actor* combatTarget;				// 128
	BSSimpleArray<Actor*>* combatTargets;				// 12C
	BSSimpleArray<Actor*>* combatAllies;				// 130
	UInt8								byte134;					// 134-
	UInt8								byte135;					// 135+
	UInt8								byte136;					// 136
	UInt8								byte137;					// 137
	UInt32								unk138;						// 138-
	UInt32								minorCrimeCount;			// 13C
	UInt32								actorFlags;					// 140	0x80000000 - IsEssential
	bool								ignoreCrime;				// 144
	UInt8								byte145;					// 145	Has to do with package evaluation
	UInt8								byte146;					// 146	Has to do with package evaluation
	UInt8								byte147;					// 147
	UInt32								unk148;						// 148-
	UInt8								inWater;					// 14C
	UInt8								isSwimming;					// 14D
	UInt8								byte14E;					// 14E
	UInt8								byte14F;					// 14F
	UInt32								unk150;						// 150-
	float								flt154;						// 154
	float								flt158;						// 158
	UInt8								byte15C;					// 15C-
	UInt8								byte15D;					// 15D-
	UInt8								byte15E;					// 15E
	UInt8								byte15F;					// 15F
	NiVector3							startingPos;				// 160
	float								startingZRot;				// 16C
	TESForm* startingWorldOrCell;		// 170
	UInt8								byte174;					// 174-
	UInt8								byte175;					// 175-
	UInt8								byte176;					// 176
	UInt8								byte177;					// 177
	float								flt178;						// 178
	float								weaponSpread1;				// 17C
	float								weaponSpread2;				// 180
	float								weaponSpread3;				// 184
	float								flt188;						// 188
	UInt8								byte18C;					// 18C-
	bool								isTeammate;					// 18D
	UInt8								byte18E;					// 18E-
	UInt8								byte18F;					// 18F
	ActorMover* actorMover;				// 190
	UInt32								unk194;						// 194-
	UInt32								unk198;						// 198-
	float								flt19C;						// 19C
	UInt32								unk1A0;						// 1A0-
	UInt32								speechExpression;			// 1A4
	UInt32								emotionValue;				// 1A8
	UInt32								sitSleepState;				// 1AC-
	UInt8								isImmobileCreature;			// 1B0-
	bool								forceHit;					// 1B1-
	UInt8								byte1B2;					// 1B2
	UInt8								byte1B3;					// 1B3

	//__forceinline void EquipItem(TESForm* objType, UInt32 equipCount = 1, ExtraDataList* itemExtraList = NULL, bool applyEnchantment = 1, bool lockEquip = 0, bool noMessage = 1)
	//{
	//	//ThisCall(0x88C650, this, objType, equipCount, itemExtraList, applyEnchantment, lockEquip, noMessage);
	//}
	//__forceinline void UnequipItem(TESForm* objType, UInt32 unequipCount = 1, ExtraDataList* itemExtraList = NULL, bool removeEnchantment = 1, bool lockUnequip = 0, bool noMessage = 1)
	//{
	//	//ThisCall(0x88C790, this, objType, unequipCount, itemExtraList, removeEnchantment, lockUnequip, noMessage);
	//}

	bool GetDead() const { return (lifeState == 1) || (lifeState == 2); }
	bool GetRestrained() const { return lifeState == 5; }

	TESActorBase* GetActorBase() const;
	bool GetLOS(Actor* target) const;
	char GetCurrentAIPackage() const;
	char GetCurrentAIProcedure() const;
	bool IsFleeing() const;
	ContChangesEntry* GetWeaponInfo() const;	//void == ContChangesEntry
	void* GetAmmoInfo() const;		//void == ContChangesEntry
	
	
	void EquipItem(TESForm* objType, UInt32 equipCount, ExtraDataList* itemExtraList, UInt32 unk3, bool lockEquip, UInt32 unk5);

	void UnequipItem(TESForm* objType, UInt32 unk1, ExtraDataList* itemExtraList, UInt32 unk3, bool lockUnequip, UInt32 unk5);

	EquippedItemsList GetEquippedItems();
	
	ExtraContainerDataArray GetEquippedEntryDataList();
	ExtraContainerExtendDataArray GetEquippedExtendDataList();
	TESObjectWEAP* GetEquippedWeapon() const;
	void AimWeapon(bool shouldAim, bool hasQueuedIdleFlags10000);
	bool SetBlocking(bool shouldBlock);
	void SetWantsWeaponOut(bool wantsWeaponOut);
	bool IsWeaponOut();
	bool IsItemEquipped(TESForm* item) const;
	bool EquippedWeaponHasMod(UInt32 modType) const;
	UInt8 EquippedWeaponHasScope() const;
	UInt8 EquippedWeaponSilenced() const;
	bool IsSneaking() const;
	void StopCombat();
	bool __fastcall IsInCombatWith(Actor* target) const;
	int __fastcall GetDetectionValue(Actor* detected) const;
	TESPackage* GetStablePackage() const;
	PackageInfo* GetPackageInfo() const;
	TESObjectREFR* GetPackageTarget() const;
	TESCombatStyle* GetCombatStyle() const;
	SInt8 GetKnockedState() const;
	bool IsWeaponOut() const;
	void UpdateActiveEffects(MagicItem* magicItem, EffectItem* effItem, bool addNew);
	bool GetIsGhost() const;
	float GetRadiationLevel() const;
	void* AddBackUpPackage(TESObjectREFR* targetRef, TESObjectCELL* targetCell, UInt32 flags); //void == BackUpPackage
	void __fastcall TurnToFaceObject(TESObjectREFR* target);
	void __vectorcall TurnAngle(float angle);
	void PlayAnimGroup(UInt32 animGroupID);
	UInt32 GetLevel() const;
	double GetKillXP() const;
	void __fastcall GetHitDataValue(UInt32 valueType, double* result) const;
	void DismemberLimb(UInt32 bodyPartID, bool explode);
	void EquipItemAlt(void* entry, UInt32 noUnequip = 0, UInt32 noMessage = 1); // void == ContChangesEntry
	bool HasNoPath() const;
	bool CanBePushed() const;
	float __vectorcall AdjustPushForce(float baseForce);
	void PushActor(float force, float angle, TESObjectREFR* originRef);
	int GetGroundMaterial() const;
	void RefreshAnimData();
	double GetPathingDistance(TESObjectREFR* target);
};


STATIC_ASSERT(offsetof(Actor, magicCaster) == 0x088);

class Character : public Actor
{
public:
	Character();
	~Character();

	virtual void		Unk_137(void);
	virtual void		Unk_138(void);

	ValidBip01Names	* validBip01Names;	// 1B4
	float			flt1B8;				// 1B8
	float			flt1BC;				// 1BC
	UInt8			byt1C0;				// 1C0
	UInt8			byt1C1;				// 1C1
	UInt16			unk1C2;				// 1C2
	float			flt1C4;				// 1C4
};

struct CombatActors;

typedef tList<BGSQuestObjective::Target> QuestObjectiveTargets;

// 9BC
class PlayerCharacter : public Character
{
public:
	PlayerCharacter();
	~PlayerCharacter();

	// used to flag controls as disabled in disabledControlFlags
	enum {
		kControlFlag_Movement		= 1 << 0,
		kControlFlag_Look			= 1 << 1,
		kControlFlag_Pipboy			= 1 << 2,
		kControlFlag_Fight			= 1 << 3,
		kControlFlag_POVSwitch		= 1 << 4,
		kControlFlag_RolloverText	= 1 << 5,
		kControlFlag_Sneak			= 1 << 6,
	};

	virtual void		Unk_139(void);
	virtual void		Unk_13A(void);

	// lotsa data

	UInt32								unk1C8[(0x244-0x1C8) >> 2];		// 1C8	0224 is a package of type 1C, 208 could be a DialogPackage, 206 questObjectiveTargets is valid
	float								unk244[0x4D];					// 244	have to be a set of ActorValue
	float								unk378[0x4D];					// 378	have to be a set of ActorValue
	UInt32								unk4AC;							// 4AC
	float								unk4B0[0x4D];					// 4B0	have to be a set of ActorValue
	BGSNote								* note;							// 5E4
	UInt32								unk574;							// 5E8
	ImageSpaceModifierInstanceDOF		* unk5EC;						// 5EC
	ImageSpaceModifierInstanceDOF		* unk5F0;						// 5F0
	ImageSpaceModifierInstanceDRB		* unk5F4;						// 5F4
	UInt32								unk5F8;							// 5F8
	tList<Actor>						teammates;						// 5FC
	UInt32								unk604[(0x648 - 0x604) >> 2];	// 604
	UInt8								unk648;							// 648
	UInt8								unk649;							// 649
	bool								unk64A;							// 64A	= not FirstPerson
	UInt8								unk64B;							// 64B
	bool								bThirdPerson;					// 64C
	UInt8								unk64D[3];	
	UInt32								unk650[(0x680 - 0x650) >> 2];	// 650 
	UInt8								disabledControlFlags;			// 680 kControlFlag_xxx
	UInt8								unk0681[3];						// 681
	UInt32								unk684[(0x68C - 0x684) >> 2];	// 684
	ValidBip01Names						* playerVB01N;					// 68C
	ExtraAnim::Animation				* extraAnimation;				// 690 ExtraDataAnim::Data
	NiNode								* playerNode;					// 694 used as node if unk64A is true
	UInt32								unk698[(0x6A8-0x698) >> 2];		// 698
	TESTopic							* topic;						// 6A8
	UInt32								unk6AC[3];						// 6AC
	TESQuest							* quest;						// 6B8
	tList<BGSQuestObjective>			questObjectiveList;				// 6BC
	UInt32								unk6C4[39];				// 6C4
	TESRegion							*currentRegion;			// 760
	TESRegionList						regionsList;			// 764
	UInt32								unk770[18];				// 770
	UInt8								gameDifficulty;			// 7B8
	UInt8								pad7B9[3];				// 7B9
	bool								isHardcore;				// 7BC
	UInt8								pad7BD[3];				// 7BD
	UInt32								unk7C0[49];				// 7C0
	tList<BGSEntryPointPerkEntry>		perkEntries[74];		// 884
	UInt32								unkAD4[164];			// AD4
	CombatActors						*combatActors;			// D64
	UInt32								unkD68[3];				// D68
	UInt8								unkD74[96];				// D74
	UInt32								unkDD4[(0x0E50 - 0x0DD4) / 4];	// DD4

		// 7C4 is a byte used during Combat evaluation (Player is targetted ?), 
		// 7C6 is a boolean meaning toddler, 
		// 7C7 byte bool PCCanUsePowerArmor, Byt0E39 referenced during LoadGame
		// Used when entering FlyCam : 7E8/7EC/7F0 stores Pos, 7F0 adjusted by scaledHeight multiplied by 0698 , 7E0 stores RotZ, 7E4 RotX
		// Perks forms a list at 87C and AD4. Caravan Cards at 614 and 618. Quest Stage LogEntry at 6B0. Quest Objectives at 6BC.
		// Hardcore flag would be E38. Byte at DF0 seems to be PlayerIsInCombat
		// tList at 6C4 is cleared when there is no current quest. There is another NiNode at 069C
		// list of perk and perkRank are at 0x087C and 0x0AD4 (alt perks). 086C is cleared after equipement change.
		// D68 counts the Teammates.
		// D74: 96 bytes that are cleared when the 3D is cleared.

	bool IsThirdPerson() { return bThirdPerson ? true : false; }
	UInt32 GetMovementFlags() { return actorMover->Unk_08(); }	// 11: IsSwimming, 9: IsSneaking, 8: IsRunning, 7: IsWalking, 0: keep moving
	bool IsPlayerSwimming() { return (GetMovementFlags()  >> 11) & 1; }

	static PlayerCharacter*	GetSingleton();
	bool SetSkeletonPath(const char* newPath);
	bool SetSkeletonPath_v1c(const char* newPath);	// Less worse version as used by some modders
	static void UpdateHead(void);
	QuestObjectiveTargets* GetCurrentQuestObjectiveTargets();

	void UpdateCamera(bool isCalledFromFunc21, bool _zero_skipUpdateLOD);
};

extern PlayerCharacter** g_thePlayer;

STATIC_ASSERT(offsetof(PlayerCharacter, ragDollController) == 0x0AC);
STATIC_ASSERT(offsetof(PlayerCharacter, questObjectiveList) == 0x6BC);
STATIC_ASSERT(offsetof(PlayerCharacter, bThirdPerson) == 0x64C);
STATIC_ASSERT(offsetof(PlayerCharacter, actorMover) == 0x190);
STATIC_ASSERT(sizeof(PlayerCharacter) == 0xE50);
