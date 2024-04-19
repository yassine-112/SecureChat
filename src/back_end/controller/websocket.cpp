#pragma once
#include "websocket.hpp"
WebSocketConnectionPtr EchoWebsock::front_conn;
void EchoWebsock::handleNewMessage(const WebSocketConnectionPtr &wsConnPtr,std::string &&message, const WebSocketMessageType &)
{
    Json::Value e;
    Json::Reader reader;
    auto resp = reader.parse(message, e);
    if (!resp) {
        wsConnPtr->send("not valid json");
        return;
    }

    if(e["event_type"].asString() == "message_sent"){
        event::message_event *send_ev = new event::message_event();
        send_ev->friend_number = e["friend_number"].asInt();
        send_ev->message = (uint8_t*)e["event_body"]["message_body"].asString().c_str();
        send_ev->length = e["event_body"]["message_body"].asString().length();
        send_ev->type = TOX_MESSAGE_TYPE_NORMAL;
        back_end::back_end_server::main_event_loop->push_event(event::async_event(event::event_type::E_NEW_MESSAGE_SENT, send_ev));
    }


    /* std::string type = e["event_type"].toStyledString(); */
    //write your application logic here
    wsConnPtr->send("test");
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
    event::message_event* ev = (event::message_event*)e.event_payload;
    front_conn->send(
        json_helper::message_recv( (char*)ev->message, ev->friend_number)
            );
}

