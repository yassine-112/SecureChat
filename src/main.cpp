#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include "event_loop/event_loop.hpp"
#include "tox_node/self_node.hpp"
#include "back_end/http_server.hpp"
#include <glog/logging.h>


void handle_msg(event::async_event _e) {
    event::message_event *e = (event::message_event*)_e.event_payload;
    LOG(INFO) << "New message: " << e->message << "from friend: "<< e->friend_number << '\n';
}

/* void msg_interface(event::event_loop *l) { */
/*     while(true) { */
/*         uint32_t num; */
/*         char * buff = (char*) malloc(sizeof(char)*256); */
/*         std::printf("enter new msg ->: "); */
/*         scanf("%d %s", &num, buff); */
/*         fflush(stdin); */

/*         event::message_event *e = new event::message_event(); */
/*         e->friend_number = num; */
/*         e->message =(uint8_t*) buff; */
/*         e->type = TOX_MESSAGE_TYPE_NORMAL; */
/*         e->length = std::strlen(buff); */

/*         /1* l->push_event({.e_type = event::event_type::E_NEW_MESSAGE_SENT, .event_payload=e}); *1/ */


/*     } */
/* } */

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);

    FLAGS_logtostderr = 1;

    LOG(INFO) << "Starting chatapp\n";

    event::event_loop *main_event_loop = new event::event_loop();
    std::pair<std::thread, std::thread> main_loop_threads = main_event_loop->spawn_thread();
    back_end::back_end_server *web_serve = new back_end::back_end_server(main_event_loop);
    std::thread web_thread = web_serve->spawn_thread();


    main_event_loop->subscribe_event(event::event_type::E_NEW_MESSAGE_RECV, handle_msg);

    tox::self_node *node = new tox::self_node(main_event_loop);
    std::thread p2p_thread = node->spawn();
    /* std::thread interface_thd = std::thread(msg_interface, main_event_loop); */


    /*getchar();*/
    /*node->stop_instance();*/
    /*LOG(INFO) << "stopped tox\n";*/

    p2p_thread.join();
    LOG(INFO) << "Tox thread exited\n";
    main_loop_threads.first.join();
    LOG(INFO) << "First event loop thread exited\n";
    /*main_loop_threads.second.join();*/
    LOG(INFO) << "Second event loop thread exited\n";
    web_thread.join();
    LOG(INFO) << "Web thread exited\n";
    /* interface_thd.join(); */


    return 0;
}
