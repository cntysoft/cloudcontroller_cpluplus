#include "server_status.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace serverstatus{

using sn::corelib::network::ApiInvoker;

void get_version_info_callback(const ApiInvokeResponse &response, void *args)
{
   ServerVersion *self = static_cast<ServerVersion*>(args);
   if(!response.getStatus()){
      self->processErrorResponse(response);
      self->getEventLoop().exit();
   }
   self->writeSubMsg(response.getDataItem("version").toString());
   self->getEventLoop().exit();
}

ServerVersion::ServerVersion(AbstractTaskContainer *taskContainer, const TaskMeta &meta)
   : AbstractNetTask(taskContainer, meta)
{
}

void ServerVersion::run()
{
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   ApiInvokeRequest request("ServerStatus/Info", "getVersionInfo1");
   apiInvoker->request(request, get_version_info_callback, (void*)this);
   m_eventLoop.exec();
}

}//serverstatus
}//upgrademgr
}//task
}//cloudcontroller