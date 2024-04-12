#pragma once
#include <copper.h>
#include <functional>
#include <map>
#include <vector>
#include <thread>
#include <iostream>
#include "tox.h"

namespace event {
    // do not modify exsiting event names!
    enum event_type {
        E_USER_NOTIFY,
        E_NEW_MESSAGE_RECV,
        E_NEW_MESSAGE_SENT,
        E_SYSTEM,
        E_SYSTEM_CONNECTED,
        E_TEST1
    };
    struct system_connected_event {
        TOX_CONNECTION connection_mode;
    };
    struct message_event {
        uint32_t friend_number; 
        TOX_MESSAGE_TYPE type; 
        const uint8_t *message;
        size_t length;
    };
    struct event {
        event_type e_type;
        void * event_payload;
    };

    typedef std::function<void(event)> callback_fn;

    class event_loop {
        public:
        event_loop();
        ~event_loop();
        bool push_event(event e);
        void subscribe_event(event_type e_type, callback_fn callback);
        std::thread spawn_thread();
        private:
        bool main_loop();
        std::map<event_type, std::vector<callback_fn>*> *callback_list;
        copper::buffered_channel<event> *main_event_queue;
        std::thread event_loop_thread;
    };
}
