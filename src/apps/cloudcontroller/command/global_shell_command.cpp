#include <signal.h>

#include "global_shell_command.h"
#include "shell/task_loop.h"
#include "io/terminal.h"

namespace cloudcontroller{
namespace command{

using sn::corelib::Terminal;

static TaskLoop* taskloop_pointer(TaskLoop* pointer = nullptr)
{
   static TaskLoop* storedPointer = nullptr;
   if(nullptr != pointer){
      storedPointer = pointer;
   }
   return storedPointer;
}

GlobalShellCommand::GlobalShellCommand(CommandRunner &runner, const CommandMeta &invokeMeta)
   : AbstractCommand(runner, invokeMeta)
{
   taskloop_pointer(&m_taskLoop);
   struct sigaction winResizeAction;
   sigemptyset(&winResizeAction.sa_mask);
   winResizeAction.sa_flags = 0;
   winResizeAction.sa_handler = [](int)->void{
      QPair<int, int> winSize = Terminal::getWindowSize();
      TaskLoop* looper = taskloop_pointer();
      if(nullptr != looper){
         looper->updateTerminalWindowSize(winSize.first, winSize.second);
      }
   };
   sigaction(SIGWINCH, &winResizeAction, nullptr);
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