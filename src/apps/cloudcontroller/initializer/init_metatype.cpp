#include <QObject>

#include "corelib/network/rpc/invoke_meta.h"
#include "initializer_cleanup_funcs.h"

namespace cloudcontroller{

using sn::corelib::network::ApiInvokeResponse;
using sn::corelib::network::ApiInvokeRequest;

void init_metatypes()
{
   qRegisterMetaType<ApiInvokeResponse>("ApiInvokeResponse");
   qRegisterMetaType<ApiInvokeRequest>("ApiInvokeRequest");
}

}//cloudcontroller