#ifndef CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_H
#define CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_H

#include <QString>
#include <QObject>
#include <QEventLoop>

#include "corelib/io/terminal.h"
#include "corelib/kernel/application.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "global/global.h"

namespace cclib{
namespace shell{

using sn::corelib::TerminalColor;
using sn::corelib::Application;
using sn::corelib::network::ApiInvokeResponse;
using sn::corelib::network::ApiInvokeRequest;

class AbstractTaskContainer;
class TaskMeta;

class CC_LIB_EXPORT AbstractTask : public QObject
{
   Q_DISABLE_COPY(AbstractTask)
public:
   AbstractTask(AbstractTaskContainer* taskContainer, const TaskMeta& meta);
   AbstractTaskContainer* getTaskContainer();
   QEventLoop& getEventLoop();
public:
   virtual void run() = 0;
   virtual ~AbstractTask();
protected:
   void writeSubMsg(const QString &msg, TerminalColor color = TerminalColor::Default, bool underline = false, bool blink = false);
   void exitTaskThread(int exitCode);
   void beginReplaceMode();
   void endReplaceMode();
   void enterCommandLoop();
protected:
   AbstractTaskContainer* m_taskContainer;
   const TaskMeta& m_invokeMeta;
   Application& m_app;
   bool m_replaceWriteMode = false;
   bool m_replaceWriteBegin = false;
   QEventLoop m_eventLoop;
};

}//shell
}//cclib

#endif // CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_H
