#ifndef CLOUD_CONTROLLER_COMMAND_RUNNER_H
#define CLOUD_CONTROLLER_COMMAND_RUNNER_H

#include "command/abstract_command_runner.h"

namespace cloudcontroller 
{

using BaseCommandRunner = sn::corelib::AbstractCommandRunner;

class Application;

class CommandRunner : public BaseCommandRunner
{
public:
   CommandRunner(Application &app);
public:
   virtual void run();
private:
   void initCommandPool();
   void initRouteItems();
};

}//cloudcontroller

#endif // CLOUD_CONTROLLER_COMMAND_RUNNER_H
