#ifndef LIBTRI_ASYNC_HANDLE_HPP
#define LIBTRI_ASYNC_HANDLE_HPP

#include <tri/details/uv_wrapper.hpp>

#include <functional>
#include <string_view>

typedef struct uv_loop_s uv_loop_t;
typedef struct uv_handle_s uv_handle_t;

extern "C" {

int uv_is_active( const uv_handle_t * );
int uv_is_closing( const uv_handle_t * );
void uv_close( uv_handle_t *, void (*)( uv_handle_t * ) );
const char *uv_handle_type_name( int );

}

namespace tri {

class AsyncHandle;

using CloseCB = std::function< void( AsyncHandle & ) >;

namespace details {

struct UvHandleWrapper {
    void *data;
    uv_loop_t *loop;
    int type;
};

class AsyncHandlePrivate {
    friend class ::tri::AsyncHandle;
    friend void FreeHandleResources( AsyncHandle * );

private:
    virtual void freeResources( void *uv ) = 0;

    CloseCB closeCB;
};

} // namespace details.

class AsyncHandle : public details::UvWrapper {
public:
    enum Type {
        UNKNOWN = 0,
        ASYNC,
        CHECK,
        FS_EVENT,
        FS_POLL,
        HANDLE,
        IDLE,
        NAMED_PIPE,
        POLL,
        PREPARE,
        PROCESS,
        STREAM,
        TCP,
        TIMER,
        TTY,
        UDP,
        SIGNAL,
        FILE
    };

    AsyncHandle( ) = default;
    AsyncHandle( AsyncHandle &&other ) :
        details::UvWrapper( std::move( other ) )
    {
    }
    AsyncHandle &operator=( AsyncHandle &&other ) {
        assign( std::move( other ) );
        return *this;
    }

    bool isActive( ) const noexcept {
        triassert( operator bool( ) );

        return uv_is_active( handle_t( ) );
    }

    bool isClosing( ) const noexcept {
        triassert( operator bool( ) );

        return uv_is_closing( handle_t( ) );
    }

    void close( CloseCB cb = CloseCB( ) ) {
        triassert( operator bool( ) );

        if( cb ) {
            auto &details = *static_cast< details::AsyncHandlePrivate * >( uvdata( ) );
            details.closeCB = std::move( cb );
        }

        uv_close( handle_t( ), GlobalCloseCB );
    }

    Type type( ) const noexcept {
        return operator bool( ) ? static_cast< Type >( wrapper( )->type ) : Type::UNKNOWN;
    }

    std::string_view HandleTypeName( Type t ) {
        return std::string_view( uv_handle_type_name( t ) );
    }

protected:
    AsyncHandle( void *ptr ) :
        details::UvWrapper( ptr )
    {
    }

private:
    details::UvHandleWrapper *wrapper( ) noexcept { return static_cast< details::UvHandleWrapper * >( uv( ) ); }
    const details::UvHandleWrapper *wrapper( ) const noexcept { return static_cast< const details::UvHandleWrapper * >( uv( ) ); }

    uv_handle_t *handle_t( ) noexcept { return static_cast< uv_handle_t * >( uv( ) ); }
    const uv_handle_t *handle_t( ) const noexcept { return static_cast< const uv_handle_t * >( uv( ) ); }

    details::AsyncHandlePrivate &details( ) noexcept { return *static_cast< details::AsyncHandlePrivate * >( uvdata( ) ); }
    const details::AsyncHandlePrivate &details( ) const noexcept { return *static_cast< const details::AsyncHandlePrivate * >( uvdata( ) ); }

    static void GlobalCloseCB( uv_handle_t * );
};

} // namespace tri.

namespace std {

template< > struct hash< tri::AsyncHandle > {
    std::size_t operator( )( const tri::AsyncHandle &ah ) const noexcept {
        return ah.hash( );
    }
};

}

#endif
