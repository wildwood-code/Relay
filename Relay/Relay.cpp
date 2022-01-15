/*******************************************************************************
* Copyright © 2022 Kerry S. Martin, martin@wild-wood.net
* Free for usage without warranty, expressed or implied; attribution required
*
* Filename   : Relay.cpp
* Description:
*   Command line utility to control USB HID relays (usb_relay_device.dll).
*
* Created    : 01/11/2022
* Modified   : 01/14/2022
* Author     : Kerry S. Martin, martin@wild-wood.net
*******************************************************************************/

#include <iostream>
#include <algorithm>
#include <regex>
#include <map>
using namespace std;

// USB HID relay interface (usb_relay_device.dll)
#include "usb_relay_device.h"
#pragma comment(lib, "x64/usb_relay_device.lib")

// version information
constexpr char APP_VERSION[] = "1.00";

// map to hold set states for relays
enum class LOGIC { H, L, X };
typedef char relay_idx_t;
constexpr relay_idx_t RELAY_IDX_ALL = '0';
constexpr relay_idx_t RELAY_IDX_MIN = '1';
constexpr relay_idx_t RELAY_IDX_MAX = '8';
typedef map<relay_idx_t, LOGIC> MODULE;
typedef map<string, MODULE> MODULE_SET;

// map to hold sernums and channels
struct channels_t { string sn = ""; int channels = 0; };
typedef vector<channels_t> MODULE_CHANNELS;

// vector to hold query lists for relays
struct queries_t { string sn = ""; string q = ""; };
typedef vector<queries_t> MODULE_QUERIES;

// errors
enum class ERROR_CODES : int { NONE = 0, SYNTAX = -1, NO_DEVICES = -2, BAD_SERNUM = -3, NO_DRIVER_INIT=-4, INVALID_CHANNEL=-5 };

// support function declarations
void PrintUsage(string strProgName);
string strip_path(string filename);
ERROR_CODES Relays_Enumerate();
ERROR_CODES Relays_Query(const MODULE_QUERIES& queries, const MODULE_CHANNELS& channels);
ERROR_CODES Relays_Set(const MODULE_SET& modules, const MODULE_CHANNELS& channels);
bool Relays_Get_Sernums(MODULE_CHANNELS& channels);
int Relays_Get_NumChannels(string sernum, const MODULE_CHANNELS& channels);
bool Is_Sernum_Present(string sernum, const MODULE_CHANNELS& channels);
LOGIC get_state(string status);
LOGIC get_state(char status);

// regex patterns for common usage
const regex regex_on_vals("^(?:ON|1|H|NO)$", regex::icase);
const regex regex_off_vals("^(?:OFF|0|L|NC)$", regex::icase);


