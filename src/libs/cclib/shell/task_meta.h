#ifndef CLOUD_CONTROLLER_LIB_SHELL_TASK_META_H
#define CLOUD_CONTROLLER_LIB_SHELL_TASK_META_H

#include <QMap>

#include "global/global.h"
#include "abstract_task.h"

namespace cclib{
namespace shell{

class CC_LIB_EXPORT TaskMeta
{
public:
   using TaskArgType = QMap<QString, QString>;
public:
   TaskMeta();
   TaskMeta(const QString& container, const QString& category, const QString& name, const TaskArgType& args);
   TaskMeta& setContainer(const QString& container);
   TaskMeta& setCategory(const QString& category);
   TaskMeta& setName(const QString& name);
   TaskMeta& setTaskArgs(const TaskArgType& args);
   
   const QString& getContainer() const;
   const QString& getCategory()const;
   const QString& getName()const;
   const TaskArgType& getTaskArgs()const;
   const QString& getTaskArg(const QString &key)const;
protected:
   QString m_container;
   QString m_category;
   QString m_name;
   TaskArgType m_args;
};

}//shell
}//cclib

#endif // CLOUD_CONTROLLER_LIB_SHELL_TASK_META_H
