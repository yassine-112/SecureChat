import { Input } from "antd";
import { useContext, useState } from "react";
import  globalContext  from '../context.js'

export default function MessageInputText({text, setText}) {
    const {globalStat, dispatch} = useContext(globalContext)
    return (
        <Input style={{flex:'0 1 80%', borderRadius:'10px', height: '90%'}} value={text} onChange={
            (e)=> {setText(e.target.value)}
        } placeholder="Hey... "/>
    );
}
