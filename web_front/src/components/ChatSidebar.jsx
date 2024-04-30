import { Flex } from "antd";
import UserBar from './UserBar';
import ChatSideBarContentSelector from './ChatSideBarContentSelector';
import ChatList from './ChatList';
import { useState, useContext } from 'react';
import ContactList from "./ContactList";
import i from '../assets/user.png';
export default function ChatSidebar() {
    const [showChat, setShowChat] = useState(true);
    return (
        <Flex vertical style={{maxHeight: '100vh', overflow:'hidden', borderRight: '1px solid grey'}}>
                    <UserBar avatarUrl={i} />
                     <ChatList />
                </Flex>
    );
                    // <ChatSideBarContentSelector onChange={(value) => value == 'chat' ? setShowChat(true) : setShowChat(false)}/>
                    // { showChat ? <ChatList /> : <ContactList />}
}
