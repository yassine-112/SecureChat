import { Input } from "antd";
import { useContext, useState } from "react";
import  globalContext  from '../context.js'

export default function MessageInputText() {
    const {globalStat, dispatch} = useContext(globalContext)
    return (
        <Input style={{flex:'0 1 80%', borderRadius:'10px', height: '90%'}} value={globalStat.input_text} onChange={
            (e)=> {dispatch({type:"INPUT_TEXT", "input_text": e.target.value})}
        } placeholder="Hey... "/>
    );
}
