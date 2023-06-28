#pragma once

#include <ntstatus.h>
#include "minhook/include/MinHook.h"

#if _WIN64 
#pragma comment(lib, "minhook/libMinHook.x64.lib")
#else
#pragma comment(lib, "minhook/libMinHook.x86.lib")
#endif

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)