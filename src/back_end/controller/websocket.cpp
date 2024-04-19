//EchoWebsock.cc
#include "websocket.hpp"
#include "../http_server.hpp"
WebSocketConnectionPtr EchoWebsock::front_conn;
void EchoWebsock::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr,std::string &&message, const WebSocketMessageType &)
{
    //write your application logic here
    wsConnPtr->send(message);
}
void EchoWebsock::handleNewConnection(const HttpRequestPtr &req,const WebSocketConnectionPtr &wsConnPtr)
{
    std::cout << "[BACKEND] new websocket connection\n";
    front_conn = wsConnPtr;
    back_end::back_end_server::main_event_loop->subscribe_event(event::event_type::E_NEW_MESSAGE_RECV, handle_new_msg);
}
void EchoWebsock::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr)
{
    //write your application logic here
}
void EchoWebsock::handle_new_msg(event::async_event e) {
    std::cout << "[WEBSOCKET]websocket backend sending new message to front\n";
    front_conn->send((char*)((event::message_event*)e.event_payload)->message);
}

