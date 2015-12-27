#include "abstract_task.h"
#include "io/terminal.h"

namespace cclib{
namespace shell{

using sn::corelib::Terminal;

AbstractTask::AbstractTask(AbstractTaskContainer* taskContainer, const TaskMeta& meta)
   : m_taskContainer(taskContainer),
     m_invokeMeta(meta),
     m_app(*Application::instance())
{
}

void AbstractTask::printConsoleMsg(const char *str, sn::corelib::TerminalColor color, bool underline, bool blink) const
{
   Terminal::writeText(str, color, underline, blink);
}

AbstractTaskContainer* AbstractTask::getTaskContainer()
{
   return m_taskContainer;
}

AbstractTask::~AbstractTask()
{}

}//shell
}//cclib