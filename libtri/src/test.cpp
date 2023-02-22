#include <functional>
#include <iostream>
#include <vector>

struct HttpRequest { };
struct HttpResponse { };

// using HttpNext = std::function< void( ) >;
struct HttpNext;

struct HttpHandler {
    template< typename CB > HttpHandler( CB cb ) {
        target = std::move( cb );
    }

    template< typename CLASS > HttpHandler( CLASS *instance, void (CLASS::*method)( HttpRequest &, HttpResponse &, HttpNext & ) ) {
        target = std::bind( method, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 );
    }

    std::function< void( HttpRequest &, HttpResponse &, HttpNext & ) > target;
};

struct HttpNext {
    HttpNext( HttpRequest &req, HttpResponse &res, std::vector< std::function< void( HttpRequest &, HttpResponse &, HttpNext & ) > > &list ) :
        m_request( req ),
        m_response( res ),
        m_uses( list ),
        m_idx( 1U )
    {
    }

    void operator()( ) noexcept {
        if( m_idx < m_uses.size( ) ) {
            auto curr = m_idx;
            ++m_idx;
            m_uses[curr]( m_request, m_response, *this );
        }
    }

    HttpRequest &m_request;
    HttpResponse &m_response;
    std::vector< std::function< void( HttpRequest &, HttpResponse &, HttpNext & ) > > m_uses;
    unsigned m_idx;
};

class HttpRouter {
public:
    void USE( HttpHandler handler ) {
        uses.push_back( std::move( handler.target ) );
    }

    void use( ) {
        HttpRequest req;
        HttpResponse res;
        HttpNext next( req, res, uses );

        if( !uses.empty( ) ) {
            uses.front( )( req, res, next );
        }
#if 0
        auto &list = uses;
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
#endif
    }

private:
    std::vector< std::function< void( HttpRequest &, HttpResponse &, HttpNext & ) > > uses;
};

static void level1( HttpRequest &, HttpResponse &, HttpNext &next ) {
    std::cout << "Enter level1\n";
    next( );
    std::cout << "Leave level1\n";
}

static void level2( HttpRequest &, HttpResponse &, HttpNext &next ) {
    std::cout << "Enter level2\n";
    next( );
    std::cout << "Leave level2\n";
}

struct Test {
    void handler( HttpRequest &, HttpResponse &, HttpNext & ) {
        std::cout << "instance::handler\n";
    }
};

int main( ) {
    HttpRouter Router;
    Test t;

    Router.USE( level1 );
    Router.USE( level2 );
    Router.USE( HttpHandler( &t, &Test::handler ) );

    Router.use( );

    return 0;
}
