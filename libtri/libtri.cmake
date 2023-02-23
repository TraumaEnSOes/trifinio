file( GLOB tri_SOURCES src/*.cpp )
file( GLOB tri_PUBLIC include/tri/*.hpp )
file( GLOB tri_HEADERS include/*.hpp )

add_library( tri "${tri_PUBLIC}" "${tri_HEADERS}" "${tri_SOURCES}" )

