cmake_minimum_required( VERSION 3.22.1 )
project( libtri )

if( LIBUV_PATH )
find_package( libuv PATHS "${LIBUV_PATH}" )
else( )
find_package( libuv )
endif( )

include( libtri.cmake )

target_include_directories( tri PRIVATE include )
target_link_libraries( tri PUBLIC uv )
