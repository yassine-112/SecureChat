import { Flex, Avatar, Badge } from "antd"
import { PushpinTwoTone } from "@ant-design/icons";
import Seenstamp from "./Seenstamp";
import Timestamp from "./Timestamp";

import './ChatListItem.css'
export default function ChatListItem({avatarUrl, chatName, lastMessage}) {
    return (
        <Flex align="center" justify="leftr" style={{padding:'0.05rem 0 0.05rem 0', cursor:'pointer', borderBottom:'1px solid grey'}} className="chatListItem">
            <div style={{margin:'1rem'}}>
                <Badge dot offset={['-12%','89%' ]} status='success'>
                    <Avatar src={avatarUrl} size={{ xs: 30, sm: 35, md: 40, lg: 45, xl: 60, xxl: 100 }} />
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
                    <Seenstamp isSeen={true}/> <ChatItemLastMessage text={lastMessage.text} />
                    <div style={{marginLeft:'auto'}}>
                        <ChatListItemProperties />
                    </div>
                </Flex>
            </Flex>
        </Flex>
    );
}

// TODO: implement these
function ChatListItemProperties() {
    const marginStyle = { margin: '1%'};
    return <Flex style={{marginRight:'0.5rem'}}>
        <PushpinTwoTone style={marginStyle} />
        <Badge count={3} style={{...marginStyle, backgroundColor:'#0a6ef2'}} />
        </Flex>
}

function ChatItemLastMessage({text}) {
    return <p style={{
        color: 'grey', fontSize:'small'
    }}>{text}</p>
}
// TODO
