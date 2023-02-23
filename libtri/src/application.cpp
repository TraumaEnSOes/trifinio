#include "tri/application.hpp"
#include "tri/details/uv_check_error.hpp"

#include "application_private.hpp"
#include "loop_instance.hpp"

#include <uv.h>

#include <cstdlib>
#include <optional>
#include <stdexcept>

static std::optional< tri::details::ApplicationPrivate > StaticApplicationPrivate;

namespace tri {

namespace details {

uv_loop_t *LoopInstance( ) noexcept {
    return &( StaticApplicationPrivate->uvLoop );
}

Application *ApplicationInstace = nullptr;

} // namespace details.

Application::~Application( ) noexcept( false ) {
    for( auto &cb : m_pimpl->onBeforeExit ) {
        cb( );
    }

    details::CheckUvError( uv_loop_close( details::LoopInstance( ) ) );

    details::ApplicationInstace = nullptr;
}

Application::Application( int argc, char **argv ) {
    if( details::ApplicationInstance ) {
        throw std::logic_error( "More than one instance of tri::Application" );
    }

    StaticApplicationPrivate = tri::details::ApplicationPrivate( );
    m_pimpl = &( *StaticApplicationPrivate );

    details::ApplicationInstance = this;

    uv_loop_init( details::LoopInstance( ) );
}

void Application::configure( ApplicationConfig config ) noexcept {
    m_pimpl->config = std::move( config );
}

int Application::exec( ) {
    // Apply uv configuration
    if( m_pimpl->config.metricsIdleTime ) {
        uv_loop_configure( details::LoopInstance( ), uv_loop_option::UV_METRICS_IDLE_TIME );
    }
    if( m_pimpl->config.ignoreSIGPROF ) {
        uv_loop_configure( details::LoopInstance( ), uv_loop_option::UV_LOOP_BLOCK_SIGNAL, SIGPROF );
    }

    auto result = uv_run( details::LoopInstance( ), uv_run_mode::UV_RUN_DEFAULT );

    return result;
}

void Application::exit( int retCode ) {
    std::exit( retCode );
}

void Application::tryQuit( int retCode ) {

}

void Application::onBeforeExit( std::function< void( ) > cb ) {
    m_pimpl->onBeforeExit.push_back( std::move( cb ) );
}

} // namespace details.
