import { Flex, Avatar, Badge } from "antd"
import './ChatListItem.css'
import { avatar_size } from "../utils";
export default function ContactListItem({avatarUrl, chatName}) {
    return (
        <Flex align="center" justify="leftr" style={{padding:'0.05rem 0 0.05rem 0', cursor:'pointer', borderBottom:'1px solid grey'}} className="chatListItem">
            <div style={{margin:'1rem'}}>
                <Badge dot offset={['-12%','89%' ]} status='success'>
                    <Avatar src={avatarUrl} size={avatar_size} />
                </Badge>
            </div>
                    <p style={{fontWeight:'600', fontSize:'large'}}>{chatName}</p>
            </Flex>
    );
}

