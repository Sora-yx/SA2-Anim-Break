#include "pch.h"
#include <string>
#include <algorithm>

#define EDIT_ANIMLIST(a) for (int i = 0; i < a##AnimList_Length; ++i) \
	a##AnimList[i].AnimNum += animcount;

using namespace std;

static constexpr int charactersCount = 16;
static constexpr int animcount = 300;
static constexpr int cmn_animcount = 86;

static AnimationIndex CharacterAnimations_r[600]{};

Trampoline* LoadRELModule_t = nullptr;

int playerCount = 0;

void MoveAnimIDJ2()
{
	for (int j = 0; j < animcount; j++) {

		if (MainCharObj2[1]->AnimInfo.Animations[j].AnimNum)
		{
			MainCharObj2[1]->AnimInfo.Animations[j].AnimNum += animcount;
		}
	}
}

void ResetCharacterAnim()
{
	for (int i = cmn_animcount; i < 300; ++i)
	{
		CharacterAnimations[i].Index = 0;
		CharacterAnimations[i].Animation = nullptr;
		CharacterAnimations[i].Count = 0;
	}
}

static AnimationIndex* LoadMTNFile_r(char* name)
{
	if (playerCount)
	{
		ResetCharacterAnim();
	}

	auto mem = LoadMTNFile(name);

	if (!playerCount) {

		for (int i = 0; i < animcount; ++i)
		{
			CharacterAnimations_r[i].Index = i;
			CharacterAnimations_r[i].Animation = CharacterAnimations[i].Animation;
			CharacterAnimations_r[i].Count = CharacterAnimations[i].Count;
		}
	}
	else
	{
		for (int i = 0; i < animcount; ++i)
		{
			CharacterAnimations_r[i + animcount].Index = i + animcount;
			CharacterAnimations_r[i + animcount].Animation = CharacterAnimations[i].Animation;
			CharacterAnimations_r[i + animcount].Count = CharacterAnimations[i].Count;
		}
	}

	if (!playerCount)
	{
		playerCount++;
	}	
	else 
	{
		playerCount = 0;
		MoveAnimIDJ2();
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


void DrawSonic_R(NJS_MOTION* mot, NJS_OBJECT* obj, float frame)
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


	return DrawMotionAndObject(mot, obj, frame);
}

static void __declspec(naked) DrawSonic_Hack()
{
	__asm
	{
		push[esp + 08h]
		push[esp + 08h]
		push ecx
		call DrawSonic_R
		pop ecx
		add esp, 4
		add esp, 4
		retn
	}
}


void DrawMiles_r(NJS_MOTION* motion, NJS_OBJECT* mdl, float frame) {

	TailsCharObj2* mCO2 = MilesCO2Extern;

	if (mCO2) {
		char pnum = mCO2->base.PlayerNum;

		int curAnim = mCO2->base.AnimInfo.Current;

		if (mCO2->base.AnimInfo.AnimationFrame == 2)
		{
			motion = mCO2->base.AnimInfo.Motion;
		}
		else
		{
			motion = CharacterAnimations_r[mCO2->base.AnimInfo.Animations[curAnim].AnimNum].Animation;
		}
	}

	return DrawMotionAndObject(motion, mdl, frame);
}

static void __declspec(naked) DrawMiles_Hack()
{
	__asm
	{
		push[esp + 08h] // frame
		push[esp + 08h] // obj
		push ecx // mtn
		call DrawMiles_r
		pop ecx // mtn
		add esp, 4 // obj
		add esp, 4 // frame
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

void PatchAnimations()
{
	LoadMotion_Hack();
	PatchPlayAnimation();
	PatchSonicAnim();
	PatchMilesAnim();

	WriteCall((void*)0x720687, DrawSonic_Hack);
	WriteCall((void*)0x750ABF, DrawMiles_Hack);
	WriteCall((void*)0x750196, DrawMiles_Hack);
	//LoadRELModule_t = new Trampoline((int)0x4549C0, (int)0x4549C8, LoadRELModule_r);

	WriteData<1>((int*)0x727DA0, 0xc3); //remove 2P icon fix miles crash in 2P
}