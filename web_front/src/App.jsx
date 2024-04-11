import './App.css';
import { Row, Col, Drawer  } from 'antd';
import ChatSidebar from './components/ChatSidebar';
import MessagingWindow from './components/MessagingWindow';
import { useState } from 'react';
import RecipientPropertiesSidebar from './components/RecipientPropertiesSidebar';

function App() {
    const [showRecipientPropertiesSidebar, setRecipientPropertiesSidebar] = useState(false);

  return (
    <>
        <Row style={{maxHeight: '100vh'}}>
            <Col span={6}>
                <ChatSidebar />
            </Col>
            <Col span={18}>
                <MessagingWindow showSideBarToggle={ () => setRecipientPropertiesSidebar(!showRecipientPropertiesSidebar) }/>
            </Col>
                <Drawer title="Contact Info" onClose={() => setRecipientPropertiesSidebar(false)} open={showRecipientPropertiesSidebar}>
                        <RecipientPropertiesSidebar />
                </Drawer>
        </Row>
    </>
  )
}


export default App
