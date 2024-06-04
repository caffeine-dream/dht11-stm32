import express from "express";
import path from "path";
import airMetricRouter from "./air-metric/air-metric.routes";

const bootstrap = () => {
  const app = express();

  app.use(express.static(path.join(__dirname, "public")));
  app.use(airMetricRouter);

  app.listen(80);
};

bootstrap();
