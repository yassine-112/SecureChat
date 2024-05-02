//EchoWebsock.h
#pragma once
#include <drogon/WebSocketController.h>
#include "../../event_loop/event_loop.hpp"
#include "../http_server.hpp"
#include "json_helper.hpp"
#include "tox.h"
#include "sodium.h"
#include <glob.h>

using namespace drogon;
class EchoWebsock:public drogon::WebSocketController<EchoWebsock>
{
public:
    static WebSocketConnectionPtr front_conn;
    static bool subscribed_event;
    
    virtual void handleNewMessage(const WebSocketConnectionPtr&,
                                std::string &&,
                                const WebSocketMessageType &)override;
    virtual void handleNewConnection(const HttpRequestPtr &,
                                    const WebSocketConnectionPtr&)override;
    virtual void handleConnectionClosed(const WebSocketConnectionPtr&)override;

    static void handle_new_msg(event::async_event e);
    static void handle_new_friend_request(event::async_event e);
    WS_PATH_LIST_BEGIN
    //list path definitions here;
    WS_PATH_ADD("/echo");
    WS_PATH_LIST_END
};

