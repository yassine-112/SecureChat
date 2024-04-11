import { EditOutlined, EllipsisOutlined, SettingOutlined } from '@ant-design/icons';
import { Avatar, Card, Flex } from "antd";

export default function RecipientCardInfo() {
    return (
        <Card style={{width:'100%'}}  actions={[
      <SettingOutlined key="setting" />
    ]}>
            <Flex vertical align='center' justify='space-between'>
                <Avatar size={{ xs: 40, sm: 45, md: 50, lg: 55, xl: 120, xxl: 100 }} src="https://static.vecteezy.com/system/resources/thumbnails/005/129/844/small/profile-user-icon-isolated-on-white-background-eps10-free-vector.jpg" />
                <h2 style={{marginTop:'3%'}}>Jackie</h2>
            </Flex>
        </Card>
    );
}
