#ifndef LIBTRI_HTTP_ROUTER_HPP
#define LIBTRI_HTTP_ROUTER_HPP

#include <functional>
#include <initializer_list>
#include <memory>
#include <string_view>

namespace tri {

class HttpRequest {
public:
//    HttpRequest( );

    std::string_view method( ) const noexcept;

private:

};

class HttpResponse {
public:
//    HttpResponse( const HttpResponse & ) = delete;
//    HttpResponse &operator=( const HttpResponse & ) = delete;

private:

};

using HttpNext = std::function< void( ) >;

struct HttpHandler {
    template< typename CB > HttpHandler( CB cb ) {
        target = std::move( cb );
    }

    template< typename CLASS > HttpHandler( CLASS *instance, void (CLASS::*method)( HttpRequest &, HttpResponse &, HttpNext & ) ) {
        target = std::bind( method, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
    }

    std::function< void( HttpRequest &, HttpResponse &, HttpNext & ) > target;
};

struct HttpRouterPrivate {
    std::vector< std::function< void( HttpRequest &, HttpResponse &, HttpNext & ) > > uses;
};

class HttpRouter {
public:
    HttpRouter( )
        : m_pimpl( std::make_unique< HttpRouterPrivate >( ) )
    {
    }

    void USE( HttpHandler handler ) {
        m_pimpl->uses.push_back( std::move( handler.target ) );
    }
    void USE( std::initializer_list< HttpHandler > handlers ) {
        for( auto &handler : handlers ) {
            m_pimpl->uses.push_back( std::move( handler.target ) );
        }
    }

    void use( ) {
        HttpRequest req;
        HttpResponse res;
        auto &list = m_pimpl->uses;
        unsigned limit = list.size( );
        unsigned idx = 1U;
        HttpNext next;
        next = [&req, &res, &limit, &idx, &list, &next]( ) -> void {
            if( idx < limit ) {
                auto curr = idx;
                ++idx;
                list[curr]( req, res, next );
            }
        };

        if( !list.empty( ) ) {
            list.front( )( req, res, next );
        }
    }

private:
    std::unique_ptr< HttpRouterPrivate > m_pimpl;
};

} // namespace tri.

#endif
