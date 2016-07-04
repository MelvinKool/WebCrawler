#include <utility>
#include <exception>
#include <map>
#include <mutex>
#include "dbconnectionpool.h"
namespace webcrawler{

    PoolConnection::PoolConnection(int id){
        this->id = id;
        connection = std::unique_ptr<DatabaseConnection>(new DatabaseConnection(db_host,db_user,db_password));
        connection->setSchema(db_name);
    }

    int PoolConnection::getId(){
        return id;
    }

    DatabaseConnection* PoolConnection::getConnection(){
        return connection.get();
    }

    DBConnectionPool::DBConnectionPool(int nrConnections){
        for(int i = 0; i < nrConnections; i++){
            connections.insert(std::pair<int,PoolConnection*>(i,new PoolConnection(i)));
            freeConnections.push(i);
        }
    }

    DBConnectionPool::~DBConnectionPool(){
            for (std::pair<int,PoolConnection*> conn : connections) {
                delete conn.second;
                conn.second = nullptr;
            }
    }

    PoolConnection* DBConnectionPool::getFreeConnection(){
        std::unique_lock<std::mutex> freeConnLocker(freeconn_mutex);
        if(freeConnections.size() == 0){
            freeConnLocker.unlock();
            throw outofconnectionsexception();
        }
        int freeConnId = freeConnections.front();
        freeConnections.pop();
        freeConnLocker.unlock();
        std::map<int,PoolConnection*>::iterator it = connections.find(freeConnId);
        if (it == connections.end()){
            throw connectionnotfoundexception();
        }
        return it->second;
    }

    void DBConnectionPool::releaseConnection(int connId){
        std::lock_guard<std::mutex> freeConnLocker(freeconn_mutex);
        freeConnections.push(connId);
    }
}
