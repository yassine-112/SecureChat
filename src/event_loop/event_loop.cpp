#include "event_loop.hpp"
using namespace event;
event_loop::event_loop() {
    this->main_event_queue = new copper::buffered_channel<event>();
    this->req_event_queue = new copper::buffered_channel<sync_event*>();
    this->callback_list = new std::map<event_type, std::vector<callback_fn>*>();
    this->pending_requests = new std::vector<pending_req_item*>();
    this->callback_list_resp = new std::map<event_type, std::vector<callback_fn_resp>*>();
}
event_loop::~event_loop() {
    this->main_event_queue->close();
    delete this->main_event_queue;
    delete this->callback_list;
}
bool event_loop::push_event(event e) {
    std::printf("event added to event loop\n");
    return this->main_event_queue->push(e);
}
void event_loop::subscribe_event(event_type e_type, callback_fn callback) {
    if (this->callback_list->count(e_type) != 1) {
        this->callback_list->insert({e_type, new std::vector<callback_fn>});
    }
    this->callback_list->at(e_type)->push_back(callback);
}
void event_loop::subscribe_event_resp(event_type e_type, callback_fn_resp callback) {
    if (this->callback_list_resp->count(e_type) != 1) {
        this->callback_list_resp->insert({e_type, new std::vector<callback_fn_resp>});
    }
    this->callback_list_resp->at(e_type)->push_back(callback);

    std::printf("list resp callbacks -------------------------\n");
    for (auto const& x : *this->callback_list_resp)
    {
        std::cout << x.first  // string (key)
            << ':' 
            << x.second // string's value 
            << std::endl;
    }
}

struct sync_event* event_loop::push_wait(sync_event * e) {
    std::printf("added push wait req");
    std::printf("this is: %p\n", this);
    bool r = this->req_event_queue->push(e);
    std::mutex* mutex_lock = new std::mutex();
    pending_req_item * req_item = new pending_req_item();
    req_item->lock = mutex_lock;
    req_item->event_id = e->event_id;
    req_item->response = nullptr;
    this->pending_requests->push_back(req_item);
    std::printf("locking push wait\n");
    mutex_lock->lock();
    std::printf("locking 1 push wait\n");
    mutex_lock->lock();
    std::printf("push unlocked");
    return req_item->response;
}

bool event_loop::main_loop() {

    std::printf("main event loop tid: %d\n", gettid());
    
    while (!this->main_event_queue->is_read_closed() && !this->req_event_queue->is_read_closed()) {
        std::printf("looping so  hard omg\n");
        std::optional<event> curr_e_opt = this->main_event_queue->try_pop();
        std::optional<sync_event*> curr_e_opt_sync = this->req_event_queue->try_pop();
        if (curr_e_opt) {
            event curr_e = curr_e_opt.value();
            if (this->callback_list->count(curr_e.e_type) != 0) {
                std::vector<callback_fn> *target_callback_list = this->callback_list->at(curr_e.e_type);
                for (callback_fn cb : *target_callback_list) {
                    cb(curr_e);
                }
            }
        }

        if(curr_e_opt_sync) {
            sync_event *curr_e = curr_e_opt_sync.value();
            if (curr_e->is_request) {
                if (this->callback_list_resp->count(curr_e->e_type) != 0) {
                    std::vector<callback_fn_resp> *target_callback_list = this->callback_list_resp->at(curr_e->e_type);
                    for (callback_fn_resp cb : *target_callback_list) {
                        cb(curr_e);
                    }
            }
            } else {
                for (pending_req_item* r : *this->pending_requests) {
                    if (r->event_id == curr_e->event_id) {
                        std::printf("unlocking a response, %p\n", curr_e->event_payload);
                        r->response = curr_e;
                        r->lock->unlock();
                    }
                }
            }
        }
    }

    return true;
}

std::thread event_loop::spawn_thread() {
    return std::thread(&event_loop::main_loop, this);
}
void event_loop::push_resp(sync_event * e) {
    this->req_event_queue->push(e);
}
