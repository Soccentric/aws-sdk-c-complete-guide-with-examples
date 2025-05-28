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

# Find required packages
find_package(AWSSDK REQUIRED COMPONENTS core s3 ec2 dynamodb)
find_package(Threads REQUIRED)
find_package(Doxygen)

# Print found AWS SDK components
message(STATUS "Found AWS SDK components:")
foreach(COMPONENT ${AWSSDK_FOUND_COMPONENTS})
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
message(STATUS "AWS SDK location:  ${AWSSDK_FOUND}")
message(STATUS "C++ Compiler:      ${CMAKE_CXX_COMPILER}")
message(STATUS "Tests:             ${BUILD_TESTS}")
if(DOXYGEN_FOUND)
    message(STATUS "Documentation:     ENABLED")
else()
    message(STATUS "Documentation:     DISABLED (Doxygen not found)")
endif()
message(STATUS "=====================================")
message(STATUS "")
