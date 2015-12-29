#include <QDebug>
#include <QSharedPointer>

#include "list_repo.h"
#include "shell/abstract_task_container.h"

#include "corelib/network/rpc/invoke_meta.h"
#include <QThread>
namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace softwarerepo{

using sn::corelib::network::ApiInvokeRequest;

ListRepo::ListRepo(AbstractTaskContainer *taskContainer, const cclib::shell::TaskMeta &meta)
   : AbstractTask(taskContainer, meta)
{
}

void ListRepo::run()
{
   //qDebug() << QThread::currentThreadId();
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   ApiInvokeRequest request("meta", "ls");
   apiInvoker->request(request);
}

}//softwarerepo
}//upgrade
}//task
}//cloudcontroller