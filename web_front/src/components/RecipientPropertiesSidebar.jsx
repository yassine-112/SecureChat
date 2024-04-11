import { Badge, Button, Card, Flex, Image, List } from "antd";
import RecipientCardInfo from "./RecipientCardInfo";
import RecipientIdentification from "./RecipientIdentification";
import RecipientIdentificationItem from "./RecipientIdentificationItem";
import ChatMedia from "./ChatMedia";
import ChatDocuments from "./ChatDocuments";

const imageStyle = {
    flex: '1 1 20%'
}
export default function RecipientPropertiesSidebar() {
    return (
        <Flex vertical>
            <RecipientCardInfo />
            <RecipientIdentification>
                <RecipientIdentificationItem type='ip_address' value='10.123.1.3' />
                <RecipientIdentificationItem type='email' value='me@me.com' />
                <RecipientIdentificationItem type='website' value='myniceblog.com' />
            </RecipientIdentification>
            
            <ChatMedia />
            <ChatDocuments />


        </Flex>
    );
}
