#ifndef DBCONNECTIONPOOL_H
#define DBCONNECTIONPOOL_H
#include <map>
#include <memory>
#include <queue>
#include <exception>
#include <mutex>
#include "databaseconnection.h"
#include "config.h"

namespace webcrawler{
    class connectionnotfoundexception : public std::exception {
      virtual const char* what() const throw()
      {
        return "Can't find the connection with the specified id";
      }
    };

    class PoolConnection {
    public:
        PoolConnection(int id);
        DatabaseConnection* getConnection();
    private:
        std::unique_ptr<DatabaseConnection> connection;
        int id;
    };

    class outofconnectionsexception : public std::exception {
      virtual const char* what() const throw()
      {
        return "The connection pool is empty";
      }
    };

    class DBConnectionPool {
    public:
        DBConnectionPool(int nrConnections);
        DatabaseConnection* getFreeConnection();
        void releaseConnection(int connId);
    private:
        std::queue<int> freeConnections;
        std::map<int,PoolConnection> connections;
        std::mutex freeconn_mutex;
    };
}
#endif
