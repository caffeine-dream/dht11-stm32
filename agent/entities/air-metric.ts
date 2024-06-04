export interface AirMetric {
  humidity: number;
  temperature: number;
  createdAt: number;
}

/**
 * Represents air metrics data from the serial port, excluding the timestamp
 * field, which is added by the database.
 */
export type SerialAirMetric = Omit<AirMetric, "createdAt">;
