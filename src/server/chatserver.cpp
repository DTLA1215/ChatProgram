#include "chatserver.hpp"
#include "json.hpp"
#include <functional>
#include <string>
#include "chatservice.hpp"
#include <iostream>

using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
            : _server(loop, listenAddr, nameArg), _loop(loop)
{
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

    _server.setThreadNum(4);
}

void ChatServer::start()
{
    _server.start();
}

void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    if(! conn->connected())
    {
        ChatService::Instance()->clientCloseException(conn);
        conn->shutdown();        
    }
}

void ChatServer::onMessage(const TcpConnectionPtr& conn, Buffer* buffer, Timestamp time)
{
    string buf = buffer->retrieveAllAsString();
    json js = json::parse(buf);

    auto msgHandler = ChatService::Instance()->GetHandler(js["msgid"].get<int>());
    msgHandler(conn, js, time);
}               