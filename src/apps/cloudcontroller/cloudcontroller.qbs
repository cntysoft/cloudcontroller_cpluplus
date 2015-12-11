import qbs 1.0
CloudControllerApplication
{
   name : "cloudcontroller"
   cpp.includePaths: base.concat(["."])
   cpp.defines: base.concat([
                               'CLOUD_CONTROLLER_VERSION="' + project.ccVersion + '"'
                            ])
   
   files:[
      "main.cpp"
   ]
}