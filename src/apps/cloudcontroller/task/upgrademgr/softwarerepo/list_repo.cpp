#include <QDebug>
#include <QSharedPointer>

#include "list_repo.h"
#include "shell/abstract_task_container.h"

#include "corelib/network/rpc/invoke_meta.h"

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
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   ApiInvokeRequest request("meta", "ls");
   int i = 0;
   while(i < 3){
      apiInvoker->request(request);
      i++;
   }
}

}//softwarerepo
}//upgrade
}//task
}//cloudcontroller