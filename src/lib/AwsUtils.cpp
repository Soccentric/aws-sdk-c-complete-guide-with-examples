/**
 * @file AwsUtils.cpp
 * @brief Implementation of AWS utility functions
 */

#include "awsexamples/AwsUtils.h"
#include <iostream>

namespace awsexamples {
namespace utils {

Aws::Client::ClientConfiguration ConfigureClient(
    const std::string& region,
    Aws::Utils::Logging::LogLevel logLevel,
    long timeoutMs,
    unsigned maxConnections) {
    
    Aws::Client::ClientConfiguration config;
    config.region = region;
    config.connectTimeoutMs = timeoutMs;
    config.requestTimeoutMs = timeoutMs;
    config.maxConnections = maxConnections;
    
    return config;
}

Aws::SDKOptions CreateDefaultSDKOptions(Aws::Utils::Logging::LogLevel logLevel) {
    Aws::SDKOptions options;
    options.loggingOptions.logLevel = logLevel;
    return options;
}

AwsApiInitializer::AwsApiInitializer(const Aws::SDKOptions& options) : options(options) {
    Aws::InitAPI(this->options);
    std::cout << "AWS SDK initialized" << std::endl;
}

AwsApiInitializer::~AwsApiInitializer() {
    Aws::ShutdownAPI(options);
    std::cout << "AWS SDK shutdown complete" << std::endl;
}

} // namespace utils
} // namespace awsexamples
