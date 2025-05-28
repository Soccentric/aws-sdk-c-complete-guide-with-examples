/**
 * @file EC2Manager.h
 * @brief EC2Manager class declaration for AWS EC2 operations
 * @author AWS Example Team
 * @date 2025-05-28
 */

#ifndef AWSEXAMPLES_EC2MANAGER_H
#define AWSEXAMPLES_EC2MANAGER_H

#include <aws/ec2/EC2Client.h>
#include <string>
#include <vector>

namespace awsexamples {

/**
 * @class EC2Manager
 * @brief A class to manage AWS EC2 operations
 *
 * This class provides a simplified interface for common EC2 operations
 * such as listing instances, starting and stopping instances, and launching new instances.
 */
class EC2Manager {
public:
    /**
     * @brief Default constructor
     * 
     * Creates an EC2Manager with default client configuration
     */
    EC2Manager();
    
    /**
     * @brief Constructor with custom client configuration
     * 
     * @param config A custom AWS client configuration
     */
    explicit EC2Manager(const Aws::Client::ClientConfiguration& config);
    
    /**
     * @brief List all EC2 instances available to the user
     */
    void ListInstances();
    
    /**
     * @brief Start an EC2 instance
     * 
     * @param instanceId The ID of the instance to start
     * @return bool True if the start request was successful, false otherwise
     */
    bool StartInstance(const std::string& instanceId);
    
    /**
     * @brief Stop an EC2 instance
     * 
     * @param instanceId The ID of the instance to stop
     * @return bool True if the stop request was successful, false otherwise
     */
    bool StopInstance(const std::string& instanceId);
    
    /**
     * @brief Launch a new EC2 instance
     * 
     * @param amiId The ID of the AMI to use
     * @param instanceType The EC2 instance type to launch
     * @param keyName Optional SSH key name to associate with the instance
     * @return std::string The ID of the newly launched instance, or empty string on failure
     */
    std::string LaunchInstance(
        const std::string& amiId, 
        const std::string& instanceType,
        const std::string& keyName = "");
    
    /**
     * @brief Terminate an EC2 instance
     * 
     * @param instanceId The ID of the instance to terminate
     * @return bool True if the terminate request was successful, false otherwise
     */
    bool TerminateInstance(const std::string& instanceId);
    
    /**
     * @brief Wait for an instance to reach a specific state
     * 
     * @param instanceId The ID of the instance to check
     * @param targetState The state to wait for (e.g., "running", "stopped")
     * @param maxWaitSeconds Maximum time to wait in seconds
     * @return bool True if the instance reached the target state, false otherwise
     */
    bool WaitForInstanceState(
        const std::string& instanceId, 
        const std::string& targetState, 
        int maxWaitSeconds = 120);

private:
    Aws::EC2::EC2Client ec2Client; ///< AWS EC2 client used for all operations
    
    /**
     * @brief Get the current state of an EC2 instance
     * 
     * @param instanceId The ID of the instance to check
     * @return std::string The current state of the instance
     */
    std::string GetInstanceState(const std::string& instanceId);
};

}  // namespace awsexamples

#endif  // AWSEXAMPLES_EC2MANAGER_H
