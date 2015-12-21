#ifndef CLOUD_CONTROLLER_SHELL_TASK_META_H
#define CLOUD_CONTROLLER_SHELL_TASK_META_H

#include <QMap>

namespace cloudcontroller{
namespace shell{

class TaskMeta
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
protected:
   QString m_container;
   QString m_category;
   QString m_name;
   TaskArgType m_args;
};

}//shell
}//cloudcontroller

#endif // CLOUD_CONTROLLER_SHELL_TASK_META_H
