/**
 * @file DynamoDBManager.cpp
 * @brief Implementation of the DynamoDBManager class
 */

#include "awsexamples/DynamoDBManager.h"
#include <aws/dynamodb/model/CreateTableRequest.h>
#include <aws/dynamodb/model/DeleteTableRequest.h>
#include <aws/dynamodb/model/AttributeDefinition.h>
#include <aws/dynamodb/model/KeySchemaElement.h>
#include <aws/dynamodb/model/ProvisionedThroughput.h>
#include <aws/dynamodb/model/PutItemRequest.h>
#include <aws/dynamodb/model/GetItemRequest.h>
#include <aws/dynamodb/model/DeleteItemRequest.h>
#include <aws/dynamodb/model/ScanRequest.h>
#include <aws/dynamodb/model/DescribeTableRequest.h>
#include <iostream>
#include <thread>
#include <chrono>

namespace awsexamples {

DynamoDBManager::DynamoDBManager() : client() {}

DynamoDBManager::DynamoDBManager(const Aws::Client::ClientConfiguration& config) : client(config) {}

bool DynamoDBManager::CreateTable(const std::string& tableName) {
    Aws::DynamoDB::Model::CreateTableRequest request;
    
    // Set table name
    request.SetTableName(tableName);
    
    // Define attributes
    Aws::DynamoDB::Model::AttributeDefinition hashKey;
    hashKey.SetAttributeName("id");
    hashKey.SetAttributeType(Aws::DynamoDB::Model::ScalarAttributeType::S);
    request.AddAttributeDefinitions(hashKey);
    
    // Define key schema
    Aws::DynamoDB::Model::KeySchemaElement keySchema;
    keySchema.SetAttributeName("id");
    keySchema.SetKeyType(Aws::DynamoDB::Model::KeyType::HASH);
    request.AddKeySchema(keySchema);
    
    // Set provisioned throughput
    Aws::DynamoDB::Model::ProvisionedThroughput throughput;
    throughput.SetReadCapacityUnits(5);
    throughput.SetWriteCapacityUnits(5);
    request.SetProvisionedThroughput(throughput);
    
    auto outcome = client.CreateTable(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Table " << tableName << " created successfully!" << std::endl;
        return true;
    } else {
        std::cerr << "Error creating table: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

bool DynamoDBManager::PutItem(const std::string& tableName, 
                            const std::string& id, 
                            const std::string& name, 
                            int age) {
    
    Aws::DynamoDB::Model::PutItemRequest request;
    
    request.SetTableName(tableName);
    
    // Create item attributes
    Aws::DynamoDB::Model::AttributeValue idAttr;
    idAttr.SetS(id);
    request.AddItem("id", idAttr);
    
    Aws::DynamoDB::Model::AttributeValue nameAttr;
    nameAttr.SetS(name);
    request.AddItem("name", nameAttr);
    
    Aws::DynamoDB::Model::AttributeValue ageAttr;
    ageAttr.SetN(std::to_string(age));
    request.AddItem("age", ageAttr);
    
    auto outcome = client.PutItem(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Item added successfully!" << std::endl;
        return true;
    } else {
        std::cerr << "Error: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

void DynamoDBManager::GetItem(const std::string& tableName, const std::string& id) {
    Aws::DynamoDB::Model::GetItemRequest request;
    
    request.SetTableName(tableName);
    
    // Set key
    Aws::DynamoDB::Model::AttributeValue keyAttr;
    keyAttr.SetS(id);
    request.AddKey("id", keyAttr);
    
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
        std::cerr << "Error: " << outcome.GetError().GetMessage() << std::endl;
    }
}

void DynamoDBManager::ScanTable(const std::string& tableName) {
    Aws::DynamoDB::Model::ScanRequest request;
    request.SetTableName(tableName);
    
    auto outcome = client.Scan(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Items in " << tableName << ":" << std::endl;
        const auto& items = outcome.GetResult().GetItems();
        
        if (items.empty()) {
            std::cout << "No items found in the table" << std::endl;
            return;
        }
        
        for (const auto& item : items) {
            std::cout << "ID: " << item.at("id").GetS() 
                      << ", Name: " << item.at("name").GetS()
                      << ", Age: " << item.at("age").GetN() << std::endl;
        }
    } else {
        std::cerr << "Error: " << outcome.GetError().GetMessage() << std::endl;
    }
}

bool DynamoDBManager::DeleteItem(const std::string& tableName, const std::string& id) {
    Aws::DynamoDB::Model::DeleteItemRequest request;
    
    request.SetTableName(tableName);
    
    // Set key to delete
    Aws::DynamoDB::Model::AttributeValue keyAttr;
    keyAttr.SetS(id);
    request.AddKey("id", keyAttr);
    
    auto outcome = client.DeleteItem(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Item deleted successfully!" << std::endl;
        return true;
    } else {
        std::cerr << "Error deleting item: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

bool DynamoDBManager::DeleteTable(const std::string& tableName) {
    Aws::DynamoDB::Model::DeleteTableRequest request;
    request.SetTableName(tableName);
    
    auto outcome = client.DeleteTable(request);
    
    if (outcome.IsSuccess()) {
        std::cout << "Table " << tableName << " deleted successfully!" << std::endl;
        return true;
    } else {
        std::cerr << "Error deleting table: " << outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}

bool DynamoDBManager::WaitForTableState(
    const std::string& tableName,
    const std::string& targetState,
    int maxWaitSeconds) {
    
    Aws::DynamoDB::Model::DescribeTableRequest request;
    request.SetTableName(tableName);
    
    int waitTimeSeconds = 0;
    bool tableInTargetState = false;
    
    while (!tableInTargetState && waitTimeSeconds < maxWaitSeconds) {
        auto outcome = client.DescribeTable(request);
        
        if (outcome.IsSuccess()) {
            const auto& tableStatus = outcome.GetResult().GetTable().GetTableStatus();
            const auto status = Aws::DynamoDB::Model::TableStatusMapper::GetNameForTableStatus(tableStatus);
            
            std::cout << "Table status: " << status << std::endl;
            
            if (status == targetState) {
                tableInTargetState = true;
                break;
            }
        } else {
            if (targetState == "DELETED" && 
                outcome.GetError().GetErrorType() == Aws::DynamoDB::DynamoDBErrors::RESOURCE_NOT_FOUND) {
                tableInTargetState = true;
                break;
            } else {
                std::cerr << "Error checking table status: " << outcome.GetError().GetMessage() << std::endl;
                return false;
            }
        }
        
        // Wait before checking again
        std::this_thread::sleep_for(std::chrono::seconds(5));
        waitTimeSeconds += 5;
    }
    
    return tableInTargetState;
}

} // namespace awsexamples
