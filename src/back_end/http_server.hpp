#pragma once
#include <drogon/drogon.h>
#include <thread>
#include "../event_loop/event_loop.hpp"

namespace back_end {
 class back_end_server {
     public:
         back_end_server(event::event_loop* event_loop);
         ~back_end_server();
         std::thread spawn_thread();
     private:
         static void main_loop();
         event::event_loop* main_event_loop;
 };
};
