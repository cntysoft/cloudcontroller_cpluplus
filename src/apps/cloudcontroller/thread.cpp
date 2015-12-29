#include "thread.h"
#include "corelib/network/rpc/invoke_meta.h"
#include <QDataStream>
#include <QObject>
#include <QTimer>
using sn::corelib::network::ApiInvokeRequest;

class HandlerCls : public QObject
{
public:
   HandlerCls(TaskRunnerThread* thread);
public slots:
   void dataReady();
   TaskRunnerThread  *m_thread;
};

HandlerCls::HandlerCls(TaskRunnerThread* thread)
{
   m_thread = thread;
}

void HandlerCls::dataReady()
{
   //   QTcpSocket *socket = (QTcpSocket*)sender();
   //   qDebug() << "xxx";
   //   qDebug() << socket->readAll();
}

void TaskRunnerThread::f()
{
   qDebug() << "connected";
   qDebug() << "f"<< QThread::currentThreadId();
//   QTcpSocket *m_socket = (QTcpSocket*)sender();
//   int i = 0;
//   while(i < 2){
//      ApiInvokeRequest request("meta", "ls");
//      QDataStream out(m_socket);
//      out << request;
//      m_socket->write("\r\n");
//      m_socket->flush();
//      i++;
//   }
}


void TaskRunnerThread::run()
{
   qDebug() << "thread"<<QThread::currentThreadId();
   m_socket = new QTcpSocket;
//   //   HandlerCls *m_handler = new HandlerCls(this);
      connect(m_socket, &QTcpSocket::connected, this, [=](){
         qDebug() << "connected";
         qDebug() << "f"<< QThread::currentThreadId();
         int i = 0;
         while(i < 2){
            ApiInvokeRequest request("meta", "ls");
            QDataStream out(m_socket);
            out << request;
            m_socket->write("\r\n");
            m_socket->flush();
            i++;
         }
      });
   
   //connect(m_socket, &QTcpSocket::connected, this, &TaskRunnerThread::f);
      int i = 0;
   //////   connect(m_socket, &QTcpSocket::readyRead, m_handler, &HandlerCls::dataReady);
      connect(m_socket, &QTcpSocket::readyRead, this, [&](){
         QByteArray d = m_socket->readAll();
//         i+= m_socket->readAll().size();
//         qDebug() << i;
         qDebug() << d;
         qDebug() << d.size();
         exit(0);
      });
       QTimer::singleShot(0, this, &TaskRunnerThread::process);
   //m_socket->connectToHost("127.0.0.1", 7777);
   int status = exec();
   return;
}

void TaskRunnerThread::process()
{
   qDebug() << "process"<< QThread::currentThreadId();
   m_socket->connectToHost("127.0.0.1", 7777);
   qDebug() << "xiuxiux";
//   exit();
}

TaskRunnerThread::~TaskRunnerThread()
{}