#include <QThread>

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
   Terminal::writeText("> ", TerminalColor::Red);
   Terminal::writeText(QString("Api call error : %1 , code : %2\n").arg(error.first).arg(error.second).toLocal8Bit(), TerminalColor::Red);
}

QSharedPointer<ApiInvoker>& AbstractNetTask::getApiInvoker()
{
   return m_apiInvoker;
}

AbstractNetTask& AbstractNetTask::setApiInvoker(QSharedPointer<ApiInvoker> &apiInvoker)
{
   m_apiInvoker = apiInvoker;
   return *this;
}

void AbstractNetTask::waitForResponse(const ApiInvokeRequest &request)
{
   m_waitPair.first = request.getSerial();
   m_waitPair.second = false;
   int current = 0;
   while(!m_waitPair.second && current < 15000){
      QThread::usleep(200);
      current += 200;
   }
}

}//shell
}//cclib