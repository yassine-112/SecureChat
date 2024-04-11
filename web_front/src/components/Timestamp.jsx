
export default function Timestamp({prefix='', time, noMargin=true}) {
    return <p style={{display:'block', color: 'grey', fontSize:'x-small', margin: !noMargin ? '0.5rem' : '0'}}>{prefix + time}</p>
}
