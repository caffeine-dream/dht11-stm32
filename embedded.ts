import env from "dotenv";
import { PrismaClient } from "@prisma/client";
import { SerialDataSource } from "./datasource/serial-datasource";

env.config();

const prisma = new PrismaClient();

const serialDataSource = new SerialDataSource(
  process.env.SERIAL_PORT!,
  Number(process.env.BAUD_RATE!)
);

serialDataSource.listen(async (airMetric) => {
  await prisma.airMetric.create({
    data: airMetric,
  });
});
