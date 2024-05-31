import ChatListItem from "./ChatListItem";
import i from "../assets/user.png"
import globalContext from "../context";
import { useContext } from "react";
import { Skeleton } from "antd";
import {fuzzy} from "fast-fuzzy"

export default function ChatList({searchStr}) {
    const {globalStat, dispatch} = useContext(globalContext);
    const getUnreadMessage = (number) =>  globalStat.message_log[number].reduce( (acc, msg) => acc + (msg.is_local_seen == false&&msg.is_sent == false ? 1 : 0 ), 0 );
    const getLastMessage = (number) =>  {
        if (globalStat.message_log[number].length == 0) return {timestamp: "NA", text:""}
        const m =  globalStat.message_log[number].reduce( (acc, msg) => new Date(acc.timestamp) < new Date(msg.timestamp) ? msg : acc )
        return {timestamp: m.timestamp, text: m.message_body}
    }
     

    let v = globalStat.friend_list
    .filter(e => searchStr == "" ? true : fuzzy(searchStr, e.name) >= 0.6 ) // TODO ADD FUZZY MATCH
    .map(e => 
        <ChatListItem avatarUrl={i} 
            lastMessage={getLastMessage(e.number)} 
            className={"lol"} 
            unreadMessages={getUnreadMessage(e.number)}
            chatName={e.name === "" ?  <Skeleton.Input size={"small"} active={true} /> : e.name} 
            key={e.number} 
            isActive={e.number == globalStat.currentFocusedFriend} 
            onClick={() => {
                dispatch({type: 'SET_FRIEND_FOCUSED', number: e.number})
            }}
        /> 

    )
    return (<div style={{overflow:'scroll'}}>
        {v}
        </div>);
}
