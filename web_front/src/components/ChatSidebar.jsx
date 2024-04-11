import { Flex } from "antd";
import UserBar from './UserBar';
import ChatSideBarContentSelector from './ChatSideBarContentSelector';
import ChatList from './ChatList';
import { useState } from 'react';
import ContactList from "./ContactList";
export default function ChatSidebar() {
    const [showChat, setShowChat] = useState(true);
    return (
        <Flex vertical style={{maxHeight: '100vh', overflow:'hidden', borderRight: '1px solid grey'}}>
                    <UserBar avatarUrl={"https://static.vecteezy.com/system/resources/thumbnails/005/129/844/small/profile-user-icon-isolated-on-white-background-eps10-free-vector.jpg"} />
                    <ChatSideBarContentSelector onChange={(value) => value == 'chat' ? setShowChat(true) : setShowChat(false)}/>
                    { showChat ? <ChatList /> : <ContactList />}
                </Flex>
    );
}
