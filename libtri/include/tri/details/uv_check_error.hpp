#ifndef LIBTRI_DETAILS_UV_CHECK_ERROR_HPP
#define LIBTRI_DETAILS_UV_CHECK_ERROR_HPP

#include <stdexcept>

extern "C" const char *uv_strerror( int err );

namespace tri {

namespace details {

template< typename T > inline int uvGenericCheckError( int value ) {
    if( value < 0 ) {
        throw T( uv_strerror( value ) );
    }

    return value;
}

inline int uvCheckError( int value ) {
    return uvGenericCheckError< ::std::runtime_error >( value );
}

} // namespace details.

} // namespace tri.

#endif
