#include <iostream>
#include <QDebug>
using namespace std;

#include "command/simple_route_item.h"
#include "ds/priority_list.h"

int main()
{
   //sn::corelib::SimpleRouteItem route("find user [--id=] [--firstName=] [--lastName=] [--email=] [--position=]");
   sn::corelib::SimpleRouteItem route("sdfasdfgsdjhfgsadjhfasdh --something");
   QStringList args{
      "sdfasdfgsdjhfgsadjhfasdh",
      "--something"
   };
   sn::corelib::PriorityList<int> list;
   list.insert("b", 1, 3);
   list.insert("c", 100, 1);
   list.insert("a", 222, 2);
   list.sort();
   qDebug() << list[1];
   route.match(args);
   cout << "Hello World!" << endl;
   return 0;
}