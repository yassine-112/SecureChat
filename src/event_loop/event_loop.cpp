#include "event_loop.hpp"
using namespace event;
event_loop::event_loop() {
    this->main_event_queue = new copper::buffered_channel<event>();
    this->callback_list = new std::map<event_type, std::vector<callback_fn>*>();
}
event_loop::~event_loop() {
    this->main_event_queue->close();
    delete this->main_event_queue;
    delete this->callback_list;
}
bool event_loop::push_event(event e) {
    return this->main_event_queue->push(e);
}
void event_loop::subscribe_event(event_type e_type, callback_fn callback) {
    if (this->callback_list->count(e_type) != 1) {
        this->callback_list->insert({e_type, new std::vector<callback_fn>});
    }
    this->callback_list->at(e_type)->push_back(callback);
}

bool event_loop::main_loop() {

    
    while (!this->main_event_queue->is_read_closed()) {
        std::optional<event> curr_e_opt = this->main_event_queue->pop();
        if (curr_e_opt) {
            event curr_e = curr_e_opt.value();
            if (this->callback_list->count(curr_e.e_type) != 0) {
                std::vector<callback_fn> *target_callback_list = this->callback_list->at(curr_e.e_type);
            for (callback_fn cb : *target_callback_list) {
                cb(curr_e);
            }
            }
        } else {
            return false;
        }
    }

    return true;
}

std::thread event_loop::spawn_thread() {
    return std::thread(&event_loop::main_loop, this);
}
