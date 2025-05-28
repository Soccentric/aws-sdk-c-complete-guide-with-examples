# AWS SDK for C++ - Complete Guide with Examples

## Overview

The AWS SDK for C++ is a collection of libraries that allows developers to integrate their C++ applications with AWS services. It provides high-performance, low-latency access to AWS APIs with modern C++ features.

## Installation

### Using vcpkg (Recommended)
```bash
# Install vcpkg if not already installed
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh  # Linux/macOS
# or
./bootstrap-vcpkg.bat  # Windows

# Install AWS SDK
./vcpkg install aws-sdk-cpp[core,s3,ec2,dynamodb]
```

### Using CMake
```bash
git clone --recurse-submodules https://github.com/aws/aws-sdk-cpp
cd aws-sdk-cpp
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DBUILD_ONLY="core;s3;ec2;dynamodb"
make -j$(nproc)
sudo make install
```

## Basic Setup

### CMakeLists.txt Configuration
```cmake
cmake_minimum_required(VERSION 3.13)
project(aws-example)

set(CMAKE_CXX_STANDARD 11)

find_package(AWSSDK REQUIRED COMPONENTS core s3 ec2 dynamodb)

add_executable(aws-example main.cpp)
target_link_libraries(aws-example ${AWSSDK_LINK_LIBRARIES})
```

### Initial Setup Code
```cpp
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>

int main() {
    // Initialize the AWS SDK
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    
    {
        // Your AWS code here
    }
    
    // Cleanup
    Aws::ShutdownAPI(options);
    return 0;
}
```

## AWS S3 Examples

### 1. List S3 Buckets
```cpp
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/ListBucketsRequest.h>
#include <iostream>

void ListBuckets() {
    Aws::S3::S3Client s3_client;
    
    auto outcome = s3_client.ListBuckets();
    
    if (outcome.IsSuccess()) {
        std::cout << "Buckets:\n";
        for (const auto& bucket : outcome.GetResult().GetBuckets()) {
            std::cout << "  " << bucket.GetName() << std::endl;
        }
    } else {
        std::cout << "Error: " << outcome.GetError().GetMessage() << std::endl;
    }
}
```

### 2. Upload File to S3
```cpp
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <fstream>

bool UploadFile(const std::string& bucket_name, 
                const std::string& key_name, 
                const std::string& file_path) {
    
    Aws::S3::S3Client s3_client;
    Aws::S3::Model::PutObjectRequest request;
    
    request.SetBucket(bucket_name);
    request.SetKey(key_name);
    
    std::shared_ptr<Aws::IOStream> input_data = 
        Aws::MakeShared<Aws::FStream>("SampleAllocationTag", 
                                      file_path.c_str(), 
                                      std::ios_base::in | std::ios_base::binary);
    
    request.SetBody(input_data);
    
    auto outcome = s3_client.PutObject(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Successfully uploaded " << key_name << std::endl;
        return true;
    } else {
        std::cout << "Error uploading: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}
```

### 3. Download File from S3
```cpp
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <fstream>

bool DownloadFile(const std::string& bucket_name, 
                  const std::string& key_name, 
                  const std::string& local_path) {
    
    Aws::S3::S3Client s3_client;
    Aws::S3::Model::GetObjectRequest request;
    
    request.SetBucket(bucket_name);
    request.SetKey(key_name);
    
    auto outcome = s3_client.GetObject(request);
    
    if (outcome.IsSuccess()) {
        std::ofstream local_file(local_path, std::ios::binary);
        local_file << outcome.GetResult().GetBody().rdbuf();
        std::cout << "Successfully downloaded " << key_name << std::endl;
        return true;
    } else {
        std::cout << "Error downloading: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}
```

## DynamoDB Examples

