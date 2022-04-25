/*******************************************************************************
* Copyright © 2022 Kerry S. Martin, martin@wild-wood.net
* Free for usage without warranty, expressed or implied; attribution required
*
* Filename   : EasyRegistry.cpp
* Description:
*   Utility to read registry and set default value if none is present
*
* Created    : 01/11/2022
* Modified   : 01/12/2022
* Author     : Kerry S. Martin, martin@wild-wood.net
*******************************************************************************/

#include <Windows.h>
#include <winreg.h>
#include <stdlib.h>
#include "EasyRegistry.h"


/*******************************************************************************
* Function   : ReadRegSZ
* Arguments  : strKey     = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              strSetting = registry value in the key (e.g., "SettingName")
*              strResult  = receives the result stored in the registry
*              strDefault = default value (or NULL if no default)
* Returns    : true = success, false = failure; strResult contains value
* Description:
*   Reads a REG_SZ setting from the registry and writes a default if non-existent.
*/
bool ReadRegSZ(std::string const& strKey, std::string const& strSetting, std::string& strResult, std::string const& strDefault)
{
    char szResult[MAX_SZ_RESULT_LENGTH + 1] = "";
    bool bResult = ReadRegSZ(strKey.c_str(), strSetting.c_str(), szResult, strDefault.c_str());
    strResult = szResult;
    return bResult;
}


/*******************************************************************************
* Function   : ReadRegSZ
* Arguments  : strKey     = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              strSetting = registry value in the key (e.g., "SettingName")
*              strResult  = receives the result stored in the registry
*              strDefault = default value (or NULL if no default)
* Returns    : true = success, false = failure; strResult contains value
* Description:
*   Reads a REG_SZ setting from the registry
*/
bool ReadRegSZ(std::string const& strKey, std::string const& strSetting, std::string& strResult)
{
    char szResult[MAX_SZ_RESULT_LENGTH + 1] = "";
    bool bResult = ReadRegSZ(strKey.c_str(), strSetting.c_str(), szResult, NULL);
    strResult = szResult;
    return bResult;
}


/*******************************************************************************
* Function   : ReadRegDWORD
* Arguments  : strKey     = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              strSetting = registry value in the key (e.g., "SettingName")
*              dwResult   = reference receives the result stored in the registry
*              dwDefault  = default value (or NULL if no default)
* Returns    : true = success, false = failure; dwResult contains value
* Description:
*   Reads a DWORD (32-bit) setting from the registry and writes a default if non-existent.
*/
bool ReadRegDWORD(std::string const& strKey, std::string const& strSetting, unsigned long& dwResult, unsigned long dwDefault)
{
    return ReadRegDWORD(strKey.c_str(), strSetting.c_str(), &dwResult, dwDefault);
}


/*******************************************************************************
* Function   : ReadRegQWORD
* Arguments  : strKey     = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              strSetting = registry value in the key (e.g., "SettingName")
*              qwResult   = reference receives the result stored in the registry
*              qwDefault  = default value (or NULL if no default)
* Returns    : true = success, false = failure; qwResult contains value
* Description:
*   Reads a QWORD (64-bit) setting from the registry and writes a default if non-existent.
*/
bool ReadRegQWORD(std::string const& strKey, std::string const& strSetting, unsigned __int64& qwResult, unsigned __int64 qwDefault)
{
    return ReadRegQWORD(strKey.c_str(), strSetting.c_str(), &qwResult, qwDefault);
}


/*******************************************************************************
* Function   : ReadRegDWORD
* Arguments  : strKey     = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              strSetting = registry value in the key (e.g., "SettingName")
*              dwResult   = reference receives the result stored in the registry
* Returns    : true = success, false = failure; dwResult contains value
* Description:
*   Reads a DWORD (32-bit) setting from the registry
*/
bool ReadRegDWORD(std::string const& strKey, std::string const& strSetting, unsigned long& dwResult)
{
    return ReadRegDWORD(strKey.c_str(), strSetting.c_str(), &dwResult);
}


