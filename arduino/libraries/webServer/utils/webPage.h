
#ifndef WEBPAGE_H
#define WEBPAGE_H

const char webPage[] PROGMEM = R"===(
<!DOCTYPE html>
<html lang="ro">
<head>
  <meta charset="UTF-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Weight Management Dashboard</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>

  <style>
    /* Viewport pentru mobil */
    meta[name="viewport"] {
      width: device-width;
      initial-scale: 1.0;
    }

    /* Stil general */
    body {
      font-family: Arial, sans-serif;
      margin: 10px;
      padding: 0;
    }

    /* Titlu */
    .weight-management-title {
      font-size: 1.2rem;
      font-weight: bold;
      text-align: center;
      margin-bottom: 10px;
    }

    /* Layout principal */
    .box-chart-tabs {
      display: flex;
      flex-direction: column;
      align-items: center;
      width: 100%;
    }

    /* Container chart */
    .weight-chart-container {
      width: 100%;
      max-width: 900px;
      height: 60vh;
      min-height: 300px;
      margin-top: 10px;
      border: 1px solid grey;
    }

    /* Tabs */
    .tabs {
      display: flex;
      justify-content: center;
      margin-top: 10px;
      gap: 5px;
      flex-wrap: wrap;
    }

    .tabs button {
      padding: 8px 14px;
      border: 1px solid #ccc;
      background: #f5f5f5;
      color: black;
      cursor: pointer;
      border-radius: 4px;
      font-size: 14px;
    }

    .tabs button.active {
      background: #007bff;
      color: #fff;
      border-color: #007bff;
    }

    /* Calendar */
    .calendar-container {
      width: 100%;
      max-width: 300px;
      border: 1px solid #ccc;
      padding: 10px;
      border-radius: 6px;
      margin: 0 auto;
    }

    .month-controls,
    .year-controls {
      display: flex;
      justify-content: space-between;
      align-items: center;
      font-size: 16px;
      margin-bottom: 5px;
    }

    .year-controls {
      justify-content: center;
      gap: 20px;
    }

    .arrow {
      cursor: pointer;
      font-size: 18px;
      padding: 5px;
      user-select: none;
    }

    .calendar-grid {
      display: grid;
      grid-template-columns: repeat(7, 1fr);
      gap: 2px;
      text-align: center;
    }

    .day {
      padding: 6px;
      cursor: pointer;
      border-radius: 4px;
      font-size: 14px;
    }

    .day:hover {
      background: #e0e0e0;
    }

    .selected {
      background: #007bff;
      color: white;
    }

    .weekday {
      font-weight: bold;
      font-size: 12px;
    }

    canvas {
      width: 100% !important;
      height: 100% !important;
    }

    /* 🔵 MEDIA QUERIES PENTRU MOBIL */
    @media (max-width: 600px) {

      body {
        margin: 5px;
      }

      .weight-management-title {
        font-size: 1rem;
      }

      .calendar-container {
        max-width: 100%;
        padding: 8px;
      }

      .month-controls,
      .year-controls {
        font-size: 14px;
      }

      .arrow {
        font-size: 16px;
      }

      .day {
        padding: 4px;
        font-size: 12px;
      }

      .weight-chart-container {
        height: 45vh;
        min-height: 250px;
        border: 1px solid gray
      }

      .tabs button {
        padding: 6px 10px;
        font-size: 12px;
      }

      canvas {
        width: 100% !important;
        height: 100% !important;
      }
    }

  </style>
</head>

