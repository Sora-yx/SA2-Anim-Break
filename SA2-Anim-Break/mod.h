#pragma once

void ReadConfig(const char* path);
void PatchAnimations();

#define TARGET_DYNAMIC(name) ((decltype(name##_r)*)name##_t->Target())
#define STATUS_FLOOR (Status_OnObjectColli | Status_Ground)


#define CURRENT_MATRIX _nj_current_matrix_ptr_
#define MATRIX_1A51A00 ((NJS_MATRIX_PTR)0x1A51A00) //matrix used for Sonic CallBack

using ModelFuncPtr = void(__cdecl*)(NJS_OBJECT*);
DataPointer(ModelFuncPtr, UpgradeDrawCallback, 0x1A55834);
FunctionPointer(void, DrawObjWithCallBack, (NJS_OBJECT* obj), 0x42E730);

static const void* const njScaleExPtr = (void*)0x429740;
static inline void njScaleEx(NJS_VECTOR* a1)
{
	__asm
	{
		mov eax, [a1]
		call njScaleExPtr
	}
}

//void __usercall SonicHeldObjectThing(EntityData1* ecx0@<ecx>, CharObj2Base* co2@<eax>)
static const void* const sonicHeldPtr = (void*)0x725C70;
static inline void SonicHeldObjectThing(EntityData1* a1, CharObj2Base* a2)
{
	__asm
	{
		mov eax, [a2]
		mov ecx, [a1]
		call sonicHeldPtr
	}
}

void njCnkMotion(NJS_OBJECT* obj, NJS_MOTION* mot, float frame);
FunctionPointer(void, sub_427040, (NJS_MATRIX_PTR a1, NJS_MATRIX_PTR a2), 0x427040); //njs_matrix_ptr not float, optimization in release = fucked

DataPointer(char, byte_1DE4400, 0x1DE4400);

static const void* const sub_487060Ptr = (void*)0x487060;
static inline void sub_487060(int a1)
{
	__asm
	{
		mov eax, [a1]
		call sub_487060Ptr
	}
}
