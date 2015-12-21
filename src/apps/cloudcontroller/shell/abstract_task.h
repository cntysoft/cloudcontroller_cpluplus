#ifndef CLOUD_CONTROLLER_SHELL_ABSTRACT_TASK_H
#define CLOUD_CONTROLLER_SHELL_ABSTRACT_TASK_H

#include "global/global.h"
#include "io/terminal.h"

namespace cloudcontroller{
namespace shell{

using sn::corelib::TerminalColor;

class AbstractTaskContainer;
class TaskMeta;

class AbstractTask
{
   Q_DISABLE_COPY(AbstractTask)
public:
   AbstractTask(AbstractTaskContainer& taskContainer, const TaskMeta& meta);
   virtual void run() = 0;
   virtual ~AbstractTask();
protected:
   void printConsoleMsg(const char* str, TerminalColor color = TerminalColor::Default, bool underline = false, bool blink = false) const;
protected:
   AbstractTaskContainer& m_taskContainer;
   const TaskMeta& m_invokeMeta;
};

}//shell
}//cloudcontroller

#endif // CLOUD_CONTROLLER_SHELL_ABSTRACT_TASK_H
