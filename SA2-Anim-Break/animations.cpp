#include "pch.h"
#include <string>
#include <algorithm>

#define EDIT_ANIMLIST(a) for (int i = 0; i < a##AnimList_Length; ++i) \
	a##AnimList[i].AnimNum += animcount * Characters_##a;

using namespace std;

static constexpr int charactersCount = 16;
static constexpr int animcount = 300;
static constexpr int cmn_animcount = 86;

static AnimationIndex CharacterAnimations_r[animcount * charactersCount]{};

static const string charArray[]{
	"sonic",
	"terios",
	"miles",
	"egg",
	"knuck",
	"rouge",
	"TWALK",
	"EWALK",
	"amy",
	"ssonic",
	"sshadow",
	"metalsonic",
	"cwalk",
	"dwalk",
	"tical",
	"chaos0",
};

static int GetIDFromMtnName(char* name)
{
	string stringCopy = name;

	std::transform(stringCopy.begin(), stringCopy.end(), stringCopy.begin(), std::tolower);

	for (int i = 0; i < LengthOfArray(charArray); i++)
	{
		if (stringCopy.find(charArray[i]) != string::npos)
		{
			return i;
		}
	}

	return -1;
}

static AnimationIndex* LoadMTNFile_r(char* name)
{
	auto mem = LoadMTNFile(name);

	auto charID = GetIDFromMtnName(name);

	if (charID >= 0)
	{
		// Copy non common animations
		for (int i = 0; i < animcount; ++i)
		{
			CharacterAnimations_r[i + animcount * charID].Index = i + animcount * charID;
			CharacterAnimations_r[i + animcount * charID].Animation = CharacterAnimations[i].Animation;
			CharacterAnimations_r[i + animcount * charID].Count = CharacterAnimations[i].Count;
		}


		PrintDebug("[AnimExpander] Successfully applied %s", name);
	}
	else
	{
		PrintDebug("[AnimExpander] Failed to apply %s", name);
	}

	return mem;
}

static void __declspec(naked) LoadMTNFile_ASM()
{
	__asm
	{
		push eax
		call LoadMTNFile_r
		add esp, 4
		retn
	}
}

static void PatchSonicAnim()
{
	// Display
	WriteData((NJS_MOTION***)0x72074C, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x71e9f8, &CharacterAnimations_r[0].Animation);

	WriteData((NJS_MOTION***)0x720670, &CharacterAnimations_r[0].Animation); //Doesn't work 

	// Action
	WriteData((NJS_MOTION***)0x71b00f, &CharacterAnimations_r[0].Animation);

	// Grinding
	WriteData((NJS_MOTION***)0x7267b2, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x726ac6, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x726b5B, &CharacterAnimations_r[0].Animation);
}

static void PatchMilesAnim()
{
	WriteData((NJS_MOTION***)0x750A94, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x750182, &CharacterAnimations_r[0].Animation);
}

static void PatchPlayAnimation()
{
	WriteData((NJS_MOTION***)0x469340, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x46934a, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x46950f, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x469779, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x4697F7, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x469a31, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x46992B, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x469acB, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x4695b6, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x46966C, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x46971B, &CharacterAnimations_r[0].Animation);

	// PSetMotion
	WriteData((NJS_MOTION***)0x469cf1, &CharacterAnimations_r[0].Animation);

	// PInitializeInterpolateMotion2_
	WriteData((NJS_MOTION***)0x469d36, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x469d48, &CharacterAnimations_r[0].Animation);

	// PInitializeInterpolateMotion2
	WriteData((NJS_MOTION***)0x469de4, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x469df9, &CharacterAnimations_r[0].Animation);
	WriteData((uint16_t**)0x469e4C, &CharacterAnimations_r[0].Count);
	WriteData((uint16_t**)0x46a91C, &CharacterAnimations_r[0].Count);

	// PTerminateInterpolateMotion
	WriteData((uint16_t**)0x46a99E, &CharacterAnimations_r[0].Count);

	// ?????
	WriteData((NJS_MOTION***)0x45a4dB, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x45e4fA, &CharacterAnimations_r[0].Animation);
}


