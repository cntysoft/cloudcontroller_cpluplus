#ifndef CLOUD_CONTROLLER_TASK_GLOBAL_METASERVER
#define CLOUD_CONTROLLER_TASK_GLOBAL_METASERVER

#include "shell/abstract_task.h"
#include "shell/abstract_task_container.h"
#include "shell/task_meta.h"

namespace cloudcontroller{
namespace task{
namespace metaserver{

using cloudcontroller::shell::AbstractTask;
using cloudcontroller::shell::AbstractTaskContainer;
using cloudcontroller::shell::TaskMeta;

class EntryTask : public AbstractTask
{
public:
   EntryTask(AbstractTaskContainer& taskContainer, const TaskMeta& meta);
   virtual void run();
};

}//global
}//task
}//cloudcontroller

#endif // CLOUD_CONTROLLER_TASK_GLOBAL_METASERVER

