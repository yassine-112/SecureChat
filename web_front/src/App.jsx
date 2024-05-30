import './App.css';
import { Row, Col, Drawer, FloatButton, Modal, Input  } from 'antd';
import ChatSidebar from './components/ChatSidebar';
import MessagingWindow from './components/MessagingWindow';
import { useState, createContext, useEffect, useReducer, useRef } from 'react';
import RecipientPropertiesSidebar from './components/RecipientPropertiesSidebar';
import globalContext from './context';
import UserInfoWindow from './components/UserInfoWindow';
import user_url from './assets/user.png'
import { friend_request_response } from './utils';
import { PlusOutlined } from '@ant-design/icons';
import TextArea from 'antd/es/input/TextArea';
import SendRequest from './components/SendRequest';
import { Notyf } from 'notyf';
import 'notyf/notyf.min.css';

const notification = new  Notyf();
const initGlobal =  {
    "user_name": "",
    "user_status": "",
    "user_online_status": "",
    "user_tox_id": "",
    "user_avatar_url": user_url,
    "tox_status": "connecting",
    "friend_list": [
//        {"name": "test user", "number": -99}
    ],
    "currentFocusedFriend": -1,
    "message_log": {
        // 0: [
        //     {"message_body": "hello", "is_sent": true},
        //     {"message_body": "hello", "is_sent": true},
        //     {"message_body": "hello", "is_sent": true},
        //     {"message_body": "hello", "is_sent": true},
        //     {"message_body": "hey", "is_sent": false},
        //     {"message_body": "l", "is_sent": false},
        //     {"message_body": "l", "is_sent": false},
        // ],
        // 1 :[]
    },
    "input_text": "",
    "notifications" : [
        // {
        //     "id": 666,
        //     "title": "A new friend request",
        //     "body": "From user6666",
        //     "class": "friend",
        //     "icon_path": 'https://s.yimg.com/ny/api/res/1.2/X.dYzBVwO1x4OmeMl_cLHA--/YXBwaWQ9aGlnaGxhbmRlcjt3PTY0MDtoPTk5Mg--/https://s.yimg.com/os/creatr-uploaded-images/2021-07/afa7d990-ef2a-11eb-bcf7-70ebfc0459ed',
        //     "actions": [
        //         {"action_name": "accept", "action_handler": () => console.log("handler")},
        //         {"action_name": "deny", "action_handler": () => console.log("handler")}
        //     ],
        //     "data": {"pub_key"}
        //     "can_delete": false
        // },

    ],


}


function reducer(state, action) {
    switch (action.type) {
        case 'UPDATE_FRIEND_NAME':
            if ( state.friend_list.find(fr => fr.number == action.number) == undefined ) return {
                ...state, friend_list: [...state.friend_list, {name:action.name, number:action.number}]
            }
            else return {
                ...state, friend_list: state.friend_list.map(fr => (fr.number === action.number) ? {...fr, name: action.name} :fr)
            }
        case 'SET_USER_ID':
        return {
                ...state, user_tox_id: action.id,
            }
        case 'SET_TOX_STATUS':
        return {
                ...state, tox_status: action.status,
            }
        case 'DEL_NOTIFICATION':
            return {
                ...state, notifications: state["notifications"].filter(e => e.id != action.id)
            }
        case 'NEW_NOTIFICATION':
            return {
                ...state, notifications: state["notifications"].concat([action.notification])
            }
        case 'SET_USER_ONLINE_STATUS':
            return {
                ...state, user_online_status: action.status
            }
        case 'SET_USER_INFO':
            return {
                ...state, user_name: action.user_name, user_status: action.user_status
            }
        case 'SET_FRIEND_FOCUSED':
            return {
                ...state, "currentFocusedFriend": action.number
            }
        case 'SET_FRIEND_LIST':
            let new_msg_log = {}
            action.friend_list.forEach(elem => {
                if (!state.message_log[elem.number])
                        new_msg_log[elem.number] = []
            })
            console.log(new_msg_log)
            return {
                ...state, "friend_list": action.friend_list, "message_log": {...state.message_log, ...new_msg_log }
            }
        case 'NEW_MESSAGE': //TODO: if array dont exist make empty one list the case above
        return {
                ...state,
                "message_log" :{
                ...state.message_log,
                    [action.number]: [...state.message_log[action.number] , {"message_body": action.message_body, is_sent: action.is_sent, timestamp: (new Date()).toISOString() }]
                }
            }
        case 'INPUT_TEXT':
            return {...state, "input_text": action.input_text}

        default:
            return state
    }
}

