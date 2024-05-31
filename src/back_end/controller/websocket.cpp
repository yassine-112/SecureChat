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
    } else if (e["event_type"].asString() == "request_accept" ){
        event::async_event send_ev = event::async_event();
        send_ev.e_type = event::event_type::E_ACCEPT_FR_REQ;
        send_ev.event_payload = (uint8_t*) malloc(TOX_PUBLIC_KEY_SIZE * sizeof(uint8_t));
        sodium_hex2bin((uint8_t*)send_ev.event_payload , TOX_PUBLIC_KEY_SIZE, e["event_body"]["public_key"].asCString(), e["event_body"]["public_key"].asString().length(), NULL, NULL, NULL);
        LOG(INFO) << "Sending event to accept friend request with public key: ";
        for (int i = 0; i < TOX_PUBLIC_KEY_SIZE; i++) {
            std::printf("%02x", ((uint8_t*)send_ev.event_payload)[i] );
        }
        printf("\n");
        back_end::back_end_server::main_event_loop->push_event(send_ev);
    } else if (e["event_type"].asString() == "send_friend_req") {
        uint8_t* addr_buffer = new uint8_t[TOX_ADDRESS_SIZE];
        sodium_hex2bin(addr_buffer, TOX_ADDRESS_SIZE, e["event_body"]["address"].asCString(), e["event_body"]["address"].asString().length(), NULL, NULL, NULL);

        LOG(INFO) << "Sending event to send friend request with TOX ID: ";
        for (int i = 0; i < TOX_ADDRESS_SIZE; i++) {
            std::printf("%02x", addr_buffer[i]);
        }
        printf("\n");
        std::printf("Req message: %s\n", e["event_body"]["message"].asCString());
        std::pair<uint8_t*, std::string*>* payload = new std::pair<uint8_t*, std::string*>(
                addr_buffer, new std::string(e["event_body"]["message"].asString())
                );
        event::sync_event *req_event = new event::sync_event(event::event_type::E_RESP_SEND_FRIEND_REQ, payload);
        auto resp = back_end::back_end_server::main_event_loop->push_wait(req_event);
        LOG(INFO) << "Answer for friend req sent is number " << *(uint32_t*)resp->event_payload << "\n";
    } else if (e["event_type"].asString() == "sys_exit") {
        back_end::back_end_server::main_event_loop->push_event(event::async_event(event::event_type::SYS_EXIT, nullptr));
        drogon::app().quit();

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
        LOG(INFO) << "[BACKEND] Registring backend ws callbacks\n";
        back_end::back_end_server::main_event_loop->subscribe_event(event::event_type::E_NEW_MESSAGE_RECV, handle_new_msg);
        back_end::back_end_server::main_event_loop->subscribe_event(event::event_type::E_NEW_FR_REQ, handle_new_friend_request);
        back_end::back_end_server::main_event_loop->subscribe_event(event::event_type::E_CONN_STATUS, handle_tox_status);
        back_end::back_end_server::main_event_loop->subscribe_event(event::event_type::E_FR_CHANGE_NAME, handle_friend_name_change);
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
void EchoWebsock::handle_tox_status(event::async_event e) {
    LOG(INFO) << "Sending tox status to front end\n";
    front_conn->send(
            json_helper::tox_status(*(std::string*)e.event_payload)
            );
}
void EchoWebsock::handle_friend_name_change(event::async_event e) {
    LOG(INFO) << "Sending changed friend name to front end\n";
    std::pair<uint32_t, std::string*>* val = (std::pair<uint32_t, std::string*>*)e.event_payload;
    if (val == nullptr || val->second == nullptr) {
        LOG(ERROR) << "Cant do crap, not doing anything val or str is null\n";
    } else {
        LOG(INFO) << "Got friend name change " << val->first << " " << *val->second << '\n';
        front_conn->send(
                json_helper::friend_name_change(val->first, *val->second)
                );
    }

}