/*******************************************************************************
* Function   : main()
* Arguments  : argc    = number of arguments including program name
*              argv[]  = arguments, argv[0] is program name
* Returns    : 0 for no errors, non-zero for errors
* Description:
*   Program entry point - parse the command line and call the relay functions
*/
int main(int argc, char* argv[])
{
    // regex patterns for parsing the command-line arguments
    const regex regex_help("^(?:/|-)?(?:H|Help|\\?)$", regex::icase);
    const regex regex_enumerate("^(?:ENUM|ENUMerate|L|List)$", regex::icase);
    const regex regex_set("^SET$", regex::icase);
    const regex regex_query("^(?:Q|Query)$", regex::icase);

    // regex patterns for parsing SET command
    const regex regex_sernum_pattern("^([a-z0-9]{5})[=:]([0L1HX_.]{1,8})$", regex::icase);
    const regex regex_pattern("^([0L1HX_.]{1,8})$", regex::icase);
    const regex regex_ch_set("^([1-8])[=:](ON|1|H|NO|OFF|0|L|NC)$", regex::icase);
    const regex regex_all("^ALL[=:](ON|1|H|NO|OFF|0|L|NC)$", regex::icase);

    // regex patterns for parsing QUERY command
    const regex regex_sernum("^([a-z0-9]{5})$", regex::icase);
    const regex regex_chlist("^([1-8]{1,8})$", regex::icase);
    const regex regex_query_chlist("^([a-z0-9]{5})[=:]([1-8]{1,8})$", regex::icase);

    ERROR_CODES error = ERROR_CODES::NONE;
    string error_sernum = "";
    const int num_args = argc - 1;

    // command-line parsing flags and variables
    bool is_help = false;
    bool is_enumerate = false;
    bool is_query = false;
    bool is_set = false;
    MODULE_SET module;
    MODULE_QUERIES queries;
    MODULE_CHANNELS channels;

    if (num_args > 0)
    {   // determine which command, and then process it
        string cmd = argv[1];

        if (regex_match(cmd, regex_help))
        {   // HELP
            if (num_args == 1)
                is_help = true;
            else
                error = ERROR_CODES::SYNTAX;
        }
        else if (regex_match(cmd, regex_enumerate))
        {   // ENUMERATE
            if (num_args == 1)
                is_enumerate = true;
            else
                error = ERROR_CODES::SYNTAX;
        }
        else
        {
            if (Relays_Get_Sernums(channels))
            {
                if (regex_match(cmd, regex_set) && num_args>1)
                {   // process SET parameters
                    //   SET pattern
                    //   SET ALL=state
                    //   SET ch:state ...
                    //   SET sernum:pattern sernum:pattern ...
                    //   SET sernum ch:state ... sernum ch:state ...
                    string cur_sn = "";
                    smatch smMatch;

                    for (auto i = 2; (error == ERROR_CODES::NONE && i <= num_args); ++i)
                    {
                        string arg = argv[i];

                        if (regex_match(arg, smMatch, regex_all))
                        {   // ALL=pattern must be 2nd and only parameter beyond SET
                            if (i == 2 && num_args == 2)
                            {   // cur_sn should still be "" denoting first enumerated module
                                module[cur_sn] = MODULE{};
                                module[cur_sn][RELAY_IDX_ALL] = get_state(smMatch[1]);
                            }
                            else
                            {
                                error = ERROR_CODES::SYNTAX;
                            }
                        }
                        else if (regex_match(arg, smMatch, regex_pattern))
                        {   // pattern alone must be 2nd and only pattern beyond set
                            if (i == 2 && num_args == 2)
                            {   // cur_sn should still be "" denoting first enumerated module
                                int num_channels = Relays_Get_NumChannels(cur_sn, channels);
                                string pattern = smMatch[1];
                                if (pattern.length() <= num_channels)
                                {
                                    module[cur_sn] = MODULE{};
                                    for (auto j = 0; j < pattern.length(); ++j)
                                        module[cur_sn]['1' + j] = get_state(pattern[j]);
                                }
                                else
                                {
                                    error = ERROR_CODES::INVALID_CHANNEL;
                                }
                            }
                            else
                            {
                                error = ERROR_CODES::SYNTAX;
                            }
                        }
                        else if (regex_match(arg, smMatch, regex_sernum))
                        {   // update to the newly specified serial number
                            cur_sn = smMatch[1];
                            std::transform(cur_sn.begin(), cur_sn.end(), cur_sn.begin(), ::toupper);
                            if (!Is_Sernum_Present(cur_sn, channels))
                            {
                                error = ERROR_CODES::BAD_SERNUM;
                                error_sernum = cur_sn;
                            }
                        }
                        else if (regex_match(arg, smMatch, regex_sernum_pattern))
                        {   // update to the newly specified serial number, then use the pattern
                            cur_sn = smMatch[1];
                            std::transform(cur_sn.begin(), cur_sn.end(), cur_sn.begin(), ::toupper);

                            if (Is_Sernum_Present(cur_sn, channels))
                            {
                                int num_channels = Relays_Get_NumChannels(cur_sn, channels);
                                string pattern = smMatch[2];
                                if (pattern.length() <= num_channels)
                                {
                                    module[cur_sn] = MODULE{};
                                    for (auto j = 0; j < pattern.length(); ++j)
                                        module[cur_sn]['1' + j] = get_state(pattern[j]);
                                }
                                else
                                {
                                    error = ERROR_CODES::INVALID_CHANNEL;
                                }
                            }
                            else
                            {
                                error = ERROR_CODES::BAD_SERNUM;
                                error_sernum = cur_sn;
                            }
                        }
                        else if (regex_match(arg, smMatch, regex_ch_set))
                        {   int num_channels = Relays_Get_NumChannels(cur_sn, channels);

                            if (!module.contains(cur_sn))
                                module[cur_sn] = MODULE{};
                            string ch = smMatch[1];
                            int nch = ch[0] - '0';
                            if (nch <= num_channels)
                            {
                                string p = smMatch[2];
                                module[cur_sn][ch[0]] = get_state(p);
                            }
                            else
                            {
                                error = ERROR_CODES::INVALID_CHANNEL;
                            }
                        }
                        else
                        {   // something illegal here
                            error = ERROR_CODES::SYNTAX;
                        }
                    }

                    if (error == ERROR_CODES::NONE)
                        is_set = true;
                }
                else if (regex_match(cmd, regex_query))
                {   // process QUERY parameters
                    //   QUERY
                    //   QUERY chlist
                    //   QUERY sernum sernum sernum
                    //   QUERY sernum:chlist sernum:chlist ...

                    smatch smMatch;

                    for (auto i = 2; (error == ERROR_CODES::NONE && i <= num_args); ++i)
                    {
                        string arg = argv[i];

                        if (regex_match(arg, smMatch, regex_chlist))
                        {   // must be first and only argument if present
                            if (i == 2 && num_args == 2)
                            {   int num_channels = Relays_Get_NumChannels("", channels);
                                queries_t q;
                                q.sn = "";
                                q.q = smMatch[1];

                                if (q.q.length() <= num_channels)
                                    queries.push_back(q);
                                else
                                    error = ERROR_CODES::INVALID_CHANNEL;
                            }
                            else
                            {
                                error = ERROR_CODES::SYNTAX;
                            }
                        }
                        else if (regex_match(arg, smMatch, regex_query_chlist))
                        {
                            queries_t q;
                            q.sn = smMatch[1];
                            std::transform(q.sn.begin(), q.sn.end(), q.sn.begin(), ::toupper);
                            int num_channels = Relays_Get_NumChannels(q.sn, channels);

                            if (Is_Sernum_Present(q.sn, channels))
                            {
                                q.q = smMatch[2];

                                if (q.q.length() <= num_channels)
                                    queries.push_back(q);
                                else
                                    error = ERROR_CODES::INVALID_CHANNEL;
                            }
                            else
                            {
                                error = ERROR_CODES::BAD_SERNUM;
                                error_sernum = q.sn;
                            }
                        }
                        else if (regex_match(arg, smMatch, regex_sernum))
                        {
                            queries_t q;
                            q.sn = smMatch[1];
                            std::transform(q.sn.begin(), q.sn.end(), q.sn.begin(), ::toupper);
                            int num_channels = Relays_Get_NumChannels(q.sn, channels);

                            if (Is_Sernum_Present(q.sn, channels))
                            {   // all channels
                                q.q = "";
                                queries.push_back(q);
                            }
                            else
                            {
                                error = ERROR_CODES::BAD_SERNUM;
                                error_sernum = q.sn;
                            }
                        }
                        else
                        {   // something illegal here
                            error = ERROR_CODES::SYNTAX;
                        }
                    }

                    if (error == ERROR_CODES::NONE)
                        is_query = true;
                }
                else
                {   // something we don't recognize - syntax error
                    error = ERROR_CODES::SYNTAX;
                }
            }
            else
            {   // DLL call returned no devices or otherwise failed
                error = ERROR_CODES::NO_DEVICES;
            }
        }
    }
    else
    {   // no arguments - show usage
        is_help = true;
    }

    if (error == ERROR_CODES::NONE)
    {
        if (is_help)
        {
            PrintUsage(strip_path(argv[0]));
        }
        else if (is_enumerate)
        {
            error = Relays_Enumerate();
        }
        else if (is_set)
        {
            error = Relays_Set(module, channels);
        }
        else if (is_query)
        {
            error = Relays_Query(queries, channels);
        }
    }

    switch (error)
    {
    case ERROR_CODES::SYNTAX:
        std::cerr << "Syntax error";
        break;
    case ERROR_CODES::NO_DEVICES:
        std::cerr << "No devices found";
        break;
    case ERROR_CODES::BAD_SERNUM:
        std::cerr << "Serial number " << error_sernum << " not found";
        break;
    case ERROR_CODES::NO_DRIVER_INIT:
        std::cerr << "Driver did not initialize";
        break;
    case ERROR_CODES::INVALID_CHANNEL:
        std::cerr << "Invalid channel specified";
        break;
    }

    return int(error);
}


