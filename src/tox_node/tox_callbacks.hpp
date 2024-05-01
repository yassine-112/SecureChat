#pragma once
#include "./self_node.hpp"
namespace tox{
    class self_node_cb {
        private:
            friend class self_node;
            static  self_node *curr_node;

            static void register_handlers();
            static void register_tox_callbacks();

            static void log(Tox *tox, Tox_Log_Level level, const char *file, uint32_t line, const char *func,
                    const char *message, void *user_data);

            // handlers to handle events from the event loop
            static void handle_message_sent(event::async_event e);

            static void handle_friend_list_req(event::sync_event * e);

            static void handle_friend_get_name(event::sync_event * e);

            static void handle_friend_get_status_message(event::sync_event * e);


            // tox callbacks: put events in the event loop
            static void friend_request_cb(Tox *tox, const uint8_t *public_key, 
                    const uint8_t *message, size_t length,
                    void *user_data); // rn we accept all requests

            static  void friend_message_cb(Tox *tox, uint32_t friend_number, 
                    TOX_MESSAGE_TYPE type, const uint8_t *message,
                    size_t length, void *user_data);

            static void self_connection_status_cb(Tox *tox, TOX_CONNECTION connection_status, 
                    void *user_data);
            static void friend_name_cb(
                    Tox *tox, Tox_Friend_Number friend_number,
                    const uint8_t name[], size_t length, void *user_data);

            static void friend_status_cb(
                    Tox *tox, Tox_Friend_Number friend_number, Tox_User_Status status, void *user_data);

            static void friend_connection_status_cb(
                    Tox *tox, Tox_Friend_Number friend_number, Tox_Connection connection_status, void *user_data);


            static void friend_typing_cb(
                    Tox *tox, Tox_Friend_Number friend_number, bool typing, void *user_data);
            static void friend_read_receipt_cb(
                    Tox *tox, Tox_Friend_Number friend_number, Tox_Friend_Message_Id message_id, void *user_data);
    };
}
