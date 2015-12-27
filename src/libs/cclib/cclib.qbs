import qbs 1.0
Product
{
   type: "dynamiclibrary"
   name : "cclib"
   targetName : "cloudcontroler"
   Depends { 
      name: "Qt"; 
      submodules: ["core"]
   }
   Depends { name:"corelib"}
   Depends { name:"cpp"}
   destinationDirectory: "lib"
   cpp.defines: {
      var defines = [];
      if(product.type == "staticlibrary"){
         defines.push("CLOUD_CONTROLLER_STATIC_LIB");
      }else{
         defines.push("CLOUD_CONTROLLER_LIBRARY");
      }
      defines = defines.concat([
                                  'CLOUD_CONTROLLER_LIB_VERSION="'+ version+'"'
                               ]);
      return defines;
   }
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
   cpp.includePaths:[".","../"]
   version : "0.1.1"
   Export {
      Depends { name: "cpp" }
      Depends { name: "Qt"; submodules: ["core"] }
      cpp.rpaths: ["$ORIGIN/../lib"]
      cpp.includePaths: [product.sourceDirectory]
   }
   Group {
      fileTagsFilter: product.type.concat("dynamiclibrary_symlink")
      qbs.install: true
      qbs.installDir: "lib"
   }
   
   Group {
      name: "global"
      prefix: name+"/"
      files: [
           "global.h",
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
         "abstract_task_loop.cpp",
         "abstract_task_loop.h",
         "task_meta.cpp",
         "task_meta.h",
      ]
   }
   
   Group {
      name: "network"
      prefix: name+"/"
      files: [
      ]
   }
}