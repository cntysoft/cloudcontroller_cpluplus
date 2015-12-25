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
      "main.cpp",
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
           "abstract_task.cpp",
           "abstract_task.h",
           "abstract_task_container.cpp",
           "abstract_task_container.h",
           "task_loop.cpp",
           "task_loop.h",
           "task_meta.cpp",
           "task_meta.h",
       ]
   }
   
   Group {
      name: "task"
      prefix: name+"/"
      files:[
           "global/metaserver.cpp",
           "global/metaserver.h",
           "upgrademgr/entry.cpp",
           "upgrademgr/entry.h",
       ]
   }
   
   Group{
      name: "container"
      prefix: name+"/"
      files:[
           "global.cpp",
           "global.h",
           "metaserver.cpp",
           "metaserver.h",
           "upgrademgr.cpp",
           "upgrademgr.h",
       ]
   }
}