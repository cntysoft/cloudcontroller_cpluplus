#include <iostream>
#include <QDebug>
using namespace std;

#include "command/route_item.h"
#include "ds/priority_list.h"
#include "command/route_stack.h"
#include "command/route_match_result.h"

int main()
{
   //sn::corelib::SimpleRouteItem route("find user [--id=] [--firstName=] [--lastName=] [--email=] [--position=]");
//   sn::corelib::SimpleRouteItem route("sdfasdfgsdjhfgsadjhfasdh --something");
   QStringList args{
      "sdfasdfgsdjhfgsadjhfasdh",
      "--something"
   };
   sn::corelib::RouteStack router;
   router.addRoute("first", sn::corelib::RouteItem("sdfasdfgsdjhfgsadjhfasdh --something",{
                                                      {"controller", "asfdasdasdsa"}
                                                   }));
   sn::corelib::RouteMatchResult result = router.match(args);
   
   //router.match(args);
//   route.match(args);
   cout << "Hello World!" << endl;
   return 0;
}