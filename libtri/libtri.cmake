file( GLOB tri_SOURCES src/*.cpp )
file( GLOB tri_PUBLIC include/uvpp/*.hpp )
file( GLOB tri_HEADERS include/*.hpp )

add_executable( tri "${tri_PUBLIC}" "${tri_HEADERS}" "${tri_SOURCES}" )

