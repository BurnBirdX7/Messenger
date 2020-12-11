//
//
//

#include "DbPool.h"

DbPool::DbPool() : DbPool(1)
{
}

DbPool::DbPool(int nConnections = 1)
{
    initPool(nConnections);
}

pqxx::result DbPool::query(const std::string& query)
{
    Connection* conn(getConnection());
    pqxx::work transaction(*conn);
    pqxx::result result = transaction.exec(query);
    transaction.commit();
    releaseConnection(conn);

    return result;
}

DbPool::Connection* DbPool::createConnection()
{
    auto* conn = new Connection(mConnectionString);
    return conn;
}

void DbPool::releaseConnection(Connection* connection)
{
    mPool.push_back(connection);
}

DbPool::Connection* DbPool::getConnection()
{
    Connection* conn = *(mPool.end() - 1);
    mPool.pop_back();
    return conn;
}

void DbPool::initPool(int nConnections)
{
    if (!mPool.empty()) {
        for (auto connection : mPool) {
            connection->close();
        }
        mPool.clear();
    }

    for (size_t i = 0; i < nConnections; ++i) {
        mPool.emplace_back(createConnection());
    }
}