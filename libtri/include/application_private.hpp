#ifndef APPLICATION_PRIVATE_HPP
#define APPLICATION_PRIVATE_HPP

#include "tri/application_config.hpp"

#include "loop_instance.hpp"

#include <uv.h>

#include <functional>
#include <vector>

namespace tri {

class Application;

namespace details {

class ApplicationPrivate {
    friend class ::tri::Application;
    friend uv_loop_t *LoopInstance( ) noexcept;

private:
    uv_loop_s uvLoop;
    ApplicationConfig config;
    std::vector< std::function< void( ) > > onBeforeExit;
    int argc;
    char **argv;
};

} // namespace tri.

} // namespace tri.

#endif
