# Applications CMakeList file

# Add examples using our library
add_executable(s3-example S3Example.cpp)
add_executable(dynamodb-example DynamoDBExample.cpp)
add_executable(ec2-example EC2Example.cpp)

# Link our examples with our library
target_link_libraries(s3-example awsexamples)
target_link_libraries(dynamodb-example awsexamples)
target_link_libraries(ec2-example awsexamples)

# Install the example applications
install(
    TARGETS 
        s3-example
        dynamodb-example
        ec2-example
    DESTINATION
        bin
    COMPONENT
        applications
)
