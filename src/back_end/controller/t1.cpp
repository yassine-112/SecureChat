#include "t1.hpp"
#include "../../event_loop/event_loop.hpp"
#include "../http_server.hpp"
void TestCtrl::asyncHandleHttpRequest(const HttpRequestPtr &req,
                                      std::function<void (const HttpResponsePtr &)> &&callback)
{
    std::printf("we are here working\n");
    event::sync_event* req_e = new event::sync_event();
    req_e->e_type = event::event_type::E_RESP_GET_FRIEND_LIST;
    req_e->event_payload = nullptr;
    req_e->event_id = -1;
    req_e->is_request = true;

    event::sync_event* e = back_end::back_end_server::main_event_loop->push_wait(req_e);
    std::printf("got resp %p\n", e);
    std::printf("got resp %d %d %d\n", e->is_request, e->e_type, e->event_id);
    std::printf("got resp %p %d\n", e->event_payload, *(int*)e->event_payload);

    Json::Value ret;
    ret["result"]="ok";
    ret["token"]=drogon::utils::getUuid();
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
