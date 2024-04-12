#pragma once
#include <copper.h>
#include <functional>
#include <map>
#include <vector>
#include <thread>
#include <iostream>

namespace event {
    enum event_type {
        E_USER_NOTIFY,
        E_NEW_MESSAGE,
        E_SYSTEM,
        E_TEST1
    };
    struct event {
        event_type e_type;
        const char * str;
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
