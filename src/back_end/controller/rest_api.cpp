#include "./rest_api.hpp"
#include "../http_server.hpp"
#include "../../event_loop/event_loop.hpp"

using namespace api;
void friends::get_friends_list(const HttpRequestPtr &req,
               std::function<void (const HttpResponsePtr &)> &&callback) {


    event::sync_event* e = new event::sync_event();
    e->e_type = event::event_type::E_RESP_GET_FRIEND_NUMBERS_LIST;
    e->is_request = true;
    e->event_payload = nullptr;
    e = back_end::back_end_server::main_event_loop->push_wait(e);
    std::pair<uint32_t, uint32_t*> * freinds_list = (std::pair<uint32_t, uint32_t*>*)e->event_payload;
    if (freinds_list->first == 0) {
        callback(drogon::HttpResponse::newHttpJsonResponse(Json::arrayValue));
    } else {
        Json::Value resp(Json::arrayValue);
        for (int i = 0; i < freinds_list->first; i++) {
            Json::Value friend_item;
            friend_item["number"] = std::to_string(freinds_list->second[i]);
            LOG(INFO) << "[WEB THREAD] waiting for fr name\n";
            event::sync_event *x = new event::sync_event(
                    event::event_type::E_RESP_GET_FRIEND_NAME, new std::int32_t(freinds_list->second[i])
                        );

            /* std::cout << "[WEB THREAD] init event of type " << x->e_type <<"\n"; */
            x = back_end::back_end_server::main_event_loop->push_wait(x);
            /* std::cout << "[WEB THREAD] got fr name\n"; */
            friend_item["name"] = *(std::string*)x->event_payload;
            x = back_end::back_end_server::back_end_server::main_event_loop->push_wait(new event::sync_event(event::event_type::E_RESP_GET_FRIEND_STATUS_MSG, new std::int32_t(freinds_list->second[i])));
            friend_item["status_message"] = *(std::string*)x->event_payload;
            resp.append(friend_item);
            std::cout << friend_item << '\n';

        }
        LOG(INFO) << "[WEB THREAD] sending fr names back\n";
        std::cout << '\t' << resp << '\n';
        auto http_resp = HttpResponse::newHttpJsonResponse(resp);
        http_resp->addHeader("Access-Control-Allow-Origin", "*");
        http_resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        callback(http_resp);
        
    }
    /* fr_item["name"] = "jack"; */
    /* fr_item["id"] = 1000; */

    /* test.append(fr_item); */
    /* test.append(fr_item); */
    /* test.append(fr_item); */
    /* test.append(fr_item); */
    /* test.append(fr_item); */

    /* auto resp = drogon::HttpResponse::newHttpJsonResponse(test); */

}
