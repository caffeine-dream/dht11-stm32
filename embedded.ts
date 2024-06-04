import { SerialPort, ReadlineParser } from "serialport";
import env from "dotenv";
import { PrismaClient } from "@prisma/client";
import type { SerialAirMetric } from "./entities/air-metric";

env.config();

const parser = new ReadlineParser();

const prisma = new PrismaClient();

const port = new SerialPort({
  path: process.env.SERIAL_PORT!,
  baudRate: Number(process.env.BAUD_RATE!),
});

port.pipe(parser);

parser.on("data", async (values: string) => {
  const nativeAirMetric = JSON.parse(values) as SerialAirMetric;

  await prisma.airMetric.create({
    data: nativeAirMetric,
  });
});
