#pragma once
#include <drogon/HttpController.h>
using namespace drogon;
class rest_controller:public drogon::HttpController<rest_controller>
{
public:
    METHOD_LIST_BEGIN
    //list path definitions here;
        METHOD_ADD(rest_controller::get_friend_list,"/api/get_friend_list",Get);
    METHOD_LIST_END
    void get_friend_list(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            std::string &&userId,
            const std::string &password);
};
