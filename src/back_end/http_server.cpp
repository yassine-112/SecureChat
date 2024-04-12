#include "./http_server.hpp"

using namespace back_end;
event::event_loop *back_end_server::main_event_loop;
back_end_server::back_end_server(event::event_loop* event_loop)
{
    // server app() configuration
    back_end_server::main_event_loop = event_loop;
    std::printf("main_event_lopp backend server init = %p\n", back_end_server::main_event_loop);
    drogon::app().loadConfigFile("./config.json");
}


void back_end_server::main_loop() {
    drogon::app().run();
}
std::thread back_end_server::spawn_thread() {
    return std::thread(back_end_server::main_loop);
}
