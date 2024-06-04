import config from "./config";
import { PrismaClient } from "@prisma/client";
import { SerialDataSource } from "./datasource/serial-datasource";

const prisma = new PrismaClient();

const serialDataSource = new SerialDataSource(config.portName, config.baudRate);

serialDataSource.listen(async (airMetric) => {
  await prisma.airMetric.create({
    data: airMetric,
  });
});
