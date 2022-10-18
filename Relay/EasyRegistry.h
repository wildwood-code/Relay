/*******************************************************************************
* Copyright © 2022 Kerry S. Martin, martin@wild-wood.net
* Free for usage without warranty, expressed or implied; attribution required
*
* Filename   : Relay_ReadReg.h
* Description:
*   Utility to read registry and set default value if none is present
*
* Created    : 01/11/2022
* Modified   : 01/12/2022
* Author     : Kerry S. Martin, martin@wild-wood.net
*******************************************************************************/
#pragma once

#include <iostream>

// registry settings
constexpr size_t MAX_KEY_LENGTH = 255;
constexpr size_t MAX_SETTING_LENGTH = 63;
constexpr size_t MAX_SZ_RESULT_LENGTH = 255;

// read and create if non-existent
bool ReadRegSZ(char const* szKey, char const* szSetting, char* szResult, char const* szDefault=NULL);
bool ReadRegSZ(std::string const& strKey, std::string const& strSetting, std::string& strResult, std::string const& strDefault);
bool ReadRegDWORD(char const* szKey, char const* szSetting, unsigned long* pdwResult, unsigned long dwDefault);
bool ReadRegQWORD(char const* szKey, char const* szSetting, unsigned __int64* pqwResult, unsigned __int64 qwDefault);
bool ReadRegDWORD(std::string const& strKey, std::string const& strSetting, unsigned long& dwResult, unsigned long dwDefault);
bool ReadRegQWORD(std::string const& strKey, std::string const& strSetting, unsigned __int64& qwResult, unsigned __int64 qwDefault);

// read and exit if non-existent
bool ReadRegSZ(std::string const& strKey, std::string const& strSetting, std::string& strResult);
bool ReadRegDWORD(char const* szKey, char const* szSetting, unsigned long* pdwResult);
bool ReadRegQWORD(char const* szKey, char const* szSetting, unsigned __int64* pqwResult);
bool ReadRegDWORD(std::string const& strKey, std::string const& strSetting, unsigned long& dwResult);
bool ReadRegQWORD(std::string const& strKey, std::string const& strSetting, unsigned __int64& qwResult);

// write
bool WriteRegSZ(char const* szKey, char const* szSetting, char const* szValue);
bool WriteRegSZ(std::string const& strKey, std::string const& strSetting, std::string const& strValue);
bool WriteRegDWORD(char const* szKey, char const* szSetting, unsigned long dwValue);
bool WriteRegQWORD(char const* szKey, char const* szSetting, unsigned __int64 qwValue);
bool WriteRegDWORD(std::string const& strKey, std::string const& strSetting, unsigned long dwValue);
bool WriteRegQWORD(std::string const& strKey, std::string const& strSetting, unsigned __int64 qwValue);

/*******************************************************************************
* Copyright © 2022 Kerry S. Martin, martin@wild-wood.net
* Free for usage without warranty, expressed or implied; attribution required
*******************************************************************************/