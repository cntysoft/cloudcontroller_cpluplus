#include <QDebug>

#include "metaserver.h"
#include "shell/task_loop.h"

namespace cloudcontroller{
namespace task{
namespace global{

using cloudcontroller::shell::TaskLoop;

MetaServer::MetaServer(AbstractTaskContainer& taskContainer, const TaskMeta& meta)
   :AbstractTask(taskContainer, meta)
{
}

void MetaServer::run()
{
   m_taskContainer.getTaskLoop().enterTaskContainer("MetaServer");
}

}//global
}//task
}//cloudcontroller