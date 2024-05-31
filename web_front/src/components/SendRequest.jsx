import { PlusOutlined } from "@ant-design/icons";
import { FloatButton, Input, Modal } from "antd";
import TextArea from "antd/es/input/TextArea";
import { useState } from "react";

const SendRequest = ({handler}) => {
    const [pubKey, setPubKey] = useState("");
    const [message, setMessage] = useState("");
    const [showAddFriend, setShowAddFriend] = useState(false);
    return (
        <>
            <FloatButton onClick={()=>setShowAddFriend(true)} style={{position: 'absolute', bottom:'20px', left:"20px"}} icon={<PlusOutlined />} tooltip={"Add a new friend"}/>
            <Modal title="Send a friend request" open={showAddFriend} okText={"Send"} onOk={() => {handler(pubKey, message); setShowAddFriend(false)}} onCancel={() => setShowAddFriend(false)}>
                <label for='friend_pub_key'>TOX id:  </label>
                <Input type="text" id='friend_pub_key' name='friend_pub_key' value={pubKey} onChange={(e) => setPubKey(e.target.value)} required/>

                <label for='req_message'>Message</label>
                <TextArea type="text" id='req_message' name="req_message" value={message} onChange={(e) => setMessage(e.target.value)} required/>

            </Modal>
        </>
    );

}

export default SendRequest;