### 1. Create Table
```cpp
#include <aws/core/Aws.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/model/CreateTableRequest.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/KeySchemaElement.h>
#include <aws/dynamodb/model/ProvisionedThroughput.h>

bool CreateTable(const std::string& table_name) {
    Aws::DynamoDB::DynamoDBClient client;
    Aws::DynamoDB::Model::CreateTableRequest request;
    
    // Set table name
    request.SetTableName(table_name);
    
    // Define attributes
    Aws::DynamoDB::Model::AttributeDefinition hash_key;
    hash_key.SetAttributeName("id");
    hash_key.SetAttributeType(Aws::DynamoDB::Model::ScalarAttributeType::S);
    request.AddAttributeDefinitions(hash_key);
    
    // Define key schema
    Aws::DynamoDB::Model::KeySchemaElement key_schema;
    key_schema.SetAttributeName("id");
    key_schema.SetKeyType(Aws::DynamoDB::Model::KeyType::HASH);
    request.AddKeySchema(key_schema);
    
    // Set provisioned throughput
    Aws::DynamoDB::Model::ProvisionedThroughput throughput;
    throughput.SetReadCapacityUnits(5);
    throughput.SetWriteCapacityUnits(5);
    request.SetProvisionedThroughput(throughput);
    
    auto outcome = client.CreateTable(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Table created successfully!" << std::endl;
        return true;
    } else {
        std::cout << "Error: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}
```

### 2. Put Item
```cpp
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/AttributeValue.h>

bool PutItem(const std::string& table_name, 
             const std::string& id, 
             const std::string& name, 
             int age) {
    
    Aws::DynamoDB::DynamoDBClient client;
    Aws::DynamoDB::Model::PutItemRequest request;
    
    request.SetTableName(table_name);
    
    // Create item attributes
    Aws::DynamoDB::Model::AttributeValue id_attr;
    id_attr.SetS(id);
    request.AddItem("id", id_attr);
    
    Aws::DynamoDB::Model::AttributeValue name_attr;
    name_attr.SetS(name);
    request.AddItem("name", name_attr);
    
    Aws::DynamoDB::Model::AttributeValue age_attr;
    age_attr.SetN(std::to_string(age));
    request.AddItem("age", age_attr);
    
    auto outcome = client.PutItem(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Item added successfully!" << std::endl;
        return true;
    } else {
        std::cout << "Error: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}
```

### 3. Get Item
```cpp
#include <aws/dynamodb/model/GetItemRequest.h>

void GetItem(const std::string& table_name, const std::string& id) {
    Aws::DynamoDB::DynamoDBClient client;
    Aws::DynamoDB::Model::GetItemRequest request;
    
    request.SetTableName(table_name);
    
    // Set key
    Aws::DynamoDB::Model::AttributeValue key_attr;
    key_attr.SetS(id);
    request.AddKey("id", key_attr);
    
    auto outcome = client.GetItem(request);
    
    if (outcome.IsSuccess()) {
        const auto& item = outcome.GetResult().GetItem();
        if (!item.empty()) {
            std::cout << "Item found:\n";
            std::cout << "ID: " << item.at("id").GetS() << std::endl;
            std::cout << "Name: " << item.at("name").GetS() << std::endl;
            std::cout << "Age: " << item.at("age").GetN() << std::endl;
        } else {
            std::cout << "Item not found" << std::endl;
        }
    } else {
        std::cout << "Error: " << outcome.GetError().GetMessage() << std::endl;
    }
}
```

## EC2 Examples

### 1. List EC2 Instances
```cpp
#include <aws/core/Aws.h>
#include <aws/ec2/EC2Client.h>
#include <aws/ec2/model/DescribeInstancesRequest.h>

void ListInstances() {
    Aws::EC2::EC2Client ec2_client;
    Aws::EC2::Model::DescribeInstancesRequest request;
    
    auto outcome = ec2_client.DescribeInstances(request);
    
    if (outcome.IsSuccess()) {
        const auto& reservations = outcome.GetResult().GetReservations();
        
        for (const auto& reservation : reservations) {
            for (const auto& instance : reservation.GetInstances()) {
                std::cout << "Instance ID: " << instance.GetInstanceId() << std::endl;
                std::cout << "State: " << Aws::EC2::Model::InstanceStateNameMapper::GetNameForInstanceStateName(instance.GetState().GetName()) << std::endl;
                std::cout << "Type: " << Aws::EC2::Model::InstanceTypeMapper::GetNameForInstanceType(instance.GetInstanceType()) << std::endl;
                std::cout << "---" << std::endl;
            }
        }
    } else {
        std::cout << "Error: " << outcome.GetError().GetMessage() << std::endl;
    }
}
```

