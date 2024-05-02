export function friend_request_response(public_key) {
    window.ws.current.send(JSON.stringify({
                "event_id" : 0,
                "event_type" : "request_accept",
                "event_body" : {
                    "public_key" : public_key
                }
    }))
}
