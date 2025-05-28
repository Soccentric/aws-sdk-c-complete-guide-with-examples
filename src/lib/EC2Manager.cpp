/**
 * @file EC2Manager.cpp
 * @brief Implementation of the EC2Manager class
 */

#include "awsexamples/EC2Manager.h"
#include <aws/ec2/model/DescribeInstancesRequest.h>
#include <aws/ec2/model/StartInstancesRequest.h>
#include <aws/ec2/model/StopInstancesRequest.h>
#include <aws/ec2/model/RunInstancesRequest.h>
#include <aws/ec2/model/TerminateInstancesRequest.h>
#include <iostream>
#include <thread>
#include <chrono>

namespace awsexamples {

EC2Manager::EC2Manager() : ec2Client() {}

EC2Manager::EC2Manager(const Aws::Client::ClientConfiguration& config) : ec2Client(config) {}

void EC2Manager::ListInstances() {
    Aws::EC2::Model::DescribeInstancesRequest request;
    
    auto outcome = ec2Client.DescribeInstances(request);
    
    if (outcome.IsSuccess()) {
        const auto& reservations = outcome.GetResult().GetReservations();
        
        if (reservations.empty()) {
            std::cout << "No EC2 instances found." << std::endl;
            return;
        }
        
        std::cout << "EC2 Instances:" << std::endl;
        for (const auto& reservation : reservations) {
            for (const auto& instance : reservation.GetInstances()) {
                std::cout << "Instance ID: " << instance.GetInstanceId() << std::endl;
                std::cout << "State: " << Aws::EC2::Model::InstanceStateNameMapper::GetNameForInstanceStateName(instance.GetState().GetName()) << std::endl;
                std::cout << "Type: " << instance.GetInstanceTypeAsString() << std::endl;
                
                // Print tags
                std::cout << "Tags: ";
                for (const auto& tag : instance.GetTags()) {
                    std::cout << tag.GetKey() << "=" << tag.GetValue() << " ";
                }
                std::cout << std::endl;
                std::cout << "---" << std::endl;
            }
        }
    } else {
        std::cerr << "Error: " << outcome.GetError().GetMessage() << std::endl;
    }
}

bool EC2Manager::StartInstance(const std::string& instanceId) {
    Aws::EC2::Model::StartInstancesRequest request;
    
    request.AddInstanceIds(instanceId);
    
    auto outcome = ec2Client.StartInstances(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Instance " << instanceId << " start initiated" << std::endl;
        return true;
    } else {
        std::cerr << "Error starting instance: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

bool EC2Manager::StopInstance(const std::string& instanceId) {
    Aws::EC2::Model::StopInstancesRequest request;
    
    request.AddInstanceIds(instanceId);
    
    auto outcome = ec2Client.StopInstances(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Instance " << instanceId << " stop initiated" << std::endl;
        return true;
    } else {
        std::cerr << "Error stopping instance: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

std::string EC2Manager::LaunchInstance(
    const std::string& amiId, 
    const std::string& instanceType,
    const std::string& keyName) {
    
    Aws::EC2::Model::RunInstancesRequest request;
    
    request.SetImageId(amiId);
    request.SetInstanceType(
        Aws::EC2::Model::InstanceTypeMapper::GetInstanceTypeForName(instanceType)
    );
    request.SetMinCount(1);
    request.SetMaxCount(1);
    
    if (!keyName.empty()) {
        request.SetKeyName(keyName);
    }
    
    auto outcome = ec2Client.RunInstances(request);
    
    if (outcome.IsSuccess()) {
        const auto& instances = outcome.GetResult().GetInstances();
        if (!instances.empty()) {
            std::string instanceId = instances[0].GetInstanceId();
            std::cout << "Launched instance " << instanceId << std::endl;
            return instanceId;
        } else {
            std::cerr << "Error: No instances were launched" << std::endl;
            return "";
        }
    } else {
        std::cerr << "Error launching instance: " << outcome.GetError().GetMessage() << std::endl;
        return "";
    }
}

bool EC2Manager::TerminateInstance(const std::string& instanceId) {
    Aws::EC2::Model::TerminateInstancesRequest request;
    
    request.AddInstanceIds(instanceId);
    
    auto outcome = ec2Client.TerminateInstances(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Instance " << instanceId << " termination initiated" << std::endl;
        return true;
    } else {
        std::cerr << "Error terminating instance: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

std::string EC2Manager::GetInstanceState(const std::string& instanceId) {
    Aws::EC2::Model::DescribeInstancesRequest request;
    
    Aws::EC2::Model::Filter filter;
    filter.SetName("instance-id");
    filter.AddValues(instanceId);
    request.AddFilters(filter);
    
    auto outcome = ec2Client.DescribeInstances(request);
    
    if (outcome.IsSuccess()) {
        const auto& reservations = outcome.GetResult().GetReservations();
        
        if (!reservations.empty() && !reservations[0].GetInstances().empty()) {
            const auto& instance = reservations[0].GetInstances()[0];
            return Aws::EC2::Model::InstanceStateNameMapper::GetNameForInstanceStateName(
                instance.GetState().GetName());
        } else {
            std::cerr << "Could not find instance: " << instanceId << std::endl;
            return "";
        }
    } else {
        std::cerr << "Error getting instance state: " << outcome.GetError().GetMessage() << std::endl;
        return "";
    }
}

bool EC2Manager::WaitForInstanceState(
    const std::string& instanceId, 
    const std::string& targetState, 
    int maxWaitSeconds) {
    
    int waitTimeSeconds = 0;
    bool instanceInTargetState = false;
    
    while (!instanceInTargetState && waitTimeSeconds < maxWaitSeconds) {
        std::string state = GetInstanceState(instanceId);
        
        if (state == targetState) {
            instanceInTargetState = true;
            break;
        } else if (state.empty()) {
            // Error occurred
            return false;
        }
        
        std::cout << "Instance " << instanceId << " state: " << state 
                  << " (waiting for " << targetState << ")" << std::endl;
        
        // Wait before checking again
        std::this_thread::sleep_for(std::chrono::seconds(10));
        waitTimeSeconds += 10;
    }
    
    return instanceInTargetState;
}

} // namespace awsexamples
