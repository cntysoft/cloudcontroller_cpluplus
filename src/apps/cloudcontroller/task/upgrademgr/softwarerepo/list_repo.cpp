#include <QDebug>
#include <QSharedPointer>
#include <QTcpSocket>
#include <QDataStream>
#include "list_repo.h"
#include "shell/abstract_task_container.h"

#include "corelib/network/rpc/invoke_meta.h"
#include "shell/task_runner_worker.h"
#include <QThread>
namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace softwarerepo{

using sn::corelib::network::ApiInvokeRequest;
using cclib::shell::TaskRunnerWorker;
ListRepo::ListRepo(AbstractTaskContainer *taskContainer, const cclib::shell::TaskMeta &meta)
   : AbstractTask(taskContainer, meta)
{
}

void ListRepo::run()
{
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   ApiInvokeRequest request("Repo/Info", "lsSoftwareRepoDir");
   apiInvoker->request(request);
}

}//softwarerepo
}//upgrade
}//task
}//cloudcontroller