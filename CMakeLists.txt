cmake_minimum_required(VERSION 3.22)
project(my_app)

## Specify your application sources and includes
file(GLOB_RECURSE APP_SOURCES src/*.c src/*.cpp src/*.xc)

set(APP_INCLUDES
   src
   src/lib
)

## Create your targets
add_executable(my_app ${APP_SOURCES})
target_include_directories(my_app PUBLIC ${APP_INCLUDES})

## Specify your compiler flags and options
target_compile_options(my_app PRIVATE
   -Os
   -g
   -report
   -fxscope
   -mcmodel=large
   -Wno-xcore-fptrgroup
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/config.xscope
   ${CMAKE_CURRENT_SOURCE_DIR}/XK_VOICE_L71.xn
)

## Specify any compile definitions
target_compile_definitions(my_app PRIVATE
   DEBUG_PRINT_ENABLE=1
   PLATFORM_SUPPORTS_TILE_0=1
   PLATFORM_SUPPORTS_TILE_1=1
   PLATFORM_SUPPORTS_TILE_2=0
   PLATFORM_SUPPORTS_TILE_3=0
   PLATFORM_USES_TILE_0=1
   PLATFORM_USES_TILE_1=1
)

## Set your link options
target_link_options(my_app PRIVATE
   -report
   ${CMAKE_CURRENT_SOURCE_DIR}/XK_VOICE_L71.xn
   # ${CMAKE_CURRENT_SOURCE_DIR}/src/config.xscope
)


## Link libraries
# target_link_libraries(my_app PUBLIC core:general io:i2c io::uart)
