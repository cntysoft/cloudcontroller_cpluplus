#ifndef CLOUD_CONTROLLER_TASK_UPGRADEMGR_ENTRY_H
#define CLOUD_CONTROLLER_TASK_UPGRADEMGR_ENTRY_H

#include "cclib/shell/abstract_task.h"
#include "cclib/shell/abstract_task_container.h"
#include "cclib/shell/task_meta.h"

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

namespace cloudcontroller{
namespace task{
namespace upgrademgr{

using cclib::shell::AbstractTask;
using cclib::shell::AbstractTaskContainer;
using cclib::shell::TaskMeta;

class EntryTask : public AbstractTask
{
public:
   EntryTask(AbstractTaskContainer& taskContainer, const TaskMeta& meta);
   virtual void run();
    virtual ~EntryTask();
protected:
   QTcpSocket* m_client = nullptr;
};

}//upgrademgr
}//task
}//cloudcontroller

#endif // CLOUD_CONTROLLER_TASK_UPGRADEMGR_ENTRY_H

