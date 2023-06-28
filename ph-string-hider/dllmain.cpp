#include "includes.h"

typedef NTSYSAPI NTSTATUS(NTAPI* tNtReadVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToRead, PULONG NumberOfBytesReaded);
tNtReadVirtualMemory NtReadVirtualMemory;

int hook(uintptr_t adress, void** _original, void* _function) {
    void* target = (void*)adress;
    if (MH_CreateHook(target, _function, _original) != MH_OK || MH_EnableHook(target) != MH_OK) {
        //printf("iror\n");
        return -1;
    }
    return 1;
}


NTSTATUS NtReadVirtualMemoryHook(_In_ HANDLE 	ProcessHandle,
    _In_opt_ PVOID 	BaseAddress,
    _Out_writes_bytes_(BufferSize) PVOID 	Buffer,
    _In_ SIZE_T 	BufferSize,
    _Out_opt_ PSIZE_T 	NumberOfBytesRead
) {
    //DWORD buff;
    //NTSTATUS nts = NtReadVirtualMemory(ProcessHandle, BaseAddress, &buff, BufferSize, NULL);

    //while (true) {
    //}
    return STATUS_SUCCESS;
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
    MH_Initialize();
    HMODULE ntdll = GetModuleHandle("ntdll.dll");
    if (ntdll) {
        uintptr_t target = (uintptr_t)GetProcAddress(ntdll, "NtReadVirtualMemory");
        NtReadVirtualMemory = (tNtReadVirtualMemory)GetProcAddress(ntdll, "NtReadVirtualMemory");

        DWORD oldProtect;
        if (target) {
            //hook(target + 8, nullptr, NtReadVirtualMemoryHook);
            hook(target, nullptr, NtReadVirtualMemoryHook);
        }
    }
    return TRUE;
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
		break;
	case DLL_PROCESS_DETACH:
		MH_DisableHook(MH_ALL_HOOKS);
		break;
	}
	return TRUE;
}