#include "./rest.hpp"

void rest_controller::get_friend_list(const HttpRequestPtr &req,
        std::function<void (const HttpResponsePtr &)> &&callback,
        std::string &&userId,
        const std::string &password)
{
    /*
     * event_loop->push_event(brit_friend_list);
     * in the other side the event got handled, and a event containing the friend list get pushed
     * how we gonna get it here??
     *
     * - Solution 1: find a way to call tox method directly (concurrencly problems)
     * - Solution 2: add a functionality to event loop to wait for the response of an event as an
     *   alternative to registering a event callback
     *      event_loop->push_wait_response(event) -> response event
     *
     *
     */
     
}
