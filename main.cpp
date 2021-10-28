#include <iostream>
#include <stdio.h>
#define WINVER 0x0A00
#include <windows.h>

#define BUFSIZE MAX_PATH
#define FILESYSNAMEBUFSIZE MAX_PATH
#define MAX_KEY_LENGTH 255

using namespace std;

#define INFO_BUFFER_SIZE 32767

//2.
double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if (!QueryPerformanceFrequency(&li))
        cout << "Function QueryPerformanceFrequency() failed!\n";

    PCFreq = double(li.QuadPart);

    printf("\n CPU frequency: %u  Hz\n", li);
    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}

double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double((li.QuadPart - CounterStart) * 1000000) / PCFreq;
}

int main()
{
    // 1.1
    DWORD dwVersion = 0;
    DWORD dwMajorVersion = 0;
    DWORD dwMinorVersion = 0;
    DWORD dwBuild = 0;

    dwVersion = GetVersion();

    // Get the Windows version.

    dwMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
    dwMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

    // Get the build number.

    if (dwVersion < 0x80000000)
        dwBuild = (DWORD)(HIWORD(dwVersion));

    printf("Version is %d.%d (%d)\n",
        dwMajorVersion,
        dwMinorVersion,
        dwBuild);

    // 1.2
    TCHAR szPath[_MAX_PATH] = { 0 };
    UINT iRet = GetSystemDirectory(szPath, _MAX_PATH);
    printf("Result function GetSystemDirectory %d\n", iRet);

    // 1.3
    char buffer[256];
    DWORD size = 256;
    GetComputerNameA(buffer, &size);
    printf("Computer name is : %s\n", buffer);

    GetUserNameA(buffer, &size);
    printf("User name is : %s\n", buffer);

    // 1.4
    char buffer2[MAX_PATH + 1];
    DWORD size2 = MAX_PATH;
    char buffer3[MAX_PATH + 1];
    DWORD  CBufLen = MAX_PATH;
    __int64 total, available, free;

    HANDLE firstVolume =  FindFirstVolumeA(buffer2, size2);
    printf("\n      %s", buffer2);

    do {
        printf("\n%s", buffer2);
        CBufLen = MAX_PATH;
        GetVolumePathNamesForVolumeNameA(buffer2, buffer3, CBufLen, &CBufLen);
        char* path = buffer3;
        printf("\npath: %s", path);
        GetDiskFreeSpaceExA(buffer2, (PULARGE_INTEGER)&available, (PULARGE_INTEGER)&total, (PULARGE_INTEGER)&free);
        printf("\nsize: %I64d  bytes ", total);
        printf("\nFree space: %I64d  bytes\n", available);



    } while (FindNextVolumeA(firstVolume, buffer2, BUFSIZE));
    FindVolumeClose(firstVolume);



    // 1.5
    DWORD dwSize;
    TCHAR szName[MAX_KEY_LENGTH];
    HKEY hKey;
    DWORD dwIndex = 0;
    DWORD retCode;
    DWORD BufferSize = 8192;
    PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(BufferSize);
    DWORD cbData = BufferSize;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        0, KEY_ALL_ACCESS, &hKey) == !ERROR_SUCCESS)
    {
        printf("Function RegOpenKeyEx() failed!\n");
    }
    else printf("\n1.5.  Startup commands:\n");

    while (1) {
        dwSize = sizeof(szName);
        retCode = RegEnumValue(hKey, dwIndex, szName, &dwSize, NULL, NULL, NULL, NULL);
        if (retCode == ERROR_SUCCESS)
        {
            RegQueryValueEx(hKey, szName, NULL, NULL, (LPBYTE)PerfData, &cbData);
            printf("      %d: %s:  %s\n", dwIndex + 1, szName, PerfData);
            dwIndex++;
        }
        else break;
    }

    RegCloseKey(hKey);

    StartCounter();
    cout << "CPU clock count: " << GetCounter() << "  us \n";
    return 0;


}
