#include "main.hpp"
#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include <stdlib.h>
#include <string>

static ModInfo modInfo;

#define INFO(value...) getLogger().info(value)
#define ERROR(value...) getLogger().error(value)


Logger &getLogger()
{
    static Logger *logger = new Logger(modInfo);
    return *logger;
}

std::string toLower(std::string string) {
    for (int i = 0; i < string.length(); i++)
      {
          string[i] = tolower(string[i]);
      }
    return string;
}

bool modWork = false;

//Thanks Waulta and Fchb
MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(GameManager_OnPrivateOKButtonPressed, "", "GameManager", "OnPrivateOKButtonPressed", void, Il2CppObject* self)
    GameManager_OnPrivateOKButtonPressed(self);
    Il2CppString* privName = CRASH_UNLESS(il2cpp_utils::GetFieldValue<Il2CppString*>(self, "privateServerName"));
    std::string roomName = toLower(to_utf8(csstrtostr(privName)));
    std::string allowedString = "mod";
    if (roomName.find(allowedString) != std::string::npos || roomName.find(allowedString) != std::string::npos) {
        modWork = true;
    }
    if (roomName.find(allowedString) == std::string::npos && roomName.find(allowedString) == std::string::npos) {
        modWork = false;
    }
}

MAKE_HOOK_FIND_CLASS_UNSAFE_INSTANCE(PlayerController_Update, "HexabodyVR.PlayerController", "HexaBodyPlayer3", "Update", void, Il2CppObject* self)
{
    PlayerController_Update(self);
    if(modWork){
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "WalkSpeed", 200.0f));
    }else{
        CRASH_UNLESS(il2cpp_utils::SetFieldValue(self, "WalkSpeed", 75.0f));
    }
}

extern "C" void setup(ModInfo &info)
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
}

extern "C" void load()
{
    il2cpp_functions::Init();
    Logger& logger = getLogger();
    logger.info("Installing hooks...");
    
    INSTALL_HOOK(getLogger(), PlayerController_Update);
    INSTALL_HOOK(getLogger(), GameManager_OnPrivateOKButtonPressed);

    logger.info("Installed all hooks!");
}
