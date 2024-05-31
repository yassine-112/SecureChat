import { Flex, Alert, Avatar, Input, Select, Button } from "antd";
import globalContext from "../context";
import { useContext, useState } from "react";
import backgroundPath from '../assets/chatBackground.jpg'
import darkBackgroundPath from '../assets/chatBackgroundDark.jpg'
import { EditTwoTone, LogoutOutlined } from "@ant-design/icons";

const UserInfoWindow = () => {
    const [moreBtnShown, setMoreBtnShwon] = useState(false);
    const {globalStat, app_exit} = useContext(globalContext);
    const flex_style = {
        minHeight: '100vh',
        backgroundAttachment:'fixed',
        backgroundImage: globalStat.dark_theme_enabled ?  `url('${darkBackgroundPath}')`: `url('${backgroundPath}')`,
        backgroundSize: 'cover', 
        backgroundPositionX: 'center',
        backgroundPositionY: 'bottom',
        backgroundRepeat:'no-repeat',
        overflow: 'none'
    }
    return (
        <Flex gap="middle" vertical  align="center" style={flex_style}>
            <div style={{marginTop:"4rem"}}></div>
                <ToxConnectionStatus /> 
                <UserInfoEdit/>
            <Button type='default' value="Logout" icon={<LogoutOutlined />} onClick={() => app_exit()} danger>Logout</Button>
            <Button type='default' value="Logout" onClick={() => setMoreBtnShwon(!moreBtnShown)}>{moreBtnShown ? "Less options": "More options"}</Button>
            {
                moreBtnShown &&
                    <div>
                        <Button style={{margin:'0.2rem'}} type='default' value="Logout" icon={<LogoutOutlined />} danger>Remove all chat log</Button>
                        <Button style={{margin:'0.2rem'}} type='default' value="Logout" icon={<LogoutOutlined />} danger>Reset Tox node</Button>
                    </div>
            }
        </Flex>
    );
}

const ToxConnectionStatus = () => {
    const {globalStat} = useContext(globalContext);
    const status_to_mtypes = {
        "online": "success", "connecting": "info", "error": "error"
    }
    return (
        <div style={{"width": "50%"}}>
            <Alert style={{margin: '0.2rem 0'}} message={"Tox status: " + globalStat.tox_status} type={status_to_mtypes[globalStat.tox_status]} showIcon />
            <Alert message={"TOX ID: " + globalStat.user_tox_id} type="info" showIcon/>
        </div>
    )
}

const UserInfoEdit  = () => {
    const [editableInputs, setEditableInputs] = useState(false);
    const {globalStat, dispatch} = useContext(globalContext);
    const [name, setName] = useState(globalStat.user_name);
    const [status, setStatus] = useState(globalStat.user_status);
    return (
        <>
        <Flex gap="middle" justify="center" align="center" >
            <Avatar src={globalStat.user_avatar_url} size={{ xs: 30, sm: 35, md: 40, lg: 45, xl: 50, xxl: 100 }}/>
                <div>
                    <label>Name: </label> 
                    <Input value={name} onChange={(e) => setName(e.target.value)} disabled={!editableInputs} placeholder="No name is set" />
                    <label>Status: </label> 
                    <Input.TextArea value={status} onChange={(e) => setStatus(e.target.value)} disabled={!editableInputs} rows={2} placeholder="No status set" />
                </div>
                <div>
                    <label>Status: </label>
                    <Select
                        defaultValue={globalStat.user_online_status}
                        style={{
                            width: 120,
                        }}
                        onChange={(e)=>dispatch({type: 'SET_USER_ONLINE_STATUS', status:e})}
                        options={[
                            {
                                value: 'online',
                                label: 'Online',
                            },
                            {
                                value: 'busy',
                                label: 'Buzy',
                            },
                            {
                                value: 'not_online',
                                label: 'Not online',
                            },
                        ]}
                    />
                </div>
        </Flex>
            <Button type='default' value="Logout" 
                onClick={() => {
                    if (editableInputs) {
                        dispatch({type: 'SET_USER_INFO', user_name: name, user_status: status})
                    }
                    setEditableInputs(!editableInputs)}
                } icon={<EditTwoTone />} 
            >{editableInputs ? "Done" : "Edit"}</Button>
        </>
    );
}
export default UserInfoWindow;
