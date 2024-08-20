# nanosockets-js

`nanosockets-js` is a lightweight and efficient library for managing low-level socket connections in Node.js, based on the powerful [NanoSockets](https://github.com/nxrighthere/NanoSockets) library. This project uses `napi-rs` to expose a simple, fast, and efficient network interface for JavaScript developers, allowing communication via UDP sockets.

## Manual Build

To manual build the library:

```bash
rm -rf build
node-gyp clean
node-gyp configure
node-gyp build
```

## Installation

To install the library via npm, run:

```bash
npm install nanosockets-js
```

## Features

- Based on the native low-level [NanoSockets](https://github.com/nxrighthere/NanoSockets) library.
- High-performance UDP socket support.
- Native integration with Node.js using Rust and `napi-rs`.
- Simple APIs for creating, sending, and receiving messages via UDP.
- Support for IPv4 and IPv6 and dynamic ports.

## Usage Example

Here is a basic example of how to use the UDP API provided by the library:

```javascript
const { UDP, Address } = require('nanosockets-js');

UDP.initialize();

const server = UDP.create(256 * 1024, 256 * 1024);

const address = Address.createFromIpPort('::0', 5001);

if (UDP.bind(server, address) == 0)
    console.log("Socket bound!");

if (UDP.setDontFragment(server) != 0)
    console.log("Don't fragment option error!");

if (UDP.setNonBlocking(server) != 0)
    console.log("Non-blocking option error!");

while (true) {
    const pollResult = UDP.poll(server, 15);
    
    if (pollResult > 0) {
        let { bytesReceived } = UDP.receive(server, 256 * 1024);

        console.log(`Message received from - IP: ${bytesReceived.address}:${bytesReceived.port}, Data: ${bytesReceived.data.toString()}`);
        UDP.send(server, Address.createFromIpPort(bytesReceived.address, bytesReceived.port), bytesReceived.data);
    }
}

UDP.destroy(server);
UDP.deinitialize();
```

## API

### `UDP.initialize()`

Initializes the UDP subsystem. Must be called before any other UDP operations.

### `UDP.create(sendBufferSize, receiveBufferSize)`

Creates a UDP socket with the specified buffer sizes.

- `sendBufferSize` (Number): The size of the send buffer.
- `receiveBufferSize` (Number): The size of the receive buffer.

### `Address.createFromIpPort(ip, port)`

Creates a UDP address from an IP and port.

- `ip` (String): The IPv4 address.
- `port` (Number): The port associated with the address.

### `UDP.send(socket, address, buffer)`

Sends data to a specific address using the UDP socket.

- `socket` (Object): The created UDP socket.
- `address` (Object): The destination address (an instance of `Address`).
- `buffer` (Buffer): The data to be sent.

### `UDP.receive(socket, bufferSize)`

Receives data from a UDP socket.

- `socket` (Object): The created UDP socket.
- `bufferSize` (Number): The maximum buffer size for receiving data.

Returns an object with the following properties:
- `bytesReceived`: The number of bytes received.
- `buffer`: The buffer containing the received data.

### `UDP.destroy(socket)`

Destroys the UDP socket and frees associated resources.

### `UDP.deinitialize()`

Finalizes the UDP subsystem. Should be called after all networking operations to free resources.

## Contribution

Contributions are welcome! If you encounter issues or have suggestions for improvements, feel free to open an issue or submit a pull request. This project is based on the [NanoSockets](https://github.com/nxrighthere/NanoSockets) library, and any enhancements are greatly appreciated.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
