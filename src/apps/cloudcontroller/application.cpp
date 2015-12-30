#include <csignal>
#include <cerrno>
#include <QDebug>
#include <QThread>

#include "application.h"

#include "corelib/kernel/errorinfo.h"
#include <QTcpSocket>
#include <QDataStream>
#include "corelib/network/rpc/invoke_meta.h"

#include "shell/task_runner_worker.h"

namespace cloudcontroller{

using sn::corelib::ErrorInfo;
using sn::corelib::network::ApiInvokeRequest;
using cclib::shell::TaskRunnerWorker;

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
//      TaskRunnerWorker *worker = new TaskRunnerWorker();
//      QThread *thread = new QThread;
//      worker->moveToThread(thread);
//      QObject::connect(this, &Application::beginTaskWorker, worker, &TaskRunnerWorker::beginRunTask);
//      QObject::connect(thread, &QThread::finished, worker, &TaskRunnerWorker::deleteLater);
//      thread->start();
//      emit beginTaskWorker();
//      thread->wait();

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