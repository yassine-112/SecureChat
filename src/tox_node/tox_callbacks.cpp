#include "./tox_callbacks.hpp"
using namespace tox;
self_node *self_node_cb::curr_node;
void self_node_cb::register_handlers() {
    LOG(INFO) << "registring handlers\n";
    curr_node->main_event_loop->subscribe_event(event::event_type::E_NEW_MESSAGE_SENT, self_node_cb::handle_message_sent);
    curr_node->main_event_loop->subscribe_event(event::event_type::E_ACCEPT_FR_REQ, self_node_cb::handle_friend_accept);
    curr_node->main_event_loop->subscribe_event(event::event_type::SYS_EXIT, self_node_cb::handle_sys_exit);

    curr_node->main_event_loop->subscribe_event_resp(event::event_type::E_RESP_GET_FRIEND_NUMBERS_LIST, self_node_cb::handle_friend_list_req);
    curr_node->main_event_loop->subscribe_event_resp(event::event_type::E_RESP_GET_FRIEND_NAME, self_node_cb::handle_friend_get_name);
    curr_node->main_event_loop->subscribe_event_resp(event::event_type::E_RESP_GET_FRIEND_STATUS_MSG, self_node_cb::handle_friend_get_status_message);
    curr_node->main_event_loop->subscribe_event_resp(event::event_type::E_RESP_GET_USER_ID, self_node_cb::handle_get_user_id);
    curr_node->main_event_loop->subscribe_event_resp(event::event_type::E_RESP_SEND_FRIEND_REQ, self_node_cb::handle_sent_friend_req);
}
void self_node_cb::handle_get_user_id(event::sync_event *e) {
    LOG(INFO) << "!!!!!!!!!!!!!!!push user tox id resp " << curr_node->user_tox_id << '\n';

    event::sync_event* req_e = new event::sync_event();
    req_e->e_type = event::event_type::E_RESP_GET_USER_ID;
    req_e->event_payload = new std::string(*curr_node->user_tox_id);
    req_e->event_id = e->event_id;
    req_e->is_request = false;
    /* std::printf("sending response payload: %d %p, of id: %d\n", req_e->event_payload, *(int*)req_e->event_payload, req_e->event_id); */

    curr_node->main_event_loop->push_resp(req_e);
}


void self_node_cb::register_tox_callbacks() {
    Tox* tox = curr_node->tox_c_instance;
    tox_callback_friend_request(tox, self_node_cb::friend_request_cb);
    tox_callback_friend_message(tox, self_node_cb::friend_message_cb);
    tox_callback_self_connection_status(tox, self_node_cb::self_connection_status_cb);
    tox_callback_friend_name(tox, self_node_cb::friend_name_cb);
    tox_callback_friend_status(tox, self_node_cb::friend_status_cb);
    tox_callback_friend_connection_status(tox, self_node_cb::friend_connection_status_cb);
    tox_callback_friend_typing(tox, self_node_cb::friend_typing_cb);
    tox_callback_friend_read_receipt(tox, self_node_cb::friend_read_receipt_cb);
}
void self_node_cb::handle_friend_accept(event::async_event e) {
    LOG(INFO) << "Got accept req event";
    Tox_Err_Friend_Add err;
    tox_friend_add_norequest(curr_node->tox_c_instance, (uint8_t*) e.event_payload, &err);
    LOG(INFO) << "accept firend STATUS"  <<tox_err_friend_add_to_string(err);
    curr_node->update_savedata_file();
    // TODO CHECK ERRORS
}
void self_node_cb::friend_request_cb(Tox *tox,const uint8_t *public_key,const  uint8_t *message, size_t length,
                void *user_data)
        {
            if (curr_node->auto_accept)
            {
                LOG(INFO) << "[TOX CALLBACK accepting fr req]\n";
                tox_friend_add_norequest(tox, public_key, NULL);
                curr_node->update_savedata_file();
            }
            else {
                char tox_id_hex[TOX_PUBLIC_KEY_SIZE*2 + 1] = {0};
                sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), public_key, TOX_PUBLIC_KEY_SIZE);
                auto x = new std::pair<std::string*, std::string*>(
                        new std::string((char*)message), new std::string(tox_id_hex)
                        );
                LOG(INFO) << "new friend request from: " << *(x->second)
                    << " message: " << *(x->first) << '\n';
                SEND_ASYNC_EV(
                        E_NEW_FR_REQ, x)
            }
        }