### 2. Start/Stop Instance
```cpp
#include <aws/ec2/model/StartInstancesRequest.h>
#include <aws/ec2/model/StopInstancesRequest.h>

bool StartInstance(const std::string& instance_id) {
    Aws::EC2::EC2Client ec2_client;
    Aws::EC2::Model::StartInstancesRequest request;
    
    request.AddInstanceIds(instance_id);
    
    auto outcome = ec2_client.StartInstances(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Instance " << instance_id << " started successfully" << std::endl;
        return true;
    } else {
        std::cout << "Error starting instance: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

bool StopInstance(const std::string& instance_id) {
    Aws::EC2::EC2Client ec2_client;
    Aws::EC2::Model::StopInstancesRequest request;
    
    request.AddInstanceIds(instance_id);
    
    auto outcome = ec2_client.StopInstances(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Instance " << instance_id << " stopped successfully" << std::endl;
        return true;
    } else {
        std::cout << "Error stopping instance: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}
```

## Configuration and Authentication

### 1. Using AWS Credentials File
```cpp
// ~/.aws/credentials
// [default]
// aws_access_key_id = YOUR_ACCESS_KEY
// aws_secret_access_key = YOUR_SECRET_KEY

// ~/.aws/config
// [default]
// region = us-west-2

// Code automatically uses these credentials
Aws::S3::S3Client s3_client; // Uses default credentials
```

### 2. Programmatic Credentials
```cpp
#include <aws/core/auth/AWSCredentialsProvider.h>

// Method 1: Direct credentials
Aws::Auth::AWSCredentials credentials("ACCESS_KEY", "SECRET_KEY");
Aws::Client::ClientConfiguration config;
config.region = "us-west-2";

Aws::S3::S3Client s3_client(credentials, config);

// Method 2: Using IAM roles (for EC2 instances)
Aws::Auth::InstanceProfileCredentialsProvider provider;
Aws::S3::S3Client s3_client(provider, config);
```

### 3. Custom Configuration
```cpp
#include <aws/core/client/ClientConfiguration.h>

Aws::Client::ClientConfiguration config;
config.region = "us-east-1";
config.maxConnections = 25;
config.requestTimeoutMs = 30000;
config.connectTimeoutMs = 30000;

Aws::S3::S3Client s3_client(config);
```

## Error Handling Best Practices

### Comprehensive Error Handling
```cpp
#include <aws/core/utils/Outcome.h>

void HandleS3Operation() {
    Aws::S3::S3Client s3_client;
    Aws::S3::Model::ListBucketsRequest request;
    
    auto outcome = s3_client.ListBuckets(request);
    
    if (outcome.IsSuccess()) {
        // Handle success
        auto buckets = outcome.GetResult().GetBuckets();
        for (const auto& bucket : buckets) {
            std::cout << bucket.GetName() << std::endl;
        }
    } else {
        // Handle error
        auto error = outcome.GetError();
        std::cerr << "Error Code: " << static_cast<int>(error.GetErrorType()) << std::endl;
        std::cerr << "Error Message: " << error.GetMessage() << std::endl;
        std::cerr << "Request ID: " << error.GetRequestId() << std::endl;
        
        // Handle specific error types
        switch (error.GetErrorType()) {
            case Aws::S3::S3Errors::NO_SUCH_BUCKET:
                std::cerr << "Bucket does not exist" << std::endl;
                break;
            case Aws::S3::S3Errors::ACCESS_DENIED:
                std::cerr << "Access denied" << std::endl;
                break;
            default:
                std::cerr << "Unknown error occurred" << std::endl;
                break;
        }
    }
}
```

## Complete Example Application

