const { UDP, Address } = require('../');
const CLIENTS_TO_WAIT_FOR = 32;
const clients = new Map();
const port = 5001;

(async () => {
    UDP.initialize();

    const bufferSize = 512 * 1024;
    const server = UDP.create(bufferSize, bufferSize);
    const address = Address.createFromIpPort('::0', port);

    if (UDP.bind(server, address) == 0) {
        console.log("Socket bound!");
    }

    if (UDP.setDontFragment(server) != 0) {
        console.log("Don't fragment option error!");
    }

    if (UDP.setNonBlocking(server) != 0) {
        console.log("Non-blocking option error!");
    }

    console.log(`Worker ${process.pid} is running on ws://localhost:${port}`);
    console.log(`Waiting for ${CLIENTS_TO_WAIT_FOR} clients to connect..`);

    let clientCounter = 0;

    while (true) {
        const pollResult = UDP.poll(server, 100);
    
        if (pollResult > 0) {
            let { bytesReceived } = UDP.receive(server, bufferSize);
            const clientKey = `${bytesReceived.address}:${bytesReceived.port}`;
            
            if (!clients.has(clientKey)) {
                clientCounter++;
                const clientName = `Client${clientCounter}`;
                clients.set(clientKey, { name: clientName, address: bytesReceived.address, port: bytesReceived.port });
                console.log(`${clientName} connected (${CLIENTS_TO_WAIT_FOR - clientCounter} remain)`);

                if (clientCounter === CLIENTS_TO_WAIT_FOR) {
                    sendReadyMessage();
                }
            }

            const message = `${clients.get(clientKey).name}: ${bytesReceived.data.toString()}`;

            for (const [key, clientInfo] of clients.entries()) {
                if (key !== clientKey) {
                    UDP.send(server, Address.createFromIpPort(clientInfo.address, clientInfo.port), Buffer.from(message));
                }
            }
        }

        //await new Promise((resolve) => { setTimeout(resolve, 1000); });
    }
})();

function sendReadyMessage() {
    console.log("All clients connected");
    
    setTimeout(() => {
        console.log("Starting benchmark");
        for (const clientInfo of clients.values()) {
            UDP.send(server, Address.createFromIpPort(clientInfo.address, clientInfo.port), Buffer.from(`ready`));
        }
    }, 100);
}

// Finalização do servidor UDP ao término
process.on('SIGINT', () => {
    console.log("Shutting down server...");
    UDP.destroy(server);
    UDP.deinitialize();
    process.exit();
});
