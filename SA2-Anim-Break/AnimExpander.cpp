#include "pch.h"
#include <string>
#include <algorithm>

using namespace std;

static constexpr int slotmax = 4;
static constexpr int animcount = 300;
static constexpr int cmn_animcount = 86;

static AnimationIndex CharacterAnimations_r[animcount * slotmax]{};

static int animslot = 0;

static void ChangeAnimationID(int pnum, int base)
{
	if (pnum == 0)
	{
		return;
	}

	auto pwp = MainCharObj2[pnum];

	if (pwp)
	{
		for (int i = 0; i < animcount; i++)
		{
			if (pwp->AnimInfo.Animations[i].AnimNum > cmn_animcount)
			{
				pwp->AnimInfo.Animations[i].AnimNum += base;
			}
		}
	}
}

static void CopyAnimationID(int pnum, int base)
{
	for (int i = 0; i < animcount; ++i)
	{
		CharacterAnimations_r[i + base].Index = i + base;
		CharacterAnimations_r[i + base].Animation = CharacterAnimations[i].Animation;
		CharacterAnimations_r[i + base].Count = CharacterAnimations[i].Count;
	}
}

static void ResetCharacterAnim()
{
	for (int i = cmn_animcount; i < animcount; ++i)
	{
		CharacterAnimations[i].Animation = nullptr;
	}
}

static AnimationIndex* LoadCharacterMTNFile(const char* name, int slot)
{
	ResetCharacterAnim();
	auto mem = LoadMTNFile((char*)name);
	auto base = animcount * slot;
	CopyAnimationID(slot, base);
	ChangeAnimationID(slot, base);
	return mem;
}

static AnimationIndex* LoadMTNFile_r(const char* name)
{
	return LoadCharacterMTNFile(name, animslot);
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

static const void* const loc_4599C5 = (void*)0x4599C5;
static void __declspec(naked) PInitialize_asm()
{
	__asm
	{
		mov animslot, ecx // Get current player number
		sub     esp, 0Ch
		push    ebx
		push    ebp
		jmp loc_4599C5
	}
}

static void PatchSonicAnim()
{
	// Display
	WriteData((NJS_MOTION***)0x72074C, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x71e9f8, &CharacterAnimations_r[0].Animation);

	// Fucked up display
	WriteData<7>((void*)0x720666, 0x90ui8);
	WriteData((uint8_t*)0x720664, 0x04ui8);
	WriteData((NJS_MOTION***)0x720670, &CharacterAnimations_r[0].Animation);

	// Action
	WriteData((NJS_MOTION***)0x71b00f, &CharacterAnimations_r[0].Animation);

	// Grinding
	WriteData((NJS_MOTION***)0x7267b2, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x726ac6, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x726b5B, &CharacterAnimations_r[0].Animation);
}

static void PatchMilesAnim()
{
	WriteData<7>((void*)0x750A83, 0x90ui8);
	WriteData((uint8_t*)0x750A7A, 0x01ui8);
	WriteData((NJS_MOTION***)0x750A94, &CharacterAnimations_r[0].Animation);

	WriteData<7>((void*)0x750173, 0x90ui8);
	WriteData((uint8_t*)0x750171, 0x14ui8);
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
	WriteJump((void*)0x4599C0, PInitialize_asm); // get player id
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

	WriteData<1>((int*)0x727DA0, 0xc3); //remove 2P icon fix miles crash in 2P
}
