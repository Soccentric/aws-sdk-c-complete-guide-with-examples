cmake_minimum_required(VERSION 3.14)

# Project definition
project(aws-cpp-examples 
    VERSION 1.0.0
    DESCRIPTION "AWS SDK for C++ Examples"
    LANGUAGES CXX
)

# Use modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include custom CMake modules
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Add compiler warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include FetchContent module
include(FetchContent)

# Use FetchContent to get AWS SDK for C++
FetchContent_Declare(
    aws-sdk-cpp
    GIT_REPOSITORY https://github.com/aws/aws-sdk-cpp.git
    GIT_TAG 1.11.143  # You can update this to the version you want
)

# Configure AWS SDK build options
set(BUILD_ONLY "core;s3;ec2;dynamodb" CACHE STRING "Only build specified components")
set(ENABLE_TESTING OFF CACHE BOOL "Disable AWS SDK tests")
set(AUTORUN_UNIT_TESTS OFF CACHE BOOL "Don't run AWS SDK tests")
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")

# Make the AWS SDK available
FetchContent_MakeAvailable(aws-sdk-cpp)

# Find other required packages
find_package(Threads REQUIRED)
find_package(Doxygen)

# Print AWS SDK components being built
message(STATUS "Building AWS SDK components:")
foreach(COMPONENT ${BUILD_ONLY})
  message(STATUS "  ${COMPONENT}")
endforeach()

# Configure Doxygen
if(DOXYGEN_FOUND)
    set(DOXYGEN_HAVE_DOT YES)
    include(FindDoxygen)
    message(STATUS "Doxygen found. Documentation targets will be generated.")
else()
    message(STATUS "Doxygen not found. Documentation targets will not be generated.")
endif()

# Define library and include directories
include_directories(include)

# Add the library
add_subdirectory(src/lib)

# Add applications
add_subdirectory(src/app)

# Add tests
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

# Package configuration
include(cmake/PackageConfig.cmake)

# Create an uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        @ONLY
    )
    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    )
endif()

# Install license and readme files
install(
    FILES 
        PROJECT_README.md
    DESTINATION 
        share/doc/${PROJECT_NAME}
    COMPONENT 
        documentation
)

# Print configuration summary
message(STATUS "")
message(STATUS "AWS C++ Examples Configuration Summary")
message(STATUS "=====================================")
message(STATUS "Version:           ${PROJECT_VERSION}")
message(STATUS "Build type:        ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix:    ${CMAKE_INSTALL_PREFIX}")
message(STATUS "AWS SDK:           Fetched via FetchContent (v1.11.143)")
message(STATUS "C++ Compiler:      ${CMAKE_CXX_COMPILER}")
message(STATUS "Tests:             ${BUILD_TESTS}")
if(DOXYGEN_FOUND)
    message(STATUS "Documentation:     ENABLED")
else()
    message(STATUS "Documentation:     DISABLED (Doxygen not found)")
endif()
message(STATUS "=====================================")
message(STATUS "")