```cpp
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/ListBucketsRequest.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <iostream>
#include <fstream>

class S3Manager {
private:
    Aws::S3::S3Client s3_client;
    
public:
    S3Manager() = default;
    
    void ListBuckets() {
        auto outcome = s3_client.ListBuckets();
        if (outcome.IsSuccess()) {
            std::cout << "Your buckets:\n";
            for (const auto& bucket : outcome.GetResult().GetBuckets()) {
                std::cout << "  " << bucket.GetName() 
                         << " (created " << bucket.GetCreationDate().ToGmtString(Aws::Utils::DateFormat::ISO_8601)
                         << ")" << std::endl;
            }
        } else {
            std::cerr << "ListBuckets error: " << outcome.GetError().GetMessage() << std::endl;
        }
    }
    
    bool CreateBucket(const std::string& bucket_name, const std::string& region = "us-west-2") {
        Aws::S3::Model::CreateBucketRequest request;
        request.SetBucket(bucket_name);
        
        if (region != "us-east-1") {
            Aws::S3::Model::CreateBucketConfiguration config;
            config.SetLocationConstraint(
                Aws::S3::Model::BucketLocationConstraintMapper::GetBucketLocationConstraintForName(region)
            );
            request.SetCreateBucketConfiguration(config);
        }
        
        auto outcome = s3_client.CreateBucket(request);
        if (outcome.IsSuccess()) {
            std::cout << "Created bucket: " << bucket_name << std::endl;
            return true;
        } else {
            std::cerr << "CreateBucket error: " << outcome.GetError().GetMessage() << std::endl;
            return false;
        }
    }
    
    bool UploadFile(const std::string& bucket_name, 
                   const std::string& key_name, 
                   const std::string& file_path) {
        
        Aws::S3::Model::PutObjectRequest request;
        request.SetBucket(bucket_name);
        request.SetKey(key_name);
        
        std::shared_ptr<Aws::IOStream> input_data = 
            Aws::MakeShared<Aws::FStream>("SampleAllocationTag", 
                                          file_path.c_str(), 
                                          std::ios_base::in | std::ios_base::binary);
        
        if (!input_data->good()) {
            std::cerr << "Failed to open file: " << file_path << std::endl;
            return false;
        }
        
        request.SetBody(input_data);
        
        auto outcome = s3_client.PutObject(request);
        if (outcome.IsSuccess()) {
            std::cout << "Successfully uploaded: " << key_name << std::endl;
            return true;
        } else {
            std::cerr << "Upload error: " << outcome.GetError().GetMessage() << std::endl;
            return false;
        }
    }
};

int main() {
    Aws::SDKOptions options;
    Aws::InitAPI(options);
    
    {
        S3Manager s3_manager;
        
        // List existing buckets
        s3_manager.ListBuckets();
        
        // Create a new bucket
        std::string bucket_name = "my-test-bucket-" + std::to_string(time(nullptr));
        if (s3_manager.CreateBucket(bucket_name)) {
            // Upload a file
            s3_manager.UploadFile(bucket_name, "test-file.txt", "/path/to/local/file.txt");
        }
    }
    
    Aws::ShutdownAPI(options);
    return 0;
}
```

## Tips and Best Practices

1. **Always initialize and shutdown the SDK properly**
2. **Use RAII patterns for resource management**
3. **Handle errors appropriately using the Outcome pattern**
4. **Configure timeouts and retry policies for production applications**
5. **Use IAM roles instead of hardcoded credentials when possible**
6. **Build only the AWS services you need to reduce binary size**
7. **Consider using async operations for better performance**
8. **Enable logging for debugging purposes**

## Building and Linking

Make sure your CMakeLists.txt properly links the required libraries:

```cmake
find_package(AWSSDK REQUIRED COMPONENTS core s3 ec2 dynamodb)
target_link_libraries(your_target ${AWSSDK_LINK_LIBRARIES})
```

This guide provides a solid foundation for working with the AWS SDK for C++. Each service has many more methods and options available - consult the official AWS SDK for C++ documentation for complete API reference.