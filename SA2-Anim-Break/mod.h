#pragma once

void ReadConfig(const char* path);
void PatchAnimations();

#define TARGET_DYNAMIC(name) ((decltype(name##_r)*)name##_t->Target())
#define STATUS_FLOOR (Status_OnObjectColli | Status_Ground)