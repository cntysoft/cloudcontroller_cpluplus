#include <QCommandLineParser>

#include "command_runner.h"
#include "application.h"
#include "kernel/errorinfo.h"
#include "io/terminal.h"
#include "command/abstract_command.h"
#include "command/command_meta.h"
#include "command/command_repo.h"

#include <QDebug>

namespace cloudcontroller
{
using sn::corelib::ErrorInfo;
using sn::corelib::TerminalColor;
using sn::corelib::CommandMeta;
using sn::corelib::AbstractCommand;
using sn::corelib::AbstractCommandRunner;

using cloudcontroller::command::GlobalVersionCommand;


CommandRunner::CommandRunner(Application &app)
   : BaseCommandRunner(app)
{
   addUsageText("welcome to use sheneninfo metaserver system\n\n", TerminalColor::Green);
   addUsageText("usage: \n\n", TerminalColor::LightBlue);
   addUsageText("--version  print main system version number\n");
   addUsageText("--help     print help document\n\n");
   initCommandPool();
   initRouteItems();
}

void CommandRunner::initCommandPool()
{
   m_cmdRegisterPool.insert("Global_Version", [](const AbstractCommandRunner& runner, const CommandMeta& meta)->AbstractCommand*{
                                                  GlobalVersionCommand* cmd = new GlobalVersionCommand(runner, meta);
                                                  return cmd;
                                               });
}


void CommandRunner::initRouteItems()
{
   addCmdRoute("version", "--version", 1, {
      {"category", "Global"},
      {"name", "Version"}
   });
}


}//metaserver