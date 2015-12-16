#include "global_shell_command.h"
#include "shell/task_loop.h"

namespace cloudcontroller{
namespace command{

GlobalShellCommand::GlobalShellCommand(CommandRunner &runner, const CommandMeta &invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{   
}

void GlobalShellCommand::exec()
{
   m_taskLoop.run();
   exit(EXIT_SUCCESS);
}

GlobalShellCommand::~GlobalShellCommand()
{}


}//commmand
}//cloudcontroller