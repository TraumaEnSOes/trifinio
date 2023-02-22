#include "tri/async_request.hpp"
#include "tri/details/uv_wrapper.hpp"

#include <uv.h>

struct AsyncRequestPrivate : public tri::details::UvWrapperPrivate {
};

static inline uv_req_t *uvrequest( void *ptr ) {
    return static_cast< uv_req_t * >( ptr );
}
static inline const uv_req_t *uvrequest( const void *ptr ) {
    return static_cast< const uv_req_t * >( ptr );
}

namespace tri {

bool AsyncRequest::cancel( ) noexcept {

    int result = uv_cancel( uvrequest( uv( ) ) );

    return result < 0 ? false : true;
}

AsyncRequest::Type AsyncRequest::type( ) const noexcept {
    int original = uvrequest( uv( ) )->type;

    return static_cast< Type >( original );
}


} // namespace tri.
