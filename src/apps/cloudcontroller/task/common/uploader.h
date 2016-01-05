#ifndef CLOUD_CONTROLLER_TASK_UPGRADEMGR_COMMON_UPLOADER_H
#define CLOUD_CONTROLLER_TASK_UPGRADEMGR_COMMON_UPLOADER_H

#include <QFile>

#include "cclib/shell/task_meta.h"
#include "corelib/network/rpc/invoke_meta.h"
#include "cclib/shell/abstract_net_task.h"

namespace cloudcontroller{
namespace task{
namespace common{

using cclib::shell::AbstractNetTask;
using cclib::shell::AbstractTaskContainer;
using cclib::shell::TaskMeta;
using sn::corelib::network::ApiInvokeRequest;
using sn::corelib::network::ApiInvokeResponse;

class Uploader : public AbstractNetTask
{
   Q_OBJECT
protected:
   friend void init_upload_handler(const ApiInvokeResponse &response, void* args);
   friend void upload_cycle_handler(const ApiInvokeResponse &response, void* args);
   friend void check_uploadfile_handler(const ApiInvokeResponse &response, void* args);
public:
   Uploader(AbstractTaskContainer *taskContainer, const TaskMeta &meta);
   Uploader& setFilename(const QString &filename);
   Uploader& setBaseDir(const QString &baseDir);
   Uploader& setCycleSize(int cycleSize);
public:
   virtual void run();
   virtual ~Uploader();
protected:
   void clearContext();
   void startUploadProcess();
   void uploadCycle();
   void checkUploadFile();
protected:
   void emitUploadErrorSignal(int errorCode, const QString errorString);
   void emitBeginUploadSignal();
   void emitUploadProgressSignal(quint64 uploaded, quint64 total);
   void emitUploadSuccessSignal();
signals:
   void prepareSignal();
   void beginUploadSignal();
   void uploadSuccessSignal();
   void uploadProgressSignal(quint64 uploaded, quint64 total);
   void checkUploadFileSignal();
   void uploadErrorSignal(int errorCode, const QString &errorString);
protected:
   QString m_filename;
   QString m_baseDir;
   quint64 m_uploaded;
   quint64 m_totalToBeUpload;
   QFile m_uploadFile;
   int m_cycleSize;
};

}//common
}//task
}//cloudcontroller

#endif // CLOUD_CONTROLLER_TASK_UPGRADEMGR_COMMON_UPLOADER_H
