#ifndef LIBTRI_TCP_CLIENT_HPP
#define LIBTRI_TCP_CLIENT_HPP

#include <tri/details/uv_wrapper.hpp>

namespace tri {

class TcpClient : public details::UvWrapper {
public:
    TcpClient( ) = default;
    TcpClient( const TcpClient &other ) :
        details::UvWrapper( other )
    {
    }
    TcpClient &operator=( const TcpClient &other ) {
        assign( other );
        return *this;
    }



};

} // namespace tri.

#endif