/*******************************************************************************
* Function   : ReadRegQWORD
* Arguments  : strKey     = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              strSetting = registry value in the key (e.g., "SettingName")
*              qwResult   = reference receives the result stored in the registry
* Returns    : true = success, false = failure; qwResult contains value
* Description:
*   Reads a QWORD (64-bit) setting from the registry
*/
bool ReadRegQWORD(std::string const& strKey, std::string const& strSetting, unsigned __int64& qwResult)
{
    return ReadRegQWORD(strKey.c_str(), strSetting.c_str(), &qwResult);
}


/*******************************************************************************
* Function   : WriteRegSZ
* Arguments  : strKey     = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              strSetting = registry value in the key (e.g., "SettingName")
*              strValue   = value to store in the registry
* Returns    : true = success, false = failure
* Description:
*   Writes a REG_SZ setting to the registry
*/
bool WriteRegSZ(std::string const& strKey, std::string const& strSetting, std::string const& strValue)
{
    return WriteRegSZ(strKey.c_str(), strSetting.c_str(), strValue.c_str());
}


/*******************************************************************************
* Function   : WriteRegDWORD
* Arguments  : strKey     = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              strSetting = registry value in the key (e.g., "SettingName")
*              dwValue    = value to store in the registry
* Returns    : true = success, false = failure
* Description:
*   Writes a DWORD (32-bit) setting to the registry
*/
bool WriteRegDWORD(std::string const& strKey, std::string const& strSetting, unsigned long dwValue)
{
    return WriteRegDWORD(strKey.c_str(), strSetting.c_str(), dwValue);
}



/*******************************************************************************
* Function   : WriteRegQWORD
* Arguments  : strKey     = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              strSetting = registry value in the key (e.g., "SettingName")
*              qwValue    = value to store in the registry
* Returns    : true = success, false = failure
* Description:
*   Writes a QWORD (64-bit) setting to the registry
*/
bool WriteRegQWORD(std::string const& strKey, std::string const& strSetting, unsigned __int64 qwValue)
{
    return WriteRegQWORD(strKey.c_str(), strSetting.c_str(), qwValue);
}


/*******************************************************************************
* Function   : ReadRegSZ
* Arguments  : szKey      = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              szSetting  = registry value in the key (e.g., "SettingName")
*              szResult   = receives the result stored in the registry
*              szDefault  = default value (or NULL if no default)
* Returns    : true = success, false = failure
* Description:
*   Reads a REG_SZ setting from the registry and (optionally) writes a default if non-existent.
*   If szDefault is NULL, then no setting is written to the registry.
*   If szDefault is NULL and the registry value does not exist, the return value is false.
*/
bool ReadRegSZ(char const* szKey, char const* szSetting, char* szResult, char const* szDefault)
{
    bool bResult = false;

    size_t nconv;
    wchar_t szwSettingsKey[MAX_KEY_LENGTH + 1];
    wchar_t szwSetting[MAX_SETTING_LENGTH + 1];
    wchar_t szwResult[MAX_SZ_RESULT_LENGTH + 1] = L"";
    DWORD cbData;

    // convert these to wide char strings
    mbstowcs_s(&nconv, szwSettingsKey, szKey, MAX_KEY_LENGTH);
    mbstowcs_s(&nconv, szwSetting, szSetting, MAX_SETTING_LENGTH);

    // try to read the value
    cbData = MAX_SZ_RESULT_LENGTH * sizeof(wchar_t);
    LSTATUS result = RegGetValueW(HKEY_CURRENT_USER, szwSettingsKey, szwSetting, RRF_RT_REG_SZ, nullptr, szwResult, &cbData);

    if (result == ERROR_SUCCESS)
    {	// read REG_SZ from registry, convert to szResult
        wcstombs_s(&nconv, szResult, (MAX_SZ_RESULT_LENGTH + 1) * sizeof(char), szwResult, MAX_SZ_RESULT_LENGTH * sizeof(char));
        bResult = true;
    }
    else if (szDefault)
    {	// create the key and value
        HKEY hKey;
        result = RegCreateKeyW(HKEY_CURRENT_USER, szwSettingsKey, &hKey);
        if (result == ERROR_SUCCESS)
        {
            mbstowcs_s(&nconv, szwResult, szDefault, MAX_SZ_RESULT_LENGTH);
            result = RegSetValueExW(hKey, szwSetting, 0, REG_SZ, (LPBYTE)szwResult, DWORD((wcslen(szwResult) + 1) * sizeof(wchar_t)));

            if (result == ERROR_SUCCESS)
            {	// copy to return value
                wcstombs_s(&nconv, szResult, (MAX_SZ_RESULT_LENGTH + 1) * sizeof(char), szwResult, MAX_SZ_RESULT_LENGTH * sizeof(char));
                bResult = true;
            }
            else
            {	// clear result to empty value
                szResult[0] = 0;
            }

            RegCloseKey(hKey);
        }
        else
        {	// clear result to empty value
            szResult[0] = 0;
        }
    }
    else
    {	// clear result to empty value
        szResult[0] = 0;
    }

    return bResult;
}



