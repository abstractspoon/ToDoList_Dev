google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(drawCharts);

var allColors = 
[
    '#3366cc',
    '#dc3912',
    '#ff9900',
    '#109618',
    '#990099',
    '#0099c6',
    '#dd4477',
    '#66aa00',
    '#b82e2e',
    '#316395',
    '#994499',
    '#22aa99',
    '#aaaa11',
    '#6633cc',
    '#e67300',
    '#8b0707',
    '#651067',
    '#329262',
    '#5574a6',
    '#3b3eac',
    '#b77322',
    '#16d620',
    '#b91383',
    '#f4359e',
    '#9c5935',
    '#a9c413',
    '#2a778d',
    '#668d1c',
    '#bea413',
    '#0c5922',
    '#743411'
];

function drawCharts()
{
    // Row 1
    drawChart(new google.visualization.BarChart(document.getElementById('curve_chart11')), 0, 1);
    drawChart(new google.visualization.ScatterChart(document.getElementById('curve_chart12')), 2, 3);
    
    // Row 2
    drawChart(new google.visualization.AreaChart(document.getElementById('curve_chart21')), 4, 5);
    drawChart(new google.visualization.ColumnChart(document.getElementById('curve_chart22')), 6, 7);
}

function drawChart(chart, color1, color2) 
{
    var data = new google.visualization.DataTable();

    data.addColumn('string', 'Task');
    data.addColumn('number', 'Priority');
    data.addColumn('number', 'Risk');
  
    for (let i = 0; i < tasks.length; i++) 
    {  
        data.addRow([tasks[i].Title, tasks[i].Priority, tasks[i].Risk]);
    }

    var options = 
    {
        animation: {"startup": true, duration: 1000, easing: 'out'},  
        colors: [ allColors[color1], allColors[color2] ],
        curveType: 'function',
        legend: { position: 'bottom' },
        title: 'Priority & Risk',
    };

    chart.draw(data, options);
}