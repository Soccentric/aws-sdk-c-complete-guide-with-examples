# Tests CMakeList file

# Add the S3Manager test
add_executable(s3manager_test S3ManagerTest.cpp)

# Link with our library
target_link_libraries(s3manager_test awsexamples)

# Add test to CTest
add_test(NAME S3ManagerTest COMMAND s3manager_test)

# Set test properties
set_tests_properties(S3ManagerTest PROPERTIES
    PASS_REGULAR_EXPRESSION "ALL TESTS PASSED"
    FAIL_REGULAR_EXPRESSION "TESTS FAILED"
    TIMEOUT 300
)

# Install the tests
install(
    TARGETS 
        s3manager_test
    DESTINATION
        bin/tests
    COMPONENT
        applications
)
