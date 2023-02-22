#ifndef LIBTRI_APPLICATION_HPP
#define LIBTRI_APPLICATION_HPP

#include <tri/application_config.hpp>
#include <tri/application_instance.hpp>

#include <functional>
#include <memory>
#include <vector>

namespace tri {

class Application {
public:
    ~Application( );
    Application( int argc, char **argv );
    Application( const Application & ) = delete;
    Application &operator=( const Application & ) = delete;

    void configure( ApplicationConfig config ) noexcept;
    ApplicationConfig configuration( ) const noexcept;

    static Application &instance( ) noexcept {
        return *details::ApplicationInstance;
    }

    int exec( );

    void exit( int retCode = 0 );

    void tryQuit( int retCode = 0 );

    void onBeforeExit( std::function< void( ) > cb );

private:
    ApplicationPrivate *m_details;
};

} // namespace tri.

#endif
