#ifndef CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_NET_TASK_H
#define CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_NET_TASK_H

#include <QSharedPointer>
#include <QPair>

#include "abstract_task.h"
#include "corelib/io/terminal.h"
#include "corelib/kernel/application.h"
#include "corelib/network/rpc/api_invoker.h"

namespace cclib{
namespace shell{

using sn::corelib::TerminalColor;
using sn::corelib::network::ApiInvokeResponse;
using sn::corelib::network::ApiInvokeRequest;
using sn::corelib::network::ApiInvoker;

class CC_LIB_EXPORT AbstractNetTask : public AbstractTask
{
   Q_DISABLE_COPY(AbstractNetTask)
public:
   AbstractNetTask(AbstractTaskContainer* taskContainer, const TaskMeta& meta);
   virtual ~AbstractNetTask();
public:
   QSharedPointer<ApiInvoker>& getApiInvoker();
   AbstractNetTask& setApiInvoker(QSharedPointer<ApiInvoker> &apiInvoker);
protected:
   void processErrorResponse(const ApiInvokeResponse &response);
   void waitForResponse(const ApiInvokeRequest &request);
protected slots:
   void responseArrivedHandler(const ApiInvokeResponse &response);
   void serverOfflineHandler();
protected:
   QSharedPointer<ApiInvoker> m_apiInvoker;
   QPair<int, bool> m_waitPair;
};

}//shell
}//cclib

#endif // CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_NET_TASK_H