const state = window.localStorage.getItem("globalState") != null ? JSON.parse(window.localStorage.getItem("globalState")) : initGlobal;
function App() {
    const [showRecipientPropertiesSidebar, setRecipientPropertiesSidebar] = useState(false);
    const [globalStat, dispatch] = useReducer(reducer, state)


    // THIS IS BAD PRACTICE, BUT I GOT NO TIME TO THINK FOR A BETTER SOLUTION DAMN IT!!!!!
    window.ws = useRef(null);

    useEffect(() => {
        ws.current = new WebSocket("ws://127.0.0.1:8080/echo");
        ws.current.onopen = () => console.log("ws opened");
        ws.current.onclose = () => console.log("ws closed");
        ws.current.onmessage = (message) => {
            console.log(message)
            try {
                let json = JSON.parse(message.data)
                if (json.event_type){
                    switch(json.event_type){
                        case "friend_changed_name":
                            notification.success("You have a new friend.");
                            dispatch({type:'UPDATE_FRIEND_NAME', number:json.event_body.number, name: json.event_body.name})
                            console.log(json)
                            break;
                        case "tox_status":
                            console.log(`got tox status: ${json.event_body.status}`)
                            let s = (json.event_body.status == "TOX_CONNECTION_NONE") ? "error" : "online" 
                            console.log(s);
                            dispatch({type: 'SET_TOX_STATUS', status: s})
                            break;
                        case "message_recv":
                            console.log(`got message ${json.event_body.message_body}`)
                            dispatch({type: 'NEW_MESSAGE', number: json.event_body.friend_number, message_body: json.event_body.message_body, is_sent:false})
                        break;
                        case 'friend_req_recv':
                            console.log(`got new friend req ${json.event_body}`)
                            dispatch({
                                type:'NEW_NOTIFICATION',
                                notification: {
                                    id: json.event_id,
                                    title: "A new friend request",
                                    body: `${json.event_body.message} from ${json.event_body.public_key}`,
                                    class: 'friend',
                                    data: {
                                        pub_key: json.event_body.public_key
                                    },
                                    actions: [
                                        {"action_name": "accept", "action_handler": "accept_req"},
                                        {"action_name": "deny", "action_handler": "deny_req"}
                                    ]

                                }
                            })
                    }

                }
            } catch(e) {

            }
        }

        const wsCurrent = ws.current;

        return () => {
            wsCurrent.close();
        };
    }, []);

    useEffect( () => {
        window.localStorage.setItem("globalState", JSON.stringify(globalStat))
        console.log(globalStat)
    }, [globalStat]);


    useEffect( ()=>{
        // get friend list
        fetch('http://127.0.0.1:8080/get_friends_list')
            .then(response => response.json())
            .then(json => {
                console.log("updating the friend list");
                dispatch({type: 'SET_FRIEND_LIST', friend_list:json})
            }) }, [globalStat.notifications] )

    useEffect( () => {
        fetch('http://127.0.0.1:8080/get_user_id')
            .then(response => response.json())
            .then(json => {
                dispatch({type: 'SET_USER_ID', id:json.id})
            })
    }, [])


    const messageSentBtnHandler = (message) => {
        const sent_msg = 
            {
                "event_id" : 0,
                "event_type" : "message_sent",
                "event_body" : {
                    "friend_number" : globalStat.currentFocusedFriend,
                    "message_body": message
                }

            }
        console.log(sent_msg)
        ws.current.send(JSON.stringify(sent_msg))
        dispatch({type: 'NEW_MESSAGE', number: globalStat.currentFocusedFriend, message_body: message, is_sent:true})
    }

    const sendFrReqHandler = (address, message) => {
        // validate args
        if(address == "" || message == "") {
            notification.error("Invalid input.");
            return;
        }
        else if (address.match(/[A-F0-9]{76}/g) == null) {
            ret = true;
            notification.error("Invalid Tox ID")
            return;
        }


        const sent_msg = 
            {
                "event_id" : 999,
                "event_type" : "send_friend_req",
                "event_body" : {
                    "address" : address,
                    "message": message
                }

            }
        console.log(sent_msg)
        ws.current.send(JSON.stringify(sent_msg))
        notification.success("A friend request is sent, waiting for them to accept it.");
    }





  return (
        <globalContext.Provider value={{globalStat, dispatch, messageSentBtnHandler, notification}}>
            <SendRequest handler={sendFrReqHandler} />
            <Row style={{maxHeight: '100vh'}}>
                <Col span={6}>
                    <ChatSidebar />
                </Col>
                <Col span={18}>
                    {
                        globalStat.currentFocusedFriend == -1 ? 
                            <UserInfoWindow/> : 
                            <MessagingWindow messageSentBtnHandler={messageSentBtnHandler} 
                                defaultView={globalStat.currentFocusedFriend == -1} 
                                showSideBarToggle={ () => setRecipientPropertiesSidebar(!showRecipientPropertiesSidebar) }/>
                    }
                </Col>
                <Drawer title="Contact Info" onClose={() => setRecipientPropertiesSidebar(false)} open={showRecipientPropertiesSidebar}>
                    <RecipientPropertiesSidebar />
                </Drawer>
            </Row>
        </globalContext.Provider>
)
}


export default App
