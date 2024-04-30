import RecipientBar from "./RecipientBar";
import MessageStream from "./MessageStream";
import { Flex } from "antd";
import MessageInputBar from "./MessageInputBar";
import i from "../assets/user.png"
import globalContext from "../context";
import { useContext } from "react";
export default function MessagingWindow(props) {
    const {globalStat} = useContext(globalContext);
    console.log(globalStat.friend_list)
    return (
        <Flex vertical style={{maxHeight: '100vh', height: '100vh', overflow:'hidden'}}>
            <RecipientBar avatarUrl={i} name={globalStat.currentFocusedFriend >= 0 ? globalStat.friend_list[globalStat.currentFocusedFriend].name : "----"} {...props}/>
            <MessageStream />
            <MessageInputBar />
        </Flex>
    );
}
