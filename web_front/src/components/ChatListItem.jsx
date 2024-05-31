import { Flex, Avatar, Badge } from "antd"
import { PushpinTwoTone } from "@ant-design/icons";
import Seenstamp from "./Seenstamp";
import Timestamp from "./Timestamp";
import globalContext from "../context";

import './ChatListItem.css'
import { avatar_size } from "../utils";
import { useContext } from "react";
export default function ChatListItem({avatarUrl, chatName, lastMessage, onClick, isActive, unreadMessages}) {
    const {globalStat} = useContext(globalContext);
    const className = globalStat.dark_theme_enabled ? "chatListItemDark" : "chatListItem" ;
    const activeClassName = globalStat.dark_theme_enabled ? "chatListItem-selected-dark" : "chatListItem-selected" ; 
    return (
        <Flex onClick={onClick} align="center" justify="leftr" style={{padding:'0.05rem 0 0.05rem 0', cursor:'pointer', borderBottom:'1px solid grey'}} 
            className={`${className} ${isActive ? activeClassName: ""}`}
        >
            <div style={{margin:'1rem'}}>
                <Badge dot offset={['-12%','89%' ]} status='success'>
                    <Avatar src={avatarUrl} size={avatar_size} />
                </Badge>
            </div>
            <Flex vertical justify='center' style={{width: '100%'}}>
                <Flex>
                    <p style={{fontWeight:'600', fontSize:'large'}}>{chatName}</p>
                    <div style={{marginLeft:'auto'}}>
                        <Timestamp noMargin={false} time={lastMessage.timestamp}/>
                    </div>
                </Flex>
                <Flex justify="center" >
                    { false && <Seenstamp isSeen={true}/>  }
                    <ChatItemLastMessage text={lastMessage.text} />
                    <div style={{marginLeft:'auto'}}>
                        <ChatListItemProperties unreadMessages={unreadMessages}/>
                    </div>
                </Flex>
            </Flex>
        </Flex>
    );
}

// TODO: implement these
function ChatListItemProperties({unreadMessages}) {

    const marginStyle = { margin: '1%'};
    return <Flex style={{marginRight:'0.5rem'}}>
        <Badge count={unreadMessages} style={{...marginStyle, backgroundColor:'#0a6ef2'}} />
        </Flex>
}

function ChatItemLastMessage({text}) {
    return <p style={{
        color: 'grey', fontSize:'small'
    }}>{text}</p>
}
// TODO
