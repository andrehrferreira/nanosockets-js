const nanosockets = require('./build/Release/nanosockets_binding.node');

class Address {
  constructor(ip, port) {
    this.ip = ip;
    this.ip = port;
  }

  static createFromIpPort(ip, port) {
    const address = new Address();
    address.ip = ip;
    address.port = port;
    return address;
  }

  equals(other) {
    return nanosockets.isEqual(this.address, other.address);
  }

  getHashCode() {
    return nanosockets.getHashCode(this.address);
  }

  toString() {
    const ip = nanosockets.getIP(this.address);
    return `IP:${ip} Port:${this.address.port}`;
  }
}

class UDP {
  static initialize() {
    return nanosockets.initialize();
  }

  static deinitialize() {
    nanosockets.deinitialize();
  }

  static create(sendBufferSize, receiveBufferSize) {
    return nanosockets.create(sendBufferSize, receiveBufferSize);
  }

  static destroy(socket) {
    nanosockets.destroy(socket.handle);
  }

  static bind(socket, address) {
    return nanosockets.bind(socket.handle, address.ip, address.port);
  }

  static connect(socket, address) {
    return nanosockets.connect(socket.handle, address.ip, address.port);
  }

  static setOption(socket, level, optionName, optionValue) {
    return nanosockets.setOption(socket.handle, level, optionName, optionValue);
  }

  static getOption(socket, level, optionName) {
    return nanosockets.getOption(socket.handle, level, optionName);
  }

  static setNonBlocking(socket, shouldBlock = false) {
    return nanosockets.setNonBlocking(socket.handle, shouldBlock);
  }

  static setDontFragment(socket) {
    return nanosockets.setDontFragment(socket.handle);
  }

  static poll(socket, timeout) {
    return nanosockets.poll(socket.handle, timeout);
  }

  static send(socket, address, buffer) {
    return nanosockets.send(socket.handle, address.ip, address.port, buffer);
  }

  static receive(socket, bufferSize) {
    const address = new Address();
    const buffer = Buffer.alloc(bufferSize);
    const bytesReceived = nanosockets.receive(socket.handle, bufferSize);
    return { bytesReceived, address, buffer };
  }

  static getAddress(socket) {
    return nanosockets.getAddress(socket);
  }

  static setIP(address, ip) {
    return nanosockets.setIP(address, ip);
  }
}

module.exports = {
  UDP,
  Address,
};