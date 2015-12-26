#include <QDebug>

#include "entry.h"
#include "shell/task_loop.h"

namespace cloudcontroller{
namespace task{
namespace metaserver{

using cloudcontroller::shell::TaskLoop;

EntryTask::EntryTask(AbstractTaskContainer& taskContainer, const TaskMeta& meta)
   :AbstractTask(taskContainer, meta)
{
}

void EntryTask::run()
{
   m_taskContainer.getTaskLoop().enterTaskContainer("MetaServer");
   m_taskContainer.getTaskLoop().enterGlobalTaskContainer();
}

}//global
}//task
}//cloudcontroller