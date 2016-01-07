#ifndef CLOUD_CONTROLLER_TASK_UPGRADEMGR_SERVERSTATUS_SERVER_STATUS_H
#define CLOUD_CONTROLLER_TASK_UPGRADEMGR_SERVERSTATUS_SERVER_STATUS_H

#include "cclib/shell/task_meta.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "cclib/shell/abstract_net_task.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace serverstatus{

using cclib::shell::AbstractNetTask;
using cclib::shell::AbstractTaskContainer;
using cclib::shell::TaskMeta;
using sn::corelib::network::ApiInvokeRequest;
using sn::corelib::network::ApiInvokeResponse;

class ServerVersion : public AbstractNetTask
{
   Q_OBJECT
   friend void get_version_info_callback(const ApiInvokeResponse &response, void *args);
public:
   ServerVersion(AbstractTaskContainer *taskContainer, const TaskMeta &meta);
   virtual void run();
};

}//serverstatus
}//upgrademgr
}//task
}//cloudcontroller

#endif // CLOUD_CONTROLLER_TASK_UPGRADEMGR_SERVERSTATUS_SERVER_STATUS_H
