#ifndef CLOUD_CONTROLLER_TASK_UPGRADEMGR_ABSTRACT_TASK_H
#define CLOUD_CONTROLLER_TASK_UPGRADEMGR_ABSTRACT_TASK_H

#include <QSharedPointer>

#include "cclib/shell/abstract_task.h"
#include "cclib/shell/abstract_task_container.h"
#include "cclib/shell/task_meta.h"

#include <corelib/network/rpc/api_invoker.h>

namespace cloudcontroller{
namespace task{
namespace upgrademgr{

using BaseAbstractTask = cclib::shell::AbstractTask;
using cclib::shell::AbstractTaskContainer;
using cclib::shell::TaskMeta;

using sn::network::ApiInvoker;

class AbstractTask : public BaseAbstractTask
{
public:
   AbstractTask(AbstractTaskContainer& taskContainer, const TaskMeta& meta);
   QSharedPointer<ApiInvoker>& getApiInvoker();
};

}//upgrademgr
}//task
}//cloudcontroller

#endif // CLOUD_CONTROLLER_TASK_UPGRADEMGR_ABSTRACT_TASK_H

