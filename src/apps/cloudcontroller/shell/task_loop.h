#ifndef CLOUD_CONTROLLER_SHELL_TASK_LOOP_H
#define CLOUD_CONTROLLER_SHELL_TASK_LOOP_H

#include "cclib/shell/abstract_task_loop.h"


namespace cloudcontroller{
namespace shell{

using cclib::shell::AbstractTaskLoop;

class TaskLoop : public AbstractTaskLoop
{
protected:
   virtual void initCommandContainer();
   virtual bool isNeedRestartSelectCall();
};

}//shell
}//cloudcontroller

#endif // CLOUD_CONTROLLER_SHELL_TASK_LOOP_H