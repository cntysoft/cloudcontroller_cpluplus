#ifndef CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_CONTAINER_H
#define CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_CONTAINER_H

#include <QPair>
#include <QString>
#include <QList>

#include "corelib/io/terminal.h"
#include "corelib/command/route_stack.h"

#include "global/global.h"

namespace cclib{
namespace shell{

using sn::corelib::TerminalColor;
using sn::corelib::RouteStack;

class AbstractTask;
class TaskMeta;
class AbstractTaskLoop;

class CC_LIB_EXPORT AbstractTaskContainer
{
   Q_DISABLE_COPY(AbstractTaskContainer)
public:
   using UsageTextItemType = QPair<QString, TerminalColor>;
   using TaskPoolType = QMap<QString, AbstractTask* (*)(AbstractTaskContainer&, const TaskMeta&)>;
public:
   AbstractTaskContainer(const QString& name, AbstractTaskLoop& loop);
   void printUsage()const;
   const QString& getName();
   void run(const QString& command);
   AbstractTaskLoop& getTaskLoop();
public:
   virtual void runTask(const TaskMeta& meta);
   virtual void loadHandler();
   virtual void unloadHandler();
   virtual ~AbstractTaskContainer();
protected:
   void addUsageText(const QString& text, TerminalColor color = TerminalColor::Default);
   void addTaskRoute(const QString& name, const QString& route, int priority = 1, const QMap<QString, QString>& defaultParams = QMap<QString, QString>());
protected:
   TaskPoolType m_taskRegisterPool;
   QList<UsageTextItemType> m_usageTextPool;
   RouteStack m_router;
   QString m_name;
   AbstractTaskLoop& m_taskLoop;
   QString m_containerPs;
   QString m_psBackup;
};

}//shell
}//cclib

#endif // CLOUD_CONTROLLER_LIB_SHELL_ABSTRACT_TASK_CONTAINER_H
