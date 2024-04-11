import { Flex, Avatar, Badge } from "antd"
import './ChatListItem.css'
export default function ContactListItem({avatarUrl, chatName}) {
    return (
        <Flex align="center" justify="leftr" style={{padding:'0.05rem 0 0.05rem 0', cursor:'pointer', borderBottom:'1px solid grey'}} className="chatListItem">
            <div style={{margin:'1rem'}}>
                <Badge dot offset={['-12%','89%' ]} status='success'>
                    <Avatar src={avatarUrl} size={{ xs: 20, sm: 20, md: 20, lg: 20, xl: 60, xxl: 100 }} />
                </Badge>
            </div>
                    <p style={{fontWeight:'600', fontSize:'large'}}>{chatName}</p>
            </Flex>
    );
}

