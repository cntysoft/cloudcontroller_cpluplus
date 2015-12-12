#include <iostream>

using namespace std;

#include "command/simple_route_item.h"

int main()
{
   //sn::corelib::SimpleRouteItem route("find user [--id=] [--firstName=] [--lastName=] [--email=] [--position=]");
   sn::corelib::RouteMatcher route("asdasd --something=");
   QStringList args{
      "sdfasdfgsdjhfgsadjhfasdh",
      "--something=aasdas"
   };
   route.match(args);
   cout << "Hello World!" << endl;
   return 0;
}

