# CPack configuration module

# Include CPack module
include(CPack)

# Set package information
set(CPACK_PACKAGE_NAME "aws-cpp-examples")
set(CPACK_PACKAGE_VENDOR "AWS Examples")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "AWS SDK C++ Examples - Demo applications for AWS services")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "AwsCppExamples")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/PROJECT_README.md")

# DEB specific configuration
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "AWS Examples Maintainer <aws-examples@example.com>")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libaws-cpp-sdk-core, libaws-cpp-sdk-s3, libaws-cpp-sdk-dynamodb, libaws-cpp-sdk-ec2")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/aws/aws-sdk-cpp")

# RPM specific configuration
set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_LICENSE "Apache 2.0")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_URL "https://github.com/aws/aws-sdk-cpp")
set(CPACK_RPM_PACKAGE_REQUIRES "aws-sdk-cpp-devel")

# Define components
set(CPACK_COMPONENTS_ALL applications libraries documentation)
set(CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "AWS C++ Example Applications")
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "AWS C++ Example Libraries")
set(CPACK_COMPONENT_DOCUMENTATION_DISPLAY_NAME "AWS C++ Example Documentation")

# Component dependencies
set(CPACK_COMPONENT_APPLICATIONS_DEPENDS libraries)

# Component descriptions
set(CPACK_COMPONENT_APPLICATIONS_DESCRIPTION "Example applications for AWS SDK C++")
set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Helper libraries for AWS SDK C++ examples")
set(CPACK_COMPONENT_DOCUMENTATION_DESCRIPTION "Documentation for AWS SDK C++ examples")

# Component grouping
set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
set(CPACK_COMPONENT_LIBRARIES_GROUP "Development")
set(CPACK_COMPONENT_DOCUMENTATION_GROUP "Documentation")

# Define component install directories
set(CPACK_COMPONENT_APPLICATIONS_INSTALL_DIRECTORY "bin")
set(CPACK_COMPONENT_LIBRARIES_INSTALL_DIRECTORY "lib")
set(CPACK_COMPONENT_DOCUMENTATION_INSTALL_DIRECTORY "share/doc/aws-cpp-examples")
