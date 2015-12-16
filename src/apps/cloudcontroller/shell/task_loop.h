#ifndef CLOUD_CONTROLLER_SHELL_TASK_LOOP_H
#define CLOUD_CONTROLLER_SHELL_TASK_LOOP_H

#include <termios.h>
#include <unistd.h>

namespace cloudcontroller{
namespace shell{

class TaskLoop
{
public:
   TaskLoop();
   void run();
   ~TaskLoop();
protected:
   void setupTerminalAttr();
protected:
   struct termios m_savedTerminalAttr;
};

}//shell
}//cloudcontroller

#endif // CLOUD_CONTROLLER_SHELL_TASK_LOOP_H