<body>
  <div class="weight-management-title">
    <span class="weight-management-title">Weight Management</span>
  </div>
  <div class="calendar-container">

  <!-- CONTROALE PENTRU AN -->
  <div class="year-controls">
    <span class="arrow" id="prevYear">&#9650;</span>
    <span id="yearLabel" style="font-size: 15px;"></span>
    <span class="arrow" id="nextYear">&#9660;</span>
  </div>

  <!-- CONTROALE PENTRU LUNĂ -->
  <div class="month-controls">
    <span class="arrow" id="prevMonth">&#9664;</span>
    <span id="monthLabel"></span>
    <span class="arrow" id="nextMonth">&#9654;</span>
  </div>

  <!-- GRID ZILE -->
  <div class="calendar-grid" id="calendarGrid">
    <div class="weekday">Lu</div>
    <div class="weekday">Ma</div>
    <div class="weekday">Mi</div>
    <div class="weekday">Jo</div>
    <div class="weekday">Vi</div>
    <div class="weekday">Sa</div>
    <div class="weekday">Du</div>
  </div>

</div>
  <div class="box-chart-tabs">
    <!-- Chart -->
    <div class="weight-chart-container">
      <canvas id="weightChart"></canvas>
    </div>
      <!-- Tabs -->
    <div class="tabs">
      <button id="tab-day" class="active">Day</button>
      <button id="tab-month">Month</button>
      <button id="tab-year">Year</button>
    </div>
  </div>

<script>
const now = new Date();  
let dayEntries = [];
let monthEntries = [];
let yearEntries = [];
let currentMode = 'day';
let chart;
let selectedYear = now.getFullYear();
let selectedMonth = now.getMonth();
let selectedDay = now.getDate();

// Load NDJSON
document.addEventListener('DOMContentLoaded', async () => {
  setTimeRTConESP();
  initTabs();
  updateEntries();
  updateChart();
});
// Get measurements per day/month/year
async function getWeightPerDay(year, month, day) {
  const response = await fetch(`/data?year=${year}&month=${month}&day=${day}`);
  const data = await response.json(); 
  return data;
}
async function getWeightPerMonth(year, month) {
  const response = await fetch(`/data?year=${year}&month=${month}`);
  const data = await response.json(); 
  return data;
}
async function getWeightPerYear(year) {
  const response = await fetch(`/data?year=${year}`);
  const data = await response.json(); 
  return data;
}
function setTimeRTConESP() {
  const now = new Date();
  const payload = {
    year: now.getFullYear(),
    month: now.getMonth() + 1,
    day: now.getDate(),
    hour: now.getHours(),
    minute: now.getMinutes(),
    second: now.getSeconds()
  };
  console.log('Setting RTC with payload:', payload);
  fetch('/setRTC', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(payload)
  })
  .then(response => response.json())
  .then(msg => {
    console.log('RTC set response:', msg);
  });
}
function updateEntries() {
  if (currentMode === 'day')
    getWeightPerDay(selectedYear, selectedMonth + 1, selectedDay).then(data => {
      dayEntries = data;
      updateChart();
    });
  if (currentMode === 'month')
    getWeightPerMonth(selectedYear, selectedMonth + 1).then(data => {
      monthEntries = data;
      updateChart();
    });
  if (currentMode === 'year')
    getWeightPerYear(selectedYear).then(data => {
      yearEntries = data;
      updateChart();
    });
}
// Tabs logic
function initTabs() {
  ['tab-day','tab-month','tab-year'].forEach(id => {
    document.getElementById(id).addEventListener('click', () => {
      currentMode = id.replace('tab-', '');
      setActiveTab(id);
      updateEntries();
    });
  });
}

function setActiveTab(id) {
  ['tab-day','tab-month','tab-year'].forEach(tid => {
    document.getElementById(tid).classList.toggle('active', tid === id);
  });
}

// Chart update
function updateChart() {
  const year = selectedYear;
  const month = selectedMonth;
  const day = selectedDay;

  let labels = [];
  let data = [];

  if (currentMode === 'year') {
    yearEntries.forEach(entry => {
      labels.push(`${entry.month}`);
      data.push(entry.data[0].weight);
    });
    renderChart(labels, data);
    return;
  }

  if (currentMode === 'month') {
    monthEntries.forEach(entry => {
      labels.push(`${entry.day}`);
      data.push(entry.weight);
    });
    renderChart(labels, data);
    return;
  }

  if (currentMode === 'day') {
    dayEntries.forEach(entry => {
      labels.push(`${entry.time}`);
      data.push(entry.weight);
    });
    renderChart(labels, data);
    return;
  }   
}

