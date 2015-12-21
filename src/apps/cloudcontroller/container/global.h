#ifndef CLOUD_CONTROLLER_CONTAINER_GLOBAL_H
#define CLOUD_CONTROLLER_CONTAINER_GLOBAL_H

#include "shell/abstract_task_container.h"
#include "shell/task_loop.h"

namespace cloudcontroller{
namespace container{

using cloudcontroller::shell::AbstractTaskContainer;
using cloudcontroller::shell::TaskLoop;

class Global : public AbstractTaskContainer
{
public:
   Global(TaskLoop& loop);
protected:
   void initUsage();
};

}//container
}//cloudcontroller

#endif // CLOUD_CONTROLLER_CONTAINER_GLOBAL_H