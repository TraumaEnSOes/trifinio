#ifndef LIBTRI_DETAILS_UV_WRAPPER_HPP
#define LIBTRI_DETAILS_UV_WRAPPER_HPP

#include <tri/details/uv_check_error.hpp>
#include <tri/details/tri_assert.hpp>

#include <functional>
#include <utility>

namespace tri {

namespace details {

class UvWrapper {
public:
    virtual ~UvWrapper( );
    UvWrapper( ) = default;
    UvWrapper( const UvWrapper &other ) = delete;
    UvWrapper( UvWrapper &&other ) :
        m_uv( other.m_uv )
    {
        if( m_uv ) {
            other.m_uv = nullptr;
        }
    }
    UvWrapper &operator=( const UvWrapper &other ) = delete;
    UvWrapper &operator=( UvWrapper &&other ) {
        assign( std::move( other ) );
        return *this;
    }

    bool operator==( const UvWrapper &other ) const noexcept {
        return m_uv == other.m_uv;
    }
    bool operator!=( const UvWrapper &other ) const noexcept {
        return m_uv != other.m_uv;
    }

    operator bool( ) const noexcept {
        return m_uv;
    }
    bool operator!( ) const noexcept {
        return !m_uv;
    }

    std::size_t hash( ) const noexcept {
        std::hash< void * > hasher;
        return hasher( m_uv );
    }

protected:
    UvWrapper( void *ptr ) :
        m_uv( ptr )
    {
    }

    void assign( void *ptr ) noexcept {
        triassert( operator!( ) );
        m_uv = ptr;
    }

    void assign( UvWrapper &&other ) noexcept {
        triassert( operator!( ) );
        m_uv = other.m_uv;
        if( m_uv ) {
            other.m_uv = nullptr;
        }
    }

    void *uv( ) noexcept {
        return m_uv;
    }
    const void *uv( ) const noexcept {
        return m_uv;
    }

    void *uvdata( ) noexcept {
        return *static_cast< void ** >( m_uv );
    }

    const void *uvdata( ) const noexcept {
        return static_cast< const void *const * >( m_uv );
    }

private:
    void *m_uv = nullptr;
};

} // namespace details.

} // namespace tri.

#endif
