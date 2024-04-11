import { Card, Badge, Flex, Image } from "antd";
export default function ChatMedia() {
    return (
            <Card title='Photos & Media' extra={<Badge count={30} color="blue"/>} style={{marginTop:'2%'}}>
                <Flex wrap="wrap" gap='2px'>
                    <Image.PreviewGroup>
                        <Image width='30%' src={'https://source.unsplash.com/random'}/>
                        <Image width='30%' src={'https://source.unsplash.com/random'}/>
                        <Image width='30%' src={'https://source.unsplash.com/random'}/>
                        <Image width='30%' src={'https://source.unsplash.com/random'}/>
                        <Image width='30%' src={'https://source.unsplash.com/random'}/>
                        <Image width='30%' src={'https://source.unsplash.com/random'}/>
                        <Image width='30%' src={'https://source.unsplash.com/random'}/>
                    </Image.PreviewGroup>
                </Flex>
            </Card>
    );
}
