#ifndef CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_CONTAINER_H
#define CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_CONTAINER_H

#include <QPair>
#include <QString>
#include <QList>
#include <QMap>
#include <QObject>
#include <QThread>

#include "corelib/io/terminal.h"
#include "corelib/command/route_stack.h"
#include "corelib/kernel/application.h"

#include "global/global.h"

namespace cclib{
namespace shell{

using sn::corelib::TerminalColor;
using sn::corelib::RouteStack;
using sn::corelib::Application;

class AbstractTask;
class TaskMeta;
class AbstractTaskLoop;
class TaskRunnerThread;

class CC_LIB_EXPORT AbstractTaskContainer : public QObject
{
   Q_OBJECT
   Q_DISABLE_COPY(AbstractTaskContainer)
public:
   using UsageTextItemType = QPair<QString, TerminalColor>;
   using TaskPoolType = QMap<QString, AbstractTask* (*)(AbstractTaskContainer*, const TaskMeta&)>;
public:
   AbstractTaskContainer(const QString& name, AbstractTaskLoop& loop);
   void printUsage()const;
   const QString& getName();
   void run(const QString& command);
   AbstractTaskLoop& getTaskLoop();
   void writeSubMsg(const QString& msg);
   QThread& getTaskRunnerThread();
public:
   virtual void runTask(const TaskMeta& meta);
   virtual void loadHandler(const QMap<QString, QString>& invokeArgs = QMap<QString, QString>());
   virtual void unloadHandler();
   virtual ~AbstractTaskContainer();
protected:
   void exitTaskThread(int exitCode);
   void addUsageText(const QString& text, TerminalColor color = TerminalColor::Default);
   void addTaskRoute(const QString& name, const QString& route, int priority = 1, const QMap<QString, QString>& defaultParams = QMap<QString, QString>());
signals:
   void beginTaskWorker();
protected:
   TaskPoolType m_taskRegisterPool;
   QList<UsageTextItemType> m_usageTextPool;
   RouteStack m_router;
   QString m_name;
   AbstractTaskLoop& m_taskLoop;
   QString m_containerPs;
   QString m_psBackup;
   Application& m_app;
   QThread m_taskRunnerThread;
};

}//shell
}//cclib

#endif // CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_CONTAINER_H
