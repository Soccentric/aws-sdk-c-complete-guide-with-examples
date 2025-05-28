/**
 * @file AwsUtils.h
 * @brief Utility functions for AWS SDK initialization and configuration
 * @author AWS Example Team
 * @date 2025-05-28
 */

#ifndef AWSEXAMPLES_AWSUTILS_H
#define AWSEXAMPLES_AWSUTILS_H

#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/core/client/ClientConfiguration.h>
#include <string>

namespace awsexamples {
namespace utils {

/**
 * @brief Configure client with custom settings
 * 
 * @param region AWS region to use
 * @param logLevel AWS SDK log level
 * @param timeoutMs Connection timeout in milliseconds
 * @param maxConnections Maximum number of connections to maintain
 * @return Aws::Client::ClientConfiguration Configured client settings
 */
Aws::Client::ClientConfiguration ConfigureClient(
    const std::string& region = "us-west-2",
    Aws::Utils::Logging::LogLevel logLevel = Aws::Utils::Logging::LogLevel::Info,
    long timeoutMs = 30000,
    unsigned maxConnections = 25);

/**
 * @brief Create default SDK options
 * 
 * @param logLevel Log level for the AWS SDK
 * @return Aws::SDKOptions Configured SDK options
 */
Aws::SDKOptions CreateDefaultSDKOptions(
    Aws::Utils::Logging::LogLevel logLevel = Aws::Utils::Logging::LogLevel::Info);

/**
 * @class AwsApiInitializer
 * @brief RAII wrapper for AWS SDK initialization/shutdown
 * 
 * This class handles the initialization and shutdown of the AWS SDK
 * automatically when it is constructed and destroyed.
 */
class AwsApiInitializer {
public:
    /**
     * @brief Constructor that initializes AWS SDK
     * 
     * @param options SDK options to use for initialization
     */
    explicit AwsApiInitializer(const Aws::SDKOptions& options = CreateDefaultSDKOptions());
    
    /**
     * @brief Destructor that shuts down AWS SDK
     */
    ~AwsApiInitializer();
    
    // Delete copy and move operations
    AwsApiInitializer(const AwsApiInitializer&) = delete;
    AwsApiInitializer& operator=(const AwsApiInitializer&) = delete;
    AwsApiInitializer(AwsApiInitializer&&) = delete;
    AwsApiInitializer& operator=(AwsApiInitializer&&) = delete;

private:
    Aws::SDKOptions options; ///< SDK options used for initialization
};

} // namespace utils
} // namespace awsexamples

#endif // AWSEXAMPLES_AWSUTILS_H