/*******************************************************************************
* Function   : WriteRegSZ
* Arguments  : szKey      = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              szSetting  = registry value in the key (e.g., "SettingName")
*              szValue    = default value
* Returns    : true = success, false = failure
* Description:
*   Writes a REG_SZ setting to the registry
*/
bool WriteRegSZ(char const* szKey, char const* szSetting, char const* szValue)
{
    bool bResult = false;

    if (szValue)
    {

        size_t nconv;
        wchar_t szwSettingsKey[MAX_KEY_LENGTH + 1];
        wchar_t szwSetting[MAX_SETTING_LENGTH + 1];
        wchar_t szwResult[MAX_SZ_RESULT_LENGTH + 1] = L"";

        // convert these to wide char strings
        mbstowcs_s(&nconv, szwSettingsKey, szKey, MAX_KEY_LENGTH);
        mbstowcs_s(&nconv, szwSetting, szSetting, MAX_SETTING_LENGTH);

        HKEY hKey;

        // open the key or create it if it does not exist
        LSTATUS result = RegCreateKeyW(HKEY_CURRENT_USER, szwSettingsKey, &hKey);

        if (result == ERROR_SUCCESS)
        {   // it either existed or was created successfully, write the value
            mbstowcs_s(&nconv, szwResult, szValue, MAX_SZ_RESULT_LENGTH);
            result = RegSetValueExW(hKey, szwSetting, 0, REG_SZ, (LPBYTE)szwResult, DWORD((wcslen(szwResult) + 1) * sizeof(wchar_t)));

            if (result == ERROR_SUCCESS)
                bResult = true;

            RegCloseKey(hKey);
        }
    }

    return bResult;
}


/*******************************************************************************
* Function   : ReadRegDWORD
* Arguments  : szKey      = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              szSetting  = registry value in the key (e.g., "SettingName")
*              pdwResult  = receives the result stored in the registry
*              dwDefault  = default value
* Returns    : true = success, false = failure
* Description:
*   Reads a DWORD (32-bit) setting from the registry and (optionally) writes a default if non-existent.
*/
bool ReadRegDWORD(char const* szKey, char const* szSetting, unsigned long* pdwResult, unsigned long dwDefault)
{
    bool bResult = false;

    size_t nconv;
    wchar_t szwSettingsKey[MAX_KEY_LENGTH + 1];
    wchar_t szwSetting[MAX_SETTING_LENGTH + 1];
    DWORD cbData;

    // convert these to wide char strings
    mbstowcs_s(&nconv, szwSettingsKey, szKey, MAX_KEY_LENGTH);
    mbstowcs_s(&nconv, szwSetting, szSetting, MAX_SETTING_LENGTH);

    // try to read the value
    cbData = sizeof(unsigned long);
    LSTATUS  result = RegGetValueW(HKEY_CURRENT_USER, szwSettingsKey, szwSetting, RRF_RT_DWORD, nullptr, pdwResult, &cbData);

    if (result == ERROR_SUCCESS)
    {   // key existed and the value has been stored in the result
        bResult = true;
    }
    else
    {	// create the key and value
        HKEY hKey;
        result = RegCreateKeyW(HKEY_CURRENT_USER, szwSettingsKey, &hKey);
        if (result == ERROR_SUCCESS)
        {
            *pdwResult = dwDefault;
            result = RegSetValueExW(hKey, szwSetting, 0, REG_DWORD, (LPBYTE)pdwResult, DWORD(sizeof(unsigned long)));

            if (result == ERROR_SUCCESS)
                bResult = true;

            RegCloseKey(hKey);
        }
    }

    return bResult;
}


