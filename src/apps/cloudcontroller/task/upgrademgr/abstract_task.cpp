#include "abstract_task.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{

AbstractTask::AbstractTask(AbstractTaskContainer& taskContainer, const TaskMeta& meta)
   :AbstractTask(taskContainer, meta)
{
}

}//upgrademgr
}//task
}//cloudcontroller