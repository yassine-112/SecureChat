import { Flex } from "antd";
import backgroundPath from '../assets/chatBackground.jpg'
import MessageTimeDelimiter from "./MessageTimeDelimiter";
import Message from "./Message";
import { useContext } from "react";
import globalContext from "../context";

export default function MessageStream() {
    const {globalStat, dispatch} = useContext(globalContext)
    return (
        <Flex  vertical style={{flex:'1 1 auto', height: '100%', overflow:'scroll', backgroundAttachment:'fixed',backgroundImage: `url('${backgroundPath}')`, backgroundSize: 'cover', backgroundPositionX: 'center', backgroundPositionY: 'bottom', backgroundRepeat:'no-repeat', padding:'1%'}} >

            {
                (globalStat.currentFocusedFriend >= 0 && globalStat.message_log[globalStat.currentFocusedFriend].length > 0 ) ? globalStat.message_log[globalStat.currentFocusedFriend].map((e,i) => <Message isSentMessage={e.is_sent} messageBody={e.message_body} key={i}/>) : 
            <MessageTimeDelimiter time={"No message"} />
            }
        </Flex>
    );
            // <MessageTimeDelimiter time={"yesterday"} />
            // <Message isSentMessage={true}/>
}
