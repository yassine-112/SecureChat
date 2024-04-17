import { EditOutlined, EllipsisOutlined, SettingOutlined } from '@ant-design/icons';
import i from '../assets/user.png'
import { Avatar, Card, Flex } from "antd";

export default function RecipientCardInfo() {
    return (
        <Card style={{width:'100%'}}  actions={[
      <SettingOutlined key="setting" />
    ]}>
            <Flex vertical align='center' justify='space-between'>
                <Avatar size={{ xs: 40, sm: 45, md: 50, lg: 55, xl: 120, xxl: 100 }} src={i}/>
                <h2 style={{marginTop:'3%'}}>Jackie</h2>
            </Flex>
        </Card>
    );
}
