//
//
//

#include "DbPool.h"

DbPool::DbPool(int nConnections)
{
    initPool(nConnections);
}

std::optional<pqxx::result> DbPool::query(const std::string& query)
{
    pqxx::result result;
    try{
        Connection* conn(getConnection());
        pqxx::work transaction(*conn);
        result = transaction.exec(query);
        transaction.commit();
        releaseConnection(conn);
    }
    catch (pqxx::sql_error const& e)
    {
        std::cerr << "[DbPool]::query: sql exception" << std::endl;
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Query was: " << e.query() << std::endl;
        return std::nullopt;
    }
    catch(DbException const& e)
    {
        std::cerr << "[DbPool]::query: db connection error" << std::endl;
        std::cerr << "<!>: " << e.what() << std::endl;
        return std::nullopt;
    }

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
    if(nConnections < 1){
        throw DbException("[DbPool]::initPool: number of connections should be greater than 0");
    }

    if (!mPool.empty()) {
        for (auto connection : mPool) {
            connection->close();
        }
        mPool.clear();
    }

    for (int i = 0; i < nConnections; ++i) {
        mPool.emplace_back(createConnection());
    }
}