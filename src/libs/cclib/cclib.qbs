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
   Depends { name:"cpp"}
   destinationDirectory: "lib"
   cpp.defines: base.concat(type == "staticlibrary" ? ["SE_STATIC_LIB"] : ["SE_LIBRARY"])
   cpp.visibility: "minimal"
   cpp.cxxLanguageVersion: "c++14"
   version : "0.1.1"
   cpp.includePaths: base.concat([
                                    "."
                                 ])
   Group {
      name: "network"
      prefix: name+"/"
      files: [
      ]
   }
}