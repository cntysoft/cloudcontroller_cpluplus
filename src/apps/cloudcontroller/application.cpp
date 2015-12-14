#include "application.h"

namespace cloudcontroller 
{

Application::Application(int &argc, char **argv)
   : BaseApplication(argc, argv)
{
   setApplicationName("cloudcontroller");
}

Application::~Application()
{}

}//metaserver