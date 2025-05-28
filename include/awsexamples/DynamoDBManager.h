/**
 * @file DynamoDBManager.h
 * @brief DynamoDBManager class declaration for AWS DynamoDB operations
 * @author AWS Example Team
 * @date 2025-05-28
 */

#ifndef AWSEXAMPLES_DYNAMODBMANAGER_H
#define AWSEXAMPLES_DYNAMODBMANAGER_H

#include <aws/dynamodb/DynamoDBClient.h>
#include <string>

namespace awsexamples {

/**
 * @class DynamoDBManager
 * @brief A class to manage AWS DynamoDB operations
 *
 * This class provides a simplified interface for common DynamoDB operations
 * such as creating tables, managing items, and querying data.
 */
class DynamoDBManager {
public:
    /**
     * @brief Default constructor
     * 
     * Creates a DynamoDBManager with default client configuration
     */
    DynamoDBManager();
    
    /**
     * @brief Constructor with custom client configuration
     * 
     * @param config A custom AWS client configuration
     */
    explicit DynamoDBManager(const Aws::Client::ClientConfiguration& config);
    
    /**
     * @brief Create a new DynamoDB table
     * 
     * Creates a table with a string hash key named "id"
     * 
     * @param tableName The name of the table to create
     * @return bool True if the table was created successfully, false otherwise
     */
    bool CreateTable(const std::string& tableName);
    
    /**
     * @brief Add an item to a DynamoDB table
     * 
     * @param tableName The name of the table to add the item to
     * @param id The ID to use as the hash key
     * @param name A name value to store in the item
     * @param age An age value to store in the item
     * @return bool True if the item was added successfully, false otherwise
     */
    bool PutItem(const std::string& tableName, 
                const std::string& id, 
                const std::string& name, 
                int age);
    
    /**
     * @brief Retrieve an item from a DynamoDB table by ID
     * 
     * @param tableName The name of the table to query
     * @param id The ID of the item to retrieve
     */
    void GetItem(const std::string& tableName, const std::string& id);
    
    /**
     * @brief Scan all items in a DynamoDB table
     * 
     * @param tableName The name of the table to scan
     */
    void ScanTable(const std::string& tableName);
    
    /**
     * @brief Delete an item from a DynamoDB table
     * 
     * @param tableName The name of the table
     * @param id The ID of the item to delete
     * @return bool True if the item was deleted successfully, false otherwise
     */
    bool DeleteItem(const std::string& tableName, const std::string& id);
    
    /**
     * @brief Delete a DynamoDB table
     * 
     * @param tableName The name of the table to delete
     * @return bool True if the table was deleted successfully, false otherwise
     */
    bool DeleteTable(const std::string& tableName);

private:
    Aws::DynamoDB::DynamoDBClient client; ///< AWS DynamoDB client used for all operations
    
    /**
     * @brief Wait for a table to be in a certain state
     * 
     * @param tableName The name of the table to check
     * @param targetState The state to wait for (e.g. "ACTIVE", "DELETING")
     * @param maxWaitSeconds Maximum time to wait in seconds
     * @return bool True if the table reached the target state, false otherwise
     */
    bool WaitForTableState(
        const std::string& tableName, 
        const std::string& targetState, 
        int maxWaitSeconds = 60);
};

}  // namespace awsexamples

#endif  // AWSEXAMPLES_DYNAMODBMANAGER_H
