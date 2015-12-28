#include <QSharedPointer>

#include "abstract_task.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{

AbstractTask::AbstractTask(AbstractTaskContainer* taskContainer, const TaskMeta& meta)
   :BaseAbstractTask(taskContainer, meta)
{
   
}

QSharedPointer<ApiInvoker>& AbstractTask::getApiInvoker()
{
   UpgradeMgrContainer* container= dynamic_cast<UpgradeMgrContainer*>(getTaskContainer());
   return container->getApiInvoker();
}

}//upgrademgr
}//task
}//cloudcontroller