#ifndef CLOUD_CONTROLLER_APPLICATION_H
#define CLOUD_CONTROLLER_APPLICATION_H

#include "kernel/application.h"

namespace cloudcontroller 
{
using BaseApplication = sn::corelib::Application;
using Settings = sn::corelib::Settings;
class Application : public BaseApplication
{
   Q_OBJECT
public:
   Application(int &argc, char **argv);
public:
   void watchImportantSignal();
public:
   virtual ~Application();
   virtual Settings::CfgInitializerFnType getDefaultCfgInitializerFn();
signals:
   void beginTaskWorker();
};

}//cloudcontroller

#endif // CLOUD_CONTROLLER_APPLICATION_H
