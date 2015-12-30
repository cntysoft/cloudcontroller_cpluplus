#include <QDebug>
#include <QSharedPointer>
#include <QTcpSocket>
#include <QDataStream>
#include "list_repo.h"
#include "shell/abstract_task_container.h"

#include "corelib/network/rpc/invoke_meta.h"
#include "shell/task_runner_worker.h"
#include <QThread>
namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace softwarerepo{

using sn::corelib::network::ApiInvokeRequest;
using cclib::shell::TaskRunnerWorker;
ListRepo::ListRepo(AbstractTaskContainer *taskContainer, const cclib::shell::TaskMeta &meta)
   : AbstractTask(taskContainer, meta)
{
}

void ListRepo::run()
{
//   UpgradeMgrContainer* taskContainer = (UpgradeMgrContainer*)m_taskContainer;
//   TaskRunnerWorker *worker = new TaskRunnerWorker(taskContainer->m_client.data());
   
//   QThread *thread = new QThread;
//   worker->moveToThread(thread);
//   taskContainer->m_client.data()->moveToThread(thread);
//   QObject::connect(this, &ListRepo::beginTaskWorker, worker, &TaskRunnerWorker::beginRunTask);
//   QObject::connect(thread, &QThread::finished, worker, &TaskRunnerWorker::deleteLater);
//   thread->start();
//   emit beginTaskWorker();
////   thread->wait();
////   qDebug() << "xiuxiux";
//   qDebug() << QThread::currentThreadId();
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   ApiInvokeRequest request("meta", "ls");
   apiInvoker->request(request);
//////   qDebug() << "thread"<<QThread::currentThreadId();
////   QTcpSocket* m_socket = new QTcpSocket;
//////   //   HandlerCls *m_handler = new HandlerCls(this);
////      connect(m_socket, &QTcpSocket::connected, this, [=](){
////         qDebug() << "connected";
////         qDebug() << "f"<< QThread::currentThreadId();
////         int i = 0;
////         while(i < 2){
////            ApiInvokeRequest request("meta", "ls");
////            QDataStream out(m_socket);
////            out << request;
////            m_socket->write("\r\n");
////            m_socket->flush();
////            i++;
////         }
////      });
   
////   //connect(m_socket, &QTcpSocket::connected, this, &TaskRunnerThread::f);
////      int i = 0;
////   //////   connect(m_socket, &QTcpSocket::readyRead, m_handler, &HandlerCls::dataReady);
////      connect(m_socket, &QTcpSocket::readyRead, this, [&](){
////         QByteArray d = m_socket->readAll();
//////         i+= m_socket->readAll().size();
//////         qDebug() << i;
////         qDebug() << d;
////         qDebug() << d.size();
////         exit(0);
////      });
////   m_socket->connectToHost("127.0.0.1", 7777);
////   //QThread::currentThread()->exit(0);
}

}//softwarerepo
}//upgrade
}//task
}//cloudcontroller