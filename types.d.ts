declare module "nanosockets" {
    export enum Status {
      OK = 0,
      Error = -1
    }
  
    export interface Address {
      address0: number;
      address1: number;
      port: number;
  
      equals(other: Address): boolean;
      getHashCode(): number;
      toString(): string;
      static createFromIpPort(ip: string, port: number): Address;
      static setIP(address: Address, ip: string): Status;
      static getIP(address: Address): string;
    }
  
    export class UDP {
      constructor();
  
      initialize(): Status;
      deinitialize(): void;
      create(sendBufferSize: number, receiveBufferSize: number): number;
      destroy(socket: number): void;
      bind(socket: number, ip: string, port: number): number;
      connect(socket: number, ip: string, port: number): number;
      setOption(socket: number, level: number, optionName: number, optionValue: number): Status;
      getOption(socket: number, level: number, optionName: number): { status: Status; value: number };
      setNonBlocking(socket: number, shouldBlock: boolean): Status;
      setDontFragment(socket: number): Status;
      poll(socket: number, timeout: number): number;
      send(socket: number, ip: string, port: number, message: string): number;
      sendOffset(socket: number, ip: string, port: number, buffer: Buffer, offset: number): number;
      receive(socket: number, bufferSize: number): { status: number; data: Buffer; address: string; port: number };
      receiveOffset(socket: number, buffer: Buffer, offset: number): { status: number; data: Buffer; address: string; port: number };
      getAddress(socket: number): { status: Status; address: string; port: number };
      isEqual(leftIP: string, rightIP: string): Status;
      setIP(ip: string): Status;
      setHostName(hostname: string): Status;
      getHostName(): { status: Status; hostname: string };
    }
  
    export namespace UnsafeUDP {
      function receive(socket: number, addressPtr: number, bufferPtr: number, bufferLength: number): number;
      function send(socket: number, addressPtr: number, bufferPtr: number, bufferLength: number): number;
    }
}
  