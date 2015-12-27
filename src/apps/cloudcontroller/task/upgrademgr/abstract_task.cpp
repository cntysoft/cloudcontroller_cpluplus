#include "abstract_task.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{

AbstractTask::AbstractTask(AbstractTaskContainer* taskContainer, const TaskMeta& meta)
   :BaseAbstractTask(taskContainer, meta)
{
}


}//upgrademgr
}//task
}//cloudcontroller