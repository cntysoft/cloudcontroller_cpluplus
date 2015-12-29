#include <csignal>
#include <cerrno>
#include <QDebug>

#include "application.h"

#include "corelib/kernel/errorinfo.h"
#include <QTcpSocket>
#include <QDataStream>
#include "corelib/network/rpc/invoke_meta.h"

namespace cloudcontroller{

using sn::corelib::ErrorInfo;
using sn::corelib::network::ApiInvokeRequest;

extern void init_defualt_cfg(Settings &settings);

Application::Application(int &argc, char **argv)
   : BaseApplication(argc, argv)
{
   setApplicationName("cloudcontroller");
}

Settings::CfgInitializerFnType Application::getDefaultCfgInitializerFn()
{
   return init_defualt_cfg;
}

void Application::connectToServer()
{
   QTcpSocket* client = new QTcpSocket;
   connect(client, &QTcpSocket::connected, this, [=](){
      qDebug() << "connected";
      int i = 0;
      while(i < 3){
         ApiInvokeRequest request("meta", "ls");
         QDataStream out(client);
         out << request;
         client->write("\r\n");
         client->flush();
         i++;
      }
      
   });
  
   
   connect(client, &QTcpSocket::readyRead, this, [=](){
      qDebug() << client->readAll();
   });
   client->connectToHost("127.0.0.1", 7777);
}

void Application::watchImportantSignal()
{
   struct sigaction sa;
   memset(&sa, 0, sizeof(sa));
   sa.sa_flags = SA_RESTART;
   sa.sa_handler = [](int sig)->void{
      Application::instance()->setCatchedSignalNumber(sig);
   };
   if(sigaction(SIGINT, &sa, 0) != 0){
      throw ErrorInfo(QString("sigaction failed errno : %1").arg(errno));
   }
}

Application::~Application()
{}

}//cloudcontroller