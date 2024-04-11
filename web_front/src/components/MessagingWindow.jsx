import RecipientBar from "./RecipientBar";
import MessageStream from "./MessageStream";
import { Flex } from "antd";
import MessageInputBar from "./MessageInputBar";
export default function MessagingWindow(props) {
    return (
        <Flex vertical style={{maxHeight: '100vh', height: '100vh', overflow:'hidden'}}>
            <RecipientBar avatarUrl="https://static.vecteezy.com/system/resources/thumbnails/005/129/844/small/profile-user-icon-isolated-on-white-background-eps10-free-vector.jpg" name={'jackie'} {...props}/>
            <MessageStream />
            <MessageInputBar />
        </Flex>
    );
}
