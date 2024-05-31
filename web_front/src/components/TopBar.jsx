import { Flex } from "antd";
import globalContext from "../context";
import { useContext } from "react";
import { dark_theme_style_container } from "../utils";
export default function TopBar({border, left, right}) {
    const {globalStat} = useContext(globalContext);
    let style = {
        padding: '1rem',
        borderBottom: border ? '1px solid grey': 'none'
    }
    if (globalStat.dark_theme_enabled) style = {...style, ...dark_theme_style_container}
    return (
        <Flex style={style} justify="center" align="center">
            {left}
            <div style={{margin: '0.3rem 0.3rem 0.3rem auto'}}>
                {right}
            </div>
        </Flex>
    );
}
