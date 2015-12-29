#ifndef CLOUD_CONTROLLER_LIB_SHELL_TASK_RUNNER_THREAD_H
#define CLOUD_CONTROLLER_LIB_SHELL_TASK_RUNNER_THREAD_H

#include "global/global.h"
#include <QThread>
#include <QTcpSocket>

#include "abstract_task_container.h"
#include "task_meta.h"

namespace cclib{
namespace shell{

class AbstractTaskContainer;
class TaskMeta;

class CC_LIB_EXPORT TaskRunnerThread : public QThread
{
   Q_OBJECT
public:
   TaskRunnerThread(AbstractTaskContainer *taskContainer);
   ~TaskRunnerThread();
   void run() Q_DECL_OVERRIDE;
   TaskRunnerThread& setTaskMeta(const TaskMeta &meta);
protected:
   QTcpSocket *m_socket;
   AbstractTaskContainer *m_taskContainer;
   TaskMeta m_taskMeta;
};

}//shell
}//cclib

#endif // CLOUD_CONTROLLER_LIB_SHELL_TASK_RUNNER_THREAD_H
