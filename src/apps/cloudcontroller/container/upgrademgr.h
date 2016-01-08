#ifndef CLOUD_CONTROLLER_CONTAINER_UPGRADEMGR_H
#define CLOUD_CONTROLLER_CONTAINER_UPGRADEMGR_H

#include <QTcpSocket>
#include <QSharedPointer>
#include <QMap>
#include <QString>

#include "cclib/shell/abstract_task_container.h"
#include "shell/task_loop.h"
#include "shell/task_meta.h"

#include "corelib/network/rpc/api_invoker.h"


namespace cloudcontroller{
namespace container{

using cclib::shell::AbstractTaskContainer;
using cloudcontroller::shell::TaskLoop;
using cclib::shell::TaskMeta;

using sn::corelib::network::ApiInvoker;

class UpgradeMgr : public AbstractTaskContainer
{
   Q_OBJECT
   Q_DISABLE_COPY(UpgradeMgr)
public:
   UpgradeMgr(TaskLoop& loop);
   bool connectToServer(const QString &host, int port);
   bool isTcpConnectionValid();
   QString getTcpConnectionErrorString();
   QSharedPointer<ApiInvoker>& getApiInvoker();
public:
   virtual void loadHandler(const QMap<QString, QString>& invokeArgs = QMap<QString, QString>());
   virtual void unloadHandler();
protected:
   virtual void runTask(const TaskMeta& meta);
protected:
   bool dispatchBuildInTask(const TaskMeta& meta);
   void initUsage();
   void initRouter();
   void initTaskPool();
protected:
   void quitTask(const TaskMeta& meta = TaskMeta());
   void helpTask(const TaskMeta& meta = TaskMeta());
protected:
   QSharedPointer<ApiInvoker> m_apiInvoker;
   QSharedPointer<QTcpSocket> m_client;
   QMap<QString, QString> m_invokeArgs;
   bool m_connectedWaitMark = false;
   bool m_activeDisconnected = false;
   bool m_needWriteDisconnectMsg = true;
};


}//container
}//cloudcontroller

#endif // CLOUD_CONTROLLER_CONTAINER_UPGRADEMGR_H
