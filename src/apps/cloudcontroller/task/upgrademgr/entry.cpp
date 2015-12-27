#include <QMap>
#include <QTcpSocket>
#include <QDebug>

#include "entry.h"
#include "shell/task_loop.h"

#include "corelib/kernel/errorinfo.h"
#include "corelib/io/terminal.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{

using cloudcontroller::shell::TaskLoop;
using sn::corelib::ErrorInfo;
using sn::corelib::Terminal;

EntryTask::EntryTask(AbstractTaskContainer& taskContainer, const TaskMeta& meta)
   :AbstractTask(taskContainer, meta)
{
}

void EntryTask::run()
{
   QMap<QString, QString> args = m_invokeMeta.getTaskArgs();
   QString host(args.value("host"));
   int port = args.value("port").toInt();
   m_client = new QTcpSocket;
   m_client->connectToHost(host, port);
   Terminal::writeText("正在连接服务器 ... \n");
   if(m_client->waitForConnected()){
      m_taskContainer.getTaskLoop().enterTaskContainer("UpgradeMgr");
   }else{
      throw ErrorInfo(m_client->errorString());
   }
}

EntryTask::~EntryTask()
{
   if(nullptr != m_client){
      delete m_client;
   }
}

}//global
}//task
}//cloudcontroller