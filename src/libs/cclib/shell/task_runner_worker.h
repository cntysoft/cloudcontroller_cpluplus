#ifndef CLOUD_CONTROLLER_LIB_SHELL_TASK_RUNNER_WORKER_H
#define CLOUD_CONTROLLER_LIB_SHELL_TASK_RUNNER_WORKER_H

#include <QObject>
#include <QTcpSocket>

#include "global/global.h"
#include "abstract_task_container.h"
#include "task_meta.h"

namespace cclib{
namespace shell{

class CC_LIB_EXPORT TaskRunnerWorker : public QObject
{
   Q_OBJECT
public:
   TaskRunnerWorker(QTcpSocket *socket);
//   TaskRunnerWorker& setTaskMeta(const TaskMeta &meta);
   ~TaskRunnerWorker();
public slots:
   void beginRunTask();
protected:
//   AbstractTaskContainer *m_taskContainer;
//   TaskMeta m_taskMeta;
   QTcpSocket *m_socket;
};

}//shell
}//cclib

#endif // CLOUD_CONTROLLER_LIB_SHELL_TASK_RUNNER_WORKER_H
