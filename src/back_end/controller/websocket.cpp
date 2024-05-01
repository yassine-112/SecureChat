#pragma once
#include "websocket.hpp"
WebSocketConnectionPtr EchoWebsock::front_conn;
bool EchoWebsock::subscribed_event = false;
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
        /* std::cout << e<< '\n'; */
        /* std::cout << e["event_body"]["friend_number"].asString() <<'\n'; */
        event::message_event *send_ev = new event::message_event();
        send_ev->friend_number = std::stoi(e["event_body"]["friend_number"].asString());
        send_ev->length = e["event_body"]["message_body"].asString().length();
        uint8_t * msg_buffer = new uint8_t[send_ev->length];
        std::memcpy(msg_buffer, e["event_body"]["message_body"].asString().c_str(), send_ev->length);
        send_ev->message = msg_buffer;
        send_ev->type = TOX_MESSAGE_TYPE_NORMAL;
        back_end::back_end_server::main_event_loop->push_event(event::async_event(event::event_type::E_NEW_MESSAGE_SENT, send_ev));
    }


    /* std::string type = e["event_type"].toStyledString(); */
    //write your application logic here
    wsConnPtr->send("test");
}
void EchoWebsock::handleNewConnection(const HttpRequestPtr &req,const WebSocketConnectionPtr &wsConnPtr)
{
    LOG(INFO) << "[BACKEND] new websocket connection\n";
    front_conn = wsConnPtr;
    if (!subscribed_event) {
        subscribed_event = true;
        back_end::back_end_server::main_event_loop->subscribe_event(event::event_type::E_NEW_MESSAGE_RECV, handle_new_msg);
        back_end::back_end_server::main_event_loop->subscribe_event(event::event_type::E_NEW_FR_REQ, handle_new_friend_request);
    }
}
void EchoWebsock::handleConnectionClosed(const WebSocketConnectionPtr &wsConnPtr)
{
    //unsubscribe
}
void EchoWebsock::handle_new_msg(event::async_event e) {
    LOG(INFO) << "[WEBSOCKET]websocket backend sending new message to front\n";
    event::message_event* ev = (event::message_event*)e.event_payload;
    front_conn->send(
        json_helper::message_recv( (char*)ev->message, ev->friend_number)
            );
}

void EchoWebsock::handle_new_friend_request(event::async_event e) {
    LOG(INFO) << "[WEBSOCKET]websocket backend sending new friend request to front\n";
    /* event::message_event* ev = (event::message_event*)e.event_payload; */
    std::pair<std::string*, std::string*> *ev = (std::pair<std::string*, std::string*> *) e.event_payload;
    front_conn->send(
        json_helper::friend_req_recv( *(ev->first), *(ev->second))
            );
}
