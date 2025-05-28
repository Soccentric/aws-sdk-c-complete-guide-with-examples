/**
 * @file S3ManagerTest.cpp
 * @brief Test cases for the S3Manager class
 */

#include "awsexamples/S3Manager.h"
#include "awsexamples/AwsUtils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <aws/core/utils/UUID.h>

// Basic test to verify S3Manager functionality
bool TestS3Manager() {
    bool allTestsPassed = true;
    
    // Create a unique bucket name with UUID to avoid conflicts
    Aws::String uuid = Aws::Utils::UUID::RandomUUID();
    std::string bucketName = "aws-cpp-test-" + std::string(uuid.c_str(), 8);
    
    std::cout << "=== S3Manager Test ===" << std::endl;
    std::cout << "Using test bucket name: " << bucketName << std::endl;
    
    awsexamples::S3Manager s3Manager;
    
    // Test listing buckets (no assertion, just informational)
    std::cout << "\n1. Listing buckets:" << std::endl;
    s3Manager.ListBuckets();
    
    // Test bucket creation
    std::cout << "\n2. Creating bucket:" << std::endl;
    bool bucketCreated = s3Manager.CreateBucket(bucketName);
    if (!bucketCreated) {
        std::cerr << "FAILED: Could not create bucket" << std::endl;
        allTestsPassed = false;
        return allTestsPassed;  // Early return as other tests depend on this
    }
    std::cout << "PASSED: Bucket created successfully" << std::endl;
    
    // Wait a moment for bucket to be fully available
    std::cout << "Waiting for bucket to be fully available..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    // Test uploading text content
    std::cout << "\n3. Uploading text content:" << std::endl;
    std::string textContent = "This is a test file for S3 upload.";
    bool textUploaded = s3Manager.UploadText(bucketName, "test.txt", textContent);
    if (!textUploaded) {
        std::cerr << "FAILED: Could not upload text" << std::endl;
        allTestsPassed = false;
    } else {
        std::cout << "PASSED: Text uploaded successfully" << std::endl;
    }
    
    // Test listing objects
    std::cout << "\n4. Listing objects:" << std::endl;
    s3Manager.ListObjects(bucketName);
    
    // Test downloading file
    std::cout << "\n5. Downloading file:" << std::endl;
    std::string downloadPath = "test-download.txt";
    bool fileDownloaded = s3Manager.DownloadFile(bucketName, "test.txt", downloadPath);
    if (!fileDownloaded) {
        std::cerr << "FAILED: Could not download file" << std::endl;
        allTestsPassed = false;
    } else {
        // Verify file contents
        std::ifstream file(downloadPath);
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        
        if (content == textContent) {
            std::cout << "PASSED: File downloaded and contents match" << std::endl;
        } else {
            std::cerr << "FAILED: File contents don't match" << std::endl;
            allTestsPassed = false;
        }
    }
    
    // Test deleting object
    std::cout << "\n6. Deleting object:" << std::endl;
    bool objectDeleted = s3Manager.DeleteObject(bucketName, "test.txt");
    if (!objectDeleted) {
        std::cerr << "FAILED: Could not delete object" << std::endl;
        allTestsPassed = false;
    } else {
        std::cout << "PASSED: Object deleted successfully" << std::endl;
    }
    
    // Test deleting bucket
    std::cout << "\n7. Deleting bucket:" << std::endl;
    bool bucketDeleted = s3Manager.DeleteBucket(bucketName);
    if (!bucketDeleted) {
        std::cerr << "FAILED: Could not delete bucket" << std::endl;
        allTestsPassed = false;
    } else {
        std::cout << "PASSED: Bucket deleted successfully" << std::endl;
    }
    
    // Clean up
    std::remove(downloadPath.c_str());
    
    return allTestsPassed;
}

int main() {
    // Initialize AWS SDK
    awsexamples::utils::AwsApiInitializer awsInitializer;
    
    // Run the test
    bool testResult = TestS3Manager();
    
    // Print final result
    if (testResult) {
        std::cout << "\nALL TESTS PASSED!" << std::endl;
        return 0;
    } else {
        std::cerr << "\nTESTS FAILED!" << std::endl;
        return 1;
    }
}
