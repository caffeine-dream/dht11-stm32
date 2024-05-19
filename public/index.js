
const fetchAirMetrics = async () => {
  const since = new Date(document.querySelector("#since").value)
  const until = new Date(document.querySelector("#until").value)

  const urlParams = `
    ${isValidDate(since) ? `since=${since.toISOString()}&` : ""}
    ${isValidDate(until) ? `until=${until.toISOString()}` : ""}  
  `

  let airMetrics = await fetch(`/air-metrics?${urlParams.trim().replaceAll("\n", "")}`)
  
  airMetrics = await airMetrics.json()

  if (airMetrics.length < 2) {
    throw alert("Not enough data!")
  }

  return airMetrics.map(airMetric => [
    dateFns.format(airMetric.createdAt, "dd MMM h:mm"),
    Number(airMetric.temperature), // it is decimal, so presented as a string
    airMetric.humidity,
  ])
}

const drawChart = async () => {
  const chartNode = document.querySelector("#chart")

  chartNode.innerHTML = ""

  const airMetrics = await fetchAirMetrics()

  const data = google.visualization.arrayToDataTable([
    ["Date", "Temperature", "Humidity"],
    ...airMetrics,
  ])

  const options = {
    title: "Temperature and humidity over time",
    curveType: "function",
    legend: {
      position: "bottom",
    },
  }

  const chart = new google.visualization.LineChart(chartNode)

  chart.draw(data, options)
}

const isValidDate = (date) => {
  return date instanceof Date && !isNaN(date)
}

google.charts.load("current", {
  packages: [
    "corechart",
  ],
})

google.charts.setOnLoadCallback(drawChart)

document.querySelector("#since").addEventListener("change", drawChart)

document.querySelector("#until").addEventListener("change", drawChart)
