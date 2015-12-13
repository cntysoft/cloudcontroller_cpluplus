#include <iostream>

using namespace std;

#include "command/simple_route_item.h"

int main()
{
   //sn::corelib::SimpleRouteItem route("find user [--id=] [--firstName=] [--lastName=] [--email=] [--position=]");
   sn::corelib::SimpleRouteItem route("sdfasdfgsdjhfgsadjhfasdh --something");
   QStringList args{
      "sdfasdfgsdjhfgsadjhfasdh",
      "--something"
   };
   route.match(args);
   cout << "Hello World!" << endl;
   return 0;
}

