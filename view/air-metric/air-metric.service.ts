import { type PrismaClient } from "@prisma/client";
import { type FindAirMetricsQuery } from "./dto/find-air-metric.dto";

export const findAirMetrics =
  (prisma: PrismaClient) => (query: FindAirMetricsQuery) => {
    return prisma.airMetric.findMany({
      where: {
        createdAt: {
          lte: query.until && new Date(query.until),
          gte: query.since && new Date(query.since),
        },
      },
      take: query.perPage && +query.perPage,
      skip: !query.page ? 0 : +query.perPage! * +query.page,
    });
  };
