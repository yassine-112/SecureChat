#include "event_loop.hpp"
using namespace event;
event_loop::event_loop() {
    this->main_event_queue = new copper::buffered_channel<async_event>();
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
bool event_loop::push_event(async_event e) {
    std::printf("event added to event loop\n");
    return this->main_event_queue->push(e);
}
void event_loop::subscribe_event(event_type e_type, callback_fn callback) {
    if (this->callback_list->count(e_type) != 1) {
        this->callback_list->insert({e_type, new std::vector<callback_fn>});
    }
    this->callback_list->at(e_type)->push_back(callback);

    for (auto const& x : *this->callback_list)
    {
        for (auto const& f: *(this->callback_list->at(x.first))) {
            std::cout << x.first  // string (key)
                << ':' 
                << &f // string's value 
                << std::endl;

        }
    }
}
void event_loop::subscribe_event_resp(event_type e_type, callback_fn_resp callback) {
    if (this->callback_list_resp->count(e_type) != 1) {
        this->callback_list_resp->insert({e_type, new std::vector<callback_fn_resp>});
    }
    this->callback_list_resp->at(e_type)->push_back(callback);

    std::printf("list resp callbacks -------------------------\n");
    for (auto const& x : *this->callback_list_resp)
    {
        for (auto const& f: *(this->callback_list_resp->at(x.first))) {
            std::cout << x.first  // string (key)
                << ':' 
                << &f // string's value 
                << std::endl;

        }
    }
}

struct sync_event* event_loop::push_wait(sync_event * e) {
    std::printf("added push wait req");
    std::printf("this is: %p\n", this);

    e->event_id = std::rand();
    e->is_request = true;
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
    
    while (!this->main_event_queue->is_read_closed() ) {
        std::printf("looping so  hard omg1\n");
        std::optional<async_event> curr_e_opt = this->main_event_queue->pop();
        if (curr_e_opt) {
            async_event curr_e = curr_e_opt.value();
            if (this->callback_list->count(curr_e.e_type) != 0) {
                std::vector<callback_fn> *target_callback_list = this->callback_list->at(curr_e.e_type);
                for (callback_fn cb : *target_callback_list) {
                    cb(curr_e);
                }
            }
        }

    }

    return true;
}
bool event_loop::main_loop_resp() {
    while(!this->req_event_queue->is_read_closed()) {
        std::optional<sync_event*> curr_e_opt_sync = this->req_event_queue->pop();
        std::printf("looping so  hard omg2 resp\n");
        if(curr_e_opt_sync) {
            sync_event *curr_e = curr_e_opt_sync.value();
            if (curr_e->is_request) {
                std::printf("[EVENT LOOP] looking for callbacks of type %d: id: %d\n", curr_e->e_type, curr_e->event_id);
                if (this->callback_list_resp->count(curr_e->e_type) != 0) {
                std::printf("[EVENT LOOP]dispatching req event %d\n", curr_e->e_type);
                    std::vector<callback_fn_resp> *target_callback_list = this->callback_list_resp->at(curr_e->e_type);
                    for (callback_fn_resp cb : *target_callback_list) {
                        cb(curr_e);
                    }
            }
            } else {
                for (pending_req_item* r : *this->pending_requests) {
                    std::printf("[EVENT LOOP] got pending req %d, currently working on %d\n", r->event_id, curr_e->event_id);
                    if (r->event_id == curr_e->event_id) {
                        std::printf("unlocking a response, %p\n", curr_e->event_payload);
                        r->response = curr_e;
                        r->lock->unlock();
                    }
                }
            }
        }

    }
}

std::pair<std::thread,std::thread> event_loop::spawn_thread() {
    return {std::thread(&event_loop::main_loop, this), std::thread(&event_loop::main_loop_resp, this)};
}
void event_loop::push_resp(sync_event * e) {
    e->is_request = false;
    this->req_event_queue->push(e);
}
event::async_event::async_event(){}
event::async_event::async_event(event_type _e_type, void * payload)
    :e_type(_e_type), event_payload(payload)
{
    std::printf("[EVENT LOOP] craeting new event of type %d\n", _e_type);
}

event::sync_event::sync_event(){}
event::sync_event::sync_event(event_type _e_type, void * payload, u_int32_t id)
    :async_event(_e_type,payload), event_id(id)
{
    std::printf("[EVENT LOOP] craeting new event of type %d\n", e_type);
}

