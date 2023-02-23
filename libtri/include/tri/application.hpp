#ifndef LIBTRI_APPLICATION_HPP
#define LIBTRI_APPLICATION_HPP

#include <tri/application_config.hpp>
#include <tri/application_instance.hpp>

#include <functional>
#include <memory>
#include <vector>

namespace tri {

namespace details {

class ApplicationPrivate;

} // namespace details.

class Application {
public:
    ~Application( ) noexcept( false );
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
    details::ApplicationPrivate *m_pimpl;
};

} // namespace tri.

#endif
