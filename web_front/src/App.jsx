import './App.css';
import { Row, Col, Drawer  } from 'antd';
import ChatSidebar from './components/ChatSidebar';
import MessagingWindow from './components/MessagingWindow';
import { useState, createContext, useEffect, useReducer, useRef } from 'react';
import RecipientPropertiesSidebar from './components/RecipientPropertiesSidebar';
import globalContext from './context';

const initGlobal = {
    "friend_list": [
        {"name": "test user", "number": -99}
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
        {
            "id": 666,
            "title": "A new friend request",
            "body": "From user6666",
            "class": "friend",
            "icon_path": 'https://s.yimg.com/ny/api/res/1.2/X.dYzBVwO1x4OmeMl_cLHA--/YXBwaWQ9aGlnaGxhbmRlcjt3PTY0MDtoPTk5Mg--/https://s.yimg.com/os/creatr-uploaded-images/2021-07/afa7d990-ef2a-11eb-bcf7-70ebfc0459ed',
            "actions": [
                {"action_name": "accept", "action_handler": () => console.log("handler")},
                {"action_name": "deny", "action_handler": () => console.log("handler")}
            ],
            "can_delete": false
        },

        {
            "id": 777,
            "title": "test notification",
            "body": "content",
            "class": "friend",
            "icon_path": 'https://s.yimg.com/ny/api/res/1.2/X.dYzBVwO1x4OmeMl_cLHA--/YXBwaWQ9aGlnaGxhbmRlcjt3PTY0MDtoPTk5Mg--/https://s.yimg.com/os/creatr-uploaded-images/2021-07/afa7d990-ef2a-11eb-bcf7-70ebfc0459ed',
            "actions": [
                {"action_name": "accept", "action_handler": () => console.log("handler")}
            ],
            "can_delete": true
        }
    ],


}


function reducer(state, action) {
    switch (action.type) {
        case 'SET_FRIEND_FOCUSED':
            return {
                ...state, "currentFocusedFriend": action.number
            }
        case 'SET_FRIEND_LIST':
            let new_msg_log = {}
            action.friend_list.map(elem => new_msg_log[elem.number] = [])
            console.log(new_msg_log)
            return {
                ...state, "friend_list": action.friend_list, "message_log": {...state.message_log, ...new_msg_log }
            }
        case 'NEW_MESSAGE': //TODO: if array dont exist make empty one list the case above
        return {
                ...state,
                "message_log" :{
                ...state.message_log,
                [action.number]: [...state.message_log[action.number] , {"message_body": action.message_body, is_sent: action.is_sent}]
                }
            }
        case 'INPUT_TEXT':
            return {...state, "input_text": action.input_text}

        default:
            return state
    }
}

function App() {
    const [showRecipientPropertiesSidebar, setRecipientPropertiesSidebar] = useState(false);
    const [globalStat, dispatch] = useReducer(reducer, initGlobal)
    const ws = useRef(null);

    useEffect(() => {
        ws.current = new WebSocket("ws://127.0.0.1:8080/echo");
        ws.current.onopen = () => console.log("ws opened");
        ws.current.onclose = () => console.log("ws closed");
        ws.current.onmessage = (message) => {
            console.log(message)
            try {
                let json = JSON.parse(message.data)
                if (json.event_type && json.event_type == "message_recv") {
                    console.log(`got message ${json.event_body.message_body}`)
                    dispatch({type: 'NEW_MESSAGE', number: json.event_body.friend_number, message_body: json.event_body.message_body, is_sent:false})
                }

            } catch(e) {

            }
        }

        const wsCurrent = ws.current;

        return () => {
            wsCurrent.close();
        };
    }, []);


    useEffect( ()=>{
        // get friend list
        fetch('http://127.0.0.1:8080/get_friends_list')
            .then(response => response.json())
            .then(json => {
            dispatch({type: 'SET_FRIEND_LIST', friend_list:json})
            })
    }, [])

    useEffect( () => console.log(globalStat), [globalStat]);

    // open socket connection


    const messageSentBtnHandler = () => {
        const sent_msg = 
            {
                "event_id" : 0,
                "event_type" : "message_sent",
                "event_body" : {
                    "friend_number" : globalStat.currentFocusedFriend,
                    "message_body": globalStat.input_text
                }

            }
        console.log(sent_msg)
        ws.current.send(JSON.stringify(sent_msg))
        dispatch({type: 'NEW_MESSAGE', number: globalStat.currentFocusedFriend, message_body: globalStat.input_text, is_sent:true})
        dispatch({type: "INPUT_TEXT", input_text: ""})
    }



  return (
    <globalContext.Provider value={{globalStat, dispatch, messageSentBtnHandler}}>
        <Row style={{maxHeight: '100vh'}}>
            <Col span={6}>
                <ChatSidebar />
            </Col>
            <Col span={18}>
                <MessagingWindow messageSentBtnHandler={messageSentBtnHandler} defaultView={globalStat.currentFocusedFriend == -1} showSideBarToggle={ () => setRecipientPropertiesSidebar(!showRecipientPropertiesSidebar) }/>
            </Col>
                <Drawer title="Contact Info" onClose={() => setRecipientPropertiesSidebar(false)} open={showRecipientPropertiesSidebar}>
                        <RecipientPropertiesSidebar />
                </Drawer>
        </Row>
    </globalContext.Provider>
)
}


export default App
