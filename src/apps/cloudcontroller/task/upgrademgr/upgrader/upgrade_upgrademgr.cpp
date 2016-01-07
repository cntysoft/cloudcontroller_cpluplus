#include <QDebug>

#include "upgrade_upgrademgr.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace upgrader{

using sn::corelib::network::ApiInvoker;

UpgradeUpgrademgr::UpgradeUpgrademgr(AbstractTaskContainer *taskContainer, const TaskMeta &meta)
   :AbstractNetTask(taskContainer, meta)
{
}

void UpgradeUpgrademgr::run()
{
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   ApiInvokeRequest request("Upgrader/UpgradeUpgrademgrMaster", "upgrade");
   apiInvoker->request(request);
   m_eventLoop.exec();
}

}//upgrader
}//upgrademgr
}//task
}//cloudcontroller