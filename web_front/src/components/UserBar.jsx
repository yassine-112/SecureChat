import {Avatar, Badge, Button, Flex, Input, List, Popover, Card, Dropdown, Switch} from 'antd';
import {SearchOutlined, BellOutlined , EllipsisOutlined, CloseOutlined, SmileOutlined } from '@ant-design/icons'
import TopBar from './TopBar';
import { useState, useContext, useReducer } from 'react';
import globalContext from '../context';
import { friend_request_response } from '../utils';

const iconStyle = {
    margin: '0.3rem'
};

export default function UserBar({avatarUrl}) {
    const [searchInputVisible, setSearchInputVisible] = useState(false);
    const {globalStat, dispatch} = useContext(globalContext)
    return (
        <TopBar border={false} left={

            <Avatar onClick={() => dispatch({type: 'SET_FRIEND_FOCUSED', number: -1})} style={{cursor: 'pointer'}}size={{ xs: 30, sm: 35, md: 40, lg: 45, xl: 50, xxl: 100 }} src={avatarUrl} />
        
        }
        right={
                <Flex align='center'>
                        {!searchInputVisible ? <Button  icon={<SearchOutlined/>} onClick={()=>setSearchInputVisible(true)}/>  : 
                        <Input prefix={<SearchOutlined/>} style={{maxWidth:'200px'}} width={"60px"} addonAfter={<Button size='small' type='text' icon={<CloseOutlined/>} onClick={()=>setSearchInputVisible(false)}/>}/>}

                    <Popover content={<NotificationList/>} placement='bottom'>
                        <Badge offset={['-7%', '15%']} count={globalStat.notifications.length}>
                            <Button style={iconStyle} icon={<BellOutlined />}/>
                        </Badge>
                    </Popover>
                    <OptionsList />

                </Flex>
            } />
    );
}

function OptionsList() {
    const [open, setOpen] = useState(false);
    const {globalStat, dispatch} = useContext(globalContext)

    const handleDarkModeChange = (status) => {
        dispatch({type:"SET_DARK_THEME", value:status});
    }
    const items = [
        {
            key: '1',
            label: (
                <>
                    <span style={{display:"inline-block", margin:"0 0.3rem 0 0.3rem"}}>
                    Dark Mode
                </span>
                    <Switch size={"small"} onChange={handleDarkModeChange} checked={globalStat.dark_theme_enabled}/>
                </>
            ),
        },
        {
            key: '2',
            label: (
                <a target="_blank" rel="noopener noreferrer" href="https://www.antgroup.com">
                    Language
                </a>
            ),
            children: [
                {key: '2-1', label: 'French'},
                {key: '2-2', label: 'English'},
            ]
        },
    ];
    return (
        <>
            <Dropdown menu={{items, }} open={open} onOpenChange={(nextOpen, info) => {if (info.source === 'trigger' || nextOpen) setOpen(nextOpen)}}>
                <Button style={iconStyle} icon={<EllipsisOutlined />}/>
            </Dropdown>
        </>
    );
}

function NotificationList() {
    const {globalStat, dispatch} = useContext(globalContext)

    const notification_handlers = {
        'accept_req': (item) => {
            friend_request_response(item.data.pub_key);
            dispatch({type:'DEL_NOTIFICATION', id:item.id});

        },
        'deny_req': (item) => {
            dispatch({type:'DEL_NOTIFICATION', id:item.id})
        }
    }

    const get_avatar = notification => {
        switch(notification.class) {
            case "friend": 
                return <Avatar src={notification.icon_path}/>;
            default:
                return undefined
        }
    }
    // TODO: Dont hardcode actions for req accept
    return (
        <List
            itemLayout="horizontal"
            dataSource={globalStat.notifications}
            renderItem={(item) => (
                <List.Item
                    actions={
                        item.actions
                        .map( i => <a onClick={() => notification_handlers[i.action_handler](item)}>{i.action_name}</a>)
                        .concat(item.can_delete ? [<a><CloseOutlined /></a>] : [])
                    }
                >
                    <Card
                        type="inner"
                        bordered={false}
                    >
                        <Card.Meta
                            avatar={get_avatar(item)}
                            title={item.title}
                            description={item.body}
                        />
                    </Card>
                </List.Item>
            )}
        />    );
}
