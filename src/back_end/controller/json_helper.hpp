#pragma once
#include "json/json.h"
#include <glog/logging.h>

namespace json_helper {
    std::string message_recv(std::string message, uint32_t friend_num) {
        Json::Value v;
        Json::Value body;
        v["event_id"] = std::rand();
        v["event_type"] = "message_recv";
        body["friend_number"] = friend_num;
        body["message_body"] = message;
        v["event_body"] = body;
        LOG(INFO) << v.toStyledString();
        return v.toStyledString();
    }
    std::string friend_req_recv(std::string message, std::string public_key) {
        Json::Value v;
        Json::Value body;
        v["event_id"] = std::rand();
        v["event_type"] = "friend_req_recv";
        body["public_key"] = public_key;
        body["message"] = message;
        v["event_body"] = body;
        LOG(INFO) << v.toStyledString();
        return v.toStyledString();
    }
};
