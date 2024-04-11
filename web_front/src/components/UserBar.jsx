import {Avatar, Badge, Button, Flex, Input, List, Popover} from 'antd';
import {SearchOutlined, BellOutlined , EllipsisOutlined, CloseOutlined } from '@ant-design/icons'
import TopBar from './TopBar';
import { useState } from 'react';

const iconStyle = {
    margin: '0.3rem'
};

export default function UserBar({avatarUrl}) {
    const [searchInputVisible, setSearchInputVisible] = useState(false);
    return (
        <TopBar border={false} left={

            <Avatar size={{ xs: 30, sm: 35, md: 40, lg: 45, xl: 50, xxl: 100 }} src={avatarUrl} />
        }
        right={
                <Flex align='center'>
                        {!searchInputVisible ? <Button  icon={<SearchOutlined/>} onClick={()=>setSearchInputVisible(true)}/>  : 
                        <Input prefix={<SearchOutlined/>} style={{maxWidth:'200px'}} width={"60px"} addonAfter={<Button size='small' type='text' icon={<CloseOutlined/>} onClick={()=>setSearchInputVisible(false)}/>}/>}

                    <Popover content={<NotificationList/>} placement='bottom'>
                        <Badge offset={['-7%', '15%']} count='4'>
                            <Button style={iconStyle} icon={<BellOutlined />}/>
                        </Badge>
                    </Popover>

                    <Button style={iconStyle} icon={<EllipsisOutlined />}/>
                </Flex>
            } />
    );
}

function NotificationList() {
    return (
        <List>
        </List>
    );
}
