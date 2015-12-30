#ifndef CLOUD_CONTROLLER_TASK_UPGRADEMGR_SOFTWARE_REPO_LIST_REPO_H
#define CLOUD_CONTROLLER_TASK_UPGRADEMGR_SOFTWARE_REPO_LIST_REPO_H

#include "task/upgrademgr/abstract_task.h"
#include "cclib/shell/task_meta.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace softwarerepo{

using cloudcontroller::task::upgrademgr::AbstractTask;
using cclib::shell::AbstractTaskContainer;
using cclib::shell::TaskMeta;

class ListRepo : public AbstractTask
{
   Q_OBJECT
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
