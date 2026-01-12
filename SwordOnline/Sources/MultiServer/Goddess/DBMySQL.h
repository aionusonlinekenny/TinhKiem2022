#ifndef MYSQL_H
#define MYSQL_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include <string>
#include <vector>
#include <sstream>

class MySQLDatabase {
public:
    MySQLDatabase(const std::string& username, const std::string& password, const std::string& server_ip, int port, const std::string& database);
    ~MySQLDatabase();
    void createCharacter(const std::string& cAccname, const std::string& cRoleName, const std::vector<unsigned char>& data);
    void updateCharacterData(const std::string& cAccname, const std::string& cRoleName, const std::vector<unsigned char>& data);
    bool characterExists(const std::string& cAccname, const std::string& cRoleName);
    int countCharactersByAccount(const std::string& cAccname);
    bool getCharacterData(const std::string& cAccname, const std::string& cRoleName, char* pDataBuffer, int& nBufLen);
    void initializeDatabase();
    void initializeTable();

private:
    void connect();

    std::string username;
    std::string password;
    std::string server_ip;
    int port;
    std::string database;
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* connection;
};

#endif // MYSQL_H
