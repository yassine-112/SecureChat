#pragma once
#include "json/json.h"

namespace json_helper {
    std::string message_recv(std::string message, uint32_t friend_num) {
        Json::Value v;
        Json::Value body;
        v["event_id"] = std::rand();
        v["event_type"] = "message_recv";
        body["friend_number"] = friend_num;
        body["message_body"] = message;
        v["event_body"] = body;
        return v.toStyledString();
    }
};
