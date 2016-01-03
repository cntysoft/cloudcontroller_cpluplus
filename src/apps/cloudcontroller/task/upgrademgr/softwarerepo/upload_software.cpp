#include <QSharedPointer>
#include <QTcpSocket>
#include <QDataStream>
#include <QStringList>
#include <QThread>
#include <iostream>

#include "upload_software.h"
#include "shell/abstract_task_container.h"
#include "shell/task_runner_worker.h"

#include "corelib/io/filesystem.h"
#include "corelib/kernel/errorinfo.h"

#include "task/common/uploader.h"

namespace cloudcontroller{
namespace task{
namespace upgrademgr{
namespace softwarerepo{

using sn::corelib::network::ApiInvokeRequest;
using sn::corelib::network::ApiInvokeResponse;
using sn::corelib::network::ApiInvoker;
using sn::corelib::Filesystem;
using sn::corelib::ErrorInfo;
using cloudcontroller::task::common::Uploader;
using cclib::shell::TaskRunnerWorker;

UploadSoftware::UploadSoftware(AbstractTaskContainer *taskContainer, const cclib::shell::TaskMeta &meta)
   : AbstractNetTask(taskContainer, meta)
{
}

void UploadSoftware::run()
{
   QMap<QString, QString> args = m_invokeMeta.getTaskArgs();
   QString filename = args.value("file");
   if(!Filesystem::fileExist(filename)){
      throw ErrorInfo(QString("指定文件：%1不存在").arg(filename).toLocal8Bit());
   }
   Uploader uploader(m_taskContainer, m_invokeMeta);
   uploader.setApiInvoker(getApiInvoker());
   uploader.setFilename(filename);
   //设置相关的事件绑定
   uploader.run();
}

}//softwarerepo
}//upgrade
}//task
}//cloudcontroller