void self_node_cb::friend_message_cb(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message,
                size_t length, void *user_data)
        {
            event::message_event *e = new event::message_event();
            e->message = message;
            e->length = length;
            e->friend_number = friend_number;
            e->type = type;
            LOG(INFO) << "TOX NODE GOT MESSAGE";

            SEND_ASYNC_EV(E_NEW_MESSAGE_RECV, e)
            /* e
             * vent::message_event *e = new event::message_event(); */
            /* e->message = message; */
            /* e->length = length; */
            /* e->friend_number = friend_number; */
            /* e->type = type; */
            /* event::async_event ev(event::event_type::E_NEW_MESSAGE_RECV, e); */
            /* curr_node->main_event_loop->push_event(ev); */
        }
void self_node_cb::self_connection_status_cb(Tox *tox, TOX_CONNECTION connection_status, void *user_data)
        {
            
            SEND_ASYNC_EV(E_CONN_STATUS, new std::string(tox_connection_to_string(connection_status)));
            switch (connection_status) {
                case TOX_CONNECTION_NONE:
                    LOG(INFO) << "Offline\n";
                    break;
                case TOX_CONNECTION_TCP:
                    LOG(INFO) << "Online, using TCP\n";
                    break;
                case TOX_CONNECTION_UDP:
                    LOG(INFO) << "Online, using UDP\n";
                    break;
            }
        }
void self_node_cb::handle_message_sent(event::async_event e) {
    // TODO: do checking
    event::message_event *curr_e = (event::message_event*) e.event_payload;
    Tox_Err_Friend_Send_Message err;
    tox_friend_send_message(curr_node->tox_c_instance, curr_e->friend_number, curr_e->type
            , curr_e->message, curr_e->length, &err);
    LOG(INFO) << "SEND MESSAGE STATUS" << tox_err_friend_send_message_to_string(err);

}

void self_node_cb::handle_friend_list_req(event::sync_event * e) {
    event::sync_event* req_e = new event::sync_event();
    req_e->e_type = event::event_type::E_RESP_GET_FRIEND_NUMBERS_LIST;
    uint32_t friends_number = tox_self_get_friend_list_size(curr_node->tox_c_instance);
    uint32_t *friends_arr = (uint32_t*)malloc(friends_number* sizeof(Tox_Friend_Number));
    tox_self_get_friend_list(curr_node->tox_c_instance, (Tox_Friend_Number* )friends_arr);
    req_e->event_payload = new std::pair<uint32_t, uint32_t*>(friends_number, friends_arr);
    req_e->event_id = e->event_id;
    req_e->is_request = false;
    /* std::printf("sending response payload: %d %p, of id: %d\n", req_e->event_payload, *(int*)req_e->event_payload, req_e->event_id); */

    curr_node->main_event_loop->push_resp(req_e);
}

void self_node_cb::handle_friend_get_name(event::sync_event * e) {
    using namespace std;
    /* printf("[TOX] handling get message name: event id is %d\n", e->event_id); */
    uint32_t fr_num = *(uint32_t*)e->event_payload;
    int n = tox_friend_get_name_size(curr_node->tox_c_instance, fr_num, NULL);
    uint8_t name[n+1];
    tox_friend_get_name(curr_node->tox_c_instance, fr_num, name, NULL);
    name[n] = '\0';
    event::sync_event *k = new event::sync_event(event::event_type::E_RESP_GET_FRIEND_NAME, new std::string((char*)name), e->event_id);
    /* printf("[TOX] init event: event id is %d\n", k->event_id); */
    curr_node->main_event_loop->push_resp(k);

    /* printf("[TOX] name sent\n"); */

}

