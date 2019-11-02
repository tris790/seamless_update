#include <stdio.h>
#include <windows.h>

int CalculateSomeThing(int *ptr)
{
    return *ptr + *(ptr + 1);
}

int main(int argc, const char *argv[])
{
    HANDLE handle = OpenFileMapping(
        FILE_MAP_ALL_ACCESS,
        FALSE,
        TEXT("SEAMLESS__MAIN"));
    int *view = (int *)(MapViewOfFile(
        handle,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        1024));

    for (int i = 0; i < 20; i++)
    {
        printf("Demo1 (v%d): \n", view[0]);
        Sleep(500);
    }
    return 0;
}