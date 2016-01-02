#include <QTimer>
#include <QDebug>
#include <QObject>

#include "corelib/command/route_item.h"
#include "corelib/ds/priority_list.h"
#include "corelib/command/route_stack.h"
#include "corelib/command/route_match_result.h"
#include "corelib/io/terminal.h"
#include "corelib/kernel/errorinfo.h"
#include "corelib/network/rpc/invoke_meta.h"

#include "application.h"
#include "command_runner.h"

using CloudControllerApplication = cloudcontroller::Application;
using CommandRunner = cloudcontroller::CommandRunner;
using ErrorInfo = sn::corelib::ErrorInfo;
using Terminal = sn::corelib::Terminal;
using TerminalColor = sn::corelib::TerminalColor;
using sn::corelib::network::ApiInvokeResponse;

//全局更新函数
namespace cloudcontroller{
void global_initializer();
}//cloudcontroller

int main(int argc, char *argv[])
{
   try{
      qAddPreRoutine(cloudcontroller::global_initializer);
      CloudControllerApplication app(argc, argv);
      app.ensureImportantDir();
      app.watchImportantSignal();
      CommandRunner cmdrunner(app);
      QTimer::singleShot(0, Qt::PreciseTimer, [&cmdrunner]{
         cmdrunner.run();
      });
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