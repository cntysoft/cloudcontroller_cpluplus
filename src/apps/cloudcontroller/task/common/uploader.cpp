#include <QVariant>
#include <QFileInfo>
#include <QSharedPointer>
#include <QPair>
#include <QMap>
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
   : AbstractNetTask(taskContainer, meta),
     m_uploaded(0),
     m_totalToBeUpload(0),
     m_cycleSize(20)
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

void upload_cycle_handler(const ApiInvokeResponse &response, void* args)
{
   Uploader *self = static_cast<Uploader*>(args);
   //元信息已经接收,开始上传进程
   if(!response.getStatus()){
      QPair<int, QString> error = response.getError();
      self->clearContext();
      self->emitUploadErrorSignal(error.first, error.second);
      return;
   }
   QMap<QString, QVariant> data = response.getData();
   if(data.contains("cycleComplete") && data.value("cycleComplete").toBool() == true){
      self->uploadCycle();
   }
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
                               QVariant(m_baseDir),QVariant(m_filename),QVariant(QString(md5)), QVariant(m_totalToBeUpload), QVariant(m_cycleSize)
                            });
   
   apiInvoker->request(request, init_upload_handler, (void*)this);
   m_eventLoop.exec();
}

void Uploader::clearContext()
{
   m_uploaded = 0;
   m_filename.clear();
   m_uploadFile.close();
}

void Uploader::startUploadProcess()
{
   
   m_uploadFile.setFileName(m_filename);
   m_uploadFile.open(QIODevice::ReadOnly);
   uploadCycle();
   //   while(!file.atEnd()){
   //      QByteArray unit = file.read(1024);
   //      ApiInvokeRequest request("Common/Uploader", "receiveData");
   //      request.setExtraData(unit.toBase64());
   //      apiInvoker->request(request);
   //      m_uploaded += unit.size();
   //      emit uploadProgressSignal(m_uploaded, m_totalToBeUpload);
   //   }
}

void Uploader::uploadCycle()
{
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   for(int i = 0; i < m_cycleSize; i++){
      if(!m_uploadFile.atEnd()){
         QByteArray unit = m_uploadFile.read(1024);
         ApiInvokeRequest request("Common/Uploader", "receiveData");
         request.setExtraData(unit.toBase64());
         apiInvoker->request(request, upload_cycle_handler, (void*)this);
         m_uploaded += unit.size();
         emit uploadProgressSignal(m_uploaded, m_totalToBeUpload);
      }
   }
}

Uploader& Uploader::setCycleSize(int cycleSize)
{
   m_cycleSize = cycleSize;
   return *this;
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