const { UDP, Address } = require('./');

UDP.initialize();

const bufferSize = 512 * 1024;
const server = UDP.create(bufferSize, bufferSize);

const address = Address.createFromIpPort('::0', 5001);

if (UDP.bind(server, address) == 0)
    console.log("Socket bound!");

if (UDP.setDontFragment(server) != 0)
    console.log("Don't fragment option error!");

if (UDP.setNonBlocking(server) != 0)
    console.log("Non-blocking option error!");

while (true) {
    const pollResult = UDP.poll(server, 100);
    
    if (pollResult > 0) {
        let { bytesReceived } = UDP.receive(server, bufferSize);
        console.log(`Message received from - IP: ${bytesReceived.address}:${bytesReceived.port}, Data: ${bytesReceived.data.toString()}`);
        UDP.send(server, Address.createFromIpPort(bytesReceived.address, bytesReceived.port), bytesReceived.data);
    }
}

UDP.destroy(server);
UDP.deinitialize();