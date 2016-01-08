#ifndef CLOUD_CONTROLLER_TASK_UPGRADEMGR_SOFTWARE_UPGRADER_UPGRADE_UPGRADEMGR_H
#define CLOUD_CONTROLLER_TASK_UPGRADEMGR_SOFTWARE_UPGRADER_UPGRADE_UPGRADEMGR_H

#include "cclib/shell/task_meta.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "cclib/shell/abstract_net_task.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace upgrader{

using cclib::shell::AbstractNetTask;
using cclib::shell::AbstractTaskContainer;
using cclib::shell::TaskMeta;
using sn::corelib::network::ApiInvokeRequest;
using sn::corelib::network::ApiInvokeResponse;

class UpgradeUpgrademgr : public AbstractNetTask
{
   Q_OBJECT
   friend void upgrade_upgrademgr_handler(const ApiInvokeResponse &response, void *args);
public:
   UpgradeUpgrademgr(AbstractTaskContainer *taskContainer, const TaskMeta &meta);
   virtual void run();
};

}//upgrader
}//upgrademgr
}//task
}//cloudcontroller
#endif // CLOUD_CONTROLLER_TASK_UPGRADEMGR_SOFTWARE_UPGRADER_UPGRADE_UPGRADEMGR_H
