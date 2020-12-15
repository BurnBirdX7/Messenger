//
//
//

#ifndef DBPOOL_H
#define DBPOOL_H

#include <iostream>
#include <vector>
#include <pqxx/pqxx>

#include "DbException.h"

class DbPool
{
public:
    explicit DbPool(int nConnections = 1);
    pqxx::result query(const std::string& query);
    void initPool(int nConnections);

private:
    using Connection = pqxx::connection;

    const std::string mConnectionString = "dbname=messenger-server host=localhost port=5432 user=postgres password=ezpez2342";
    std::vector<Connection*> mPool;

    Connection* createConnection();
    Connection* getConnection();
    void        releaseConnection(Connection* connection);
};


#endif //DBPOOL_H
