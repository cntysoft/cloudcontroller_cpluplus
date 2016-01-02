#ifndef CLOUD_CONTROLLER_TASK_UPGRADEMGR_SOFTWARE_REPO_LIST_REPO_H
#define CLOUD_CONTROLLER_TASK_UPGRADEMGR_SOFTWARE_REPO_LIST_REPO_H

#include "task/upgrademgr/abstract_task.h"
#include "cclib/shell/task_meta.h"
#include "corelib/network/rpc/invoke_meta.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace softwarerepo{

using cloudcontroller::task::upgrademgr::AbstractTask;
using cclib::shell::AbstractTaskContainer;
using cclib::shell::TaskMeta;
using sn::corelib::network::ApiInvokeRequest;
using sn::corelib::network::ApiInvokeResponse;

class ListRepo : public AbstractTask
{
   Q_OBJECT
   friend void ls_software_repo_callback(const ApiInvokeResponse &response, void* args);
public:
   ListRepo(AbstractTaskContainer* taskContainer, const TaskMeta& meta);
   virtual void run();
signals:
   void beginTaskWorker();
};

}//softwarerepo
}//upgrade
}//task
}//cloudcontroller

#endif // CLOUD_CONTROLLER_TASK_UPGRADEMGR_SOFTWARE_REPO_LIST_REPO_H
