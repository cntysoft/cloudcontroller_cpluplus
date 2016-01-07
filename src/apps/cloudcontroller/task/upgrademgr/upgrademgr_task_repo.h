#ifndef CLOUD_CONTROLLER_TASK_UPGRADEMGR_UPGRADEMGR_TASK_REPO_H
#define CLOUD_CONTROLLER_TASK_UPGRADEMGR_UPGRADEMGR_TASK_REPO_H

#include "softwarerepo/list_repo.h"
#include "softwarerepo/upload_software.h"
#include "upgrader/upgrade_upgrademgr.h"
#include "serverstatus/server_status.h"

#define USE_SOFTWARE_REPO_TASK_LIST \
using SoftwareRepoListRepoTask = cloudcontroller::task::upgrademgr::softwarerepo::ListRepo;\
using SoftwareRepoUploadSoftware = cloudcontroller::task::upgrademgr::softwarerepo::UploadSoftware;\
using UpgraderUpgradeUpgrademgr = cloudcontroller::task::upgrademgr::upgrader::UpgradeUpgrademgr;\
using ServerStatusServerVersion = cloudcontroller::task::upgrademgr::serverstatus::ServerVersion;
#endif // CLOUD_CONTROLLER_TASK_UPGRADEMGR_UPGRADEMGR_TASK_REPO_H
