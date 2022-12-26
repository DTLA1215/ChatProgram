// g++ muduo_server.cpp -o server -lmuduo_net -lmuduo_base -lpthread
/*
    f1 + c++_json: 编译配置文件     /usr/include          /usr/local/include
    f1 + build
*/

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <string>
#include <functional>
using namespace std;
using namespace muduo;
using namespace muduo::net;
using namespace placeholders;

class ChatServer
{
    public:
        ChatServer(EventLoop* loop, const InetAddress& listenAddr, const string& nameArg)
            : _server(loop, listenAddr, nameArg), _loop(loop) 
        {
            _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, placeholders::_1));

            _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, placeholders::_1, placeholders::_2, placeholders::_3));
        
            _server.setThreadNum(4);
        }

        void start()
        {
            _server.start();
        }
        
    private:
        void onConnection(const TcpConnectionPtr& conn)
        {
            if(conn->connected())
            {
                cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state:online" << endl;
            }
            else
            {
                cout << conn->peerAddress().toIpPort() << " -> " << conn->localAddress().toIpPort() << " state:offline" << endl;
                conn->shutdown();
            }
        }

        void onMessage (const TcpConnectionPtr& conn,
                            Buffer* buf,
                            Timestamp time)
        {
            string data = buf->retrieveAllAsString();
            cout << "recv data: " << data << " time: " << time.toString() << endl;
            conn->send(data);
        }

        TcpServer _server;
        EventLoop* _loop;
};

int main()
{
    EventLoop loop;

    InetAddress addr("127.0.0.1", 6000);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();

    return 0;
}