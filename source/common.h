#pragma once

#include <coreinit/mcp.h>
#include <coreinit/thread.h>
#include <coreinit/time.h>
#include <nn/act.h>
#include <whb/log.h>
#include <whb/log_console.h>

#include <string>
#include <string.h>
#include <malloc.h>
#include <sstream>
#include <vector>
#include <array>
#include <map>
#include <list>
#include <algorithm>
#include <iterator>
#include <functional>
#include <fstream>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>

// Enums and Structs

enum class dumpLocation : uint8_t {
    Unknown,
    SDFat,
    USBFat,
    USBExFAT, // TODO: Add exFAT support
    USBNTFS, // TODO: Add NTFS support
};

enum class titleLocation : uint8_t {
    Unknown,
    Nand,
    USB,
    Disc,
};

enum class dumpTypeFlags : uint8_t {
    GAME = 1 << 0,
    UPDATE = 1 << 1,
    DLC = 1 << 2,
    SYSTEM_APP = 1 << 3,
    SAVE = 1 << 4,
    COMMONSAVE = 1 << 5,
    CUSTOM = 1 << 6,
};

struct dumpingConfig {
    dumpTypeFlags filterTypes;
    dumpTypeFlags dumpTypes;
    nn::act::PersistentId accountID = 0;
    bool queue = false;
    dumpLocation location = dumpLocation::SDFat;
};

inline dumpTypeFlags operator|(dumpTypeFlags a, dumpTypeFlags b) {
    return static_cast<dumpTypeFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline dumpTypeFlags operator&(dumpTypeFlags a, dumpTypeFlags b) {
    return static_cast<dumpTypeFlags>(static_cast<int>(a) & static_cast<int>(b));
}

struct userAccount {
    bool currAccount = false;
    bool defaultAccount = false;
    bool networkAccount;
    bool passwordCached;
    std::string miiName;
    nn::act::SlotNo slot;
    nn::act::PersistentId persistentId;
};

struct titlePart {
    std::string path;
    std::string outputPath;
    uint16_t version;
    MCPAppType type;
    uint32_t partHighID;
    titleLocation location;
};

struct titleSave {
    userAccount* account;
    std::string path;
};

struct titleSaveCommon {
    std::string path;
    titleLocation location;
};

struct titleEntry {
    uint32_t titleLowID;
    std::string shortTitle = "";
    std::string productCode = "";
    std::string normalizedTitle = "";
    bool hasBase = false;
    titlePart base;
    bool hasUpdate = false;
    titlePart update;
    bool hasDLC = false;
    titlePart dlc;
    std::vector<titleSave> saves;
    titleSaveCommon commonSave;
};