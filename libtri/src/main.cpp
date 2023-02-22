#include <tri/http_router.hpp>

#include <iostream>

static void level1( tri::HttpRequest &, tri::HttpResponse &, tri::HttpNext &next ) {
    std::cout << "Enter level1\n";
    next( );
    std::cout << "Leave level1\n";
}

static void level2( tri::HttpRequest &, tri::HttpResponse &, tri::HttpNext &next ) {
    std::cout << "Enter level2\n";
    next( );
    std::cout << "Leave level2\n";
}

struct Test {
    void handler( tri::HttpRequest &, tri::HttpResponse &, tri::HttpNext & ) {
        std::cout << "instance::handler\n";
    }
};

int main( ) {
    tri::HttpRouter Router;
    Test t;

    Router.USE( tri::HttpHandler( &level1 ) );
    Router.USE( level2 );
    Router.USE( tri::HttpHandler( &t, &Test::handler ) );

    Router.use( );

    return 0;
}
