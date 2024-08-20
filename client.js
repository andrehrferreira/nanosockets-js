const { UDP, Address } = require('./');

const client = UDP.create(256 * 1024, 256 * 1024);
const connectionAddress = Address.createFromIpPort('::0', 5001);

if (UDP.connect(client, connectionAddress) == 0) {
    console.log("Socket connected!");
} else {
    console.log("Connection error!");
}

if (UDP.setDontFragment(client) != 0) {
    console.log("Don't fragment option error!");
}

if (UDP.setNonBlocking(client) != 0) {
    console.log("Non-blocking option error!");
}

const buffer = Buffer.alloc(1024);
UDP.send(client, connectionAddress, buffer);

while (true) {
    const pollResult = UDP.poll(client, 15); 

    if (pollResult > 0) {
        let dataLength = 0;

        while ((dataLength = UDP.receive(client, buffer)) > 0) {
            console.log(`Message received from server - Data length: ${dataLength}`);
            UDP.send(client, connectionAddress, buffer.slice(0, dataLength));
        }
    }
}

UDP.destroy(client);
UDP.deinitialize();
