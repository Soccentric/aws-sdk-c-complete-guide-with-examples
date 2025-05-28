/**
 * @file S3Manager.cpp
 * @brief Implementation of the S3Manager class
 */

#include "awsexamples/S3Manager.h"
#include <aws/s3/model/ListBucketsRequest.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <aws/s3/model/DeleteBucketRequest.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/DeleteObjectRequest.h>
#include <aws/s3/model/ListObjectsV2Request.h>
#include <aws/core/utils/memory/stl/AWSStreamFwd.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <fstream>
#include <iostream>

namespace awsexamples {

S3Manager::S3Manager() : s3Client() {}

S3Manager::S3Manager(const Aws::Client::ClientConfiguration& config) : s3Client(config) {}

void S3Manager::ListBuckets() {
    auto outcome = s3Client.ListBuckets();
    if (outcome.IsSuccess()) {
        std::cout << "Your S3 buckets:\n";
        for (const auto& bucket : outcome.GetResult().GetBuckets()) {
            std::cout << "  " << bucket.GetName() 
                     << " (created " << bucket.GetCreationDate().ToGmtString(Aws::Utils::DateFormat::ISO_8601)
                     << ")" << std::endl;
        }
    } else {
        std::cerr << "ListBuckets error: " << outcome.GetError().GetMessage() << std::endl;
    }
}

bool S3Manager::CreateBucket(const std::string& bucketName, const std::string& region) {
    Aws::S3::Model::CreateBucketRequest request;
    request.SetBucket(bucketName);
    
    if (region != "us-east-1") {
        Aws::S3::Model::CreateBucketConfiguration config;
        config.SetLocationConstraint(
            Aws::S3::Model::BucketLocationConstraintMapper::GetBucketLocationConstraintForName(region)
        );
        request.SetCreateBucketConfiguration(config);
    }
    
    auto outcome = s3Client.CreateBucket(request);
    if (outcome.IsSuccess()) {
        std::cout << "Created bucket: " << bucketName << std::endl;
        return true;
    } else {
        std::cerr << "CreateBucket error: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

bool S3Manager::DeleteBucket(const std::string& bucketName) {
    Aws::S3::Model::DeleteBucketRequest request;
    request.SetBucket(bucketName);
    
    auto outcome = s3Client.DeleteBucket(request);
    if (outcome.IsSuccess()) {
        std::cout << "Deleted bucket: " << bucketName << std::endl;
        return true;
    } else {
        std::cerr << "DeleteBucket error: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

bool S3Manager::UploadFile(const std::string& bucketName, 
                         const std::string& keyName, 
                         const std::string& filePath) {
    
    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(bucketName);
    request.SetKey(keyName);
    
    std::shared_ptr<Aws::IOStream> inputData = 
        Aws::MakeShared<Aws::FStream>("SampleAllocationTag", 
                                      filePath.c_str(), 
                                      std::ios_base::in | std::ios_base::binary);
    
    if (!inputData->good()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }
    
    request.SetBody(inputData);
    
    auto outcome = s3Client.PutObject(request);
    if (outcome.IsSuccess()) {
        std::cout << "Successfully uploaded: " << keyName << std::endl;
        return true;
    } else {
        std::cerr << "Upload error: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

bool S3Manager::UploadText(const std::string& bucketName, 
                         const std::string& keyName,
                         const std::string& content) {
    
    Aws::S3::Model::PutObjectRequest request;
    request.SetBucket(bucketName);
    request.SetKey(keyName);
    
    auto inputData = Aws::MakeShared<Aws::StringStream>("SampleAllocationTag");
    *inputData << content;
    
    request.SetBody(inputData);
    
    auto outcome = s3Client.PutObject(request);
    if (outcome.IsSuccess()) {
        std::cout << "Successfully uploaded text content as: " << keyName << std::endl;
        return true;
    } else {
        std::cerr << "Text upload error: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

bool S3Manager::DownloadFile(const std::string& bucketName, 
                           const std::string& keyName, 
                           const std::string& localPath) {
    
    Aws::S3::Model::GetObjectRequest request;
    request.SetBucket(bucketName);
    request.SetKey(keyName);
    
    auto outcome = s3Client.GetObject(request);
    
    if (outcome.IsSuccess()) {
        std::ofstream localFile(localPath, std::ios::binary);
        localFile << outcome.GetResult().GetBody().rdbuf();
        std::cout << "Successfully downloaded " << keyName << " to " << localPath << std::endl;
        return true;
    } else {
        std::cerr << "Download error: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

bool S3Manager::DeleteObject(const std::string& bucketName, const std::string& keyName) {
    Aws::S3::Model::DeleteObjectRequest request;
    request.SetBucket(bucketName);
    request.SetKey(keyName);
    
    auto outcome = s3Client.DeleteObject(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Successfully deleted " << keyName << " from " << bucketName << std::endl;
        return true;
    } else {
        std::cerr << "Delete object error: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

void S3Manager::ListObjects(const std::string& bucketName) {
    Aws::S3::Model::ListObjectsV2Request request;
    request.SetBucket(bucketName);
    
    auto outcome = s3Client.ListObjectsV2(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Objects in " << bucketName << ":" << std::endl;
        for (const auto& object : outcome.GetResult().GetContents()) {
            std::cout << "  " << object.GetKey() << " - Size: " << object.GetSize() << " bytes" <<
                " - Last Modified: " << object.GetLastModified().ToGmtString(Aws::Utils::DateFormat::ISO_8601) << std::endl;
        }
    } else {
        std::cerr << "ListObjects error: " << outcome.GetError().GetMessage() << std::endl;
    }
}

} // namespace awsexamples
