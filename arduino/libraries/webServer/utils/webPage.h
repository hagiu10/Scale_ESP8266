#ifndef WEBPAGE_H
#define WEBPAGE_H

#ifndef PROGMEM
#define PROGMEM                                                                \
  __attribute__((section("\".irom.text." __FILE__ "." __STRINGIZE(             \
      __LINE__) "." __STRINGIZE(__COUNTER__) "\"")))
#endif

const char webPage_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="ro">
<head>
<meta charset="UTF-8">
<title>Monitorizare Greutate</title>
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<style>
  body{margin:0;background:#0f172a;color:#e5e7eb;font-family:Arial,sans-serif;padding:20px}
  .container{max-width:1100px;margin:auto;background:#1e293b;padding:20px;border-radius:12px}
  h1{margin-top:0}
  .tabs{display:flex;gap:10px;margin-bottom:20px}
  .tab-btn{padding:10px 20px;border:none;border-radius:20px;cursor:pointer;background:#334155;color:#e2e8f0}
  .tab-btn.active{background:#22c55e;color:#0f172a;font-weight:bold}
  .stats{display:flex;gap:20px;margin-bottom:20px}
  .stat-box{flex:1;background:#0f172a;padding:15px;border-radius:10px;border:1px solid:#334155}
  .stat-title{font-size:14px;color:#94a3b8}
  .stat-value{font-size:26px;font-weight:bold;margin-top:5px}
</style>
</head>
<body>
<div class="container">
  <h1>Monitorizare Greutate</h1>

  <div class="tabs">
    <button class="tab-btn active" data-range="day">Zi</button>
    <button class="tab-btn" data-range="month">Lună</button>
    <button class="tab-btn" data-range="year">An</button>
  </div>

  <div class="stats">
    <div class="stat-box">
      <div class="stat-title">Total creștere</div>
      <div class="stat-value" id="up-total">0 kg</div>
    </div>
    <div class="stat-box">
      <div class="stat-title">Total scădere</div>
      <div class="stat-value" id="down-total">0 kg</div>
    </div>
    <div class="stat-box">
      <div class="stat-title">Variație totală</div>
      <div class="stat-value" id="sum-total">0 kg</div>
    </div>
  </div>

  <canvas id="chart" height="120"></canvas>
</div>

<script>
const segmentPlugin = {
  id: "segmentColor",
  beforeDatasetDraw(chart) {
    const {ctx} = chart;
    const dataset = chart.data.datasets[0];
    const meta = chart.getDatasetMeta(0);
    const points = meta.data;
    ctx.save();
    for (let i = 1; i < points.length; i++) {
      const prev = dataset.data[i - 1];
      const curr = dataset.data[i];
      let color = "#f97316";
      if (curr > prev) color = "#22c55e";
      if (curr < prev) color = "#ef4444";
      ctx.strokeStyle = color;
      ctx.lineWidth = 2;
      ctx.beginPath();
      ctx.moveTo(points[i - 1].x, points[i - 1].y);
      ctx.lineTo(points[i].x, points[i].y);
      ctx.stroke();
    }
    ctx.restore();
    return false;
  }
};
Chart.register(segmentPlugin);

let chart;
let jsonData = {day:[],month:[],year:[]};

function updateStats(values){
  if(!values.length) return;
  let up=0,down=0;
  for(let i=1;i<values.length;i++){
    const diff=values[i]-values[i-1];
    if(diff>0) up+=diff;
    if(diff<0) down+=Math.abs(diff);
  }
  document.getElementById("up-total").textContent   = up.toFixed(2)+" kg";
  document.getElementById("down-total").textContent = down.toFixed(2)+" kg";
  document.getElementById("sum-total").textContent  = (up+down).toFixed(2)+" kg";
}

function updateChart(range){
  let values=[];
  if(range==="day")   values=jsonData.day.map(x=>x.weight);
  if(range==="month") values=jsonData.month.map(x=>x.weight);
  if(range==="year")  values=jsonData.year.map(x=>x.weight);

  if(chart) chart.destroy();
  chart=new Chart(document.getElementById("chart"),{
    type:"line",
    data:{
      labels:values.map((_,i)=>i),
      datasets:[{
        label:"Greutate (kg)",
        data:values,
        borderColor:"transparent",
        pointRadius:0,
        tension:0.3
      }]
    },
    options:{responsive:true,scales:{y:{beginAtZero:false}}}
  });
  updateStats(values);
}

document.querySelectorAll(".tab-btn").forEach(btn=>{
  btn.addEventListener("click",()=>{
    document.querySelectorAll(".tab-btn").forEach(b=>b.classList.remove("active"));
    btn.classList.add("active");
    updateChart(btn.dataset.range);
  });
});

let socket = new WebSocket("ws://" + window.location.host + "/ws");
socket.onopen = () => {
  console.log("WS connected");
  socket.send("getData");
};
socket.onmessage = (event) => {
  const json = JSON.parse(event.data);
  jsonData.day   = json.day   || [];
  jsonData.month = json.month || [];
  jsonData.year  = json.year  || [];
  const active = document.querySelector(".tab-btn.active").dataset.range;
  updateChart(active);
};
</script>
</body>
</html>

)=====";

#endif // WEBPAGE_H