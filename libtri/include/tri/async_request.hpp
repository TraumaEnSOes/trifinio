#ifndef LIBTRI_ASYNC_REQUEST_HPP
#define LIBTRI_ASYNC_REQUEST_HPP

#include <tri/details/tri_assert.hpp>
#include <tri/details/uv_check_error.hpp>
#include <tri/details/uv_wrapper.hpp>

typedef struct uv_req_s uv_req_t;

extern "C" int uv_cancel( uv_req_t *req );

namespace tri {

class AsyncRequest;

namespace details {

class UvReqWrapper {
    void* data;
    uv_req_type type;
};

} // namespace details.

class AsyncRequest : public details::UvWrapper {
public:
    enum class Type : int {
        UNKNOWN_REQ = 0,
        REQ,
        CONNECT,
        WRITE,
        SHUTDOWN,
        UDP_SEND,
        FS,
        WORK,
        GETADDRINFO,
        GETNAMEINFO,
    };

    AsyncRequest( ) = default;
    AsyncRequest( AsyncRequest &&other ) :
        details::UvWrapper( std::move( other ) )
    {
    }
    AsyncRequest &operator=( const AsyncRequest &other ) {
        assign( std::move( other ) );
        return *this;
    }

    bool cancel( ) noexcept {
        triassert( operator bool( ) );
        return details::uvCheckError( uv_cancel( req_t( ) ) );
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
