export interface Address {
  address: {
    port?: number;
  };

  static createFromIpPort(ip: string, port: number): Address;

  equals(other: Address): boolean;

  getHashCode(): number;

  toString(): string;
}

export interface Socket {
  // You may add specific properties or methods for the socket object as needed.
}

export interface UDP {
  static initialize(): void;

  static deinitialize(): void;

  static create(sendBufferSize: number, receiveBufferSize: number): Socket;

  static destroy(socket: Socket): void;

  static bind(socket: Socket, address: Address): number;

  static connect(socket: Socket, address: Address): number;

  static setOption(socket: Socket, level: number, optionName: number, optionValue: number): number;

  static getOption(socket: Socket, level: number, optionName: number): number;

  static setNonBlocking(socket: Socket, shouldBlock?: boolean): number;

  static setDontFragment(socket: Socket): number;

  static poll(socket: Socket, timeout: number): number;

  static send(socket: Socket, address: Address, buffer: Buffer): number;

  static receive(socket: Socket, bufferSize: number): {
    bytesReceived: number;
    address: Address;
    buffer: Buffer;
  };

  static getAddress(socket: Socket): Address;
}

export const UDP: UDP;
export const Address: Address;
