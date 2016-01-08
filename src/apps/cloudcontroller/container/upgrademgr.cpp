#include <QString>
#include <QDebug>
#include <QThread>
#include <signal.h>
#include <unistd.h>

#include "const.h"
#include "corelib/io/terminal.h"
#include "corelib/kernel/settings.h"
#include "corelib/kernel/errorinfo.h"

#include "upgrademgr.h"
#include "cclib/shell/task_meta.h"
#include "cclib/shell/abstract_task.h"
#include "cclib/shell/abstract_net_task.h"

#include "task/upgrademgr/upgrademgr_task_repo.h"


namespace cloudcontroller{
namespace container{

using sn::corelib::TerminalColor;
using sn::corelib::Terminal;
using sn::corelib::Settings;
using sn::corelib::ErrorInfo;
using cclib::shell::AbstractNetTask;
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
      QString key(meta.getContainer()+ '_' +meta.getCategory() + '_' + meta.getName());
      Q_ASSERT_X(m_taskRegisterPool.contains(key), "AbstractTaskContainer::run()", QString("command : %1 is not exist").arg(key).toLatin1());
      AbstractTask* (*initializer)(AbstractTaskContainer*, const TaskMeta&) = m_taskRegisterPool[key];
      QScopedPointer<AbstractNetTask> task(static_cast<AbstractNetTask*>(initializer(this, meta)));
      task->setApiInvoker(getApiInvoker());
      task->run();
   }
}

void UpgradeMgr::initUsage()
{
   addUsageText("you can use commands as fowllow:\n", TerminalColor::LightGreen);
   addUsageText("list_software_repo   查看当前软件仓库里面的软件种类\n");
   addUsageText("upload_software --file=<需要上传的文件路径>   上传指定的软件到软件仓库\n");
   addUsageText("upgrade_upgrademgr 更新更新管理程序自己\n");
   addUsageText("master_server_version 获取主更新程序的版本号\n");
   addUsageText("quit   退出更新管理程序\n");
}

void UpgradeMgr::initRouter()
{
   addTaskRoute("quit", "quit", 1, {
                   {"category", "UpgradeMgr"},
                   {"name", "Quit"}
                });
   addTaskRoute("help", "help", 1, {
                   {"category", "UpgradeMgr"},
                   {"name", "Help"}
                });
   addTaskRoute("softwarerepolistrepo", "list_software_repo", 1, {
                   {"category", "SoftwareRepo"},
                   {"name", "ListRepo"}
                });
   addTaskRoute("softwarerepouploadsoftware", "upload_software --file=", 1, {
                   {"category", "SoftwareRepo"},
                   {"name", "UploadSoftware"}
                });
   addTaskRoute("upgraderupgradeupgrademgr", "upgrade_upgrademgr --version=", 1, {
                   {"category", "Upgrader"},
                   {"name", "UpgradeUpgrademgr"}
                });
   addTaskRoute("serverstatusserverversion", "master_server_version", 1, {
                   {"category", "ServerStatus"},
                   {"name", "ServerVersion"}
                });
   
}

void UpgradeMgr::initTaskPool()
{
   m_taskRegisterPool.insert("UpgradeMgr_SoftwareRepo_ListRepo", [](AbstractTaskContainer* container, const TaskMeta& meta)->AbstractTask*{
      SoftwareRepoListRepoTask* task = new SoftwareRepoListRepoTask(container, meta);
      return task;
   });
   m_taskRegisterPool.insert("UpgradeMgr_SoftwareRepo_UploadSoftware", [](AbstractTaskContainer* container, const TaskMeta& meta)->AbstractTask*{
      SoftwareRepoUploadSoftware* task = new SoftwareRepoUploadSoftware(container, meta);
      return task;
   });
   m_taskRegisterPool.insert("UpgradeMgr_Upgrader_UpgradeUpgrademgr", [](AbstractTaskContainer* container, const TaskMeta& meta)->AbstractTask*{
      UpgraderUpgradeUpgrademgr* task = new UpgraderUpgradeUpgrademgr(container, meta);
      return task;
   });
   m_taskRegisterPool.insert("UpgradeMgr_ServerStatus_ServerVersion", [](AbstractTaskContainer* container, const TaskMeta& meta)->AbstractTask*{
      ServerStatusServerVersion* task = new ServerStatusServerVersion(container, meta);
      return task;
   });
}

bool UpgradeMgr::dispatchBuildInTask(const TaskMeta& meta)
{
   QString key(meta.getCategory()+'_'+meta.getName());
   if(key == "UpgradeMgr_Quit"){
      quitTask(meta);
      return true;
   }else if(key == "UpgradeMgr_Help"){
      helpTask(meta);
      return true;
   }
   return false;
}

void UpgradeMgr::loadHandler(const QMap<QString, QString> &invokeArgs)
{
   
   m_invokeArgs = invokeArgs;
   QSharedPointer<ApiInvoker>& invoker = getApiInvoker();
   writeSubMsg("正在连接服务器 ... ");
   m_activeDisconnected = true;
   invoker->connectToServer();
   while(!m_connectedWaitMark){
      QThread::usleep(100);
   }
   m_connectedWaitMark = false;
}

void UpgradeMgr::unloadHandler()
{
   if(m_needWriteDisconnectMsg){
      if(SIGINT == m_app.getCatchedSignalNumber()){
         Terminal::writeText("\n");
      }
      writeSubMsg("正在断开连接 ... ");
   }
   QSharedPointer<ApiInvoker>& invoker = getApiInvoker();
   invoker->disconnectFromServer();
   AbstractTaskContainer::unloadHandler();
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
         m_connectedWaitMark = true;
         AbstractTaskContainer::loadHandler(m_invokeArgs);
      }, Qt::DirectConnection);
      connect(m_apiInvoker.data(), &ApiInvoker::connectErrorSignal, this, [&, host, port](ApiInvoker::ErrorType, const QString&){
         writeSubMsg(QString("连接服务器失败 [%1:%2]").arg(host).arg(port));
         m_connectedWaitMark = true;
         m_needWriteDisconnectMsg = false;
         exitCurrentCommandCycle();
      }, Qt::DirectConnection);
      connect(m_apiInvoker.data(), &ApiInvoker::serverOfflineSignal, this, [&](){
         m_activeDisconnected = false;
         m_connectedWaitMark = true;
         Terminal::writeText("\n");
         writeSubMsg(QString("服务器终止连接"));
         m_taskLoop.exitRequest();
         kill(getpid(), SIGUSR1);
      }, Qt::DirectConnection);
   }
   return m_apiInvoker;
}

void UpgradeMgr::quitTask(const TaskMeta&)
{
   m_activeDisconnected = true;
   exitCurrentCommandCycle();
}
void UpgradeMgr::helpTask(const TaskMeta&)
{
   printUsage();
}

}//container
}//cloudcontroller