#include "global.h"
#include "io/terminal.h"

namespace cloudcontroller{
namespace container{

using sn::corelib::TerminalColor;

Global::Global(shell::TaskLoop &loop)
   : AbstractTaskContainer("Global", loop)
{
   m_containerPs = "cloudcontroller >> ";
   initUsage();
}

void Global::initUsage()
{
   addUsageText("you can use commands as fowllow:\n\n", TerminalColor::LightGreen);
   addUsageText("version    show version info\n");
   addUsageText("help       get online usage doc\n");
   addUsageText("quit       exit application\n");
   addUsageText("metaserver connect --host=\n");
}

}//container
}//cloudcontroller