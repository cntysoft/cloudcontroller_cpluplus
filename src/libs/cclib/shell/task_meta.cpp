#include "task_meta.h"

namespace cclib{
namespace shell{

TaskMeta::TaskMeta()
{}

TaskMeta::TaskMeta(const QString &container, const QString &category, const QString &name, const TaskArgType &args)
   : m_container(container),
     m_category(category),
     m_name(name),
     m_args(args)
{
}

TaskMeta& TaskMeta::setContainer(const QString &container)
{
   m_container = container;
   return *this;
}

const QString& TaskMeta::getContainer() const
{
   return m_container;
}

TaskMeta& TaskMeta::setCategory(const QString &category)
{
   m_category = category;
   return *this;
}

const QString& TaskMeta::getCategory() const
{
   return m_category;
}

TaskMeta& TaskMeta::setName(const QString &name)
{
   m_name = name;
   return *this;
}

const QString& TaskMeta::getName() const
{
   return m_name;
}

TaskMeta& TaskMeta::setTaskArgs(const TaskArgType &args)
{
   m_args = args;
   return *this;
}

const TaskMeta::TaskArgType & TaskMeta::getTaskArgs() const
{
   return m_args;
}

}//shell
}//cclib