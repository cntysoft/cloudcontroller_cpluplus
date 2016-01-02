#include "abstract_net_task.h"

namespace cclib{
namespace shell{

using sn::corelib::Terminal;

AbstractNetTask::AbstractNetTask(AbstractTaskContainer *taskContainer, const TaskMeta &meta)
   : AbstractTask(taskContainer, meta)
{
}

void AbstractNetTask::processErrorResponse(const ApiInvokeResponse &response)
{
   QPair<int, QString> error = response.getError();
   Terminal::writeText(" >", TerminalColor::Red);
   Terminal::writeText(QString("Api call error : %1 , code : %2").arg(error.first).arg(error.second).toLocal8Bit(), TerminalColor::Red);
}

}//shell
}//cclib