import { Flex } from "antd";
export default function TopBar({border, left, right}) {
    return (
        <Flex style={{padding: '1rem', borderBottom: border ? '1px solid grey': 'none'}} justify="center" align="center">
            {left}
            <div style={{margin: '0.3rem 0.3rem 0.3rem auto'}}>
                {right}
            </div>
        </Flex>
    );
}