/*******************************************************************************
* Function   : ReadRegDWORD
* Arguments  : szKey      = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              szSetting  = registry value in the key (e.g., "SettingName")
*              pdwResult  = receives the result stored in the registry
* Returns    : true = success, false = failure
* Description:
*   Reads a DWORD (32-bit) setting from the registry
*/
bool ReadRegDWORD(char const* szKey, char const* szSetting, unsigned long* pdwResult)
{
    bool bResult = false;

    size_t nconv;
    wchar_t szwSettingsKey[MAX_KEY_LENGTH + 1];
    wchar_t szwSetting[MAX_SETTING_LENGTH + 1];
    DWORD cbData;

    // convert these to wide char strings
    mbstowcs_s(&nconv, szwSettingsKey, szKey, MAX_KEY_LENGTH);
    mbstowcs_s(&nconv, szwSetting, szSetting, MAX_SETTING_LENGTH);

    // try to read the value
    cbData = sizeof(unsigned long);
    LSTATUS  result = RegGetValueW(HKEY_CURRENT_USER, szwSettingsKey, szwSetting, RRF_RT_DWORD, nullptr, pdwResult, &cbData);

    if (result == ERROR_SUCCESS)
    {   // key existed and the value has been stored in the result
        bResult = true;
    }

    return bResult;
}


/*******************************************************************************
* Function   : ReadRegQWORD
* Arguments  : szKey      = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              szSetting  = registry value in the key (e.g., "SettingName")
*              pqwResult  = receives the result stored in the registry
*              qwDefault  = default value
* Returns    : true = success, false = failure
* Description:
*   Reads a QWORD (64-bit) setting from the registry and (optionally) writes a default if non-existent.
*/
bool ReadRegQWORD(char const* szKey, char const* szSetting, unsigned __int64* pqwResult, unsigned __int64 qwDefault)
{
    bool bResult = false;

    size_t nconv;
    wchar_t szwSettingsKey[MAX_KEY_LENGTH + 1];
    wchar_t szwSetting[MAX_SETTING_LENGTH + 1];
    DWORD cbData;

    // convert these to wide char strings
    mbstowcs_s(&nconv, szwSettingsKey, szKey, MAX_KEY_LENGTH);
    mbstowcs_s(&nconv, szwSetting, szSetting, MAX_SETTING_LENGTH);

    // try to read the value
    cbData = sizeof(unsigned __int64);
    LSTATUS  result = RegGetValueW(HKEY_CURRENT_USER, szwSettingsKey, szwSetting, RRF_RT_QWORD, nullptr, pqwResult, &cbData);

    if (result == ERROR_SUCCESS)
    {   // key existed and the value has been stored in the result
        bResult = true;
    }
    else
    {	// create the key and value
        HKEY hKey;
        result = RegCreateKeyW(HKEY_CURRENT_USER, szwSettingsKey, &hKey);
        if (result == ERROR_SUCCESS)
        {
            *pqwResult = qwDefault;
            result = RegSetValueExW(hKey, szwSetting, 0, REG_QWORD, (LPBYTE)pqwResult, DWORD(sizeof(unsigned __int64)));

            if (result == ERROR_SUCCESS)
                bResult = true;

            RegCloseKey(hKey);
        }
    }

    return bResult;
}