void self_node_cb::handle_friend_get_status_message(event::sync_event * e) {
    using namespace std;
    uint32_t fr_num = *(uint32_t*)e->event_payload;
    int n = tox_friend_get_status_message_size(curr_node->tox_c_instance, fr_num, NULL);
    uint8_t status_message[n+1];
    status_message[n] = '\0';
    tox_friend_get_status_message(curr_node->tox_c_instance, fr_num, status_message, NULL);
    event::sync_event *k = new event::sync_event(event::event_type::E_RESP_GET_FRIEND_STATUS_MSG, new std::string((char*)status_message), e->event_id);
    /* printf("[TOX] init event: event id is %d\n", k->event_id); */
    curr_node->main_event_loop->push_resp(k);

    /* printf("[TOX] name sent\n"); */

}
void self_node_cb::friend_name_cb(
        Tox *tox, Tox_Friend_Number friend_number,
        const uint8_t name[], size_t length, void *user_data) {
    if (name == NULL) {
        LOG(INFO) << ("[TOX CALLBACK] cant do crap name is null\n");
        return;
    }

    LOG(INFO) << "A friend changed his name to"<< name << "\n";
    auto r = new std::pair<uint32_t, std::string*>(friend_number, new std::string((char*)name));

    SEND_ASYNC_EV(E_FR_CHANGE_NAME, r);
}

void self_node_cb::friend_status_cb(
        Tox *tox, Tox_Friend_Number friend_number, Tox_User_Status status, void *user_data) {
    auto r = new std::pair<uint32_t, Tox_User_Status>(friend_number, status);
//    SEND_ASYNC_EV(E_FR_CHANGE_NAME, r);
}


void self_node_cb::friend_connection_status_cb(
            Tox *tox, Tox_Friend_Number friend_number, Tox_Connection connection_status, void *user_data) {
    auto r = new std::pair<uint32_t, Tox_Connection>(friend_number, connection_status);
 //   SEND_ASYNC_EV(E_FR_CHANGE_NAME, r);
}
void self_node_cb::friend_typing_cb(
        Tox *tox, Tox_Friend_Number friend_number, bool typing, void *user_data) {
    auto r = new std::pair<uint32_t, bool>(friend_number, typing);
  //  SEND_ASYNC_EV(E_FR_CHANGE_NAME, r);
}
void self_node_cb::friend_read_receipt_cb(
        Tox *tox, Tox_Friend_Number friend_number, Tox_Friend_Message_Id message_id, void *user_data) {
    auto r = new std::pair<uint32_t, uint32_t>(friend_number, message_id);
   // SEND_ASYNC_EV(E_FR_CHANGE_NAME, r);
}
void self_node_cb::log(Tox *tox, Tox_Log_Level level, const char *file, uint32_t line, const char *func,
                        const char *message, void *user_data) {
    // to this in a better damn way
    FILE* f = fopen("./tox_trace", "a+");
    if (f) {
    fprintf(f, "[TOXCORE][%s]%s:%d:%s - %s\n", tox_log_level_to_string(level), file, line, func, message);
    fclose(f);
    }
}

void self_node_cb::handle_sent_friend_req(event::sync_event *e) {
    LOG(INFO) << "handling sent friend req\n";

    if (e->event_payload == nullptr) {
        LOG(ERROR) << "got null event payload\n";
        exit(1);
    }
    std::pair<uint8_t*, std::string*>* payload = (std::pair<uint8_t*, std::string*>*)e->event_payload;
    if (payload->first == nullptr) {
        LOG(ERROR) << "payload pair fist is null\n";
        exit(1);
    }
    if (payload->second == nullptr) {
        LOG(ERROR) << "payload pair second is null\n";
        exit(1);
    }
    Tox_Err_Friend_Add err;
    uint32_t v = tox_friend_add(curr_node->tox_c_instance, payload->first, (uint8_t*)(*payload->second).c_str(), (*payload->second).length()+1, &err);
    LOG(INFO) << "ADD FRIEND STATUS " << tox_err_friend_add_to_string(err);
    uint32_t *friend_num = new uint32_t(v);
    event::sync_event *k = new event::sync_event(event::event_type::E_RESP_SEND_FRIEND_REQ, friend_num, e->event_id);
    curr_node->main_event_loop->push_resp(k);
}

void self_node_cb::handle_sys_exit(event::async_event e){
    LOG(INFO) << "TOX got exit signal, exiting\n";
    curr_node->stop_instance();
}