/*******************************************************************************
* Function   : PrintUsage
* Arguments  : strProgName  = name of the program (*.exe)
* Returns    : none
* Description:
*   This function simply prints the command-line usage to std::cout
*/
void PrintUsage(std::string strProgName)
{
    std::cout << "USB HID Relay utility v" << APP_VERSION << "\n";
    std::cout << "Kerry S. Martin, martin@wild-wood.net\n";
    std::cout << "Usage:\n";
    std::cout << "  " << strProgName << " ENUMerate|list                              # list all devices by sn(#channels)\n";
    std::cout << "  " << strProgName << " Query                                       # query all channels for first device\n";
    std::cout << "  " << strProgName << " Query chlist                                # query given channels\n";
    std::cout << "  " << strProgName << " Query sernum {sernum ...}                   # query all channels for specific SNs\n";
    std::cout << "  " << strProgName << " Query sernum:chlist {sernum:chlist ...}     # query given channels for specifc SNs\n";
    std::cout << "  " << strProgName << " SET pattern                                 # set given pattern on first device\n";
    std::cout << "  " << strProgName << " SET ALL=state                               # set all channels of first device\n";
    std::cout << "  " << strProgName << " SET ch:state {ch:state ...}                 # set given channels on first device\n";
    std::cout << "  " << strProgName << " SET sernum:pattern {sernum:pattern ...}     # set given patterns on specific SNs\n";
    std::cout << "  " << strProgName << " SET sernum ch:state {ch:state ...}          # set given channels on specific SNs\n\n";
    std::cout << "    sernum = 5-character serial number\n";
    std::cout << "    state = 0|1|OFF|ON|L|H|NO|NC\n";
    std::cout << "    pattern = qq...    where q = 0|1|L|H|X\n";
}


