#include "self_node.hpp"
#include <string>
using namespace tox;
self_node *self_node_cb::curr_node;
void self_node::update_savedata_file()
{
size_t size = tox_get_savedata_size(tox_c_instance);
uint8_t *savedata = (uint8_t*)malloc(size);
tox_get_savedata(tox_c_instance, savedata);
FILE *f = fopen(savedata_tmp_filename, "wb");
fwrite(savedata, size, 1, f);
fclose(f);
rename(savedata_tmp_filename, savedata_filename);
}
self_node::self_node(
        event::event_loop *main_event_loop,
        std::list<dht_node> *dht_node_list ,
        std::string *serialization_path )
    : main_event_loop(main_event_loop), dht_node_list(dht_node_list),  serialization_path(serialization_path)
{

    tox_options_default(&node_options);
    if (enable_trace) node_options.log_callback = self_node_cb::log;

    FILE *f = fopen(savedata_filename, "rb");
    if (f) {
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);
        uint8_t *savedata = (uint8_t*)malloc(fsize);
        fread(savedata, fsize, 1, f);
        fclose(f);
        node_options.savedata_type = TOX_SAVEDATA_TYPE_TOX_SAVE;
        node_options.savedata_data = savedata;
        node_options.savedata_length = fsize;
        tox_c_instance = tox_new(&node_options, NULL);
        free(savedata);
    } else {
        tox_c_instance = tox_new(&node_options, NULL);
    }
    /* this->tox_c_instance = tox_new(&node_options, NULL); */
    this->user_name = new std::string("test dev build");
    this->user_status = new std::string("test dev build");
    dht_node nodes[] =
    {
        {"85.143.221.42",                      33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43"},
        /* {"2a04:ac00:1:9f00:5054:ff:fe01:becd", 33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43"}, */
        {"78.46.73.141",                       33445, "02807CF4F8BB8FB390CC3794BDF1E8449E9A8392C5D3F2200019DA9F1E812E46"},
        /* {"2a01:4f8:120:4091::3",               33445, "02807CF4F8BB8FB390CC3794BDF1E8449E9A8392C5D3F2200019DA9F1E812E46"}, */
        {"tox.initramfs.io",                   33445, "3F0A45A268367C1BEA652F258C85F4A66DA76BCAA667A49E770BCC4917AB6A25"},
        {"tox2.abilinski.com",                 33445, "7A6098B590BDC73F9723FC59F82B3F9085A64D1B213AAF8E610FD351930D052D"},
        {"205.185.115.131",                       53, "3091C6BEB2A993F1C6300C16549FABA67098FF3D62C6D253828B531470B53D68"},
        {"tox.kurnevsky.net",                  33445, "82EF82BA33445A1F91A7DB27189ECFC0C013E06E3DA71F588ED692BED625EC23"}
    };
     for (size_t i = 0; i < sizeof(nodes)/sizeof(dht_node); i ++) {
        unsigned char key_bin[TOX_PUBLIC_KEY_SIZE];
        sodium_hex2bin(key_bin, sizeof(key_bin), nodes[i].key_hex, sizeof(nodes[i].key_hex)-1,
                       NULL, NULL, NULL);
        tox_bootstrap(this->tox_c_instance, nodes[i].ip, nodes[i].port, key_bin, NULL);
    }

     update_savedata_file();
     
      uint8_t tox_id_bin[TOX_ADDRESS_SIZE];
    tox_self_get_address(this->tox_c_instance, tox_id_bin);
 
    char tox_id_hex[TOX_ADDRESS_SIZE*2 + 1];
    sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), tox_id_bin, sizeof(tox_id_bin));
 
    for (size_t i = 0; i < sizeof(tox_id_hex)-1; i ++) {
        tox_id_hex[i] = toupper(tox_id_hex[i]);
    }
    this->user_tox_id = new std::string(tox_id_hex);
    std::printf("id: %s\n", tox_id_hex);

    this->register_handlers();
    this->register_tox_callbacks();
}
self_node::~self_node() {
    printf("killing tox instance\n");
    tox_kill(this->tox_c_instance);
}
void self_node::main_loop() {
    Tox* tox = this->tox_c_instance;
     while (1) {
        tox_iterate(tox, NULL);
 
        usleep(tox_iteration_interval(tox) * 1000);
    }

}
std::thread self_node::spawn() {
    return std::thread(&self_node::main_loop, this);
}
void self_node::register_handlers() {
    std::printf("registring handlers\n");
    this->main_event_loop->subscribe_event(event::event_type::E_NEW_MESSAGE_SENT, self_node_cb::handle_message_sent);
    this->main_event_loop->subscribe_event_resp(event::event_type::E_RESP_GET_FRIEND_NUMBERS_LIST, self_node_cb::handle_friend_list_req);
    this->main_event_loop->subscribe_event_resp(event::event_type::E_RESP_GET_FRIEND_NAME, self_node_cb::handle_friend_get_name);
    this->main_event_loop->subscribe_event_resp(event::event_type::E_RESP_GET_FRIEND_STATUS_MSG, self_node_cb::handle_friend_get_status_message);
}
void self_node::register_tox_callbacks() {
    Tox* tox = this->tox_c_instance;
    self_node_cb::curr_node = this;
    tox_callback_friend_request(tox, self_node_cb::friend_request_cb);
    tox_callback_friend_message(tox, self_node_cb::friend_message_cb);
    tox_callback_self_connection_status(tox, self_node_cb::self_connection_status_cb);
    tox_callback_friend_name(tox, self_node_cb::friend_name_cb);
    tox_callback_friend_status(tox, self_node_cb::friend_status_cb);
    tox_callback_friend_connection_status(tox, self_node_cb::friend_connection_status_cb);
    tox_callback_friend_typing(tox, self_node_cb::friend_typing_cb);
    tox_callback_friend_read_receipt(tox, self_node_cb::friend_read_receipt_cb);
}
void self_node_cb::friend_request_cb(Tox *tox,const uint8_t *public_key,const  uint8_t *message, size_t length,
                void *user_data)
        {
            if (curr_node->auto_accept)
            {
                printf("[TOX CALLBACK accepting fr req]\n");
                tox_friend_add_norequest(tox, public_key, NULL);
                curr_node->update_savedata_file();
            }
            else {
                auto x = new std::pair<std::string*, std::string*>(
                        new std::string((char*)public_key), new std::string((char*)message)
                        );
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
            SEND_ASYNC_EV(E_CONN_STATUS, new int(connection_status));
            switch (connection_status) {
                case TOX_CONNECTION_NONE:
                    printf("Offline\n");
                    break;
                case TOX_CONNECTION_TCP:
                    printf("Online, using TCP\n");
                    break;
                case TOX_CONNECTION_UDP:
                    printf("Online, using UDP\n");
                    break;
            }
        }
void self_node_cb::handle_message_sent(event::async_event e) {
    // TODO: do checking
    event::message_event *curr_e = (event::message_event*) e.event_payload;
    tox_friend_send_message(curr_node->tox_c_instance, curr_e->friend_number, curr_e->type
            , curr_e->message, curr_e->length, NULL);

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
    std::printf("sending response payload: %d %p, of id: %d\n", req_e->event_payload, *(int*)req_e->event_payload, req_e->event_id);

    curr_node->main_event_loop->push_resp(req_e);
}

void self_node_cb::handle_friend_get_name(event::sync_event * e) {
    using namespace std;
    printf("[TOX] handling get message name: event id is %d\n", e->event_id);
    uint32_t fr_num = *(uint32_t*)e->event_payload;
    int n = tox_friend_get_name_size(curr_node->tox_c_instance, fr_num, NULL);
    uint8_t name[n+1];
    tox_friend_get_name(curr_node->tox_c_instance, fr_num, name, NULL);
    name[n] = '\0';
    event::sync_event *k = new event::sync_event(event::event_type::E_RESP_GET_FRIEND_NAME, new std::string((char*)name), e->event_id);
    printf("[TOX] init event: event id is %d\n", k->event_id);
    curr_node->main_event_loop->push_resp(k);

    printf("[TOX] name sent\n");

}

void self_node_cb::handle_friend_get_status_message(event::sync_event * e) {
    using namespace std;
    uint32_t fr_num = *(uint32_t*)e->event_payload;
    int n = tox_friend_get_status_message_size(curr_node->tox_c_instance, fr_num, NULL);
    uint8_t status_message[n+1];
    status_message[n] = '\0';
    tox_friend_get_status_message(curr_node->tox_c_instance, fr_num, status_message, NULL);
    event::sync_event *k = new event::sync_event(event::event_type::E_RESP_GET_FRIEND_STATUS_MSG, new std::string((char*)status_message), e->event_id);
    printf("[TOX] init event: event id is %d\n", k->event_id);
    curr_node->main_event_loop->push_resp(k);

    printf("[TOX] name sent\n");

}
void self_node_cb::friend_name_cb(
        Tox *tox, Tox_Friend_Number friend_number,
        const uint8_t name[], size_t length, void *user_data) {
    if (name == NULL) {
        printf("[TOX CALLBACK] cant do crap name is null\n");
        return;
    }
    auto r = new std::pair<uint32_t, std::string*>(friend_number, new std::string((char*)name));
    SEND_ASYNC_EV(E_FR_CHANGE_NAME, r);
}

void self_node_cb::friend_status_cb(
        Tox *tox, Tox_Friend_Number friend_number, Tox_User_Status status, void *user_data) {
    auto r = new std::pair<uint32_t, Tox_User_Status>(friend_number, status);
    SEND_ASYNC_EV(E_FR_CHANGE_NAME, r);
}


void self_node_cb::friend_connection_status_cb(
            Tox *tox, Tox_Friend_Number friend_number, Tox_Connection connection_status, void *user_data) {
    auto r = new std::pair<uint32_t, Tox_Connection>(friend_number, connection_status);
    SEND_ASYNC_EV(E_FR_CHANGE_NAME, r);
}
void self_node_cb::friend_typing_cb(
        Tox *tox, Tox_Friend_Number friend_number, bool typing, void *user_data) {
    auto r = new std::pair<uint32_t, bool>(friend_number, typing);
    SEND_ASYNC_EV(E_FR_CHANGE_NAME, r);
}
void self_node_cb::friend_read_receipt_cb(
        Tox *tox, Tox_Friend_Number friend_number, Tox_Friend_Message_Id message_id, void *user_data) {
    auto r = new std::pair<uint32_t, uint32_t>(friend_number, message_id);
    SEND_ASYNC_EV(E_FR_CHANGE_NAME, r);
}
void self_node::stop_instance() {
    tox_kill(tox_c_instance);
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
