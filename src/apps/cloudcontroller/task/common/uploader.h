#ifndef CLOUD_CONTROLLER_TASK_UPGRADEMGR_COMMON_UPLOADER_H
#define CLOUD_CONTROLLER_TASK_UPGRADEMGR_COMMON_UPLOADER_H

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
public:
   Uploader(AbstractTaskContainer *taskContainer, const TaskMeta &meta);
   Uploader& setFilename(const QString &filename);
   Uploader& setBaseDir(const QString &baseDir);
public:
   virtual void run();
   virtual ~Uploader();
signals:
   void beginUploadSignal(const QString &filename);
protected:
   QString m_filename;
   QString m_baseDir;
};

}//common
}//task
}//cloudcontroller

#endif // CLOUD_CONTROLLER_TASK_UPGRADEMGR_COMMON_UPLOADER_H
