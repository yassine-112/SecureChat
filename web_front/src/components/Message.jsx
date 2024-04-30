import MessageContent from "./MessageContent";
import MessageText from "./MessageText";
import Timestamp from "./Timestamp";
import Seenstamp from "./Seenstamp";
import { Flex } from "antd";

import './Message.css';

export default function Message(props) {
    return (
        <Flex className={props.isSentMessage === true ? 'right' : '' } vertical style={{
        "minWidth": "35%",
        "maxWidth": "35%",
        }}>
            <MessageContent {...props}>
                <MessageText text={props.messageBody} />
            </MessageContent>
            <Flex  justify={props.isSentMessage === true ? 'right' : 'left'} style={{margin:'1% 0 1% 0'}}>
                <Timestamp  time={'11:11 PM'}/>
                { props.isSentMessage === true && <div style={{marginLeft:'1%'}}><Seenstamp isSeen={true} /></div>}
            </Flex>
        </Flex>
    );
}