function renderChart(labels, data) {
  if (!labels.length || !data.length) {
    labels.push("No data for selected period!");
    data.push(0);
  }
  setTimeout(() => {
    const ctx = document.getElementById('weightChart').getContext('2d');
    if (chart) chart.destroy();
    chart = new Chart(ctx, {
      type: 'line',
      data: {
        labels,
        datasets: [{
          label: 'Weight (Kg)',
          data,
          borderColor: '#007bff',
          backgroundColor: 'rgba(0, 123, 255, 0.1)',
          tension: 0.2,
          pointRadius: 3
        }]
      },
      options: {
        maintainAspectRatio: false,
        responsive: true,
        scales: {
          x: {
            title: {
              display: true,
              text: currentMode === 'day' ? 'Time (HH:MM)' : (currentMode === 'month' ? 'Days' : 'Months')
            }
          },
          y: {
            title: {
              display: true,
              text: 'Weight (Kg)'
            }
          }
        }
      }
    });
  }, 150); // Delay pentru a asigura că canvas-ul este gata

  console.log(
    "\nCanvas size:",
    document.getElementById('weightChart').width,
    document.getElementById('weightChart').height,
    "\nCanvas labels:", labels,
    "\nCanvas data:", data
  );
}
// Calendar logic
function renderCalendar() {
  const grid = document.getElementById("calendarGrid");
  const monthLabel = document.getElementById("monthLabel");
  const yearLabel = document.getElementById("yearLabel");

  grid.innerHTML = `
    <div class="weekday">Lu</div>
    <div class="weekday">Ma</div>
    <div class="weekday">Mi</div>
    <div class="weekday">Jo</div>
    <div class="weekday">Vi</div>
    <div class="weekday">Sa</div>
    <div class="weekday">Du</div>
  `;

  const months = [
    "Ianuarie","Februarie","Martie","Aprilie","Mai","Iunie",
    "Iulie","August","Septembrie","Octombrie","Noiembrie","Decembrie"
  ];

  monthLabel.textContent = months[selectedMonth];
  yearLabel.textContent = selectedYear;

  const firstDay = new Date(selectedYear, selectedMonth, 1).getDay();
  const daysInMonth = new Date(selectedYear, selectedMonth + 1, 0).getDate();

  let start = firstDay === 0 ? 6 : firstDay - 1;

  for (let i = 0; i < start; i++) {
    const empty = document.createElement("div");
    grid.appendChild(empty);
  }

  for (let d = 1; d <= daysInMonth; d++) {
    const day = document.createElement("div");
    day.classList.add("day");
    day.textContent = d;

    if (d === selectedDay) {
      day.classList.add("selected");
    }

    day.addEventListener("click", () => {
      selectedDay = d;
      renderCalendar();
      console.log(`Ai selectat: ${selectedYear}-${selectedMonth+1}-${selectedDay}`);
    });

    grid.appendChild(day);
  }
}

// Navigare lună
document.getElementById("prevMonth").addEventListener("click", () => {
  selectedMonth--;
  if (selectedMonth < 0) {
    selectedMonth = 11;
    selectedYear--;
  }
  renderCalendar();
});

document.getElementById("nextMonth").addEventListener("click", () => {
  selectedMonth++;
  if (selectedMonth > 11) {
    selectedMonth = 0;
    selectedYear++;
  }
  renderCalendar();
});

// Navigare an
document.getElementById("prevYear").addEventListener("click", () => {
  selectedYear--;
  renderCalendar();
});

document.getElementById("nextYear").addEventListener("click", () => {
  selectedYear++;
  renderCalendar();
});

renderCalendar();
</script>

</body>
</html>

)===";

#endif // WEBPAGE_H
