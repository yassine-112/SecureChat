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
            self_node(
        event::event_loop *main_event_loop,
        std::list<dht_node> *dht_node_list = nullptr,
        Tox_Options  *node_options = nullptr,
        std::string *serialization_path = nullptr );
        ~self_node();
        std::thread spawn();
        event::event_loop *main_event_loop;
        private:
        friend class self_node_cb;
        std::string * serialization_path;
        std::string * user_name;
        std::string * user_status;
        std::string * user_tox_id;
        std::list<dht_node> *dht_node_list;
        Tox_Options  * node_options;
        bool node_first_run;
        /* node_status */
        Tox* tox_c_instance;
        void main_loop();
        void register_handlers(); 
        void register_tox_callbacks();
        bool auto_accept = true;

        // handlers to handle events from the event loop

    };
    class self_node_cb {
        private:
        friend class self_node;
        static  self_node *curr_node;

        // handlers to handle events from the event loop
        static void handle_message_sent(event::event e);


        // tox callbacks: put events in the event loop
        static void friend_request_cb(Tox *tox, const uint8_t *public_key, 
                const uint8_t *message, size_t length,
                void *user_data);

        static  void friend_message_cb(Tox *tox, uint32_t friend_number, 
                TOX_MESSAGE_TYPE type, const uint8_t *message,
                size_t length, void *user_data);

        static void self_connection_status_cb(Tox *tox, TOX_CONNECTION connection_status, 
                void *user_data);
    };
};
