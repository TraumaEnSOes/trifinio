#ifndef LIBTRI_ASYNC_STREAM_HPP
#define LIBTRI_ASYNC_STREAM_HPP

#include <tri/async_handle.hpp>
#include <tri/async_request.hpp>

#include <cassert>
#include <cstddef>

typedef struct uv_stream_s uv_stream_t;
typedef struct uv_buf_s uv_buf_t;

extern "C" {

int uv_shutdown( uv_stream_t *, void (*)( uv_stream_t *, int ) );
int uv_listen(uv_stream_t *stream, int backlog, void (*)( uv_stream_t *, int ) );
int uv_accept( uv_stream_t *server, uv_stream_t *client );
int uv_read_start( uv_stream_t *stream, void (*alloc)( uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf), void (cb)( uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf ) );
int uv_read_stop( uv_stream_t* );
int uv_write( uv_req_t *req, uv_stream_t *handle, const uv_buf_t bufs[], unsigned int nbufs, uv_write_cb cb );

}

namespace tri {

class AsyncStream;

using ShutdownCB = std::function< void( AsyncStream &, int ) >;
using ListenCB = std::function< void( AsyncStream &, int ) >;
using ReadCB = std::function< void( AsyncStream & ) >;
using WriteCB = std::function< void( AsyncStream & ) >;

namespace details {

struct AsyncStreamPrivate : public AsyncHandlePrivate {
    ShutdownCB shutdownCB;
    ListenCB listenCB;
};

} // namespace details.

class AsyncStream : public AsyncHandle {
protected:
    void shutdown( ShutdownCB cb = ShutdownCB( ) ) {
        if( cb ) {
            auto &details = *static_cast< details::AsyncStreamPrivate * >( data( ) );
            details.shutdownCB = std::move( cb );
        }

        uv_shutdown( stream_t( ), GlobalShutdownCB );
    }

    void listen( int backlog, ListenCB cb = ListenCB( ) ) {
        if( cb ) {
            auto &details = *static_cast< details::AsyncStreamPrivate * >( data( ) );
            details.listenCB = std::move( cb );
        }

        uv_listen( stream_t( ), backlog, GlobalListenCB );
    }

    void readStart();
    void readStop( ) noexcept {
        assert( operator bool( ) );
        uv_read_stop( stream_t( ) );
    }

    AsyncRequest write( );

private:
    uv_stream_t *stream_t( ) noexcept {
        return reinterpret_cast< uv_stream_t * >( this );
    }
    const uv_stream_t *stream_t( ) const noexcept {
        return reinterpret_cast< const uv_stream_t * >( this );
    }

    static void GlobalShutdownCB( uv_stream_t *, int );
    static void GlobalListenCB( uv_stream_t *, int );
};

class IStream : public AsyncStream {

};

class OStream : public AsyncStream {

};

class IOStream : public IStream, public OStream {

};

} // namespace tri.

#endif
