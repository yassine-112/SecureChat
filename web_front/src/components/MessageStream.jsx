import { Flex } from "antd";
import backgroundPath from '../assets/chatBackground.jpg'
import MessageTimeDelimiter from "./MessageTimeDelimiter";
import Message from "./Message";

export default function MessageStream() {
    return (
        <Flex  vertical style={{flex:'1 1 auto', height: '100%', overflow:'scroll', backgroundAttachment:'fixed',backgroundImage: `url('${backgroundPath}')`, backgroundSize: 'cover', backgroundPositionX: 'center', backgroundPositionY: 'bottom', backgroundRepeat:'no-repeat', padding:'1%'}} >

            <MessageTimeDelimiter time={"yesterday"} />
            <Message isSentMessage={true}/>
            <Message isSentMessage={false}/>
            <Message isSentMessage={false}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <Message isSentMessage={true}/>
            <MessageTimeDelimiter time={"yesterday"} />
        </Flex>
    );
}
