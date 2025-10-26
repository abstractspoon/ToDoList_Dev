google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(drawCharts);

function drawCharts()
{
    drawChart('curve_chart11');
    drawChart('curve_chart12');
    drawChart('curve_chart21');
    drawChart('curve_chart22');
}


function drawChart(chartName) 
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

    var chart = new google.visualization.BarChart(document.getElementById(chartName));
    chart.draw(data, options);
}