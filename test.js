const { UDP, Address } = require('./');

UDP.initialize();

const socket = UDP.create(1024, 1024);

const address = Address.createFromIpPort('127.0.0.1', 8080);

UDP.send(socket, address, Buffer.from('Hello World'));

const { bytesReceived, buffer } = UDP.receive(socket, 1024);
console.log('Received:', buffer.toString('utf-8', 0, bytesReceived));

UDP.destroy(socket);

UDP.deinitialize();
