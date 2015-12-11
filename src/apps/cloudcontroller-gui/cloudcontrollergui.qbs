import qbs 1.0
CloudControllerGuiApplication
{
    files: [
        "main.cpp",
    ]
   name : "cloudcontroller-gui"
   Depends { 
      name: "Qt"; 
      submodules: [
         "sql",
         "network"
      ]
   }
   Depends { name: "corelib"}
   cpp.includePaths: base.concat([
                                    "."
                                 ])
   cpp.defines: base.concat([
                               'META_SERVER_VERSION="' + project.rmmgrVersion + '"'
                            ])
}