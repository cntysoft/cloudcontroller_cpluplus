#ifndef CLOUD_CONTROLLER_COMMAND_GLOBAL_SHELL_COMMAND_H
#define CLOUD_CONTROLLER_COMMAND_GLOBAL_SHELL_COMMAND_H

#include "command/abstract_command.h"
#include "command_runner.h"
#include "shell/task_loop.h"

namespace cloudcontroller{
namespace command{

using sn::corelib::AbstractCommand;
using sn::corelib::CommandMeta;
using cloudcontroller::CommandRunner;
using cloudcontroller::shell::TaskLoop;

class GlobalShellCommand : public AbstractCommand 
{
public:
   GlobalShellCommand(CommandRunner& runner, const CommandMeta& invokeMeta);
public:
   virtual void exec();
   virtual ~GlobalShellCommand();
protected:
   TaskLoop m_taskLoop;
};

}//command
}//cloudcontroller

#endif // CLOUD_CONTROLLER_COMMAND_GLOBAL_SHELL_COMMAND_H
