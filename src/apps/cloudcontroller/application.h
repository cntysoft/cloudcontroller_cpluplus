#ifndef CLOUD_CONTROLLER_APPLICATION_H
#define CLOUD_CONTROLLER_APPLICATION_H

#include "kernel/application.h"

namespace cloudcontroller 
{
using BaseApplication = sn::corelib::Application;
using Settings = sn::corelib::Settings;
class Application : public BaseApplication
{
public:
   Application(int &argc, char **argv);
public:
   virtual ~Application();
   virtual Settings::CfgInitializerFnType getDefaultCfgInitializerFn();
};

}//cloudcontroller

#endif // CLOUD_CONTROLLER_APPLICATION_H
