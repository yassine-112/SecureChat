import globalContext from "../context";
import { useContext } from "react";
export default function MessageContent({isSentMessage,children}) {
    const {globalStat} = useContext(globalContext)
    let messageStyle =  { 
        "backgroundColor": globalStat.dark_theme_enabled ? "rgb(10, 110, 242)" : "#3997fe",
        "borderRadius": "27px",
        "padding": "1.2rem 0.3rem 1.2rem 1rem",
    };
    if (isSentMessage === true) messageStyle = {...messageStyle, "borderBottomRightRadius": "0"}
    else messageStyle = {...messageStyle, "borderBottomLeftRadius": "0"}

    return (
        <div style={messageStyle}>
            {children} 
        </div>
    );
}
