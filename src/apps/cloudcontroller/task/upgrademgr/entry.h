#ifndef CLOUD_CONTROLLER_TASK_UPGRADEMGR_ENTRY_H
#define CLOUD_CONTROLLER_TASK_UPGRADEMGR_ENTRY_H

#include "shell/abstract_task.h"
#include "shell/abstract_task_container.h"
#include "shell/task_meta.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{

using cloudcontroller::shell::AbstractTask;
using cloudcontroller::shell::AbstractTaskContainer;
using cloudcontroller::shell::TaskMeta;

class EntryTask : public AbstractTask
{
public:
   EntryTask(AbstractTaskContainer& taskContainer, const TaskMeta& meta);
   virtual void run();
};

}//upgrademgr
}//task
}//cloudcontroller

#endif // CLOUD_CONTROLLER_TASK_UPGRADEMGR_ENTRY_H

