#include "tri/async_handle.hpp"

#include <uv.h>

namespace {

} // namespace.

namespace tri {

AsyncHandle::~AsyncHandle( ) {

}

void AsyncHandle::GlobalCloseCB( uv_handle_t *uv ) {
    AsyncHandle self( uv );
    auto &details = self.details( );

    if( details.closeCB ) {
        details.closeCB( self );
    }

    details.freeResources( uv );
}

} // namespace tri.
