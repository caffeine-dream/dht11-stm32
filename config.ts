require("dotenv").config();

const config = {
  portName: process.env.SERIAL_PORT!,
  baudRate: Number(process.env.BAUD_RATE),
};

export default config;
