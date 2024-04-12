#include "./http_server.hpp"

using namespace back_end;
back_end_server::back_end_server(event::event_loop* event_loop)
    :main_event_loop(event_loop)
{
    // server app() configuration
    drogon::app().loadConfigFile("./config.json");
}


void back_end_server::main_loop() {
    drogon::app().run();
}
std::thread back_end_server::spawn_thread() {
    return std::thread(back_end_server::main_loop);
}
