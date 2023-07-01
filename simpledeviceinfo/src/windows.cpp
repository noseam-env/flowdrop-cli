/*
 * This file is part of flowdrop-cli.
 *
 * For license and copyright information please follow this link:
 * https://github.com/FlowDrop/flowdrop-cli/blob/master/LEGAL
 */

#if defined(_WIN32)

#include "simpledeviceinfo.hpp"
#include <Windows.h>
#include <Shlwapi.h>
#include <comdef.h>
#include <Wbemidl.h>
#include <iostream>

#pragma comment(lib, "wbemuuid.lib")

typedef LONG NTSTATUS, *PNTSTATUS;
#define STATUS_SUCCESS (0x00000000)

typedef NTSTATUS(WINAPI *RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

RTL_OSVERSIONINFOW GetRealOSVersion() {
    HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
    if (hMod) {
        auto fxPtr = (RtlGetVersionPtr) ::GetProcAddress(hMod, "RtlGetVersion");
        if (fxPtr != nullptr) {
            RTL_OSVERSIONINFOW rovi = {0};
            rovi.dwOSVersionInfoSize = sizeof(rovi);
            if (STATUS_SUCCESS == fxPtr(&rovi)) {
                return rovi;
            }
        }
    }
    RTL_OSVERSIONINFOW rovi = {0};
    return rovi;
}

std::optional<std::string> GetWindowsVersion() {
    auto osvi = GetRealOSVersion();
    if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0) {
        if (osvi.dwBuildNumber >= 22000 && osvi.dwBuildNumber < 23000) {
            return "11";
        }
        return "10";
    } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3) {
        return "8.1";
    } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2) {
        return "8";
    } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) {
        return "7";
    } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0) {
        return "Vista";
    } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
        return "Server 2003 or XP x64 Edition";
    } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) {
        return "XP";
    } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
        return "2000";
    }
    return {};
}

std::optional<std::string> GetComputerModel() {
    std::string computerModel;

    HRESULT hres;

    // Инициализация COM библиотеки
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        return std::nullopt;
    }

    // Инициализация безопасности COM
    hres = CoInitializeSecurity(
            nullptr,
            -1,
            nullptr,
            nullptr,
            RPC_C_AUTHN_LEVEL_DEFAULT,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            nullptr,
            EOAC_NONE,
            nullptr);
    if (FAILED(hres)) {
        CoUninitialize();
        return std::nullopt;
    }

    // Инициализация WMI
    IWbemLocator *pLoc = nullptr;
    hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator,
            reinterpret_cast<LPVOID *>(&pLoc));
    if (FAILED(hres)) {
        CoUninitialize();
        return std::nullopt;
    }

    // Подключение к WMI
    IWbemServices *pSvc = nullptr;
    hres = pLoc->ConnectServer(
            _bstr_t(L"ROOT\\CIMV2"),
            nullptr,
            nullptr,
            nullptr,
            0,
            nullptr,
            nullptr,
            &pSvc);
    if (FAILED(hres)) {
        pLoc->Release();
        CoUninitialize();
        return std::nullopt;
    }

    // Установка безопасности для вызовов WMI
    hres = CoSetProxyBlanket(
            pSvc,
            RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE,
            nullptr,
            RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            nullptr,
            EOAC_NONE);
    if (FAILED(hres)) {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return std::nullopt;
    }

    // Выполнение запроса WMI для получения модели компьютера
    IEnumWbemClassObject *pEnumerator = nullptr;
    hres = pSvc->ExecQuery(
            bstr_t("WQL"),
            bstr_t("SELECT * FROM Win32_ComputerSystem"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            nullptr,
            &pEnumerator);
    if (SUCCEEDED(hres)) {
        IWbemClassObject *pclsObj = nullptr;
        ULONG uReturn = 0;

        // Получение первого объекта
        hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 1) {
            VARIANT vtProp;

            // Получение свойства "Model" из объекта
            hres = pclsObj->Get(L"Model", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hres)) {
                // Конвертация VARIANT в std::string
                computerModel = _bstr_t(vtProp.bstrVal);

                VariantClear(&vtProp);
            }
        }

        pclsObj->Release();
    }

    pEnumerator->Release();
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    if (!computerModel.empty()) {
        return computerModel;
    } else {
        return std::nullopt;
    }
}

void SDIFetch(SDIInfo &info) {
    info.platform = "Windows";
    info.system_version = GetWindowsVersion();

    // uuid
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        char uuid[256];
        DWORD uuidSize = sizeof(uuid);
        if (RegQueryValueExA(hKey, "MachineGuid", nullptr, nullptr,
                             reinterpret_cast<LPBYTE>(uuid), &uuidSize) == ERROR_SUCCESS) {
            info.uuid = uuid;
        }
        RegCloseKey(hKey);
    }

    // name
    char computerName[256];
    DWORD computerNameSize = sizeof(computerName);
    if (GetComputerNameExA(ComputerNameDnsHostname, computerName, &computerNameSize)) {
        info.name = computerName;
    }

    // model
    info.model = GetComputerModel();
}

#endif
