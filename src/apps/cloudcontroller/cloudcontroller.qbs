import qbs 1.0
CloudControllerApplication
{
   name : "cloudcontroller"
   cpp.includePaths: base.concat([".", "../../libs"])
   cpp.defines: base.concat([
                               'CLOUD_CONTROLLER_VERSION="' + project.ccVersion + '"'
                            ])
   
   files:[
        "application.cpp",
        "application.h",
        "command_runner.cpp",
        "command_runner.h",
        "const.h",
        "main.cpp",
        "thread.cpp",
        "thread.h",
    ]
   
   Group {
      name: "command"
      prefix: name+"/"
      files: [
         "command_repo.h",
         "global_shell_command.cpp",
         "global_shell_command.h",
         "global_version_command.cpp",
         "global_version_command.h",
      ]
   }
   
   Group {
      name: "shell"
      prefix: name+"/"
      files:[
         "task_loop.cpp",
         "task_loop.h"
      ]
   }
   
   Group {
      name: "task"
      prefix: name+"/"
      files:[
           "global/enter_upgrademgr_task.cpp",
           "global/enter_upgrademgr_task.h",
           "upgrademgr/abstract_task.cpp",
           "upgrademgr/abstract_task.h",
           "upgrademgr/softwarerepo/list_repo.cpp",
           "upgrademgr/softwarerepo/list_repo.h",
           "upgrademgr/upgrademgr_task_repo.h",
       ]
   }
   
   Group {
      name: "container"
      prefix: name+"/"
      files:[
           "global.cpp",
           "global.h",
           "upgrademgr.cpp",
           "upgrademgr.h",
       ]
   }
   
   Group {
      name: "utils"
      prefix: name+"/"
      files:[
           "default_cfg_initializer.cpp",
       ]
   }
}