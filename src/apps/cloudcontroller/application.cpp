#include <csignal>
#include <QDebug>

#include "application.h"

#include "corelib/kernel/errorinfo.h"

namespace cloudcontroller{

using sn::corelib::ErrorInfo;

extern void init_defualt_cfg(Settings &settings);

Application::Application(int &argc, char **argv)
   : BaseApplication(argc, argv)
{
   setApplicationName("cloudcontroller");
}

Settings::CfgInitializerFnType Application::getDefaultCfgInitializerFn()
{
   return init_defualt_cfg;
}

void Application::watchImportantSignal()
{
   struct sigaction sa;
   memset(&sa, 0, sizeof(sa));
   sa.sa_flags = SA_RESTART;
   sa.sa_handler = [](int sig)->void{
      Application::instance()->setCatchedSignalNumber(sig);
   };
   if(sigaction(SIGINT, &sa, 0) != 0){
      throw ErrorInfo(QString("sigaction failed errno : %1").arg(errno));
   }
}

Application::~Application()
{}

}//cloudcontroller