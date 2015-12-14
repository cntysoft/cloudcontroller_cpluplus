#include "command/route_item.h"
#include "ds/priority_list.h"
#include "command/route_stack.h"
#include "command/route_match_result.h"


#include <QTimer>

#include "application.h"
#include "io/terminal.h"
#include "command_runner.h"
#include "kernel/errorinfo.h"

using CloudControllerApplication = cloudcontroller::Application;
using CommandRunner = cloudcontroller::CommandRunner;
using ErrorInfo = sn::corelib::ErrorInfo;
using Terminal = sn::corelib::Terminal;
using TerminalColor = sn::corelib::TerminalColor;

int main(int argc, char *argv[])
{
   try{
      CloudControllerApplication app(argc, argv);
      //探测服务器环境，一般只会运行一次;
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
//   //sn::corelib::SimpleRouteItem route("find user [--id=] [--firstName=] [--lastName=] [--email=] [--position=]");
////   sn::corelib::SimpleRouteItem route("sdfasdfgsdjhfgsadjhfasdh --something");
//   QStringList args{
//      "sdfasdfgsdjhfgsadjhfasdh",
//      "--something"
//   };
//   sn::corelib::RouteStack router;
//   router.addRoute("first", sn::corelib::RouteItem("sdfasdfgsdjhfgsadjhfasdh --something",{
//                                                      {"controller", "asfdasdasdsa"}
//                                                   }));
//   sn::corelib::RouteMatchResult result = router.match(args);
   
//   //router.match(args);
////   route.match(args);
//   cout << "Hello World!" << endl;
}