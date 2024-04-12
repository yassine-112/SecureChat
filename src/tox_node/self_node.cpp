#include "self_node.hpp"
using namespace tox;
event::event_loop *self_node::main_event_loop;
self_node::self_node(
        event::event_loop *main_event_loop,
        std::list<dht_node> *dht_node_list ,
        Tox_Options  *node_options ,
        std::string *serialization_path )
    :  dht_node_list(dht_node_list), node_options(node_options), serialization_path(serialization_path)
{

    self_node::main_event_loop = main_event_loop;
    this->tox_c_instance = tox_new(NULL, NULL);
    this->user_name = new std::string("test dev build");
    this->user_status = new std::string("test dev build");
    dht_node nodes[] =
    {
        {"85.143.221.42",                      33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43"},
        {"2a04:ac00:1:9f00:5054:ff:fe01:becd", 33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43"},
        {"78.46.73.141",                       33445, "02807CF4F8BB8FB390CC3794BDF1E8449E9A8392C5D3F2200019DA9F1E812E46"},
        {"2a01:4f8:120:4091::3",               33445, "02807CF4F8BB8FB390CC3794BDF1E8449E9A8392C5D3F2200019DA9F1E812E46"},
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
      uint8_t tox_id_bin[TOX_ADDRESS_SIZE];
    tox_self_get_address(this->tox_c_instance, tox_id_bin);
 
    char tox_id_hex[TOX_ADDRESS_SIZE*2 + 1];
    sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), tox_id_bin, sizeof(tox_id_bin));
 
    for (size_t i = 0; i < sizeof(tox_id_hex)-1; i ++) {
        tox_id_hex[i] = toupper(tox_id_hex[i]);
    }
    this->user_tox_id = new std::string(tox_id_hex);
    std::printf("id: %s\n", tox_id_hex);

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
void self_node::register_tox_callbacks() {
    Tox* tox = this->tox_c_instance;
    // binding as work arround
    tox_callback_friend_request(tox, this->friend_request_cb);
    tox_callback_friend_message(tox, this->friend_message_cb);
    tox_callback_self_connection_status(tox, this->self_connection_status_cb);
}
void self_node::friend_request_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length,
                void *user_data)
        {
            tox_friend_add_norequest(tox, public_key, NULL);
        }
void self_node::friend_message_cb(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message,
                size_t length, void *user_data)
        {
            tox_friend_send_message(tox, friend_number, type, message, length, NULL);
            std::printf("ptr: %p\n", self_node::main_event_loop);
            self_node::main_event_loop->push_event({.e_type = event::event_type::E_NEW_MESSAGE,
                    .str = (char *)message});
        }
void self_node::self_connection_status_cb(Tox *tox, TOX_CONNECTION connection_status, void *user_data)
        {
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
