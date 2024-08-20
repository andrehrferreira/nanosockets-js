const nanosockets = require('./build/Release/nanosockets_binding.node');

class Address {
  constructor() {
    this.address = {};
  }

  static createFromIpPort(ip, port) {
    const address = new Address();
    nanosockets.setIP(address.address, ip);
    address.address.port = port;
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
    nanosockets.destroy(socket);
  }

  static bind(socket, address) {
    return nanosockets.bind(socket, address.address);
  }

  static connect(socket, address) {
    return nanosockets.connect(socket, address.address);
  }

  static setOption(socket, level, optionName, optionValue) {
    return nanosockets.setOption(socket, level, optionName, optionValue);
  }

  static getOption(socket, level, optionName) {
    return nanosockets.getOption(socket, level, optionName);
  }

  static setNonBlocking(socket, shouldBlock = false) {
    return nanosockets.setNonBlocking(socket, shouldBlock ? 0 : 1);
  }

  static setDontFragment(socket) {
    return nanosockets.setDontFragment(socket);
  }

  static poll(socket, timeout) {
    return nanosockets.poll(socket, timeout);
  }

  static send(socket, address, buffer) {
    return nanosockets.send(socket, address.address, buffer, buffer.length);
  }

  static receive(socket, bufferSize) {
    const address = new Address();
    const buffer = Buffer.alloc(bufferSize);
    const bytesReceived = nanosockets.receive(socket, address.address, buffer, bufferSize);
    return { bytesReceived, address, buffer };
  }

  static getAddress(socket) {
    const address = new Address();
    nanosockets.getAddress(socket, address.address);
    return address;
  }
}

module.exports = {
  UDP,
  Address,
};
