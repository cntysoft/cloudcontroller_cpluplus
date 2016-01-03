#include <QVariant>
#include <QFileInfo>
#include <QSharedPointer>
#include <QPair>

#include "uploader.h"

#include "cclib/shell/task_meta.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "cclib/shell/abstract_net_task.h"

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
   self->startUploadProcess();
}

void Uploader::emitUploadErrorSignal(int errorCode, const QString errorString)
{
   emit Uploader::uploadErrorSignal(errorCode, errorString);
}

void Uploader::run()
{
   emit beginUploadSignal();
   QFileInfo fileInfo(m_filename);
   QSharedPointer<ApiInvoker>& apiInvoker = getApiInvoker();
   ApiInvokeRequest request("Common/Uploader", "init", {
                               QVariant(m_baseDir),QVariant(m_filename),QVariant(fileInfo.size())
                            });
   apiInvoker->request(request, init_upload_handler, (void*)this);
   waitForResponse(request);
}

void Uploader::startUploadProcess()
{
   
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
   
}

}//common
}//task
}//cloudcontroller