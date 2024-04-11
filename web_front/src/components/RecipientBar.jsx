import { Avatar, Badge, Button } from "antd";
import TopBar from "./TopBar";
import { MoreOutlined } from "@ant-design/icons";
import Timestamp from "./Timestamp";

export default function RecipientBar({avatarUrl, name, showSideBarToggle}) {
    // TODO: FIX avtar size of this and chatListitem
    return (
        <TopBar  border={false} left={
            <>
            <Badge dot offset={['-8%','89%' ]} status='success'>
            <Avatar src={avatarUrl} size={{ xs: 30, sm: 35, md: 40, lg: 45, xl: 50, xxl: 100 }}/>
            </Badge>

                <div style={{ marginLeft: '1%'}}>
                    <p style={{fontWeight:'600', fontSize:'large', marginBottom:'6%'}}>{name}</p>
                    <Timestamp noMargin={true} time={"11:11 PM"} prefix={"Last seen at: "}/>
                </div>
            </>
        } 
        right={
                <Button icon={<MoreOutlined />} type='default' onClick={showSideBarToggle}/>
            } />
    );
}
