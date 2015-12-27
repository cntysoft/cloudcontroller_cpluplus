#include <QString>

#include "metaserver.h"
#include "io/terminal.h"
#include "cclib/shell/task_meta.h"
#include "cclib/shell/abstract_task.h"

namespace cloudcontroller{
namespace container{

using sn::corelib::TerminalColor;
using cclib::shell::AbstractTask;
using cclib::shell::TaskMeta;

MetaServer::MetaServer(TaskLoop& loop)
   :AbstractTaskContainer("MetaServer", loop)
{
   m_containerPs = "metaserver >> ";
   initUsage();
   initRouter();
}

void MetaServer::runTask(const TaskMeta& meta)
{
   if(!dispatchBuildInTask(meta)){
      AbstractTaskContainer::runTask(meta);
   }
}

void MetaServer::initUsage()
{
   addUsageText("you can use commands as fowllow:\n", TerminalColor::LightGreen);
   addUsageText("quit       exit metaserver task container\n");
}

void MetaServer::initRouter()
{
   addTaskRoute("quit", "quit", 1, {
                   {"category", "MetaServer"},
                   {"name", "Quit"}
                });
}

bool MetaServer::dispatchBuildInTask(const TaskMeta& meta)
{
   QString key(meta.getCategory()+'_'+meta.getName());
   if(key == "MetaServer_Quit"){
      quitTask(meta);
      return true;
   }
   return false;
}

void MetaServer::quitTask(const TaskMeta&)
{
   m_taskLoop.enterGlobalTaskContainer();
}

}//container
}//cloudcontroller