import RecipientBar from "./RecipientBar";
import MessageStream from "./MessageStream";
import { Flex } from "antd";
import MessageInputBar from "./MessageInputBar";
import i from "../assets/user.png"
export default function MessagingWindow(props) {
    return (
        <Flex vertical style={{maxHeight: '100vh', height: '100vh', overflow:'hidden'}}>
            <RecipientBar avatarUrl={i} name={'jackie'} {...props}/>
            <MessageStream />
            <MessageInputBar />
        </Flex>
    );
}
