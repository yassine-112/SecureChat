import { Segmented, Avatar, Flex, ConfigProvider } from "antd";
import { MessageOutlined, UserOutlined  } from "@ant-design/icons";

export default function ChatSideBarContentSelector({onChange}) {
    return (
        <ConfigProvider 
            theme={{
                token: {borderRadius: '0'},
                components: {
                    Segmented: {
                    },
                },
            }}
        >
            <Segmented block={true} style={{width:'100%', borderBottom: '1px solid grey', borderRadius: '0'}} onChange={onChange}
                options={[
                    {
                        label: (
                            <div style={{ padding: 4 }}>
                                <MessageOutlined />
                                <div>Chat</div>
                            </div>
                        ),
                            value: 'chat',
                    },
                    {
                        label: (
                            <div style={{ padding: 4 }}>
                                <UserOutlined />
                                <div>Contact</div>
                            </div>
                        ),
                        value: 'contact',
                    },
                ]}
            /></ConfigProvider>
    )
}
