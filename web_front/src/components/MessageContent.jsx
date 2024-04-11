
export default function MessageContent({isSentMessage,children}) {
    let messageStyle =  { 
        "backgroundColor": "#3997fe",
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
