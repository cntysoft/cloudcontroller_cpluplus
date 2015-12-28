#include "abstract_task.h"
#include "corelib/io/terminal.h"

#include "shell/abstract_task_container.h"

namespace cclib{
namespace shell{

using sn::corelib::Terminal;
using sn::corelib::TerminalColor;

AbstractTask::AbstractTask(AbstractTaskContainer* taskContainer, const TaskMeta& meta)
   : m_taskContainer(taskContainer),
     m_invokeMeta(meta),
     m_app(*Application::instance())
{
}

void AbstractTask::writeSubMsg(const QString& msg, TerminalColor color, bool underline, bool blink) const
{
   Terminal::writeText(" >", TerminalColor::Green);
   Terminal::writeText(msg.toLocal8Bit(), color, underline, blink);
}

AbstractTaskContainer* AbstractTask::getTaskContainer()
{
   return m_taskContainer;
}

AbstractTask::~AbstractTask()
{}

}//shell
}//cclib