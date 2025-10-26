google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(drawChart);

function drawChart() 
{
  var data = new google.visualization.DataTable();
  
  data.addColumn('string', 'Year');
  data.addColumn('number', 'Sales');
  data.addColumn('number', 'Expenses');
  
  for (let i = 0; i < 5; i++) 
  {  
	  data.addRow([tasks[i].Title, tasks[i].Priority, tasks[i].Risk]);
  }

  var options = 
  {
    title: 'Company Performance',
    curveType: 'function',
    legend: { position: 'bottom' }
  };

  var chart = new google.visualization.BarChart(document.getElementById('curve_chart'));

  chart.draw(data, options);
}