import { SerialPort, ReadlineParser } from "serialport";
import { SerialAirMetric } from "../entities/air-metric";

export class SerialDataSource {
  readonly serialPort!: SerialPort;

  constructor(portName: string, baudRate: number) {
    const serialPort = new SerialPort({
      path: portName,
      baudRate,
    });

    const readlineParser = new ReadlineParser();

    serialPort.pipe(readlineParser);

    this.serialPort = serialPort;
  }

  listen = (callback: (value: SerialAirMetric) => void) => {
    this.serialPort.on("data", async (value: string) => {
      callback(JSON.parse(value) as SerialAirMetric);
    });
  };
}
