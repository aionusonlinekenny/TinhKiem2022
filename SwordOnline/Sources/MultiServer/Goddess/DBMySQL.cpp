#include "stdafx.h"
#define check      __goddess_check_shadow
#define check_lib  __goddess_check_lib_shadow

#include <string>
#include <map>

void __cdecl __goddess_check_shadow(const std::string&) {}
void __cdecl __goddess_check_shadow(const std::map<std::string, std::string>&) {}
inline void __goddess_check_lib_shadow() {}
// ---- END HOTFIX ----

// (giữ nguyên các include còn lại)
#include "DBMySQL.h"
#include <iostream>

MySQLDatabase::MySQLDatabase(const std::string& username, const std::string& password, const std::string& server_ip, int port, const std::string& database)
	: username(username), password(password), server_ip(server_ip), port(port), database(database) {
	connect();
	initializeDatabase();
	initializeTable();
}

MySQLDatabase::~MySQLDatabase() {
	delete connection;
}

void MySQLDatabase::connect() {
	driver = sql::mysql::get_mysql_driver_instance();
	connection = driver->connect("tcp://" + server_ip + ":" + std::to_string(port), username, password);
}

void MySQLDatabase::initializeDatabase() {
	sql::Statement* stmt = connection->createStatement();
	stmt->execute("CREATE DATABASE IF NOT EXISTS " + database);
	delete stmt;
	connection->setSchema(database);
}

void MySQLDatabase::initializeTable() {
	sql::Statement* stmt = connection->createStatement();
	stmt->execute(
		"CREATE TABLE IF NOT EXISTS Role ("
		"iid INT AUTO_INCREMENT PRIMARY KEY, "
		"cAccname VARCHAR(255), "
		"cRoleName VARCHAR(255), "
		"Data BLOB)"
	);
	delete stmt;
}

void MySQLDatabase::createCharacter(const std::string& cAccname, const std::string& cRoleName, const std::vector<unsigned char>& data) {
	sql::PreparedStatement* pstmt = connection->prepareStatement("INSERT INTO Role (cAccname, cRoleName, Data) VALUES (?, ?, ?)");
	pstmt->setString(1, cAccname);
	pstmt->setString(2, cRoleName);

	// Chuyển đổi vector<unsigned char> thành std::istream
	std::stringstream ss;
	ss.write(reinterpret_cast<const char*>(data.data()), data.size());
	std::istream* blob_stream = &ss;

	pstmt->setBlob(3, blob_stream);
	pstmt->execute();
	delete pstmt;
}

void MySQLDatabase::updateCharacterData(const std::string& cAccname, const std::string& cRoleName, const std::vector<unsigned char>& data) {
	sql::PreparedStatement* pstmt = connection->prepareStatement("UPDATE Role SET Data = ? WHERE cAccname = ? AND cRoleName = ?");

	// Chuyển đổi vector<unsigned char> thành std::istream
	std::stringstream ss;
	ss.write(reinterpret_cast<const char*>(data.data()), data.size());
	std::istream* blob_stream = &ss;

	pstmt->setBlob(1, blob_stream);
	pstmt->setString(2, cAccname);
	pstmt->setString(3, cRoleName);
	pstmt->execute();
	delete pstmt;
}

bool MySQLDatabase::characterExists(const std::string& cAccname, const std::string& cRoleName) {
	sql::PreparedStatement* pstmt = connection->prepareStatement("SELECT COUNT(*) FROM characters WHERE cAccname = ? AND cRoleName = ?");
	pstmt->setString(1, cAccname);
	pstmt->setString(2, cRoleName);
	sql::ResultSet* res = pstmt->executeQuery();
	res->next();
	bool exists = res->getInt(1) > 0;
	delete res;
	delete pstmt;
	return exists;
}

int MySQLDatabase::countCharactersByAccount(const std::string& cAccname) {
	sql::PreparedStatement* pstmt = connection->prepareStatement("SELECT COUNT(*) FROM characters WHERE cAccname = ?");
	pstmt->setString(1, cAccname);
	sql::ResultSet* res = pstmt->executeQuery();
	res->next();
	int count = res->getInt(1);
	delete res;
	delete pstmt;
	return count;
}

bool MySQLDatabase::getCharacterData(const std::string& cAccname, const std::string& cRoleName, char* pDataBuffer, int& nBufLen) {
	sql::PreparedStatement* pstmt = connection->prepareStatement("SELECT Data FROM characters WHERE cAccname = ? AND cRoleName = ?");
	pstmt->setString(1, cAccname);
	pstmt->setString(2, cRoleName);
	sql::ResultSet* res = pstmt->executeQuery();

	if (res->next()) {
		std::istream* blob_stream = res->getBlob(1);
		blob_stream->seekg(0, std::istream::end);
		nBufLen = blob_stream->tellg();
		blob_stream->seekg(0, std::istream::beg);

		pDataBuffer = new char[nBufLen];
		blob_stream->read(pDataBuffer, nBufLen);

		delete res;
		delete pstmt;
		return true;
	}
	else {
		delete res;
		delete pstmt;
		return false;
	}
}