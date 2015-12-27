#ifndef CLOUD_CONTROLLER_SHELL_TASK_LOOP_H
#define CLOUD_CONTROLLER_SHELL_TASK_LOOP_H

#include "cclib/shell/abstract_task_loop.h"

//#include "container/global.h"
//#include "container/metaserver.h"
//#include "container/upgrademgr.h"

namespace cloudcontroller{
namespace shell{

using cclib::shell::AbstractTaskLoop;

class TaskLoop : public AbstractTaskLoop
{
protected:
   virtual void initCommandContainer();
};

}//shell
}//cloudcontroller

#endif // CLOUD_CONTROLLER_SHELL_TASK_LOOP_H