#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <string>
#include <vector>
#include <pqxx/pqxx>

using namespace std;

template<typename T>
class DatabaseConnection {
private:
    pqxx::connection* connection;
    pqxx::work* transaction;
    
public:
    DatabaseConnection(string connectionString) {
        connection = new pqxx::connection(connectionString);
    }
    
    ~DatabaseConnection() {
        if (transaction) {
            delete transaction;
        }
        if (connection) {
            connection->disconnect();
            delete connection;
        }
    }
    
    vector<vector<string>> executeQuery(string query) {
        pqxx::nontransaction work(*connection);
        pqxx::result result = work.exec(query);
        
        vector<vector<string>> rows;
        for (auto row : result) {
            vector<string> columns;
            for (auto field : row) {
                columns.push_back(field.c_str());
            }
            rows.push_back(columns);
        }
        return rows;
    }
    
    void executeNonQuery(string query) {
        pqxx::work work(*connection);
        work.exec(query);
        work.commit();
    }
    
    void beginTransaction() {
        if (!transaction) {
            transaction = new pqxx::work(*connection);
        }
    }
    
    void commitTransaction() {
        if (transaction) {
            transaction->commit();
            delete transaction;
            transaction = nullptr;
        }
    }
    
    void rollbackTransaction() {
        if (transaction) {
            transaction->abort();
            delete transaction;
            transaction = nullptr;
        }
    }
    
    void createFunction(string name, string definition) {
        string query = "CREATE OR REPLACE FUNCTION " + name + " " + definition;
        executeNonQuery(query);
    }
    
    void createTrigger(string name, string definition) {
        string query = "CREATE TRIGGER " + name + " " + definition;
        executeNonQuery(query);
    }
    
    bool getTransactionStatus() {
        return transaction != nullptr;
    }
};

#endif
