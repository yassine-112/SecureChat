import { Flex } from "antd";
import UserBar from './UserBar';
import ChatSideBarContentSelector from './ChatSideBarContentSelector';
import ChatList from './ChatList';
import { useState, useContext, useReducer } from 'react';
import ContactList from "./ContactList";
import i from '../assets/user.png';
import globalContext from "../context";
import { dark_theme_style_container } from "../utils";
export default function ChatSidebar() {
    const [showChat, setShowChat] = useState(true);
    const {globalStat} = useContext(globalContext);
    let style = {
        maxHeight: '100vh',
        overflow:'hidden',
        borderRight: '1px solid grey',
    }
    if (globalStat.dark_theme_enabled) style = {...style, ...dark_theme_style_container}

    return (
        <Flex vertical style={style}>
                    <UserBar avatarUrl={i} />
                     <ChatList />
                </Flex>
    );
                    // <ChatSideBarContentSelector onChange={(value) => value == 'chat' ? setShowChat(true) : setShowChat(false)}/>
                    // { showChat ? <ChatList /> : <ContactList />}
}