/*******************************************************************************
* Function   : ReadRegQWORD
* Arguments  : szKey      = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              szSetting  = registry value in the key (e.g., "SettingName")
*              pqwResult  = receives the result stored in the registry
* Returns    : true = success, false = failure
* Description:
*   Reads a QWORD (64-bit) setting from the registry
*/
bool ReadRegQWORD(char const* szKey, char const* szSetting, unsigned __int64* pqwResult)
{
    bool bResult = false;

    size_t nconv;
    wchar_t szwSettingsKey[MAX_KEY_LENGTH + 1];
    wchar_t szwSetting[MAX_SETTING_LENGTH + 1];
    DWORD cbData;

    // convert these to wide char strings
    mbstowcs_s(&nconv, szwSettingsKey, szKey, MAX_KEY_LENGTH);
    mbstowcs_s(&nconv, szwSetting, szSetting, MAX_SETTING_LENGTH);

    // try to read the value
    cbData = sizeof(unsigned __int64);
    LSTATUS  result = RegGetValueW(HKEY_CURRENT_USER, szwSettingsKey, szwSetting, RRF_RT_QWORD, nullptr, pqwResult, &cbData);

    if (result == ERROR_SUCCESS)
    {   // key existed and the value has been stored in the result
        bResult = true;
    }

    return bResult;
}



/*******************************************************************************
* Function   : WriteRegDWORD
* Arguments  : szKey      = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              szSetting  = registry value in the key (e.g., "SettingName")
*              dwValue    = default value
* Returns    : true = success, false = failure
* Description:
*   Writes a DWORD (32-bit) setting to the registry
*/
bool WriteRegDWORD(char const* szKey, char const* szSetting, unsigned long dwValue)
{
    bool bResult = false;

    size_t nconv;
    wchar_t szwSettingsKey[MAX_KEY_LENGTH + 1];
    wchar_t szwSetting[MAX_SETTING_LENGTH + 1];

    // convert these to wide char strings
    mbstowcs_s(&nconv, szwSettingsKey, szKey, MAX_KEY_LENGTH);
    mbstowcs_s(&nconv, szwSetting, szSetting, MAX_SETTING_LENGTH);

    // create the key and value
    HKEY hKey;
    LSTATUS result = RegCreateKeyW(HKEY_CURRENT_USER, szwSettingsKey, &hKey);
    if (result == ERROR_SUCCESS)
    {
        unsigned long dwValueB = dwValue;
        result = RegSetValueExW(hKey, szwSetting, 0, REG_DWORD, (LPBYTE)&dwValueB, DWORD(sizeof(unsigned long)));

        if (result == ERROR_SUCCESS)
            bResult = true;

        RegCloseKey(hKey);
    }

    return bResult;
}


/*******************************************************************************
* Function   : WriteRegQWORD
* Arguments  : szKey      = registry key in HKCU (e.g., "SOFTWARE\\Company\\ProgName\\Settings\\"
*              szSetting  = registry value in the key (e.g., "SettingName")
*              qwValue    = default value
* Returns    : true = success, false = failure
* Description:
*   Writes a QWORD (64-bit) setting to the registry
*/
bool WriteRegQWORD(char const* szKey, char const* szSetting, unsigned __int64 qwValue)
{
    bool bResult = false;

    size_t nconv;
    wchar_t szwSettingsKey[MAX_KEY_LENGTH + 1];
    wchar_t szwSetting[MAX_SETTING_LENGTH + 1];

    // convert these to wide char strings
    mbstowcs_s(&nconv, szwSettingsKey, szKey, MAX_KEY_LENGTH);
    mbstowcs_s(&nconv, szwSetting, szSetting, MAX_SETTING_LENGTH);

    // create the key and value
    HKEY hKey;
    LSTATUS result = RegCreateKeyW(HKEY_CURRENT_USER, szwSettingsKey, &hKey);
    if (result == ERROR_SUCCESS)
    {
        unsigned __int64 dwValueB = qwValue;
        result = RegSetValueExW(hKey, szwSetting, 0, REG_DWORD, (LPBYTE)&dwValueB, DWORD(sizeof(unsigned __int64)));

        if (result == ERROR_SUCCESS)
            bResult = true;

        RegCloseKey(hKey);
    }

    return bResult;
}


/*******************************************************************************
* Copyright © 2022 Kerry S. Martin, martin@wild-wood.net
* Free for usage without warranty, expressed or implied; attribution required
*******************************************************************************/