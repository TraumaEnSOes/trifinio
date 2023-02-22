#ifndef LOOP_INSTANCE_HPP
#define LOOP_INSTANCE_HPP

typedef struct uv_loop_s uv_loop_t;

namespace tri {

namespace details {

uv_loop_t *LoopInstance( ) noexcept;

} // namespace details.

} // namespace tri.

#endif
