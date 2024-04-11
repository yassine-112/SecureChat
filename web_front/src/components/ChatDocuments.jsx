import { FilePdfOutlined } from '@ant-design/icons';
import {Card, Flex, List, Badge} from 'antd';
export default function ChatDocuments() {
    return (
        <Card title='Documents & Files' extra={<Badge count={30} color="blue"/>} style={{marginTop:'2%'}}>
            <Flex wrap="wrap" gap='2px'>
                <List split={true} style={{width:'100%'}}>
                    <ChatDocumentItem />
                    <ChatDocumentItem />
                    <ChatDocumentItem />
                    <ChatDocumentItem />
                    <ChatDocumentItem />
                </List>
            </Flex>
        </Card>
    );
}

function ChatDocumentItem() {
    return (
        <Flex style={{minWidth:'100%', cursor:'pointer'}} align='center'>
            <div style={{margin:'3%', fontSize:'1.3rem'}}>
                <FilePdfOutlined /> 
            </div>
            <div>
                <p>Filename</p>
                <p>11KB 11:11 PM</p>
            </div>
        </Flex>
    )
}
