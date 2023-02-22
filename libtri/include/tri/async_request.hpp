#ifndef LIBTRI_ASYNC_REQUEST_HPP
#define LIBTRI_ASYNC_REQUEST_HPP

#include <tri/details/uv_wrapper.hpp>

namespace tri {

class AsyncRequest;

namespace details {

class UvReqWrapper {
    void* data;
    uv_req_type type;
};

} // namespace details.

class AsyncRequest {
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

    virtual ~AsyncRequest( );
    AsyncRequest( ) = default;
    AsyncRequest( const AsyncRequest &other ) :
        details::UvWrapper( other )
    {
    }
    AsyncRequest &operator=( const AsyncRequest &other ) {
        assign( other );
        return *this;
    }

    bool cancel( ) noexcept;

    Type type( ) const noexcept;

private:
    AsyncRequest( void *ptr ) :
        details::UvWrapper( ptr )
    {
    }
};

} // namespace tri.

#endif
