/**
 * @file S3Manager.h
 * @brief S3Manager class declaration for AWS S3 operations
 * @author AWS Example Team
 * @date 2025-05-28
 */

#ifndef AWSEXAMPLES_S3MANAGER_H
#define AWSEXAMPLES_S3MANAGER_H

#include <aws/s3/S3Client.h>
#include <string>

namespace awsexamples {

/**
 * @class S3Manager
 * @brief A class to manage AWS S3 operations
 *
 * This class provides a simplified interface for common S3 operations
 * such as listing buckets, creating buckets, uploading and downloading files.
 */
class S3Manager {
public:
    /**
     * @brief Default constructor
     * 
     * Creates an S3Manager with default client configuration
     */
    S3Manager();
    
    /**
     * @brief Constructor with custom client configuration
     * 
     * @param config A custom AWS client configuration
     */
    explicit S3Manager(const Aws::Client::ClientConfiguration& config);
    
    /**
     * @brief List all S3 buckets available to the user
     */
    void ListBuckets();
    
    /**
     * @brief Create a new S3 bucket
     * 
     * @param bucketName The name of the bucket to create
     * @param region The AWS region where the bucket should be created
     * @return bool True if the bucket was created successfully, false otherwise
     */
    bool CreateBucket(const std::string& bucketName, const std::string& region = "us-west-2");
    
    /**
     * @brief Delete an S3 bucket
     * 
     * @param bucketName The name of the bucket to delete
     * @return bool True if the bucket was deleted successfully, false otherwise
     */
    bool DeleteBucket(const std::string& bucketName);
    
    /**
     * @brief Upload a file to S3
     * 
     * @param bucketName The name of the bucket to upload to
     * @param keyName The key (object name) to assign to the uploaded file
     * @param filePath Local path to the file to be uploaded
     * @return bool True if the file was uploaded successfully, false otherwise
     */
    bool UploadFile(const std::string& bucketName, 
                   const std::string& keyName, 
                   const std::string& filePath);
    
    /**
     * @brief Upload text content to S3
     * 
     * @param bucketName The name of the bucket to upload to
     * @param keyName The key (object name) to assign to the uploaded content
     * @param content The text content to upload
     * @return bool True if the content was uploaded successfully, false otherwise
     */
    bool UploadText(const std::string& bucketName, 
                   const std::string& keyName,
                   const std::string& content);
    
    /**
     * @brief Download a file from S3
     * 
     * @param bucketName The name of the bucket to download from
     * @param keyName The key (object name) of the file to download
     * @param localPath Local path where the file should be saved
     * @return bool True if the file was downloaded successfully, false otherwise
     */
    bool DownloadFile(const std::string& bucketName, 
                     const std::string& keyName, 
                     const std::string& localPath);
    
    /**
     * @brief Delete an object from S3
     * 
     * @param bucketName The name of the bucket containing the object
     * @param keyName The key (object name) of the object to delete
     * @return bool True if the object was deleted successfully, false otherwise
     */
    bool DeleteObject(const std::string& bucketName, const std::string& keyName);
    
    /**
     * @brief List objects in an S3 bucket
     * 
     * @param bucketName The name of the bucket to list objects from
     */
    void ListObjects(const std::string& bucketName);

private:
    Aws::S3::S3Client s3Client; ///< AWS S3 client used for all operations
};

}  // namespace awsexamples

#endif  // AWSEXAMPLES_S3MANAGER_H
