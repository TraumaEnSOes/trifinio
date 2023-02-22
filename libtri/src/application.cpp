#include "tri/application.hpp"
#include "application_private.hpp"
#include "loop_instance.hpp"

#include <uv.h>

#include <cstdlib>
#include <optional>
#include <stdexcept>

static std::optional< tri::ApplicationPrivate > StaticApplicationPrivate;

namespace tri {

namespace details {

Application *ApplicationInstace = nullptr;

} // namespace details.

Application::~Application( ) {
    for( auto &cb : m_details->onBeforeExit ) {
        cb( );
    }

    uv_loop_close( details::LoopInstance( ) );

    details::ApplicationInstace = nullptr;
}

Application::Application( int argc, char **argv ) {
    if( details::ApplicationInstance ) {
        throw std::logic_error( "More than one instance of tri::Application" );
    }

    StaticApplicationPrivate = tri::ApplicationPrivate( );
    m_details = &( *StaticApplicationPrivate );

    details::ApplicationInstance = this;

    uv_loop_init( details::LoopInstance( ) );
}

void Application::configure( ApplicationConfig config ) noexcept {
    m_details->config = std::move( config );
}

int Application::exec( ) {
    // Apply uv configuration
    if( m_details->config.MetricsIdleTime ) {
        uv_loop_configure( details::LoopInstance( ), uv_loop_option::UV_METRICS_IDLE_TIME );
    }
    if( m_details->config.ignoreSIGPROF ) {
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
    m_details->onBeforeExit.push_back( std::move( cb ) );
}

} // namespace details.
