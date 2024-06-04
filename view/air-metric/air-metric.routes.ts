import { Router } from "express";
import { findAirMetrics } from "./air-metric.service";
import { type FindAirMetricsQuery } from "./dto/find-air-metric.dto";
import { PrismaClient } from "@prisma/client";

const router = Router();

const prismaClient = new PrismaClient();

router.get("/air-metrics", async (req, res) => {
  const query = req.query as FindAirMetricsQuery;
  const airMetrics = await findAirMetrics(prismaClient)(query);
  res.json(airMetrics);
});

export default router;
