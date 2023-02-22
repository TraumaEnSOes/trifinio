#ifndef LIBTRI_ASYNC_HANDLE_HPP
#define LIBTRI_ASYNC_HANDLE_HPP

#include <cassert>
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

} // namespace details;

class AsyncHandle {
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

    virtual ~AsyncHandle( );
    AsyncHandle( ) = default;
    AsyncHandle( const AsyncHandle & ) = delete;
    AsyncHandle( AsyncHandle &&other ) :
        m_uv( other.m_uv )
    {
        if( m_uv ) {
            other.m_uv = nullptr;
        }
    }
    AsyncHandle &operator=( const AsyncHandle & ) = delete;
    AsyncHandle &operator=( AsyncHandle &&other ) {
        assign( std::move( other ) );
        return *this;
    }

    bool operator==( const AsyncHandle &other ) const noexcept {
        return m_uv == other.m_uv;
    }
    bool operator!=( const AsyncHandle &other ) const noexcept {
        return m_uv != other.m_uv;
    }

    operator bool( ) const noexcept {
        return m_uv;
    }
    bool operator!( ) const noexcept {
        return !m_uv;
    }

    bool isActive( ) const noexcept {
        assert( operator bool( ) );

        return uv_is_active( handle_t( ) );
    }

    bool isClosing( ) const noexcept {
        assert( operator bool( ) );

        return uv_is_closing( handle_t( ) );
    }

    void close( CloseCB cb = CloseCB( ) ) {
        assert( operator bool( ) );

        if( cb ) {
            auto &details = *static_cast< details::AsyncHandlePrivate * >( data( ) );
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
        m_uv( ptr )
    {
    }

    void *uv( ) noexcept { return m_uv; }
    const void *uv( ) const noexcept { return m_uv; }

    void *data( ) noexcept { return wrapper( )->data; }
    const void *data( ) const noexcept { return wrapper( )->data; }

    void assign( AsyncHandle &&other ) noexcept {
        assert( operator!( ) );
        m_uv = other.m_uv;
        if( m_uv ) {
            other.m_uv = nullptr;
        }
    }
    void assign( void *ptr ) noexcept {
        assert( operator!( ) );
        m_uv = ptr;
    }

private:
    details::UvHandleWrapper *wrapper( ) noexcept { return static_cast< details::UvHandleWrapper * >( uv( ) ); }
    const details::UvHandleWrapper *wrapper( ) const noexcept { return static_cast< const details::UvHandleWrapper * >( uv( ) ); }

    uv_handle_t *handle_t( ) noexcept { return static_cast< uv_handle_t * >( uv( ) ); }
    const uv_handle_t *handle_t( ) const noexcept { return static_cast< const uv_handle_t * >( uv( ) ); }

    details::AsyncHandlePrivate &details( ) noexcept { return *static_cast< details::AsyncHandlePrivate * >( data( ) ); }
    const details::AsyncHandlePrivate &details( ) const noexcept { return *static_cast< const details::AsyncHandlePrivate * >( data( ) ); }

    static void GlobalCloseCB( uv_handle_t * );

    void *m_uv = nullptr;
};

} // namespace tri.

#endif
