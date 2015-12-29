#include "task_runner_thread.h"
#include <QDebug>
#include "corelib/kernel/errorinfo.h"
#include "corelib/io/terminal.h"
namespace cclib{
namespace shell{
using sn::corelib::ErrorInfo;
using sn::corelib::Terminal;
using sn::corelib::TerminalColor;

TaskRunnerThread::TaskRunnerThread(AbstractTaskContainer *taskContainer)
   :m_taskContainer(taskContainer)
{
}

void TaskRunnerThread::run()
{
   try{
      m_taskContainer->runTask(m_taskMeta);
   }catch(ErrorInfo errorInfo){
      QString str(errorInfo.toString());
      if(str.size() > 0){
         str += '\n';
         Terminal::writeText(str.toLocal8Bit(), TerminalColor::Red);
      }
      exit(1);
   }
   int status = exec();
   return;
}

TaskRunnerThread& TaskRunnerThread::setTaskMeta(const TaskMeta &meta)
{
   m_taskMeta = meta;
   return *this;
}

TaskRunnerThread::~TaskRunnerThread()
{
   
}

}//shell
}//cclib