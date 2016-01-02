#include "initializer/initializer_cleanup_funcs.h"

namespace cloudcontroller{

void global_initializer()
{
   init_metatypes();
}

}//cloudcontroller
