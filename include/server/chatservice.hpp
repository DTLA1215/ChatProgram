#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>
using namespace std;
using namespace muduo;
using namespace muduo::net;

#include "redis.hpp"
#include "groupmodel.hpp"
#include "friendmodel.hpp"
#include "usermodel.hpp"
#include "offlinemsgmodel.hpp"
#include "json.hpp"
using json = nlohmann::json;

using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp time)>;

class ChatService
{
public:
    static ChatService *Instance();

    void login(const TcpConnectionPtr &conn, json &js, Timestamp time);

    void loginout(const TcpConnectionPtr &conn, json &js, Timestamp time);

    void reg(const TcpConnectionPtr &conn, json &js, Timestamp time);

    MsgHandler GetHandler(int msgid);

    void clientCloseException(const TcpConnectionPtr &conn);

    void oneChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    void reset();

    void addFriend(const TcpConnectionPtr &conn, json &js, Timestamp time);

    // 创建群组业务
    void createGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 加入群组业务
    void addGroup(const TcpConnectionPtr &conn, json &js, Timestamp time);
    // 群组聊天业务
    void groupChat(const TcpConnectionPtr &conn, json &js, Timestamp time);

    void handleRedisSubscribeMessage(int, string);

private:
    ChatService();

    unordered_map<int, MsgHandler> _msgHandlerMap;

    UserModel _userModel;

    unordered_map<int, TcpConnectionPtr> _userConnMap;

    mutex _connMutex;

    OfflineMsgModel _oflineMsgModel;

    FriendModel _friendModel;

    GroupModel _groupModel;

    Redis _redis;
};

#endif