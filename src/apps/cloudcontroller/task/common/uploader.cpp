#include "uploader.h"

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

Uploader::Uploader(AbstractTaskContainer *taskContainer, const TaskMeta &meta)
   : AbstractNetTask(taskContainer, meta)
{
   
}

void Uploader::run()
{
   qDebug() << "开始上传";
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