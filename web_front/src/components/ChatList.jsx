import ChatListItem from "./ChatListItem";
import i from "../assets/user.png"
export default function ChatList() {
    return (<div style={{overflow:'scroll'}}>

                <ChatListItem avatarUrl={i} lastMessage={{timestamp:'11:11 PM', text:'this is the last message'}} chatName={'jackie'} />
                <ChatListItem avatarUrl={i} lastMessage={{timestamp:'11:11PM', text:'this is the last message'}} chatName={'jackie'} />
                <ChatListItem avatarUrl={i} lastMessage={{timestamp:'11:11PM', text:'this is the last message'}} chatName={'jackie'} />
                <ChatListItem avatarUrl={i} lastMessage={{timestamp:'11:11PM', text:'this is the last message'}} chatName={'jackie'} />
                <ChatListItem avatarUrl={i} lastMessage={{timestamp:'11:11PM', text:'this is the last message'}} chatName={'jackie'} />
                <ChatListItem avatarUrl={i} lastMessage={{timestamp:'11:11PM', text:'this is the last message'}} chatName={'jackie'} />
                <ChatListItem avatarUrl={i} lastMessage={{timestamp:'11:11PM', text:'this is the last message'}} chatName={'jackie'} />
                <ChatListItem avatarUrl={i} lastMessage={{timestamp:'11:11PM', text:'this is the last message'}} chatName={'jackie'} />
                <ChatListItem avatarUrl={i} lastMessage={{timestamp:'11:11PM', text:'this is the last message'}} chatName={'jackie'} />
                <ChatListItem avatarUrl={i} lastMessage={{timestamp:'11:11PM', text:'this is the last message'}} chatName={'jackie'} />
                <ChatListItem avatarUrl={i} lastMessage={{timestamp:'11:11PM', text:'this is the last message'}} chatName={'jackie'} />
        </div>);
}
