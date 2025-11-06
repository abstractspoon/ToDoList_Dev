google.charts.load('current', {'packages':['corechart']});
google.charts.setOnLoadCallback(OnLoad);

// General data and functions -------------------------------------------------------------
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

function ShowHideView(divName, viewId)
{
    var show = (viewId == divName);
    var display = (show ? 'block' : "none");
    
    document.getElementById(divName).style.display = display;
    RedrawActiveView();
}

function OnChangeView(newView) 
{
    ShowHideView('dashboard_id', newView);
    ShowHideView('treemap_id', newView);
    // New views here
}
        
function OnLoad()
{
    RedrawActiveView();
    chrome.webview.addEventListener('message', OnAppMessage);
}

function RedrawActiveView()
{
    var view = document.getElementById('views').value;

    switch (view)
    {
        case 'dashboard_id':
            InitDashboard();
            PopulateDashboard();
            DrawDashboard();
            break;
              
        case 'treemap_id':
            // TODO
            break;
    }
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
        var view = document.getElementById('views').value;

        switch (view)
        {
            case 'dashboard_id':
                SelectDashboardTask(id, false);
                break;
                
            case 'treemap_id':
                // TODO
                break;
        }
    }
}

// Dashboard data and functions---------------------------------------------------------------

var dashboardDataTable = null;
var dashboardRow2TaskMapping = null;
var dashboardTask2RowMapping = null;

var dashboardChart11 = null;
var dashboardChart12 = null;
var dashboardChart21 = null;
var dashboardChart22 = null;

// -----------------------------------------

function InitDashboard()
{
    dashboardChart11 = new google.visualization.BarChart    (document.getElementById('dashboard_chart11'));
    dashboardChart12 = new google.visualization.ScatterChart(document.getElementById('dashboard_chart12'));
    dashboardChart21 = new google.visualization.AreaChart   (document.getElementById('dashboard_chart21'));
    dashboardChart22 = new google.visualization.ColumnChart (document.getElementById('dashboard_chart22'));
    
    google.visualization.events.addListener(dashboard_chart11, 'select', OnDashboard11Select);
    google.visualization.events.addListener(dashboard_chart12, 'select', OnDashboard12Select);
    google.visualization.events.addListener(dashboard_chart21, 'select', OnDashboard21Select);
    google.visualization.events.addListener(dashboard_chart22, 'select', OnDashboard22Select);
}

function PopulateDashboard()
{
    dashboardDataTable = new google.visualization.DataTable();
    
    dashboardRow2TaskMapping = new Map();
    dashboardTask2RowMapping = new Map();

    dashboardDataTable.addColumn('string', 'Task');
    dashboardDataTable.addColumn('number', 'Priority');
    dashboardDataTable.addColumn('number', 'Risk');

    for (let i = 0; i < tasks.length; i++) 
    {
        var id = tasks[i]["Task ID"];
        var title = tasks[i].Title + ' (' + id + ')';
        
        dashboardDataTable.addRow([title, tasks[i].Priority, tasks[i].Risk]);
        dashboardRow2TaskMapping[i] = id;
        dashboardTask2RowMapping[id] = i;
    }
}

function OnDashboard11Select(e)
{
    OnSelectDashboardTask(dashboard_chart11);
}

function OnDashboard12Select(e)
{
    OnSelectDashboardTask(dashboard_chart12);
}

function OnDashboard21Select(e)
{
    OnSelectDashboardTask(dashboard_chart21);
}

function OnDashboard22Select(e)
{
    OnSelectDashboardTask(dashboard_chart22);
}

function OnSelectDashboardTask(chart)
{
    var row = chart.getSelection()[0].row;
    var id = dashboardRow2TaskMapping[row];
    
    SelectDashboardTask(id, true);
}

function SelectDashboardTask(id, fromChart)
{
    selectedId = id;
    var row = dashboardTask2RowMapping[id];
    
    dashboardChart11.setSelection([{'row': row}]);
    dashboardChart12.setSelection([{'row': row}]);
    dashboardChart21.setSelection([{'row': row}]);
    dashboardChart22.setSelection([{'row': row}]);
    
    if (fromChart == true)
    {
        // Notify the app
        window.chrome.webview.postMessage('SelectTask=' + id);
    }
}

function DrawDashboard()
{
    DrawDashboardChart(dashboardChart11, 0, 1);
    DrawDashboardChart(dashboardChart12, 2, 3);
    DrawDashboardChart(dashboardChart21, 4, 5);
    DrawDashboardChart(dashboardChart22, 6, 7);
}

function DrawDashboardChart(chart, color1, color2) 
{
    var options = 
    {
        animation: {"startup": true, duration: 1000, easing: 'out'},  
        colors: [ allColors[color1], allColors[color2] ],
        curveType: 'function',
        legend: { position: 'bottom' },
        title: 'Priority & Risk',
    };

    chart.draw(dashboardDataTable, options);
}

// TreeMap data and functions---------------------------------------------------------------

