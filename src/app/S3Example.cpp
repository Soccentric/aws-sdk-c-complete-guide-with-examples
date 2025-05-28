/**
 * @file S3Example.cpp
 * @brief Example application using the S3Manager class
 */

#include "awsexamples/S3Manager.h"
#include "awsexamples/AwsUtils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <aws/core/utils/UUID.h>

/**
 * This example demonstrates how to use the S3Manager class to:
 * - List existing buckets
 * - Create a new bucket with a unique name
 * - Upload text content and files
 * - List objects in the bucket
 * - Download files
 * - Delete objects and the bucket
 */
int main(int argc, char** argv) {
    // Initialize AWS SDK
    awsexamples::utils::AwsApiInitializer awsInitializer;
    
    try {
        // Generate a unique bucket name with UUID to avoid conflicts
        Aws::String uuid = Aws::Utils::UUID::RandomUUID();
        std::string bucketName = "aws-cpp-sample-" + std::string(uuid.c_str(), 8);
        
        std::cout << "S3 Example - Using bucket name: " << bucketName << std::endl;
        
        // Create S3 Manager
        awsexamples::S3Manager s3Manager;
        
        // List existing buckets before creating new one
        std::cout << "\n=== Existing Buckets ===" << std::endl;
        s3Manager.ListBuckets();
        
        // Create a new bucket
        if (s3Manager.CreateBucket(bucketName)) {
            // Wait for bucket to be created
            std::cout << "Waiting for bucket to be fully created..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
            
            // Upload some content
            s3Manager.UploadText(bucketName, "hello.txt", "Hello, AWS S3!");
            s3Manager.UploadText(bucketName, "data/config.json", "{ \"app\": \"aws-example\", \"version\": \"1.0.0\" }");
            
            // Create a local file for testing
            std::string testFilePath = "test-upload.txt";
            {
                std::ofstream testFile(testFilePath);
                testFile << "This is a test file for AWS S3 upload.\n";
                testFile << "It contains multiple lines of text.\n";
                testFile << "The AWS SDK for C++ makes S3 operations easy!" << std::endl;
            }
            
            // Upload the file
            s3Manager.UploadFile(bucketName, "files/test-upload.txt", testFilePath);
            
            // List objects in the bucket
            std::cout << "\n=== Objects in Bucket ===" << std::endl;
            s3Manager.ListObjects(bucketName);
            
            // Download a file
            std::string downloadPath = "downloaded-hello.txt";
            s3Manager.DownloadFile(bucketName, "hello.txt", downloadPath);
            
            // Display the downloaded content
            {
                std::ifstream file(downloadPath);
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                std::cout << "Downloaded content: " << content << std::endl;
            }
            
            // Delete objects
            std::cout << "\n=== Cleaning Up ===" << std::endl;
            s3Manager.DeleteObject(bucketName, "hello.txt");
            s3Manager.DeleteObject(bucketName, "data/config.json");
            s3Manager.DeleteObject(bucketName, "files/test-upload.txt");
            
            // List objects after deletion
            std::cout << "\n=== Objects After Deletion ===" << std::endl;
            s3Manager.ListObjects(bucketName);
            
            // Delete the bucket
            std::cout << "Waiting a moment before deleting bucket..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(5));
            s3Manager.DeleteBucket(bucketName);
            
            // List buckets after deletion
            std::cout << "\n=== Buckets After Deletion ===" << std::endl;
            s3Manager.ListBuckets();
            
            // Clean up local files
            std::remove(testFilePath.c_str());
            std::remove(downloadPath.c_str());
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
