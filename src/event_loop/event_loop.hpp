#pragma once
#include <copper.h>
#include <functional>
#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <utility>
#include <iostream>
#include <list>
#include <chrono>
#include "unistd.h"
#include "tox.h"

namespace event {
    // do not modify exsiting event names!
    enum event_type {
// reponse types
        E_RESP_GET_FRIEND_STATUS_MSG,
        E_RESP_GET_FRIEND_NAME,
        E_RESP_GET_FRIEND_NUMBERS_LIST,
// no reponse_types
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
    struct async_event {
        async_event();
        async_event(event_type _e_type, void * payload);
        event_type e_type;
        void * event_payload;
    };
    struct sync_event:async_event {
        sync_event();
        sync_event(event_type _e_type, void * payload, uint32_t id = -1);
        uint32_t event_id;
        bool is_request;
    };
    struct pending_req_item {
        std::mutex * lock;
        uint32_t event_id;
        sync_event* response;
    };

    typedef std::function<void(async_event)> callback_fn;
    typedef std::function<void(sync_event*)> callback_fn_resp;

    class event_loop {
        public:
        event_loop();
        ~event_loop();
        bool push_event(async_event e);
        sync_event* push_wait(sync_event * e);
        void push_resp(sync_event * e);
        void subscribe_event(event_type e_type, callback_fn callback);
        void subscribe_event_resp(event_type e_type, callback_fn_resp callback);
        std::pair<std::thread, std::thread > spawn_thread();
        private:
        bool main_loop();
        bool main_loop_resp();
        std::map<event_type, std::vector<callback_fn>*> *callback_list;
        std::map<event_type, std::vector<callback_fn_resp>*> *callback_list_resp;
        copper::buffered_channel<async_event> *main_event_queue;
        copper::buffered_channel<sync_event*> *req_event_queue;
        std::thread event_loop_thread;
        std::vector<pending_req_item*> * pending_requests;
    };
}
