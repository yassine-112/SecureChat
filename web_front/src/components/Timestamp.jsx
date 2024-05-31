import { formatter } from "../utils"
export default function Timestamp({prefix='', time, noMargin=true}) {
     const f_time = formatter(time)
    return <p style={{display:'block', color: 'grey', fontSize:'x-small', margin: !noMargin ? '0.5rem' : '0'}}>{f_time}</p>
}
