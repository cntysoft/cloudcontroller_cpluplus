#include <QDebug>
#include <QSharedPointer>
#include <QTcpSocket>
#include <QDataStream>
#include "list_repo.h"
#include "shell/abstract_task_container.h"


#include "shell/task_runner_worker.h"
#include <QThread>
namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace softwarerepo{

using sn::corelib::network::ApiInvokeRequest;
using sn::corelib::network::ApiInvokeResponse;
using cclib::shell::TaskRunnerWorker;

void ls_software_repo_callback(const ApiInvokeResponse &response, void *args)
{
   ListRepo *self = (ListRepo*)args;
   self->processErrorResponse(response);
}

ListRepo::ListRepo(AbstractTaskContainer *taskContainer, const cclib::shell::TaskMeta &meta)
   : AbstractTask(taskContainer, meta)
{
}

void ListRepo::run()
{
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   ApiInvokeRequest request("Repo/Info", "lsSoftwareRepoDir");
   apiInvoker->request(request, ls_software_repo_callback, (void*)this);
   
}

}//softwarerepo
}//upgrade
}//task
}//cloudcontroller