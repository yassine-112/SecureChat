import { HomeOutlined, LinkOutlined, MailOutlined } from "@ant-design/icons";
import { Card, Flex } from "antd";

export default function RecipientIdentificationItem({type, value}) {
    const typeIcon = {
        'ip_address': <LinkOutlined/>,
        'email': <MailOutlined />,
        'website': <HomeOutlined />
    }
    return (
        <Flex style={{width: '100%', padding: '1% 0', fontSize: '1.05rem',justifyContent: 'start'}}>
            {typeIcon[type]}
            <p style={{marginLeft:'5%', fontSize:'0.95rem'}}>{value}</p>
        </Flex>
    );
}
