#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include "event_loop/event_loop.hpp"
#include "tox_node/self_node.hpp"


void handle_msg(event::event e) {
    std::printf("got message: %s \n", e.str);

}

int main() {

    event::event_loop *main_event_loop = new event::event_loop();
    tox::self_node::main_event_loop = main_event_loop;
    main_event_loop->subscribe_event(event::event_type::E_NEW_MESSAGE, handle_msg);

    tox::self_node *node = new tox::self_node(main_event_loop);
    std::thread main_loop_thread = main_event_loop->spawn_thread();
    std::thread p2p_thread = node->spawn();

    p2p_thread.join();
    main_loop_thread.join();


    return 0;
}
