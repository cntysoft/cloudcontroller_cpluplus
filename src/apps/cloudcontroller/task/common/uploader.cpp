#include <QVariant>
#include <QFileInfo>
#include <QSharedPointer>
#include <QPair>
#include <QFile>
#include <QByteArray>
#include <QCryptographicHash>
#include <QDebug>

#include "uploader.h"

#include "cclib/shell/task_meta.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "cclib/shell/abstract_net_task.h"
#include <iostream>

namespace cloudcontroller{
namespace task{
namespace common{

using sn::corelib::network::ApiInvoker;

Uploader::Uploader(AbstractTaskContainer *taskContainer, const TaskMeta &meta)
   : AbstractNetTask(taskContainer, meta)
{
}

void init_upload_handler(const ApiInvokeResponse &response, void* args)
{
   Uploader *self = static_cast<Uploader*>(args);
   //元信息已经接收,开始上传进程
   if(!response.getStatus()){
      QPair<int, QString> error = response.getError();
      self->emitUploadErrorSignal(error.first, error.second);
      return;
   }
   emit self->beginUploadSignal();
   self->startUploadProcess();
}

void Uploader::emitUploadErrorSignal(int errorCode, const QString errorString)
{
   emit Uploader::uploadErrorSignal(errorCode, errorString);
}

void Uploader::run()
{
   emit prepareSignal();
   QFileInfo fileInfo(m_filename);
   QFile file(m_filename);
   file.open(QIODevice::ReadOnly);
   QByteArray fileContent;
   while(!file.atEnd()){
      fileContent.append(file.read(2048));
   }
   file.close();
   m_totalToBeUpload = fileInfo.size();
   QByteArray md5(QCryptographicHash::hash(fileContent, QCryptographicHash::Md5).toHex());
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   ApiInvokeRequest request("Common/Uploader", "init", {
                               QVariant(m_baseDir),QVariant(m_filename),QVariant(QString(md5)), QVariant(m_totalToBeUpload)
                            });
   
   apiInvoker->request(request, init_upload_handler, (void*)this);
}

void Uploader::clearContext()
{
   m_uploaded = 0;
   m_filename.clear();
}

void Uploader::startUploadProcess()
{
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   QFile file(m_filename);
   file.open(QIODevice::ReadOnly);
   while(!file.atEnd()){
      QByteArray unit = file.read(1024);
      ApiInvokeRequest request("Common/Uploader", "receiveData");
      request.setExtraData(unit.toBase64());
      apiInvoker->request(request);
      m_uploaded += unit.size();
      emit uploadProgressSignal(m_uploaded, m_totalToBeUpload);
   }
}

Uploader& Uploader::setFilename(const QString &filename)
{
   m_filename = filename;
   return *this;
}

Uploader& Uploader::setBaseDir(const QString &baseDir)
{
   m_baseDir = baseDir;
   return *this;
}

Uploader::~Uploader()
{
   clearContext();
}

}//common
}//task
}//cloudcontroller