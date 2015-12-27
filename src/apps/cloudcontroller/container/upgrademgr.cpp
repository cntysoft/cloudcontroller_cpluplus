#include <QString>
#include <QDebug>

#include "const.h"
#include "corelib/io/terminal.h"
#include "corelib/kernel/settings.h"
#include "corelib/kernel/errorinfo.h"

#include "upgrademgr.h"
#include "cclib/shell/task_meta.h"
#include "cclib/shell/abstract_task.h"


namespace cloudcontroller{
namespace container{

using sn::corelib::TerminalColor;
using sn::corelib::Settings;
using sn::corelib::ErrorInfo;
using cclib::shell::AbstractTask;
using cclib::shell::TaskMeta;

UpgradeMgr::UpgradeMgr(TaskLoop& loop)
   :AbstractTaskContainer("UpgradeMgr", loop),
     m_client(new QTcpSocket)
{
   m_containerPs = "upgrademgr >> ";
   initUsage();
   initRouter();
}

void UpgradeMgr::runTask(const TaskMeta& meta)
{
   if(!dispatchBuildInTask(meta)){
      AbstractTaskContainer::runTask(meta);
   }
}

void UpgradeMgr::initUsage()
{
   addUsageText("you can use commands as fowllow:\n", TerminalColor::LightGreen);
   addUsageText("quit       exit upgrademgr task container\n");
}

void UpgradeMgr::initRouter()
{
   addTaskRoute("quit", "quit", 1, {
                   {"category", "UpgradeMgr"},
                   {"name", "Quit"}
                });
}

bool UpgradeMgr::dispatchBuildInTask(const TaskMeta& meta)
{
   QString key(meta.getCategory()+'_'+meta.getName());
   if(key == "UpgradeMgr_Quit"){
      quitTask(meta);
      return true;
   }
   return false;
}

void UpgradeMgr::loadHandler(const QMap<QString, QString> &invokeArgs)
{
   QString host;
   int port;
   Settings &settings = m_app.getSettings();
   if(invokeArgs.contains("host")){
      host = invokeArgs.value("host");
   }else{
      //获取配置文件
      host = settings.getValue("upgrademgrHost", CC_CFG_GROUP_UPGRADEMGR).toString();
   }
   if(invokeArgs.contains("port")){
      port = invokeArgs.value("port").toInt();
   }else{
      port = settings.getValue("upgrademgrPort", CC_CFG_GROUP_UPGRADEMGR).toInt();
   }
   writeSubMsg("正在连接服务器 ... ");
   if(!connectToServer(host, port)){
      m_taskLoop.enterGlobalTaskContainer();
      throw ErrorInfo(QString("连接服务器失败 : %1").arg(getTcpConnectionErrorString()));
   }else{
      writeSubMsg(QString("连接服务器成功 [%1:%2]").arg(host).arg(port));
      AbstractTaskContainer::loadHandler(invokeArgs);
      
   }
}

bool UpgradeMgr::connectToServer(const QString &host, int port)
{
   m_client->connectToHost(host, port);
   if(m_client->waitForConnected()){
      return true;
   }else{
      return false;
   }
}

bool UpgradeMgr::isTcpConnectionValid()
{
   if(nullptr == m_client){
      return false;
   }
   return m_client->isValid();
}

QString UpgradeMgr::getTcpConnectionErrorString()
{
   if(nullptr == m_client){
      return QString();
   }else{
      return m_client->errorString();
   }
}

UpgradeMgr& UpgradeMgr::setApiInvoker(ApiInvoker *apiInvoker)
{
   if(nullptr != apiInvoker){
      m_apiInvoker.reset(apiInvoker);
   }
   return *this;
}

QSharedPointer<ApiInvoker>& UpgradeMgr::getApiInvoker()
{
   return m_apiInvoker;
}

void UpgradeMgr::quitTask(const TaskMeta&)
{
   m_taskLoop.enterGlobalTaskContainer();
}

}//container
}//cloudcontroller