import ChatListItem from "./ChatListItem";
import i from "../assets/user.png"
import globalContext from "../context";
import { useContext } from "react";
import { Skeleton } from "antd";
import {fuzzy} from "fast-fuzzy"

export default function ChatList({searchStr}) {
    const {globalStat, dispatch} = useContext(globalContext);
    let v = globalStat.friend_list
    .filter(e => searchStr == "" ? true : fuzzy(searchStr, e.name) >= 0.6 ) // TODO ADD FUZZY MATCH
    .map(e => 
        <ChatListItem avatarUrl={i} 
            lastMessage={{timestamp:'todo', text:'todo'}} 
            className={"lol"} 
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
