import ChatListItem from "./ChatListItem";
import i from "../assets/user.png"
import globalContext from "../context";
import { useContext } from "react";
export default function ChatList() {
    const {globalStat, dispatch} = useContext(globalContext);
    let v = globalStat.friend_list.map(e => <ChatListItem avatarUrl={i} lastMessage={{timestamp:'todo', text:'todo'}} className={"lol"} chatName={e.name} key={e.number} isActive={e.number == globalStat.currentFocusedFriend} onClick={() => {
        dispatch({type: 'SET_FRIEND_FOCUSED', number: e.number})
    }}/> )
    return (<div style={{overflow:'scroll'}}>
        {v}
        </div>);
}
