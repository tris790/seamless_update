#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <vector>

void CreateNewInstance(STARTUPINFO *info, PROCESS_INFORMATION *processInfo, HANDLE shared_mem)
{
    if (!CreateProcess(TEXT("server1.exe"), NULL, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, info, processInfo))
    {
        printf("Error creating process\n");
    }
}

HANDLE CreateSharedMemory(int size)
{
    return CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                             0, size, TEXT("SEAMLESS__MAIN"));
}

struct shared_view
{
    bool inUse;
    DWORD pid;
    WSAPROTOCOL_INFO protocol_info;
};

int main()
{
    printf("Seamless_update\n");
    std::vector<PROCESS_INFORMATION> processes;
    // int data[5] = {
    // 0,
    // 4,
    // 5,
    // 2,
    // 76};
    // SOCKET data[2]{INVALID_SOCKET, INVALID_SOCKET};
    shared_view data = {false, 0, 0};
    HANDLE shared_mem = CreateSharedMemory(sizeof(shared_view));
    if (!shared_mem)
    {
        printf("Couldn't create shared memory... %d\n", GetLastError());
        return -1;
    }

    shared_view *view = (shared_view *)(MapViewOfFile(
        shared_mem,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        sizeof(shared_view)));

    memcpy(view, &data, sizeof(data));

    while (true)
    {
        if (processes.size() == 0)
        {
            view->inUse = false;
        }

        printf("(1) kill, (2) new instance [%d running]\n", processes.size());
        int result = 0;
        scanf("%d", &result);
        printf("val: %d\n", result);
        // view[0] = processes.size();
        if (result == 1)
        {
            for (auto e : processes)
            {
                TerminateProcess(e.hProcess, 0);
                CloseHandle(e.hProcess);
                CloseHandle(e.hThread);
            }
            processes.clear();
        }
        else if (result == 2)
        {
            STARTUPINFO info = {sizeof(info)};
            PROCESS_INFORMATION processInfo;
            CreateNewInstance(&info, &processInfo, shared_mem);
            processes.push_back(processInfo);
        }
        else
        {
            printf("???\n");
        }
    }
    return 0;
}