import { Flex } from "antd";
import backgroundPath from '../assets/chatBackground.jpg'
import darkBackgroundPath from '../assets/chatBackgroundDark.jpg'
import MessageTimeDelimiter from "./MessageTimeDelimiter";
import Message from "./Message";
import { useContext, useEffect, useRef } from "react";
import globalContext from "../context";

export default function MessageStream() {
    const {globalStat, dispatch} = useContext(globalContext)
    const messagesEndRef = useRef(null)

    const scrollToBottom = () => {
        messagesEndRef.current?.scrollIntoView()
    }

    useEffect(() => {
        scrollToBottom()
    }, [globalStat.message_log]);
    return (
        <Flex  vertical style={{flex:'1 1 auto',
            height: '100%',
            overflow:'scroll',
            backgroundAttachment:'fixed',
            backgroundImage: globalStat.dark_theme_enabled ?  `url('${darkBackgroundPath}')`: `url('${backgroundPath}')`,
            backgroundSize: 'cover',
            backgroundPositionX: 'center',
            backgroundPositionY: 'bottom',
            backgroundRepeat:'no-repeat', 
            padding:'1%'}} >

            {
                (globalStat.currentFocusedFriend >= 0 && globalStat.message_log[globalStat.currentFocusedFriend].length > 0 ) ? 
                    globalStat.message_log[globalStat.currentFocusedFriend].map((e,i) => <Message isSentMessage={e.is_sent} messageBody={e.message_body} key={i} timestamp={e.timestamp}/>) : 
            <MessageTimeDelimiter time={"No message"} />
            }

            <div ref={messagesEndRef} />
        </Flex>
    );
            // <MessageTimeDelimiter time={"yesterday"} />
            // <Message isSentMessage={true}/>
}
