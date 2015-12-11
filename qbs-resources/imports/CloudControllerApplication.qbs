import qbs 1.0
Product {
   type : "application"
   Depends { 
      name: "Qt"; 
      submodules: ["core","network"]
   }
   Depends {name:"cpp"}
   Depends { name: "corelib"}
   Depends { name: "cclib"}
   consoleApplication: true
   destinationDirectory: "bin"
   cpp.cxxLanguageVersion: "c++14"
   cpp.defines: base.concat([
                               'CLOUD_CONTROLLER_INSTALL_ROOT="' + qbs.installRoot + '"',
                               'CLOUD_CONTROLLER_SHARE_RES_DIR="' + qbs.installRoot+'/'+project.resourcesInstallDir+ '"'
                            ])
   Group {
      fileTagsFilter: product.type
      qbs.install: true
      qbs.installDir: project.appInstallDir
   }
}
