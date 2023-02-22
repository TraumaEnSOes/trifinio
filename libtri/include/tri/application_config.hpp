#ifndef LIBTRI_APPLICATION_CONFIG_HPP
#define LIBTRI_APPLICATION_CONFIG_HPP

namespace tri {

struct ApplicationConfig {
    bool ignoreSIGPROF = false;
    bool metricsIdleTime = false;
};

} // namespace tri.

#endif
