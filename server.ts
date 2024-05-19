import express from "express"
import { PrismaClient } from "@prisma/client"
import path from "path"

const prisma = new PrismaClient()

const app = express()

app.use(express.static(path.join(__dirname, "public")))

interface FindAirMetricsQuery {
  page?: number
  perPage?: number
  since?: string
  until?: string
}

app.get("/air-metrics", async (req, res) => {
  const query = req.query as FindAirMetricsQuery

  res.json(
    await prisma.airMetric.findMany({
      where: {
        createdAt: {
          lte: query.until && new Date(query.until),
          gte: query.since && new Date(query.since),
        },
      },
      take: query.perPage && +query.perPage,
      skip: !query.page ? 0 : +query.perPage! * +query.page,
    })
  )
})

app.listen(80)

