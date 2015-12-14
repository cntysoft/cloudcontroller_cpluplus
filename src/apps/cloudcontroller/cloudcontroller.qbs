import qbs 1.0
CloudControllerApplication
{
   name : "cloudcontroller"
   cpp.includePaths: base.concat(["."])
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
       ]
   }  
}