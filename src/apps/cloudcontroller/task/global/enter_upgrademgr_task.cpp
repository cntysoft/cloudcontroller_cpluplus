#include <QMap>

#include "const.h"
#include "enter_upgrademgr_task.h"
#include "shell/task_loop.h"

#include "corelib/kernel/errorinfo.h"
#include "corelib/io/terminal.h"
#include "corelib/kernel/settings.h"

#include "container/upgrademgr.h"

namespace cloudcontroller{
namespace task{
namespace global{

using cloudcontroller::shell::TaskLoop;
using sn::corelib::ErrorInfo;
using sn::corelib::Terminal;
using sn::corelib::Settings;
using UpgradeMgrContainer = cloudcontroller::container::UpgradeMgr;

EnterUpgradeMgrTask::EnterUpgradeMgrTask(AbstractTaskContainer* taskContainer, const TaskMeta& meta)
   :AbstractTask(taskContainer, meta)
{
}

void EnterUpgradeMgrTask::run()
{
   m_taskContainer->getTaskLoop().enterTaskContainer("UpgradeMgr", m_invokeMeta.getTaskArgs());
}

}//global
}//task
}//cloudcontroller