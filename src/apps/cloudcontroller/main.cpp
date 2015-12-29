#include <QTimer>
#include <QDebug>
#include <QObject>

#include "corelib/command/route_item.h"
#include "corelib/ds/priority_list.h"
#include "corelib/command/route_stack.h"
#include "corelib/command/route_match_result.h"
#include "corelib/io/terminal.h"
#include "corelib/kernel/errorinfo.h"

#include "application.h"
#include "command_runner.h"
#include "thread.h"
using CloudControllerApplication = cloudcontroller::Application;
using CommandRunner = cloudcontroller::CommandRunner;
using ErrorInfo = sn::corelib::ErrorInfo;
using Terminal = sn::corelib::Terminal;
using TerminalColor = sn::corelib::TerminalColor;
int main(int argc, char *argv[])
{
   try{
      CloudControllerApplication app(argc, argv);
//      app.ensureImportantDir();
//      app.watchImportantSignal();
//      CommandRunner cmdrunner(app);
//      QTimer::singleShot(0, Qt::PreciseTimer, [&cmdrunner]{
////         cmdrunner.run();
         
        
//      });
      TaskRunnerThread *thread = new TaskRunnerThread;
//      QObject::connect(thread, &TaskRunnerThread::finished, &hand, &HandlerCls::dataReady);
      thread->moveToThread(thread);
      thread->start();
      thread->wait();
      qDebug() << "main"<< QThread::currentThreadId();
      qDebug() << "main";
      //app.connectToServer();
      return app.exec();
   }catch(const ErrorInfo& errorInfo){
      QString str(errorInfo.toString());
      if(str.size() > 0){
         str += '\n';
         Terminal::writeText(str.toLatin1(), TerminalColor::Red);
      }
      return EXIT_FAILURE;
   }
}