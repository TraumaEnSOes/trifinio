#include "loop_instance.hpp"

#include <uv.h>

static uv_loop_t *LoopInit( uv_loop_t *loop ) {
    uv_loop_init( loop );
    return loop;
}

namespace tri {

namespace details {

uv_loop_t *LoopInstance( ) {
    static uv_loop_t StaticLoop;
    static uv_loop_t *instance = LoopInit( &StaticLoop );

    return instance;
}

} // namespace details.

} // namespace 