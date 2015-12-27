#ifndef CLOUD_CONTROLLER_CONTAINER_UPGRADEMGR_H
#define CLOUD_CONTROLLER_CONTAINER_UPGRADEMGR_H

#include <QTcpSocket>
#include <QSharedPointer>

#include "cclib/shell/abstract_task_container.h"
#include "shell/task_loop.h"

#include "corelib/network/rpc/api_invoker.h"

namespace cloudcontroller{
namespace container{

using cclib::shell::AbstractTaskContainer;
using cloudcontroller::shell::TaskLoop;
using cclib::shell::TaskMeta;

using sn::network::ApiInvoker;

class UpgradeMgr : public AbstractTaskContainer
{
   Q_DISABLE_COPY(UpgradeMgr)
public:
   UpgradeMgr(TaskLoop& loop);
   UpgradeMgr& connectToServer(const QString &host, int port);
protected:
   virtual void runTask(const TaskMeta& meta);
   UpgradeMgr& setApiInvoker(ApiInvoker* apiInvoker);
   QSharedPointer<ApiInvoker>& getApiInvoker();
protected:
   bool dispatchBuildInTask(const TaskMeta& meta);
   void initUsage();
   void initRouter();
protected:
   void quitTask(const TaskMeta& meta);
protected:
   QSharedPointer<ApiInvoker> m_apiInvoker;
   QSharedPointer<QTcpSocket> m_client;
};


}//container
}//cloudcontroller

#endif // CLOUD_CONTROLLER_CONTAINER_UPGRADEMGR_H
