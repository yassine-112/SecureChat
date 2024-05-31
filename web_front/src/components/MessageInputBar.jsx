import { Button, Flex } from "antd";
import { SendOutlined } from "@ant-design/icons";
import MessageInputEmoji from "./MessageInputEmoji";
import MessageInputText from "./MessageInputText";
import globalContext from "../context";
import { useContext, useState } from "react";
import { dark_theme_style_container } from "../utils";

export default function MessageInputBar() {
    const {globalStat, messageSentBtnHandler } = useContext(globalContext)
    const [inputText, setInputText] = useState("")
    let style = {
        flex:'0 1 8%', minWidth:'100%', padding:'0.5%' 
    }
    if (globalStat.dark_theme_enabled) style = {...style, ...dark_theme_style_container}
    return (
        <Flex style={style} justify="space-evenly" align="center">
            <MessageInputEmoji /> 
            <MessageInputText  text={inputText} setText={setInputText}/>

            <Button icon={<SendOutlined />} type="primary"  shape="circle" size="large" style={{margin:'0 1% 0 0'}} onClick={() => {messageSentBtnHandler(inputText); setInputText("")}}/>

    </Flex>
    );
}
