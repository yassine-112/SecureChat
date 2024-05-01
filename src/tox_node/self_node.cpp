#include "self_node.hpp"
#include "./tox_callbacks.hpp"
#include <string>

using namespace tox;

void self_node::update_savedata_file() {
    size_t size = tox_get_savedata_size(tox_c_instance);
    uint8_t *savedata = (uint8_t*)malloc(size);
    tox_get_savedata(tox_c_instance, savedata);
    FILE *f = fopen(savedata_tmp_filename, "wb");
    fwrite(savedata, size, 1, f);
    fclose(f);
    rename(savedata_tmp_filename, savedata_filename);
}

self_node::self_node(event::event_loop *main_event_loop, 
        std::list<dht_node> *dht_node_list ,
        std::string *serialization_path)
    : main_event_loop(main_event_loop), 
    dht_node_list(dht_node_list),  
    serialization_path(serialization_path)
{
    tox_options_default(&node_options);
    if (enable_trace) node_options.log_callback = self_node_cb::log;

    setup_options();
    tox_c_instance = tox_new(&node_options, NULL);
    /* this->user_name = new std::string("test dev build"); */ // TODO: get these in an other way
    /* this->user_status = new std::string("test dev build"); */
    node_bootstrap();
    update_savedata_file();
    set_tox_id();
    connect_cb();
}

void self_node::connect_cb() {
    self_node_cb::curr_node = this;
    self_node_cb::register_handlers();
    self_node_cb::register_tox_callbacks();
}
void self_node::set_tox_id() {
    uint8_t tox_id_bin[TOX_ADDRESS_SIZE];
    tox_self_get_address(this->tox_c_instance, tox_id_bin);

    char tox_id_hex[TOX_ADDRESS_SIZE*2 + 1];
    sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), tox_id_bin, sizeof(tox_id_bin));

    for (size_t i = 0; i < sizeof(tox_id_hex)-1; i ++) {
        tox_id_hex[i] = toupper(tox_id_hex[i]);
    }
    this->user_tox_id = new std::string(tox_id_hex);
    LOG(INFO) << "id: "  << tox_id_hex << '\n';

}
bool self_node::setup_options() {
    FILE *f = fopen(savedata_filename, "rb");
    node_first_run = f == NULL;
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
    }
    return f != NULL;
}
void self_node::node_bootstrap() {
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


}
self_node::~self_node() {
    LOG(INFO) << "killing tox instance\n";
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
void self_node::stop_instance() {
    tox_kill(tox_c_instance);
}
