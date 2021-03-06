#include "pch.h"
#include <string>
#include <algorithm>

using namespace std;

static constexpr int slotmax = 4;
static constexpr int animcount = 300;
static constexpr int cmn_animcount = 86;

static AnimationIndex CharacterAnimations_r[animcount * slotmax]{};

static int animslot = 0;

Trampoline* LoadRELModule_t = nullptr;

extern "C"
{
	__declspec(dllexport) AnimationIndex* GetCharacterAnim_r()
	{
		return CharacterAnimations_r;
	}

	__declspec(dllexport) void SetCharacterAnim(uint16_t Index, uint16_t Count, NJS_MOTION* Animation)
	{
		CharacterAnimations_r[Index].Animation = Animation;
		CharacterAnimations_r[Index].Count = Count;
		CharacterAnimations_r[Index].Index = Index;
	}

	__declspec(dllexport) int getCharAnimSize() {
		return LengthOfArray(CharacterAnimations_r);
	}
}


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
			pwp->AnimInfo.Animations[i].AnimNum += base;
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
	for (int i = 0; i < animcount; ++i)
	{
		CharacterAnimations[i].Animation = nullptr;
	}
}

static AnimationIndex* LoadCharacterMTNFile(const char* name, int slot)
{

	ResetCharacterAnim();
	auto mem = LoadMTNFile((char*)name);
	auto base = animcount * slot;

	//only load common anim if the motion of the character didn't have them included
	if (CharacterAnimations[0].Animation == nullptr)
	{
		PrintDebug("Load common Anim\n");
		LoadMTNFile((char*)"PLCOMMTN.PRS");
	}

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
	WriteData((NJS_MOTION***)0x726c5E, &CharacterAnimations_r[0].Animation);
}

static void PatchKnuxAnim()
{
	//spiral after image
	WriteData((NJS_MOTION***)0x72dcb1, &CharacterAnimations_r[0].Animation);
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

	//Super
	WriteData((NJS_MOTION***)0x49ccaa, &CharacterAnimations_r[0].Animation);
}

static void PatchObjects()
{

	//iron bar RH
	WriteData((NJS_MOTION***)0x4e2b3d, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x4e23b2, &CharacterAnimations_r[0].Animation);

	//iron bar CE
	WriteData((NJS_MOTION***)0x5e9700, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x5e9cca, &CharacterAnimations_r[0].Animation);

	//iron bar PC
	WriteData((NJS_MOTION***)0x706cc9, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x707246, &CharacterAnimations_r[0].Animation);

	//board CE
	WriteData((NJS_MOTION***)0x5ebedd, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x5ebe48, &CharacterAnimations_r[24].Animation);
	//board CE display 2
	WriteData((NJS_MOTION***)0x5ec197, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x5ec108, &CharacterAnimations_r[24].Animation);

	//Obj Jump (GF)
	WriteData((NJS_MOTION***)0x5f6eaf, &CharacterAnimations_r[0].Animation);

	//board MH
	WriteData((NJS_MOTION***)0x6f7dd5, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x6f7d3b, &CharacterAnimations_r[24].Animation);
}


void PatchMHAnimTest()
{
	WriteData((NJS_MOTION***)0x6f36ea, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x6f3709, &CharacterAnimations_r[0].Animation);
	WriteData((uint16_t**)0x6f36ff, &CharacterAnimations_r[0].Count);

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



void __cdecl CopyFinalAnim_r()
{
	int start = 0;
	int goal = 300;

	for (int j = 0; j < 4; j++) {

		if (MainCharObj1[j]) {

			if (j > 0)
			{
				start = 300 * j;
				goal = start + 300;
			}

			for (int i = start; i < goal; i++)
			{
				if (CharacterAnimations_r[i].Animation == nullptr)
				{
					CharacterAnimations_r[i].Animation = CharacterAnimations[i - start].Animation;
					CharacterAnimations_r[i].Index = i;
					CharacterAnimations_r[i].Count = CharacterAnimations[i - start].Count;
				}
			}
		}
	}

	ResetMusic();
}

void PatchAnimations()
{
	LoadMotion_Hack();
	PatchPlayAnimation();
	PatchObjects();
	PatchSonicAnim();
	PatchMilesAnim();

	WriteData<1>((int*)0x727DA0, 0xc3); //remove 2P icon fix miles crash in 2P

	WriteCall((void*)0x43CBCB, CopyFinalAnim_r);
}
