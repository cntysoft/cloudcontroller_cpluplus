#include <QThread>
#include <QTcpSocket>
#include "task_runner_worker.h"
#include "corelib/kernel/errorinfo.h"
#include "corelib/io/terminal.h"
#include "corelib/network/rpc/invoke_meta.h"
#include <QDataStream>

#include <QDebug>
namespace cclib{
namespace shell{

using sn::corelib::ErrorInfo;
using sn::corelib::Terminal;
using sn::corelib::network::ApiInvokeRequest;

TaskRunnerWorker::TaskRunnerWorker(QTcpSocket *socket)
//   :m_taskContainer(taskContainer)
   :m_socket(socket)
{
   qDebug() << QThread::currentThreadId();
}

void TaskRunnerWorker::beginRunTask()
{
   qDebug() << QThread::currentThreadId();
   qDebug()  << m_socket->isValid();
//   m_socket->moveToThread(QThread::currentThread());
//      QTcpSocket* client = new QTcpSocket;
      connect(m_socket, &QTcpSocket::connected, this, [=](){
         qDebug() << "connected";
         int i = 0;
         while(i < 3){
            ApiInvokeRequest request("meta", "ls");
            QDataStream out(m_socket);
            out << request;
            m_socket->write("\r\n");
            m_socket->flush();
            i++;
         }
         
      }, Qt::QueuedConnection);
     
      int i = 0;
      while(i < 3){
         ApiInvokeRequest request("meta", "ls");
         QDataStream out(m_socket);
         out << request;
         m_socket->write("\r\n");
         m_socket->flush();
         i++;
      }
      connect(m_socket, &QTcpSocket::readyRead, this, [=](){
         qDebug() << m_socket->readAll();
      }, Qt::QueuedConnection);
//      client->connectToHost("127.0.0.1", 7777);
//   try{
//      qDebug() << "thread" << QThread::currentThreadId();
//      m_taskContainer->runTask(m_taskMeta);
//   }catch(ErrorInfo errorInfo){
//      QString str(errorInfo.toString());
//      if(str.size() > 0){
//         str += '\n';
//         Terminal::writeText(str.toLocal8Bit(), TerminalColor::Red);
//      }
//      QThread::currentThread()->exit(0);
//   }
}

//TaskRunnerWorker& TaskRunnerWorker::setTaskMeta(const TaskMeta &meta)
//{
//   m_taskMeta = meta;
//   return *this;
//}


TaskRunnerWorker::~TaskRunnerWorker()
{
  
}

}//shell
}//cclib