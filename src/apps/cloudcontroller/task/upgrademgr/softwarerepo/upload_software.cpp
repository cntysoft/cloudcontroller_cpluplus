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
#include "corelib/io/terminal.h"

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
using sn::corelib::TerminalColor;
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
   m_filename = filename;
   Uploader uploader(m_taskContainer, m_invokeMeta);
   uploader.setApiInvoker(getApiInvoker());
   uploader.setFilename(filename);
   //设置相关的事件绑定
   connect(&uploader, &Uploader::prepareSignal, this, &UploadSoftware::prepareUploadHandler);
   connect(&uploader, &Uploader::beginUploadSignal, this, &UploadSoftware::startUploadHandler);
   connect(&uploader, &Uploader::uploadErrorSignal, this, &UploadSoftware::uploadErrorHandler);
   connect(&uploader, &Uploader::uploadProgressSignal, this, &UploadSoftware::uploadProcessHandler);
   connect(&uploader, &Uploader::checkUploadFileSignal, this, &UploadSoftware::checkUploadSumHandler);
   connect(&uploader, &Uploader::uploadSuccessSignal, this, &UploadSoftware::uploadSuccessHandler);
   uploader.run();
}

void UploadSoftware::prepareUploadHandler()
{
   writeSubMsg(QString("开始计算文件 : %1大小和MD5校验数据\n").arg(m_filename));
}

void UploadSoftware::startUploadHandler()
{
   writeSubMsg(QString("开始上传文件 : %1\n").arg(m_filename));
   beginReplaceMode();
}

void UploadSoftware::uploadProcessHandler(quint64 uploaded, quint64 total)
{
   writeSubMsg(QString("上传进度 : %1%\n").arg(100 * ((float)uploaded / (float)total)));
}

void UploadSoftware::checkUploadSumHandler()
{
   beginReplaceMode();
   writeSubMsg(QString("服务器开始进行MD5数据校验\n"));
}

void UploadSoftware::uploadSuccessHandler()
{
   writeSubMsg(QString("上传成功\n"));
}

void UploadSoftware::uploadErrorHandler(int, const QString &errorString)
{
   writeSubMsg(QString("上传文件错误 : %1\n").arg(errorString), TerminalColor::Red);
}

}//softwarerepo
}//upgrade
}//task
}//cloudcontroller