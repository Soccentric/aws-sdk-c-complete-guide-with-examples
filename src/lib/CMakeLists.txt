# Library CMakeList file

# Create a target for the library
add_library(awsexamples
    AwsUtils.cpp
    S3Manager.cpp
    DynamoDBManager.cpp
    EC2Manager.cpp
)

# Set library properties
set_target_properties(awsexamples PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "../include/awsexamples/S3Manager.h;../include/awsexamples/DynamoDBManager.h;../include/awsexamples/EC2Manager.h;../include/awsexamples/AwsUtils.h"
)

# Link dependencies
target_link_libraries(awsexamples
    PUBLIC
    ${AWSSDK_LINK_LIBRARIES}
    Threads::Threads
)

# Include directories
target_include_directories(awsexamples
    PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Generate API documentation with Doxygen
if(DOXYGEN_FOUND)
    add_doxygen(awsexamples)
endif()

# Install targets
install(
    TARGETS awsexamples
    ARCHIVE DESTINATION lib COMPONENT libraries
    LIBRARY DESTINATION lib COMPONENT libraries
    RUNTIME DESTINATION bin COMPONENT libraries
    PUBLIC_HEADER DESTINATION include/awsexamples COMPONENT libraries
)
