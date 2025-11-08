google.charts.load('current', {'packages':['corechart']});
google.charts.load('current', {'packages':['treemap']});

google.charts.setOnLoadCallback(OnLoad);

// General data and functions -------------------------------------------------------------
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

function SupportsHTML5Storage() 
{
    try 
    {
        return 'sessionStorage' in window && window['sessionStorage'] !== null;
    } 
    catch (e) 
    {
        return false;
    }
}

function ShowHideView(divName, viewId)
{
    var show = (viewId == divName);
    var display = (show ? 'block' : 'none');
    
    document.getElementById(divName).style.display = display;
}

function OnChangeView(view) 
{
    sessionStorage.setItem('SelectedView', view);
    
    ShowHideView('dashboard_id', view);
    ShowHideView('treemap_id', view);
    // New views here
    
    RefreshSelectedView();
}
        
function OnLoad()
{
    if (!SupportsHTML5Storage())
        alert('local storage not supported');

    RefreshSelectedView();
    chrome.webview.addEventListener('message', OnAppMessage);
}

function RefreshSelectedView()
{
    var view = sessionStorage.getItem('SelectedView');
    document.getElementById('views').value = view;

    switch (view)
    {
        case 'dashboard_id':
            InitDashboard();
            DrawDashboard();
            break;
              
        case 'treemap_id':
            InitTreeMap();
            DrawTreeMap();
            break;
    }
    
    RestoreSelectedTask();
}

function SelectTask(id, fromChart)
{
    var view = sessionStorage.getItem('SelectedView');

    switch (view)
    {
        case 'dashboard_id':
        case '':
            SelectDashboardTask(id, fromChart);
            break;
              
        case 'treemap_id':
            // TODO
            break;
    }
    sessionStorage.setItem('SelectedId', id);
}

function RestoreSelectedTask()
{
    var id = sessionStorage.getItem('SelectedId');
    SelectTask(id, false);
}

function OnAppMessage(message)
{  
    if (message.data == 'Refresh')
    {
        location.reload(location.href);
    }
    else if (message.data.indexOf('SelectTask=') == 0)
    {
        var id = message.data.substr(11);
        SelectTask(id, false);
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
    if (dashboardChart11 == null)
    {
        dashboardChart11 = new google.visualization.BarChart    (document.getElementById('dashboard_chart11'));
        dashboardChart12 = new google.visualization.ScatterChart(document.getElementById('dashboard_chart12'));
        dashboardChart21 = new google.visualization.AreaChart   (document.getElementById('dashboard_chart21'));
        dashboardChart22 = new google.visualization.ColumnChart (document.getElementById('dashboard_chart22'));
        
        google.visualization.events.addListener(dashboardChart11, 'select', OnDashboard11Select);
        google.visualization.events.addListener(dashboardChart12, 'select', OnDashboard12Select);
        google.visualization.events.addListener(dashboardChart21, 'select', OnDashboard21Select);
        google.visualization.events.addListener(dashboardChart22, 'select', OnDashboard22Select);
        
        PopulateDashboard()
    }
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
        var id = tasks[i]['Task ID'];
        var title = tasks[i].Title + ' (' + id + ')';
        
        dashboardDataTable.addRow([title, tasks[i].Priority, tasks[i].Risk]);
        dashboardRow2TaskMapping[i] = id;
        dashboardTask2RowMapping[id] = i;
    }
}

function OnDashboard11Select(e)
{
    OnSelectDashboardTask(dashboardChart11);
}

function OnDashboard12Select(e)
{
    OnSelectDashboardTask(dashboardChart12);
}

function OnDashboard21Select(e)
{
    OnSelectDashboardTask(dashboardChart21);
}

function OnDashboard22Select(e)
{
    OnSelectDashboardTask(dashboardChart22);
}

function OnSelectDashboardTask(chart)
{
    var row = chart.getSelection()[0].row;
    var id = dashboardRow2TaskMapping[row];
    
    SelectDashboardTask(id, true);
}

function SelectDashboardTask(id, fromChart)
{
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
        animation: {'startup': true, duration: 1000, easing: 'out'},  
        colors: [ allColors[color1], allColors[color2] ],
        curveType: 'function',
        legend: { position: 'bottom' },
        title: 'Priority & Risk',
    };

    chart.draw(dashboardDataTable, options);
}

// TreeMap data and functions---------------------------------------------------------------

var treeMapDataTable = null;
var treeMapRow2TaskMapping = null;
var treeMapTask2RowMapping = null;

var treeMapChart = null;

// -------------------------------------------------

function InitTreeMap()
{
    if (treeMapChart == null)
    {
        treeMapChart = new google.visualization.TreeMap(document.getElementById('treemap_chart'));

        //google.visualization.events.addListener(treeMapChart, 'select', OnTreeMapSelect);
        
        PopulateTreeMap();
   }
}

function PopulateTreeMap()
{
    treeMapDataTable = new google.visualization.DataTable();

    treeMapRow2TaskMapping = new Map();
    treeMapTask2RowMapping = new Map();

    treeMapDataTable.addColumn('string', 'Task');
    treeMapDataTable.addColumn('string', 'Parent');
    treeMapDataTable.addColumn('number', 'Priority');
    treeMapDataTable.addColumn('number', 'Risk');

    treeMapDataTable.addRow(['Tasklist', null, 0, 0]);

    for (let i = 0; i < tasks.length; i++) 
    {
        AddTaskToTreeMap(tasks[i], 'Tasklist');
    }
}

function AddTaskToTreeMap(task, parentName)
{
    var id = task['Task ID'];
    var title = task.Title + ' (' + id + ')';
    
    treeMapDataTable.addRow([title, parentName, task.Priority, task.Risk]);
    
    var row = (treeMapDataTable.getNumberOfRows() - 1);
    treeMapRow2TaskMapping[row] = id;
    treeMapTask2RowMapping[id] = row;
        
    if (task.Subtasks != null)
    {
        for (let i = 0; i < task.Subtasks.length; i++) 
        {
            AddTaskToTreeMap(task.Subtasks[i], title); // RECURSIVE CALL
        }
    }
}

function DrawTreeMap() 
{
    var options = 
    {
//        animation: {'startup': true, duration: 1000, easing: 'out'},  
//        colors: [ allColors[color1], allColors[color2] ],
//        curveType: 'function',
//        legend: { position: 'bottom' },
//        title: 'Priority & Risk',
    };

    treeMapChart.draw(treeMapDataTable, options);
}
