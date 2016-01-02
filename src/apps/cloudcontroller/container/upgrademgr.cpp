#include <QString>
#include <QDebug>
#include <QThread>

#include "const.h"
#include "corelib/io/terminal.h"
#include "corelib/kernel/settings.h"
#include "corelib/kernel/errorinfo.h"

#include "upgrademgr.h"
#include "cclib/shell/task_meta.h"
#include "cclib/shell/abstract_task.h"

#include "task/upgrademgr/upgrademgr_task_repo.h"

namespace cloudcontroller{
namespace container{

using sn::corelib::TerminalColor;
using sn::corelib::Terminal;
using sn::corelib::Settings;
using sn::corelib::ErrorInfo;
using cclib::shell::AbstractTask;
using cclib::shell::TaskMeta;

USE_SOFTWARE_REPO_TASK_LIST

UpgradeMgr::UpgradeMgr(TaskLoop& loop)
   :AbstractTaskContainer("UpgradeMgr", loop)
{
   m_containerPs = "upgrademgr >> ";
   initUsage();
   initRouter();
   initTaskPool();
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
   addTaskRoute("softwarerepolistrepo", "list_software_repo", 1, {
                   {"category", "SoftwareRepo"},
                   {"name", "ListRepo"}
                });
}

void UpgradeMgr::initTaskPool()
{
   m_taskRegisterPool.insert("UpgradeMgr_SoftwareRepo_ListRepo", [](AbstractTaskContainer* container, const TaskMeta& meta)->AbstractTask*{
      SoftwareRepoListRepoTask* task = new SoftwareRepoListRepoTask(container, meta);
      return task;
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
   m_invokeArgs = invokeArgs;
   QSharedPointer<ApiInvoker>& invoker = getApiInvoker();
   writeSubMsg("正在连接服务器 ... ");
   m_activeDisconnected = false;
   invoker->connectToServer();
   while(!m_connectedMark){
      QThread::usleep(100);
   }
   m_connectedMark = false;
}

void UpgradeMgr::unloadHandler()
{
   if(m_activeDisconnected){
      writeSubMsg("正在断开连接 ... ");
   }
   QSharedPointer<ApiInvoker>& invoker = getApiInvoker();
   invoker->disconnectFromServer();
   while(!m_disconnectedMark){
      QThread::usleep(100);
   }
   m_disconnectedMark = false;
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

QSharedPointer<ApiInvoker>& UpgradeMgr::getApiInvoker()
{
   if(m_apiInvoker.isNull()){
      QString host;
      int port;
      Settings &settings = m_app.getSettings();
      if(m_invokeArgs.contains("host")){
         host = m_invokeArgs.value("host");
      }else{
         //获取配置文件
         host = settings.getValue("upgrademgrHost", CC_CFG_GROUP_UPGRADEMGR).toString();
      }
      if(m_invokeArgs.contains("port")){
         port = m_invokeArgs.value("port").toInt();
      }else{
         port = settings.getValue("upgrademgrPort", CC_CFG_GROUP_UPGRADEMGR).toInt();
      }
      m_apiInvoker.reset(new ApiInvoker(host, port));
      connect(m_apiInvoker.data(), &ApiInvoker::connectedToServerSignal, this, [&, host, port](){
         writeSubMsg(QString("连接服务器成功 [%1:%2]").arg(host).arg(port));
         m_connectedMark = true;
         AbstractTaskContainer::loadHandler(m_invokeArgs);
      }, Qt::DirectConnection);
      connect(m_apiInvoker.data(), &ApiInvoker::connectErrorSignal, this, [&, host, port](ApiInvoker::ErrorType, const QString&){
         writeSubMsg(QString("连接服务器失败 [%1:%2]").arg(host).arg(port));
         m_connectedMark = true;
         m_disconnectedMark = true;
         exitCurrentCommandCycle();
      }, Qt::DirectConnection);
      connect(m_apiInvoker.data(), &ApiInvoker::serverOfflineSignal, this, [&](){
         m_disconnectedMark = true;
         if(!m_activeDisconnected){
            Terminal::writeText("\n");
            writeSubMsg(QString("服务器终止连接"));
            exitCurrentCommandCycle();
         }
      }, Qt::DirectConnection);
   }
   return m_apiInvoker;
}

void UpgradeMgr::quitTask(const TaskMeta&)
{
   m_activeDisconnected = true;
   exitCurrentCommandCycle();
}

}//container
}//cloudcontroller