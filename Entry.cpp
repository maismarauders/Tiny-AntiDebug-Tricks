//
// Anti-Debug techniques taken from: https://anti-debug.checkpoint.com/techniques
//
// TODO: Add additional small anti-debug techniques
//
#include <stdio.h>
#include <windows.h>

// https://anti-debug.checkpoint.com/techniques/object-handles.html#closehandle
bool CloseHandleTrick()
{
    __try
    {
        CloseHandle((HANDLE)0xC4F3B4BE);
        return false;
    }
    __except (EXCEPTION_INVALID_HANDLE == GetExceptionCode()
        ? EXCEPTION_EXECUTE_HANDLER
        : EXCEPTION_CONTINUE_SEARCH)
    {
        return true;
    }
}

// https://anti-debug.checkpoint.com/techniques/object-handles.html#loadlibrary
bool LoadLibraryTrick()
{
    CHAR szBuffer[] = { "C:\\Windows\\System32\\calc.exe" };
    LoadLibraryA(szBuffer);
    return INVALID_HANDLE_VALUE == CreateFileA(szBuffer, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
}

// https://anti-debug.checkpoint.com/techniques/interactive.html#outputdebugstring
bool DebugStringTrick()
{
    DWORD dwLastError = GetLastError();
    OutputDebugString(L"Uh oh");
    return GetLastError() != dwLastError;
}

int main(int argc, char* argv)
{
    printf("[>] Starting tricks...\n\n");

    int total_tests = 3;
    int failed = 0;

    bool detected = CloseHandleTrick();
    printf("[%s] CloseHandleTrick\n", detected ? "+" : "-");
    if (detected)
        failed++;

    detected = LoadLibraryTrick();
    printf("[%s] LoadLibraryTrick\n", detected ? "+" : "-");
    if (detected)
        failed++;
    
    detected = DebugStringTrick();
    printf("[%s] DebugStringTrick\n", detected ? "+" : "-");
    if (detected)
        failed++;

    printf("\n[>] Total tests: %d, Detection rate: %d/%d\n", total_tests, failed, total_tests);
    system("pause");
	return 0;
}