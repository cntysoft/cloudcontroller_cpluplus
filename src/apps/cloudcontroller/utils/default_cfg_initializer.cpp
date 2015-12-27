#include <QDir>

#include "const.h"
#include "kernel/settings.h"

namespace cloudcontroller{

using sn::corelib::Settings;

static void init_global_cfg(Settings &settings);

void init_defualt_cfg(Settings& settings)
{
   init_global_cfg(settings);
}

static void init_global_cfg(Settings& settings)
{
   QString runtimeDir = QDir::tempPath()+QDir::separator()+"cloudcontroller";
   settings.setValue("runtimeDir", runtimeDir, CC_CFG_GROUP_GLOBAL);
   settings.setValue("upgrademgrHost", "127.0.0.1", CC_CFG_GROUP_UPGRADEMGR);
   settings.setValue("upgrademgrPort", 7777, CC_CFG_GROUP_UPGRADEMGR);
}

}//cloudcontroller