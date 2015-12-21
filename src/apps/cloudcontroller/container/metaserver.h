#ifndef CLOUD_CONTROLLER_CONTAINER_METASERVER_H
#define CLOUD_CONTROLLER_CONTAINER_METASERVER_H

#include "shell/abstract_task_container.h"
#include "shell/task_loop.h"

namespace cloudcontroller{
namespace container{

using cloudcontroller::shell::AbstractTaskContainer;
using cloudcontroller::shell::TaskLoop;
using cloudcontroller::shell::TaskMeta;

class MetaServer : public AbstractTaskContainer
{
public:
   MetaServer(TaskLoop& loop);   
protected:
   virtual void runTask(const TaskMeta& meta);
protected:
   bool dispatchBuildInTask(const TaskMeta& meta);
   void initUsage();
   void initRouter();
protected:
   void quitTask(const TaskMeta& meta);
};

}//container
}//cloudcontroller

#endif // CLOUD_CONTROLLER_CONTAINER_METASERVER_H

