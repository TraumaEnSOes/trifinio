#ifndef APPLICATION_PRIVATE_HPP
#define APPLICATION_PRIVATE_HPP

#include "tri/application_config.hpp"

#include <functional>
#include <vector>

namespace tri {

struct ApplicationPrivate {
    ApplicationConfig config;
    std::vector< std::function< void( ) > > onBeforeExit;
    int argc;
    char **argv;
};

} // namespace tri.

#endif
