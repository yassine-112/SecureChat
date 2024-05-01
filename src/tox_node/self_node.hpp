#pragma once
#include <tox.h>
#include <string>
#include <list>
#include <sodium.h>
#include <unistd.h>
#include <thread>
#include "../event_loop/event_loop.hpp"

namespace tox{
    struct dht_node {
        const char *ip;
        uint16_t port;
        const char key_hex[TOX_PUBLIC_KEY_SIZE*2 + 1];
    };
    class self_node {
        public:
            self_node( event::event_loop *main_event_loop,
                    std::list<dht_node> *dht_node_list = nullptr,
                    std::string *serialization_path = nullptr );
            ~self_node();
            std::thread spawn();
            void stop_instance();
            event::event_loop *main_event_loop;
        private:
            friend class self_node_cb;
            std::string * serialization_path;
            std::string * user_name;
            std::string * user_status;
            std::string * user_tox_id;
            std::list<dht_node> *dht_node_list;
            Tox_Options   node_options;
            bool node_first_run;
            /* node_status */
            Tox* tox_c_instance;
            void main_loop();
            void register_handlers(); 
            void register_tox_callbacks();
            void update_savedata_file();
            bool auto_accept = true;
            bool enable_trace = false;
            const char *savedata_filename = "savedata.tox";
            const char *savedata_tmp_filename = "savedata.tox.tmp";

        // handlers to handle events from the event loop

    };
};
