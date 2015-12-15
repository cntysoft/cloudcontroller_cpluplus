#include "global_version_command.h"
#include "io/terminal.h"

namespace cloudcontroller 
{
namespace command 
{

using sn::corelib::TerminalColor;

GlobalVersionCommand::GlobalVersionCommand(CommandRunner &runner, const CommandMeta &invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{   
}

void GlobalVersionCommand::exec()
{
   printConsoleMsg("cloudcontroller version ");
   printConsoleMsg(QString("%1\n").arg(CLOUD_CONTROLLER_VERSION).toLocal8Bit(), TerminalColor::Cyan);
   exit(EXIT_SUCCESS);
}

GlobalVersionCommand::~GlobalVersionCommand()
{}

}//command
}//cloudcontroller
