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
	"terios"
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
			co2->AnimInfo.Animations[i].AnimNum = co2->AnimInfo.Animations[i].AnimNum;
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


//replace every single existence of the vanilla CharacterAnimations array
void PatchAnimations()
{
	//can't trampoline LoadMTN for whatever reason so here we go WriteCall baby!

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


	WriteData((NJS_MOTION***)0x750A94, &CharacterAnimations_r[0].Animation);
	WriteData((NJS_MOTION***)0x750182, &CharacterAnimations_r[0].Animation);
}