/*******************************************************************************
* Function   : strip_path()
* Arguments  : filename = full filename to the file
* Returns    : just the base filename, without the path
* Description:
*   This function strips the pathname from a filename.
*   ex/ C:\Projects\MyProject\Release\MyProject.exe => MyProject.exe
*/
string strip_path(string filename)
{
    const regex re("^(?:.*)(?:\\/|\\\\)(.+?)$");
    smatch smMatch;

    if (regex_match(filename, smMatch, re))
        return smMatch[1];
    else
        return filename;
}


/*******************************************************************************
* Function   : Relays_Set
* Arguments  : modules   = structure of modules/channels to set
*              channels  = structure of enumerated channels
* Returns    : ERROR_CODES::NONE for success, otherwise failure
* Description:
*   This function sets individual relays in the modules to open or closed
*/
ERROR_CODES Relays_Set(const MODULE_SET& modules, const MODULE_CHANNELS& channels)
{
    ERROR_CODES return_value = ERROR_CODES::NONE;

    if (usb_relay_init() == 0)
    {
        for (auto const& [sernum, module] : modules)
        {
            char szRelaySN[6] = "";

            if (sernum.empty())
            {
                channels_t s = channels[0];
                string sn = s.sn;
                for (int i = 0; i < 5; ++i)
                    szRelaySN[i] = toupper(sn[i]);
                szRelaySN[5] = '\0';
            }
            else
            {
                // sernum is exactly 5 characters
                for (int i = 0; i < 5; ++i)
                    szRelaySN[i] = toupper(sernum[i]);
                szRelaySN[5] = '\0';
            }

            intptr_t hHandle = usb_relay_device_open_with_serial_number(szRelaySN, (unsigned int)strlen(szRelaySN));

            if (hHandle)
            {
                for (auto const& [ch, st] : module)
                {
                    if (ch == RELAY_IDX_ALL)
                    {
                        switch (st)
                        {
                        case LOGIC::H:
                            usb_relay_device_open_all_relay_channel(hHandle);
                            break;
                        case LOGIC::L:
                            usb_relay_device_close_all_relay_channel(hHandle);
                            break;
                        }
                    }
                    else
                    {
                        switch (st)
                        {
                        case LOGIC::H:
                            usb_relay_device_open_one_relay_channel(hHandle, ch - RELAY_IDX_MIN + 1);
                            break;
                        case LOGIC::L:
                            usb_relay_device_close_one_relay_channel(hHandle, ch - RELAY_IDX_MIN + 1);
                            break;
                        }
                    }
                }

                usb_relay_device_close(hHandle);
            }
        }

        usb_relay_exit();
    }
    else
    {
        return_value = ERROR_CODES::NO_DRIVER_INIT;
    }

    return return_value;
}


