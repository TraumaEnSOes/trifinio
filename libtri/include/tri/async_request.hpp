#ifndef LIBTRI_ASYNC_REQUEST_HPP
#define LIBTRI_ASYNC_REQUEST_HPP

#include <tri/details/tri_assert.hpp>
#include <tri/details/uv_check_error.hpp>
#include <tri/details/uv_wrapper.hpp>

#include <uv.h>

namespace tri {

class AsyncRequest;

namespace details {

} // namespace details.

class AsyncRequest : public details::UvWrapper {
public:
    using Type = uv_req_type;

    AsyncRequest( ) = default;
    AsyncRequest( AsyncRequest &&other ) :
        details::UvWrapper( std::move( other ) )
    {
    }
    AsyncRequest &operator=( AsyncRequest &&other ) {
        assign( std::move( other ) );
        return *this;
    }

    bool cancel( ) noexcept {
        triassert( operator bool( ) );
        return details::CheckUvError( uv_cancel( req_t( ) ) );
    }

    Type type( ) const noexcept;

private:
    AsyncRequest( void *ptr ) :
        details::UvWrapper( ptr )
    {
    }

    uv_req_t *req_t( ) noexcept {
        return static_cast< uv_req_t * >( uv( ) );
    }
};

} // namespace tri.

#endif
