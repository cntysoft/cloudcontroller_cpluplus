#ifndef CLOUD_CONTROLLER_SHELL_TASK_CONTAINER_H
#define CLOUD_CONTROLLER_SHELL_TASK_CONTAINER_H

#include <QPair>
#include <QString>
#include <QList>

#include "io/terminal.h"
#include "command/route_stack.h"

namespace cloudcontroller{
namespace shell{

using sn::corelib::TerminalColor;
using sn::corelib::RouteStack;

class AbstractTask;
class TaskMeta;
class TaskLoop;

class AbstractTaskContainer
{
   Q_DISABLE_COPY(AbstractTaskContainer)
public:
   using UsageTextItemType = QPair<QString, TerminalColor>;
   using TaskPoolType = QMap<QString, AbstractTask* (*)(AbstractTaskContainer&, const TaskMeta&)>;
public:
   AbstractTaskContainer(const QString& name, TaskLoop& loop);
   void printUsage()const;
   const QString& getName();
public:
   virtual void run(const QString& command);
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
   TaskLoop& m_taskLoop;
   QString m_containerPs;
   QString m_psBackup;
};

}//shell
}//cloudcontroller

#endif // CLOUD_CONTROLLER_SHELL_TASK_CONTAINER_H