/*******************************************************************************
* Function   : Relays_Enumerate
* Arguments  : none
* Returns    : ERROR_CODES::NONE for success, otherwise failure
* Description:
*   This function enumerates the relays to stdout
*/
ERROR_CODES Relays_Enumerate()
{
    ERROR_CODES error = ERROR_CODES::NONE;

    if (usb_relay_init() == 0)
    {
        pusb_relay_device_info_t pdevice = usb_relay_device_enumerate();

        if (pdevice)
        {
            while (pdevice)
            {
                std::cout << pdevice->serial_number << "(";
                switch (pdevice->type)
                {
                case USB_RELAY_DEVICE_ONE_CHANNEL:
                    std::cout << "1)";
                    break;
                case USB_RELAY_DEVICE_TWO_CHANNEL:
                    std::cout << "2)";
                    break;
                case USB_RELAY_DEVICE_FOUR_CHANNEL:
                    std::cout << "4)";
                    break;
                case USB_RELAY_DEVICE_EIGHT_CHANNEL:
                    std::cout << "8)";
                    break;
                default:
                    std::cout << "?)";
                    break;
                }

                if (pdevice->next)
                    std::cout << ",";

                pdevice = pdevice->next;
            }

            usb_relay_device_free_enumerate(pdevice);
        }
        else
        {
            error = ERROR_CODES::NO_DEVICES;
        }

        usb_relay_exit();
    }
    else
    {
        error = ERROR_CODES::NO_DRIVER_INIT;
    }

    return error;
}



/*******************************************************************************
* Function   : Relays_Query
* Arguments  : queries   = structure of modules/channels to query
*              channels  = structure of enumerated channels
* Returns    : ERROR_CODES::NONE for success, otherwise failure
* Description:
*   This function queries individual relays in the modules for open/closed state
*/
ERROR_CODES Relays_Query(const MODULE_QUERIES& queries, const MODULE_CHANNELS& channels)
{
    ERROR_CODES error = ERROR_CODES::NONE;

    if (usb_relay_init() == 0)
    {   
        MODULE_QUERIES more_queries = queries;  // may need to modify this if empty, so modify a copy

        if (more_queries.empty())
        {   // query all relays for the first enumerated device
            queries_t q;
            q.sn = "";
            q.q = "";
            more_queries.push_back(q);
        }

        for (queries_t Q : more_queries)
        {
            string sernum = Q.sn;
            string q = Q.q;
            int num_channels = 0;

            char szRelaySN[6] = "";

            if (sernum.empty())
            {   // use first enumerated module
                channels_t s = channels[0];
                string sn = s.sn;
                for (int i = 0; i < 5; ++i)
                    szRelaySN[i] = toupper(sn[i]);
                szRelaySN[5] = '\0';

                num_channels = s.channels;
            }
            else
            {
                // sernum is exactly 5 characters
                for (int i = 0; i < 5; ++i)
                    szRelaySN[i] = toupper(sernum[i]);
                szRelaySN[5] = '\0';

                // find the sernum in channels to get the # of channels and verify that
                // the sernum exists
                for (channels_t s : channels)
                {
                    if (s.sn == szRelaySN)
                    {
                        num_channels = s.channels;
                        break;
                    }
                }

                if (num_channels < 1)
                {
                    error = ERROR_CODES::BAD_SERNUM;
                }
            }

            if (num_channels > 0)
            {

                intptr_t hHandle = usb_relay_device_open_with_serial_number(szRelaySN, (unsigned int)strlen(szRelaySN));

                if (hHandle)
                {
                    unsigned int status;

                    usb_relay_device_get_status(hHandle, &status);

                    if (q.empty())
                    {   // query all channels if empty
                        for (auto i = 1; i <= num_channels; ++i)
                            q.append(1, '0' + i);
                    }

                    for (char c : q)
                    {
                        if (c >= '1' && c <= '8')
                        {
                            int ch = int(c - '0');

                            if (ch >= 1 && ch <= num_channels)
                            {
                                bool st = (status & (0x0001) << (ch - 1)) ? true : false;

                                if (st)
                                    std::cout << "1";
                                else
                                    std::cout << "0";
                            }
                        }
                    }

                    usb_relay_device_close(hHandle);
                }

                std::cout << " ";
            }
        }

        usb_relay_exit();
    }
    else
    {
        error = ERROR_CODES::NO_DRIVER_INIT;
    }

    return error;
}


