const char MAIN_page[] PROGMEM = R"=====(
<!doctype html>
<html>
 
<head>
	<title>Project Raven</title>
	
	<script src = "https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.3/Chart.min.js"></script>	
	<style>
	canvas{
		-moz-user-select: none;
		-webkit-user-select: none;
		-ms-user-select: none;
	}
 
	/* Data Table Styling */
	#dataTable {
	  font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
	  border-collapse: collapse;
	  width: 100%;
	}
 
	#dataTable td, #dataTable th {
	  border: 1px solid #ddd;
	  padding: 8px;
	}
 
	#dataTable tr:nth-child(even){background-color: #f2f2f2;}
 
	#dataTable tr:hover {background-color: #ddd;}
 
	#dataTable th {
	  padding-top: 12px;
	  padding-bottom: 12px;
	  text-align: left;
	  background-color: #4CAF50;
	  color: white;
	}
	</style>
</head>
 
<body>
    <div style="text-align:center;"><b>Project Raven</b><br>Background Radiation Detector</div>
    <div class="chart-container" position: relative; height:350px; width:100%">
        <canvas id="Chart" width="400" height="400"></canvas>
    </div>
<div>
	<table id="dataTable">
<th>Time</th>
<th>Alpha</th>
<th>Beta</th>
<th>Gamma</th>
</tr>
</table>
</div>
<br>
<br>	
 
<script>

var AlphaVal = [];
var BetaVal = [];
var GammaVal = [];
var timeStamp = [];
var MaxDataPoints = 20;

function Destroy(){
  if(AlphaVal.length == 20){
    AlphaVal.shift();
    BetaVal.shift();
    GammaVal.shift();
    timeStamp.shift();
  }
}

function showGraph()
{
    for (i = 0; i < arguments.length; i++) {
    AlphaVal.push(arguments[i]);
	  BetaVal.push(arguments[i]);
	  GammaVal.push(arguments[i]);
  
   }
    var ctx = document.getElementById("Chart").getContext('2d');
    var Chart2 = new Chart(ctx, {
        type: 'line',
        data: {
            labels: timeStamp,	//Bottom Labeling
            datasets: [{
                label: "Alpha",
                fill: false,  //Try with true
                backgroundColor: 'rgba( 243, 156, 18 , 1)', //Dot marker color
                borderColor: 'rgba( 243, 156, 18 , 1)', //Graph Line Color
                data: AlphaVal,},{
                label: "Beta",
                fill: false,  //Try with true
                backgroundColor: 'rgba( 178,34,34 , 1)', //Dot marker color
                borderColor: 'rgba( 178,34,34 , 1)',  //Graph Line Color
                data: BetaVal,},{
                label: "Gamma",
                fill: false,  //Try with true
                backgroundColor: 'rgba( 0,255,255 , 1)', //Dot marker color
                borderColor: 'rgba( 0,255,255 , 1)',  //Graph Line Color
                data: GammaVal,
            }],
        },
        options: {
            title: {
                    display: true,
                    text: "Background Radiation"
                },
            maintainAspectRatio: false,
            elements: {
            line: {
                    tension: 0.5 //Smoothening (Curved) of data lines
                }
            },
            scales: {
                    yAxes: [{
                        ticks: {
                            beginAtZero:true
                        }
                    }]
            }
        }
    });
 
}
 
//On Page load show graphs
window.onload = function() {
	console.log(new Date().toLocaleTimeString());
 showGraph();
};
 
setInterval(function() {
  // Call a function repetatively with 5 Second interval
  getData();
}, 5000); //5000mSeconds update rate
 
function getData() {
  var xhttp = new XMLHttpRequest();
   
  
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
     //Push the data in array
	var time = new Date().toLocaleTimeString();
  var data = this.responseText;
  var dataFix = data.split('~');
	var AlphaValue = dataFix[0];
	var BetaValue = dataFix[1];
	var GammaValue = dataFix[2]; 
      AlphaVal.push(AlphaValue);
      BetaVal.push(BetaValue);
      GammaVal.push(GammaValue);
      timeStamp.push(time);
      Destroy();
      showGraph();	//Update Graphs
	//Update Data Table
	  var table = document.getElementById("dataTable");
	  var row = table.insertRow(1);	//Add after headings
	  var cell1 = row.insertCell(0);
	  var cell2 = row.insertCell(1);
	  var cell3 = row.insertCell(2);
	  var cell4 = row.insertCell(3);
	  cell1.innerHTML = time;
	  cell2.innerHTML = AlphaValue;
	  cell3.innerHTML = BetaValue;
	  cell4.innerHTML = GammaValue;
    }
  };
  xhttp.open("GET", "readData", true); 
  xhttp.send();
  //xhttp.open("GET", "readBeta", true);
  //xhttp.send();
  //xhttp.open("GET", "readGamma", true);
  //xhttp.send();
}
		
</script>
</body>
 
</html>
 
)=====";