static void LoadMotion_Hack()
{
	WriteCall((void*)0x49AA87, LoadMTNFile_ASM); // ss
	WriteCall((void*)0x49ACE8, LoadMTNFile_ASM); // ssh
	WriteCall((void*)0x716F98, LoadMTNFile_ASM); // sonic
	WriteCall((void*)0x717215, LoadMTNFile_ASM); // amy
	WriteCall((void*)0x7174F8, LoadMTNFile_ASM); // shadow
	WriteCall((void*)0x717771, LoadMTNFile_ASM); // MS	
	WriteCall((void*)0x728301, LoadMTNFile_ASM); // Knux
	WriteCall((void*)0x728651, LoadMTNFile_ASM); // Rouge
	WriteCall((void*)0x7289FA, LoadMTNFile_ASM); // Tikal
	WriteCall((void*)0x728C99, LoadMTNFile_ASM); // Chaos
	WriteCall((void*)0x73C346, LoadMTNFile_ASM); // Eggman	
	WriteCall((void*)0x740DEE, LoadMTNFile_ASM); // Mech Eggman
	WriteCall((void*)0x74104E, LoadMTNFile_ASM); // Mech Tails	
	WriteCall((void*)0x741240, LoadMTNFile_ASM); // Chao Walker	
	WriteCall((void*)0x741420, LoadMTNFile_ASM); // Dark Chao Walker	
	WriteCall((void*)0x74D02B, LoadMTNFile_ASM); // Miles
}

void njCnkMotion_R(NJS_MOTION* mot, NJS_OBJECT* obj, float frame)
{
	int curAnim = SonicCO2PtrExtern->base.AnimInfo.Current;
	char pnum = SonicCO2PtrExtern->base.PlayerNum;

	if (SonicCO2PtrExtern->base.AnimInfo.AnimationFrame == 2)
	{
		mot = SonicCO2PtrExtern->base.AnimInfo.Motion;
	}
	else
	{
		if ((MainCharObj1[pnum]->Status & Status_Ball) != 0
			&& SonicCO2PtrExtern->base.CharID2 != Characters_MetalSonic
			&& (SonicCO2PtrExtern->gap35E[2] & 0x11) != 0)
		{
			curAnim = 30;
		}

		mot = CharacterAnimations_r[SonicCO2PtrExtern->base.AnimInfo.Animations[curAnim].AnimNum].Animation;
	}

	*(int*)0x25EFE54 = 0x25EFE60;
	njSetMotion(mot, frame);
	MotionDrawCallback = (ObjectFuncPtr)0x42E660;
	DrawObjMotion(obj);
}

static void __declspec(naked) DrawMotionAndObject_()
{
	__asm
	{
		push[esp + 08h] 
		push[esp + 08h] 
		push ecx 
		call njCnkMotion_R
		pop ecx
		add esp, 4 
		add esp, 4
		retn
	}
}


void PatchAnimations()
{
	LoadMotion_Hack();
	PatchPlayAnimation();
	PatchSonicAnim();
	PatchMilesAnim();

	EDIT_ANIMLIST(Shadow);
	EDIT_ANIMLIST(Tails);
	EDIT_ANIMLIST(Eggman);
	EDIT_ANIMLIST(Knuckles);
	EDIT_ANIMLIST(Rouge);
	EDIT_ANIMLIST(MechTails);
	EDIT_ANIMLIST(MechEggman);
	EDIT_ANIMLIST(Amy);
	EDIT_ANIMLIST(SuperSonic);
	EDIT_ANIMLIST(SuperShadow);
	EDIT_ANIMLIST(MetalSonic);
	EDIT_ANIMLIST(ChaoWalker);
	EDIT_ANIMLIST(DarkChaoWalker);
	EDIT_ANIMLIST(Tikal);
	EDIT_ANIMLIST(Chaos);

	//WriteJump((void*)0x4692A0, PlayAnimationThingASM);

	WriteCall((void*)0x720687, DrawMotionAndObject_);
}