/*******************************************************************************
* Function   : Relays_Get_Sernums
* Arguments  : channels  = enumerate all sernums into this structure
* Returns    : true = success, false = failure
* Description:
*   Enumerates all of the relay modules to determine sernums and # of channels
*/
bool Relays_Get_Sernums(MODULE_CHANNELS& channels)
{
    channels = MODULE_CHANNELS{};
    bool bResult = false;

    usb_relay_init();
    pusb_relay_device_info_t pdevice = usb_relay_device_enumerate();

    if (pdevice)
    {
        while (pdevice)
        {
            channels_t ch;
            ch.sn = pdevice->serial_number;
            ch.channels = int(pdevice->type);
            channels.push_back(ch);
            pdevice = pdevice->next;
        }

        usb_relay_device_free_enumerate(pdevice);
        bResult = true;
    }

    usb_relay_exit();

    return bResult;
}


/*******************************************************************************
* Function   : get_state
* Arguments  : status   = string containing the status (0|1|l|H|OFF|ON|X, etc)
* Returns    : LOGIC::H, LOGIC::L, or LOGIC::X
* Description:
*   This function determines the logic state of the string 'status'
*/
LOGIC get_state(string status)
{
    if (regex_match(status, regex_on_vals))
        return LOGIC::H;
    else if (regex_match(status, regex_off_vals))
        return LOGIC::L;
    else
        return LOGIC::X;
}


/*******************************************************************************
* Function   : get_state
* Arguments  : status   = char containing the status (0|1|l|H|X, etc)
* Returns    : LOGIC::H, LOGIC::L, or LOGIC::X
* Description:
*   This function determines the logic state of the char in 'status'
*/
LOGIC get_state(char status)
{
    string p = "";
    p.append(1, status);

    if (regex_match(p, regex_on_vals))
        return LOGIC::H;
    else if (regex_match(p, regex_off_vals))
        return LOGIC::L;
    else
        return LOGIC::X;
}


/*******************************************************************************
* Function   : Is_Sernum_Present
* Arguments  : sernum   = sernum to look for
*              channels = structure of enumerated channels
* Returns    : true if sernum is present, false otherwise
* Description:
*   This function searches for the given sernum in the enumerated list of channels
*/
bool Is_Sernum_Present(string sernum, const MODULE_CHANNELS& channels)
{
    bool bResult = false;

    if (sernum.empty())
    {   // first channel if sernum is blank
        if (!channels.empty())
            bResult = true;
    }
    else
    {   // search for the matching sernum channel
        for (channels_t s : channels)
        {
            if (s.sn == sernum)
            {
                bResult = true;
                break;
            }
        }
    }

    return bResult;
}


/*******************************************************************************
* Function   : Relays_Get_NumChannels
* Arguments  : sernum   = sernum to look for
*              channels = structure of enumerated channels
* Returns    : number of channels for the given sernum module, or 0 if not found
* Description:
*   This function searches for the given sernum in the enumerated list of channels
*   and determines how many channels are present
*/
int Relays_Get_NumChannels(string sernum, const MODULE_CHANNELS& channels)
{
    int nChannels = 0;

    if (sernum.empty())
    {   // first channel if sernum is blank
        if (!channels.empty())
            nChannels = channels[0].channels;
    }
    else
    {   // search for the matching sernum channel
        for (channels_t s : channels)
        {
            if (s.sn == sernum)
            {
                nChannels = s.channels;
                break;
            }
        }
    }

    return nChannels;
}


/*******************************************************************************
* Copyright © 2022 Kerry S. Martin, martin@wild-wood.net
* Free for usage without warranty, expressed or implied; attribution required
*******************************************************************************/
