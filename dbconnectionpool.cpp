#include <utility>
#include <exception>
#include <map>
#include <mutex>
#include "dbconnectionpool.h"
namespace webcrawler{

    PoolConnection::PoolConnection(int id){
        this->id = id;
        this->connection = std::unique_ptr<DatabaseConnection>(new DatabaseConnection(db_host,db_name,db_user,db_password));
    }

    DatabaseConnection* PoolConnection::getConnection(){
        return connection.get();
    }

    DBConnectionPool::DBConnectionPool(int nrConnections){
        for(int i = 0; i < nrConnections; i++){
            connections.insert(std::pair<int,PoolConnection>(i,PoolConnection(i)));
            freeConnections.push(i);
        }
    }

    DatabaseConnection* DBConnectionPool::getFreeConnection(){
        std::unique_lock<std::mutex> freeConnLocker(freeconn_mutex);
        if(freeConnections.size() == 0){
            freeConnLocker.unlock();
            throw outofconnectionsexception();
        }
        int freeConnId = freeConnections.front();
        freeConnections.pop();
        freeConnLocker.unlock();
        std::map<int,PoolConnection>::iterator it = connections.find(freeConnId);
        if (it == connections.end()){
            throw connectionnotfoundexception();
        }
        return it->second.getConnection();
    }

    void DBConnectionPool::releaseConnection(int connId){
        std::lock_guard<std::mutex> freeConnLocker(freeconn_mutex);
        freeConnections.push(connId);
    }
}
