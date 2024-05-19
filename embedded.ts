import { SerialPort, ReadlineParser } from "serialport"
import env from "dotenv"
import { PrismaClient } from "@prisma/client"

env.config()

const parser = new ReadlineParser()

const prisma = new PrismaClient()

const port = new SerialPort({ 
  path: process.env.SERIAL_PORT!, 
  baudRate: Number(process.env.BAUD_RATE!),
})

port.pipe(parser)

interface AirMetricInterface {
  humidity: number
  temperature: number
  createdAt: number
}

type NativeAirMetric = Omit<AirMetricInterface, "createdAt">

parser.on("data", async (values: string) => {
  const nativeAirMetric = JSON.parse(values) as NativeAirMetric
  
  await prisma.airMetric.create({
    data: nativeAirMetric,
  })
})
