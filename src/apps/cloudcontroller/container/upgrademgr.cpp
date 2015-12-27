#include <QString>

#include "corelib/io/terminal.h"

#include "upgrademgr.h"
#include "cclib/shell/task_meta.h"
#include "cclib/shell/abstract_task.h"

namespace cloudcontroller{
namespace container{

using sn::corelib::TerminalColor;
using cclib::shell::AbstractTask;
using cclib::shell::TaskMeta;

UpgradeMgr::UpgradeMgr(TaskLoop& loop)
   :AbstractTaskContainer("UpgradeMgr", loop)
{
   m_containerPs = "upgrademgr >> ";
   initUsage();
   initRouter();
}

void UpgradeMgr::runTask(const TaskMeta& meta)
{
   if(!dispatchBuildInTask(meta)){
      AbstractTaskContainer::runTask(meta);
   }
}

void UpgradeMgr::initUsage()
{
   addUsageText("you can use commands as fowllow:\n", TerminalColor::LightGreen);
   addUsageText("quit       exit upgrademgr task container\n");
}

void UpgradeMgr::initRouter()
{
   addTaskRoute("quit", "quit", 1, {
                   {"category", "UpgradeMgr"},
                   {"name", "Quit"}
                });
}

bool UpgradeMgr::dispatchBuildInTask(const TaskMeta& meta)
{
   QString key(meta.getCategory()+'_'+meta.getName());
   if(key == "UpgradeMgr_Quit"){
      quitTask(meta);
      return true;
   }
   return false;
}

UpgradeMgr& UpgradeMgr::setApiInvoker(ApiInvoker *apiInvoker)
{
   if(nullptr != apiInvoker){
      m_apiInvoker.reset(apiInvoker);
   }
   return *this;
}

QSharedPointer<ApiInvoker>& UpgradeMgr::getApiInvoker()
{
   return m_apiInvoker;
}

void UpgradeMgr::quitTask(const TaskMeta&)
{
   m_taskLoop.enterGlobalTaskContainer();
}

}//container
}//cloudcontroller