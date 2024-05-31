import { Avatar, Badge, Button } from "antd";
import TopBar from "./TopBar";
import { MoreOutlined } from "@ant-design/icons";
import Timestamp from "./Timestamp";
import { avatar_size } from "../utils";

export default function RecipientBar({avatarUrl, name, showSideBarToggle}) {
    // TODO: FIX avtar size of this and chatListitem

    return (
        <TopBar  border={false} left={
            <>
            <Badge dot offset={['-8%','89%' ]} status='success'>
            <Avatar src={avatarUrl} size={avatar_size}/>
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
