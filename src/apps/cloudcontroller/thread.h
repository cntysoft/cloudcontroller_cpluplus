#ifndef THREAD
#define THREAD
#include <QThread>
#include <QTcpSocket>


class  TaskRunnerThread : public QThread
{
protected:
   void run() Q_DECL_OVERRIDE;
public:
   ~TaskRunnerThread();
   
public slots:
   void f();
   void process();
//protected:
   QTcpSocket *m_socket;
//   HandlerCls *m_handler;
};

#endif // THREAD

