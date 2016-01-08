#include <QDebug>
#include <QVariant>

#include "upgrade_upgrademgr.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace upgrader{

using sn::corelib::network::ApiInvoker;

void upgrade_upgrademgr_handler(const ApiInvokeResponse &response, void *args)
{
   UpgradeUpgrademgr *self = static_cast<UpgradeUpgrademgr*>(args);
   if(!response.getStatus()){
      self->processErrorResponse(response);
      self->getEventLoop().exit();
      return;
   }
   self->writeSubMsg(response.getDataItem("msg").toString()+"\n");
   bool lasted = response.getDataItem("lasted").toBool();
   if(lasted){
      self->getEventLoop().exit();
      return;
   }
   if(response.isFinal()){
      self->getEventLoop().exit();
      return;
   }
}

UpgradeUpgrademgr::UpgradeUpgrademgr(AbstractTaskContainer *taskContainer, const TaskMeta &meta)
   :AbstractNetTask(taskContainer, meta)
{
}

void UpgradeUpgrademgr::run()
{
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   ApiInvokeRequest request("Upgrader/UpgradeUpgrademgrMaster", "upgrade");
   QString version = m_invokeMeta.getTaskArg("version");
   request.appendArg(QVariant(version));
   apiInvoker->request(request, upgrade_upgrademgr_handler, static_cast<void*>(this));
   m_eventLoop.exec();
}

}//upgrader
}//upgrademgr
}//task
}//cloudcontroller