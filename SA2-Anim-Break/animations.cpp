#include "pch.h"
#include <algorithm>

using namespace std;

static constexpr int charactersCount = 16;
static constexpr int animcount = 300;
static constexpr int charAnimStart = 86;

static AnimationIndex CharacterAnimations_r[animcount * charactersCount]{};

string charArray[]
{
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

void Move_CharacterAnimID(CharObj2Base* co2, char charID)
{
	for (int i = charAnimStart; i < animcount; i++)
	{
		if (co2->AnimInfo.Animations[i].AnimNum)
		{
			co2->AnimInfo.Animations[i].AnimNum = CharacterAnimations_r[i + animcount * charID].Index;
		}
	}
}

int GetIDFromMtnName(char* name)
{
	string stringCopy = name;

	std::transform(stringCopy.begin(), stringCopy.end(), stringCopy.begin(), std::tolower);

	if (stringCopy.find("plcom") != string::npos)
		return -2;

	for (uint8_t i = 0; i < LengthOfArray(charArray); i++) {

		if (stringCopy.find(charArray[i]) != string::npos)
		{
			return i + 1;
		}
	}

	return -1;
}

AnimationIndex* LoadMTNFile_r(char* name)
{
	auto mem = LoadMTNFile(name);

	auto charID = GetIDFromMtnName(name);

	 if (charID == -2) //common anim
	 {
		for (int i = 0; i < animcount; ++i)
		{
			CharacterAnimations_r[i].Index = CharacterAnimations[i].Index;
			CharacterAnimations_r[i].Animation = CharacterAnimations[i].Animation;
			CharacterAnimations_r[i].Count = CharacterAnimations[i].Count;
		}

		PrintDebug("Successfully copied common anim %s \n", name);
	 }
	 else if (charID > 0) {

		 for (int i = charAnimStart; i < animcount; ++i)
		 {
			 CharacterAnimations_r[i + animcount * charID].Index = i + animcount * charID;
			 CharacterAnimations_r[i + animcount * charID].Animation = CharacterAnimations[i].Animation;
			 CharacterAnimations_r[i + animcount * charID].Count = CharacterAnimations[i].Count;
		 }

		 PrintDebug("Successfully copied %s \n", name);

	 }
	else {
		PrintDebug("Failed to detect the character animation, animation couldn't be copied.\n");
	}

	 for (int i = 0; i < 2; i++) {

		 if (MainCharObj2[i]) {

			 auto curChar = charID - 1;

			 if (curChar >= Characters_B) {
				 curChar += 1;
				 charID += 1;
			 }

			 if (MainCharObj2[i]->CharID2 == curChar)
				 Move_CharacterAnimID(MainCharObj2[i], charID);
		 }
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

void PatchSonicAnim()
{
	//display
	WriteData((NJS_MOTION***)0x72075C, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x71e9f8, &CharacterAnimations_r[0].Animation);	
	WriteData((NJS_MOTION***)0x720670, &CharacterAnimations_r[0].Animation);
	
	//action
	WriteData((NJS_MOTION***)0x71b00f, &CharacterAnimations_r[0].Animation);	

	//grinding
	WriteData((uint32_t**)0x7267b2, &CharacterAnimations_r[0].Animation->nbFrame);
	WriteData((uint32_t**)0x726ac6, &CharacterAnimations_r[0].Animation->nbFrame);	
	WriteData((uint32_t**)0x726b5B, &CharacterAnimations_r[0].Animation->nbFrame);
	

}

void PatchMilesAnim()
{
	WriteData((NJS_MOTION***)0x750A94, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x750182, &CharacterAnimations_r[0].Animation);
}

//the most important hack
void PatchPlayAnimation()
{
	WriteData((NJS_MOTION***)0x469340, &CharacterAnimations_r[0].Animation);	
	WriteData((NJS_MOTION***)0x46934a, &CharacterAnimations_r[0].Animation);	
	WriteData((NJS_MOTION***)0x46950f, &CharacterAnimations_r[0].Animation);	
	WriteData((NJS_MOTION***)0x4695b6, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x46966C, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x46971B, &CharacterAnimations_r[0].Animation);	
	WriteData((NJS_MOTION***)0x469779, &CharacterAnimations_r[0].Animation);	
	WriteData((NJS_MOTION***)0x4697F7, &CharacterAnimations_r[0].Animation);	
	WriteData((NJS_MOTION***)0x46992B, &CharacterAnimations_r[0].Animation);	
	WriteData((NJS_MOTION***)0x469a31, &CharacterAnimations_r[0].Animation);	
	WriteData((NJS_MOTION***)0x469acB, &CharacterAnimations_r[0].Animation);	

	//PSetMotion
	WriteData((NJS_MOTION***)0x469cf1, &CharacterAnimations_r[0].Animation);	

	//PInitializeInterpolateMotion2_
	WriteData((NJS_MOTION***)0x469d36, &CharacterAnimations_r[0].Animation);	
	WriteData((NJS_MOTION***)0x469d48, &CharacterAnimations_r[0].Animation);

	//PInitializeInterpolateMotion2
	WriteData((NJS_MOTION***)0x469de4, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x469df9, &CharacterAnimations_r[0].Animation);	
	WriteData((uint16_t**)0x469e4C, &CharacterAnimations_r[0].Count);
	WriteData((uint16_t**)0x46a91C, &CharacterAnimations_r[0].Count);

	//PTerminateInterpolateMotion
	WriteData((uint16_t**)0x46a99E, &CharacterAnimations_r[0].Count);	
	
	//?????
	WriteData((uint32_t**)0x45a4dB, &CharacterAnimations_r[0].Animation->nbFrame);	
	WriteData((uint32_t**)0x45e4fA, &CharacterAnimations_r[0].Animation->nbFrame);


	
}


void LoadMotion_Hack()
{
	WriteCall((void*)0x45946E, LoadMTNFile_ASM); //com
	WriteCall((void*)0x49AA87, LoadMTNFile_ASM); //ss
	WriteCall((void*)0x49ACE8, LoadMTNFile_ASM); //ssh
	WriteCall((void*)0x716F98, LoadMTNFile_ASM); //sonic
	WriteCall((void*)0x717215, LoadMTNFile_ASM); //amy
	WriteCall((void*)0x7174F8, LoadMTNFile_ASM); //shadow
	WriteCall((void*)0x717771, LoadMTNFile_ASM); //MS	
	WriteCall((void*)0x728301, LoadMTNFile_ASM); //Knux
	WriteCall((void*)0x728651, LoadMTNFile_ASM); //Rouge
	WriteCall((void*)0x7289FA, LoadMTNFile_ASM); //Tikal
	WriteCall((void*)0x728C99, LoadMTNFile_ASM); //Chaos
	WriteCall((void*)0x73C346, LoadMTNFile_ASM); //Eggman	
	WriteCall((void*)0x740DEE, LoadMTNFile_ASM); //Mech Eggman
	WriteCall((void*)0x74104E, LoadMTNFile_ASM); //Mech Tails	
	WriteCall((void*)0x741240, LoadMTNFile_ASM); //Chao Walker	
	WriteCall((void*)0x741420, LoadMTNFile_ASM); //Dark Chao Walker	
	WriteCall((void*)0x74D02B, LoadMTNFile_ASM); //Miles

}

//replace every single existence of the vanilla CharacterAnimations array
void PatchAnimations()
{
	//can't trampoline LoadMTN for whatever reason so here we go WriteCall baby!
	LoadMotion_Hack();

	PatchPlayAnimation();

	PatchSonicAnim();
	PatchMilesAnim();

}