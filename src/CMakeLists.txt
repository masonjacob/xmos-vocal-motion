# Minimum required CMake version
cmake_minimum_required(VERSION 3.0)

# Project name
project(XMOS_xCORE_Project)

# Set the xCORE toolchain file
set(CMAKE_TOOLCHAIN_FILE <path_to_xcore_toolchain_file>)

# Specify the target
add_executable(xcore_project)

# Link any libraries or dependencies if needed
# target_link_libraries(xcore_project <library_name>)

# Set the compiler to "xcc"
set_target_properties(xcore_project PROPERTIES
    COMPILE_FLAGS "XK_VOICE_L71.xn"
)

# Additional compiler flags if necessary 
# target_compile_options(xcore_project PRIVATE <your_compiler_flags>)

# Specify the source files for the target
target_sources(xcore_project PRIVATE
    main.xc
    # Add more source files if you have them
)

target_include_directories(xcore_project PRIVATE 
    lib
    # Add more source directories if you have them
)

# Set the output directory for the built binary
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Set the installation directory for the built binary (optional)
# install(TARGETS xcore_project RUNTIME DESTINATION bin)
