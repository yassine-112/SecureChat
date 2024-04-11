import { Divider } from "antd";
import Timestamp from "./Timestamp";

export default function MessageTimeDelimiter({time}) {
    return (
    <Divider plain>
        <Timestamp time={time}/>
    </Divider>
    );
}
