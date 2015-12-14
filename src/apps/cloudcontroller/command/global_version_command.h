#ifndef CLOUD_CONTROLLER_COMMAND_GLOBAL_VERSION_COMMAND
#define CLOUD_CONTROLLER_COMMAND_GLOBAL_VERSION_COMMAND

#include "command/abstract_command.h"

namespace cloudcontroller 
{
namespace command 
{

using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;
using sn::corelib::CommandMeta;

class GlobalVersionCommand : public AbstractCommand 
{
public:
   GlobalVersionCommand(const AbstractCommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
   virtual ~GlobalVersionCommand();
};

}//command
}//cloudcontroller

#endif // CLOUD_CONTROLLER_COMMAND_GLOBAL_VERSION_COMMAND

