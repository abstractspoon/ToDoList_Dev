google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(OnLoad);

var data = null;
var rowIdMapping = null;
var idRowMapping = null;

var barChart = null;
var scatterChart = null;
var areaChart = null;
var columnChart = null;

var selectedId = 0;

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

function InitCharts()
{
    barChart = new google.visualization.BarChart(document.getElementById('curve_chart11'));
    scatterChart = new google.visualization.ScatterChart(document.getElementById('curve_chart12'));
    areaChart = new google.visualization.AreaChart(document.getElementById('curve_chart21'));
    columnChart = new google.visualization.ColumnChart(document.getElementById('curve_chart22'));
    
    google.visualization.events.addListener(barChart, 'select', OnSelectBar);
    google.visualization.events.addListener(scatterChart, 'select', OnSelectScatter);
    google.visualization.events.addListener(areaChart, 'select', OnSelectArea);
    google.visualization.events.addListener(columnChart, 'select', OnSelectColumn);
}

function PopulateData()
{
    data = new google.visualization.DataTable();
    rowIdMapping = new Map();
    idRowMapping = new Map();

    data.addColumn('string', 'Task');
    data.addColumn('number', 'Priority');
    data.addColumn('number', 'Risk');

    for (let i = 0; i < tasks.length; i++) 
    {
        var id = tasks[i]["Task ID"];
        var title = tasks[i].Title + ' (' + id + ')';
        
        data.addRow([title, tasks[i].Priority, tasks[i].Risk]);
        rowIdMapping[i] = id;
        idRowMapping[id] = i;
    }
}

function OnLoad()
{
    InitCharts();
    PopulateData();
    DrawCharts();
    
    chrome.webview.addEventListener('message', OnAppMessage);
}

function OnAppMessage(message)
{  
    if (message.data == 'Refresh')
    {
        location.reload(location.href);
        SelectTask(selectedId, false);
    }
    else if (message.data.indexOf('SelectTask=') == 0)
    {
        var id = message.data.substr(11);
        SelectTask(id, false);
    }
}

function SelectTask(id, fromChart)
{
    selectedId = id;
    var row = idRowMapping[id];
    
    barChart.setSelection([{'row': row}]);
    scatterChart.setSelection([{'row': row}]);
    areaChart.setSelection([{'row': row}]);
    columnChart.setSelection([{'row': row}]);
    
    if (fromChart == true)
    {
        // Notify the app
        window.chrome.webview.postMessage('SelectTask=' + id);
    }
}

function OnSelectBar(e)
{
    OnSelectChart(barChart);
}

function OnSelectScatter(e)
{
    OnSelectChart(scatterChart);
}

function OnSelectArea(e)
{
    OnSelectChart(areaChart);
}

function OnSelectColumn(e)
{
    OnSelectChart(columnChart);
}

function OnSelectChart(chart)
{
    var row = chart.getSelection()[0].row;
    var id = rowIdMapping[row];
    
    SelectTask(id, true);
}

function DrawCharts()
{
    // Row 1
    DrawChart(barChart, 0, 1);
    DrawChart(scatterChart, 2, 3);
    
    // Row 2
    DrawChart(areaChart, 4, 5);
    DrawChart(columnChart, 6, 7);
}

function DrawChart(chart, color1, color2) 
{
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