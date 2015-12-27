#ifndef CLOUD_CONTROLLER_TASK_ENTRY_UPGRADEMGR_TASK_H
#define CLOUD_CONTROLLER_TASK_ENTRY_UPGRADEMGR_TASK_H

#include "cclib/shell/abstract_task.h"
#include "cclib/shell/abstract_task_container.h"
#include "cclib/shell/task_meta.h"

namespace cloudcontroller{
namespace task{
namespace global{

using cclib::shell::AbstractTask;
using cclib::shell::AbstractTaskContainer;
using cclib::shell::TaskMeta;

class EnterUpgradeMgrTask : public AbstractTask
{
public:
   EnterUpgradeMgrTask(AbstractTaskContainer* taskContainer, const TaskMeta& meta);
   virtual void run();
};

}//global
}//task
}//cloudcontroller

#endif // CLOUD_CONTROLLER_TASK_ENTRY_UPGRADEMGR_TASK_H
