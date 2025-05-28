/**
 * @file EC2Example.cpp
 * @brief Example application using the EC2Manager class
 */

#include "awsexamples/EC2Manager.h"
#include "awsexamples/AwsUtils.h"
#include <iostream>
#include <string>

/**
 * This example demonstrates how to use the EC2Manager class to:
 * - List existing EC2 instances
 * - Start and stop instances
 * - Launch new instances and terminate them
 * 
 * NOTE: This example primarily lists existing instances.
 * The code for launching/starting/stopping instances is commented out
 * to prevent accidental AWS resource creation and potential charges.
 */
int main(int argc, char** argv) {
    // Initialize AWS SDK
    awsexamples::utils::AwsApiInitializer awsInitializer;
    
    try {
        std::cout << "EC2 Example" << std::endl;
        
        // Create EC2 Manager
        awsexamples::EC2Manager ec2Manager;
        
        // List existing instances
        std::cout << "\n=== Listing Existing EC2 Instances ===" << std::endl;
        ec2Manager.ListInstances();
        
        // The following sections are commented out to prevent accidental
        // creation of AWS resources that could incur charges.
        // Uncomment and modify if you want to test these features.
        
        /*
        // Get an instance ID from command line if provided
        std::string instanceId;
        if (argc > 1) {
            instanceId = argv[1];
            std::cout << "\n=== Using instance ID: " << instanceId << " ===" << std::endl;
            
            // Start the instance
            std::cout << "\n=== Starting Instance ===" << std::endl;
            if (ec2Manager.StartInstance(instanceId)) {
                // Wait for instance to start
                std::cout << "Waiting for instance to start..." << std::endl;
                if (ec2Manager.WaitForInstanceState(instanceId, "running", 120)) {
                    std::cout << "Instance is now running!" << std::endl;
                }
                
                // Wait a bit before stopping
                std::cout << "Waiting 30 seconds before stopping..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(30));
                
                // Stop the instance
                std::cout << "\n=== Stopping Instance ===" << std::endl;
                if (ec2Manager.StopInstance(instanceId)) {
                    // Wait for instance to stop
                    std::cout << "Waiting for instance to stop..." << std::endl;
                    ec2Manager.WaitForInstanceState(instanceId, "stopped", 120);
                }
            }
        } else {
            std::cout << "\nNo instance ID provided. Skipping start/stop operations." << std::endl;
            std::cout << "To test with an existing instance, run with instance ID as parameter." << std::endl;
        }
        
        // Launch a new instance
        // WARNING: This will create a real EC2 instance that may incur charges
        std::cout << "\n=== Launching a New Instance ===" << std::endl;
        // Replace with a valid AMI ID for your region
        std::string amiId = "ami-12345678"; // REPLACE WITH VALID AMI ID
        std::string instanceType = "t2.micro";
        std::string keyName = ""; // Set to your key name if you want SSH access
        
        std::string newInstanceId = ec2Manager.LaunchInstance(amiId, instanceType, keyName);
        
        if (!newInstanceId.empty()) {
            std::cout << "Waiting for instance to start..." << std::endl;
            if (ec2Manager.WaitForInstanceState(newInstanceId, "running", 120)) {
                std::cout << "Instance is now running!" << std::endl;
                
                // Wait before terminating
                std::cout << "Waiting 60 seconds before terminating..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(60));
                
                // Terminate the instance
                std::cout << "\n=== Terminating Instance ===" << std::endl;
                if (ec2Manager.TerminateInstance(newInstanceId)) {
                    // Wait for instance to terminate
                    std::cout << "Waiting for instance to terminate..." << std::endl;
                    ec2Manager.WaitForInstanceState(newInstanceId, "terminated", 120);
                }
            }
        }
        */
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
