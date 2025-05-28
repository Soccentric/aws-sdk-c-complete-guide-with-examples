# FindDoxygen.cmake is included in CMake by default
# This module provides additional functionality to use Doxygen in our project

find_package(Doxygen REQUIRED)

# Function to enable doxygen for a target
function(add_doxygen TARGET_NAME)
    if(NOT TARGET ${TARGET_NAME})
        message(FATAL_ERROR "add_doxygen called with invalid target: ${TARGET_NAME}")
    endif()
    
    get_target_property(TARGET_SOURCES ${TARGET_NAME} SOURCES)
    get_target_property(TARGET_INCLUDE_DIRS ${TARGET_NAME} INCLUDE_DIRECTORIES)
    
    # If no include directories are set, use the project's include directory
    if(NOT TARGET_INCLUDE_DIRS)
        set(TARGET_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include")
    endif()
    
    # Setup Doxygen configuration
    set(DOXYGEN_OUTPUT_DIR "${CMAKE_BINARY_DIR}/docs/${TARGET_NAME}")
    set(DOXYGEN_INDEX_FILE "${DOXYGEN_OUTPUT_DIR}/html/index.html")
    set(DOXYGEN_INPUT_DIRS "${TARGET_INCLUDE_DIRS}")
    
    # Add source directories
    get_target_property(TARGET_SOURCE_DIR ${TARGET_NAME} SOURCE_DIR)
    if(TARGET_SOURCE_DIR)
        set(DOXYGEN_INPUT_DIRS "${DOXYGEN_INPUT_DIRS} ${TARGET_SOURCE_DIR}")
    endif()
    
    # Create Doxyfile
    configure_file(
        "${CMAKE_SOURCE_DIR}/cmake/Doxyfile.in"
        "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
        @ONLY
    )
    
    # Add custom command to generate documentation
    add_custom_command(
        OUTPUT ${DOXYGEN_INDEX_FILE}
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${TARGET_SOURCES}
        COMMENT "Generating Doxygen documentation for ${TARGET_NAME}"
        VERBATIM
    )
    
    # Add custom target for building the documentation
    add_custom_target(${TARGET_NAME}_docs
        DEPENDS ${DOXYGEN_INDEX_FILE}
    )
    
    # Add a dependency between the main target and the documentation
    add_dependencies(${TARGET_NAME} ${TARGET_NAME}_docs)
    
    # Install the documentation
    install(
        DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html
        DESTINATION share/doc/${TARGET_NAME}
        COMPONENT documentation
        OPTIONAL
    )
endfunction()
