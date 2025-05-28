/**
 * @file DynamoDBExample.cpp
 * @brief Example application using the DynamoDBManager class
 */

#include "awsexamples/DynamoDBManager.h"
#include "awsexamples/AwsUtils.h"
#include <iostream>
#include <string>
#include <chrono>

/**
 * This example demonstrates how to use the DynamoDBManager class to:
 * - Create a new DynamoDB table
 * - Add items to the table
 * - Retrieve items from the table
 * - Scan the table for all items
 * - Delete items and the table
 */
int main(int argc, char** argv) {
    // Initialize AWS SDK
    awsexamples::utils::AwsApiInitializer awsInitializer;
    
    try {
        // Generate a unique table name with timestamp to avoid conflicts
        auto now = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(
            now.time_since_epoch()).count();
        std::string tableName = "Users_" + std::to_string(timestamp);
        
        std::cout << "DynamoDB Example - Using table name: " << tableName << std::endl;
        
        // Create DynamoDB Manager
        awsexamples::DynamoDBManager dynamodbManager;
        
        // Create table
        std::cout << "\n=== Creating DynamoDB Table ===" << std::endl;
        if (dynamodbManager.CreateTable(tableName)) {
            // Wait for table to become active
            std::cout << "Waiting for table to become active..." << std::endl;
            if (dynamodbManager.WaitForTableState(tableName, "ACTIVE", 60)) {
                std::cout << "Table is now active!" << std::endl;
                
                // Add items
                std::cout << "\n=== Adding Items to Table ===" << std::endl;
                dynamodbManager.PutItem(tableName, "user1", "Alice Smith", 28);
                dynamodbManager.PutItem(tableName, "user2", "Bob Johnson", 32);
                dynamodbManager.PutItem(tableName, "user3", "Charlie Brown", 45);
                
                // Get an item
                std::cout << "\n=== Retrieving an Item ===" << std::endl;
                dynamodbManager.GetItem(tableName, "user1");
                
                // Scan all items
                std::cout << "\n=== Scanning All Items ===" << std::endl;
                dynamodbManager.ScanTable(tableName);
                
                // Delete an item
                std::cout << "\n=== Deleting an Item ===" << std::endl;
                dynamodbManager.DeleteItem(tableName, "user2");
                
                // Scan after deletion
                std::cout << "\n=== Items After Deletion ===" << std::endl;
                dynamodbManager.ScanTable(tableName);
                
                // Delete the table
                std::cout << "\n=== Deleting Table ===" << std::endl;
                if (dynamodbManager.DeleteTable(tableName)) {
                    std::cout << "Waiting for table to be deleted..." << std::endl;
                    dynamodbManager.WaitForTableState(tableName, "DELETED", 60);
                }
            } else {
                std::cerr << "Timed out waiting for table to become active" << std::endl;
            }
        }
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
