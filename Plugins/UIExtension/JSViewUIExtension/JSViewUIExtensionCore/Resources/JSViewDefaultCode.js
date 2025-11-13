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
    
    if (view == null)
        view = 'dashboard_id';
    
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

function SelectTask(id, fromChart)
{
    var view = sessionStorage.getItem('SelectedView');
    sessionStorage.setItem('SelectedId', id);

    switch (view)
    {
        case 'dashboard_id':
        case '':
            SelectDashboardTask(id, fromChart);
            break;
              
        case 'treemap_id':
            SelectTreeMapTask(id, fromChart);
            break;
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

        google.visualization.events.addListener(treeMapChart, "ready", OnMapTreeReady);
        google.visualization.events.addListener(treeMapChart, "drilldown", OnMapTreeDrilldown);
        google.visualization.events.addListener(treeMapChart, "rollup", OnMapTreeRollup);
        google.visualization.events.addListener(treeMapChart, "highlight", OnMapTreeHighlight);
       
        PopulateTreeMap();
    }
}

function SelectTreeMapTask(id, fromChart)
{
    var row = treeMapTask2RowMapping[id];
    FixupTreeMapTextAndColors(row);
    
    if (fromChart == true)
    {
        // Notify the app
        window.chrome.webview.postMessage('SelectTask=' + id);
    }
}

function PopulateTreeMap()
{
    treeMapDataTable = new google.visualization.DataTable();

    treeMapRow2TaskMapping = new Map();
    treeMapTask2RowMapping = new Map();

    treeMapDataTable.addColumn('string', 'TaskId');
    treeMapDataTable.addColumn('string', 'ParentId');
    treeMapDataTable.addColumn('number', 'Size');
    treeMapDataTable.addColumn('number', 'ColorVal');
    treeMapDataTable.addColumn('string', 'WebColor');
    treeMapDataTable.addColumn('string', 'Title');

    treeMapDataTable.addRow(['0', null, 1, 1, '#000000', null]);
    
    for (let i = 0; i < tasks.length; i++) 
    {
        AddTaskToTreeMap(tasks[i], '0');
    }
}

function AddTaskToTreeMap(task, parentId)
{
    var id = task['Task ID'].toString();

    treeMapDataTable.addRow(
    [
        id,
        parentId, 
        1,
        ((task['Completion Date'] == '') ? 0 : 1),
        task['Colour'],
        task['Title']
    ]);
    
    var row = (treeMapDataTable.getNumberOfRows() - 1);
    treeMapRow2TaskMapping[row] = id;
    treeMapTask2RowMapping[id] = row;
        
    if (task.Subtasks != null)
    {
        for (let i = 0; i < task.Subtasks.length; i++) 
        {
            AddTaskToTreeMap(task.Subtasks[i], id); // RECURSIVE CALL
        }
    }
}

function DrawTreeMap() 
{
    var options = 
    {
        enableHighlight: false,
        maxDepth: 1,
        maxPostDepth: 0,
//        minHighlightColor: '#8c6bb1',
//        midHighlightColor: '#9ebcda',
//        maxHighlightColor: '#edf8fb',
//        minColor: '#009688',
//        midColor: '#f7f7f7',
//        maxColor: '#ee8100',
        headerHeight: 0,
        height: 500,
        useWeightedAverageForAggregation: true,
        showTooltips: false,
        
        // Use click to highlight and double-click to drill down.
        eventsConfig: 
        {
          highlight: ['click'],
          unhighlight: [/*'mouseout'*/], // disable (for now)
          rollup: ['contextmenu'],
          drilldown: ['dblclick'],
        }
    };

    treeMapChart.draw(treeMapDataTable, options);
}

function OnMapTreeReady()
{
    FixupTreeMapTextAndColors(-1);
}

function OnMapTreeDrilldown()
{
    FixupTreeMapTextAndColors(-1);
}

function OnMapTreeHighlight(row)
{
    var hiliteRow = row["row"];
    FixupTreeMapTextAndColors(hiliteRow);
    
    // Notify the app
    var id = treeMapRow2TaskMapping[hiliteRow];
    window.chrome.webview.postMessage('SelectTask=' + id);
}

function OnMapTreeUnhighlight(row)
{
    FixupTreeMapTextAndColors(-1);
}

function OnMapTreeRollup(row)
{
    var hiliteRow = row["row"];
    var id = treeMapRow2TaskMapping[hiliteRow];
    
    FixupTreeMapTextAndColors(-1);
}

function FixupTreeMapTextAndColors(hiliteRow) 
{
    if (hiliteRow == -1)
    {
        var id = sessionStorage.getItem('SelectedId');
        
        if (id != null)
            hiliteRow = treeMapTask2RowMapping[id];
    }
    
    var treechart = $("#treemap_id");
    var svg = treechart.find("svg");
    var cells = svg.find("g");

    cells.each
    (
        // Note: 'i' is an index into the array of svg elements
        // in the graph and bears no relationship whatsoever with
        // the 'row' index into treeMapDataTable, hence we require
        // a somewhat elaborate method for converting between them
        function(i, item) 
        {
            var cell = $(item);
            
            // If we've been here before then the item text, which
            // was originally the task ID, will have been replaced
            // with the task title and the ID will have been inserted
            // as a 'foreign object' so we look for that first and 
            // add it if it wasn't found
            var id = cell.find('foreignObject').attr('id');
            
            if (id == null)
            {
                id = cell.find('text').text(); // default
                
                var foreignObject = document.createElementNS('http://www.w3.org/2000/svg', 'foreignObject' );
                $(foreignObject).attr('id', id);
                cell.append(foreignObject);
            }
                
            if (id != null)
            {
                var row = treeMapTask2RowMapping[id];
                
                if (row != null)
                {
                    var fillColor = treeMapDataTable.getValue(row, 4);
                    var borderColor = '#FFFFFF';
                    
                    if (hiliteRow && (row == hiliteRow))
                    {
                        fillColor = '#A0D7FF';
                        borderColor = '#5AB4FF';
                    }
                    
                    cell.find('rect')
                        .css('fill', fillColor)
                        .css('stroke', borderColor);
                    
                    // We always replace the text because the tree map
                    // will frequently restore the original text when
                    // we least expect it
                    cell.find('text')
                        .text(treeMapDataTable.getValue(row, 5));
                            
                    if (replaceText)
                    {
                        var foreignObject = document.createElementNS('http://www.w3.org/2000/svg', 'foreignObject' );
                        $(foreignObject).attr('id', id);
                        cell.append(foreignObject);
                    }
                }
            }
        }
    );
}
