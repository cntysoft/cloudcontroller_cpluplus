#ifndef CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_H
#define CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_H

#include <QString>

#include "corelib/io/terminal.h"
#include "corelib/kernel/application.h"
#include "global/global.h"

namespace cclib{
namespace shell{

using sn::corelib::TerminalColor;
using sn::corelib::Application;

class AbstractTaskContainer;
class TaskMeta;

class CC_LIB_EXPORT AbstractTask
{
   Q_DISABLE_COPY(AbstractTask)
public:
   AbstractTask(AbstractTaskContainer* taskContainer, const TaskMeta& meta);
   AbstractTaskContainer* getTaskContainer();
public:
   virtual void run() = 0;
   virtual ~AbstractTask();
protected:
   void writeSubMsg(const QString &msg, TerminalColor color = TerminalColor::Default, bool underline = false, bool blink = false) const;
protected:
   AbstractTaskContainer* m_taskContainer;
   const TaskMeta& m_invokeMeta;
   Application& m_app;
};

}//shell
}//cclib

#endif // CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_H
