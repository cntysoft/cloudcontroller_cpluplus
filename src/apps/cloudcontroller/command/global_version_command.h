#ifndef CLOUD_CONTROLLER_COMMAND_GLOBAL_VERSION_COMMAND
#define CLOUD_CONTROLLER_COMMAND_GLOBAL_VERSION_COMMAND

#include "command/abstract_command.h"
#include "command_runner.h"

namespace cloudcontroller {
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::CommandMeta;

using cloudcontroller::CommandRunner;

class GlobalVersionCommand : public AbstractCommand 
{
public:
   GlobalVersionCommand(CommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
   virtual ~GlobalVersionCommand();
};

}//command
}//cloudcontroller

#endif // CLOUD_CONTROLLER_COMMAND_GLOBAL_VERSION_COMMAND

