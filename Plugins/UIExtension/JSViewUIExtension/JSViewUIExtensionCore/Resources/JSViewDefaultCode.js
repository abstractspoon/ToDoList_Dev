google.charts.load('current', {'packages':['corechart']});
google.charts.load('current', {'packages':['treemap']});

google.charts.setOnLoadCallback(OnLoad);

window.onresize = RefreshSelectedView;

// General data and functions -------------------------------------------------------------

function OnLoad()
{
    if (!SupportsHTML5Storage())
        alert('local storage not supported');

    if (GetPreference('BackColor', null))
        document.body.style.backgroundColor = GetPreference('BackColor', null);
    
    RefreshSelectedView();
  
    chrome.webview.addEventListener('message', OnAppMessage);
}

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

function OnAppMessage(message)
{  
    if (message.data == 'Refresh')
    {
        location.reload(location.href);
    }
    else if (message.data.indexOf('Refresh=') == 0)
    {
        let json = message.data.substr(8);
        MergeSelectedTaskAttributes(JSON.parse(json));
    }
    else if (message.data.indexOf('SelectTask=') == 0)
    {
        let id = message.data.substr(11);
        SelectTask(id, false);
    }
    else if (message.data.indexOf('Preferences=') == 0)
    {
        let colorTaskBkgnd = GetPreference('ColorTaskBackground', false);
        let strikethruDone = GetPreference('StrikethroughDone', true);
        let backColor = GetPreference('BackColor', null);
        
        let prefs = message.data.substr(12);
        sessionStorage.setItem('Preferences', prefs);
        
        if (backColor != GetPreference('BackColor', null))
            document.body.style.backgroundColor = GetPreference('BackColor', null);
        
        if ((GetPreference('ColorTaskBackground', false) != colorTaskBkgnd) ||
            (GetPreference('StrikethroughDone', true) != strikethruDone))
        {
            RefreshSelectedView();
        }
    }
}

function GetPreference(pref, defValue)
{
    var prefs = JSON.parse(sessionStorage.getItem('Preferences'));
    
    if (!prefs || (prefs[pref] == null))
        return defValue;
    
    return prefs[pref];
}

function OnChangeView(view) 
{
    sessionStorage.setItem('SelectedView', view);
    
    ShowHideView('dashboard_id', view);
    ShowHideView('treemap_id', view);
    // New views here
    
    RefreshSelectedView();
}
        
function GetSelectedView()
{
    var view = sessionStorage.getItem('SelectedView');
    
    if (view == null)
        view = 'dashboard_id';
    
    return view;
}

function RefreshSelectedView()
{
    var view = GetSelectedView();
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

function MergeSelectedTaskAttributes(selTasks)
{
    // Update global tasklist so that a subsequent refresh
    // will also have the changed tasks
    UpdateGlobalTasks(selTasks);
    
    switch (GetSelectedView())
    {
        case 'dashboard_id':
            {
                // Keep all views up to date but we only redraw if the dashboard changed
                UpdateTreeMapSelectedTasks(selTasks);
                
                if (UpdateDashboardSelectedTasks(selTasks))
                    DrawDashboard();
            }
            break;
              
        case 'treemap_id':
            {
                // Keep all views up to date but we only redraw if the treemap changed
                UpdateDashboardSelectedTasks(selTasks);
                
                if (UpdateTreeMapSelectedTasks(selTasks))
                    DrawTreeMap();
            }
            break;
    }
    
    RestoreSelectedTask();
}

function RestoreSelectedTask()
{
    var id = GetSelectedTaskId();
    SelectTask(id, false);
}

function SelectTask(id, fromChart)
{
    let prevId = GetSelectedTaskId();
    sessionStorage.setItem('SelectedId', id);

    switch (GetSelectedView())
    {
        case 'dashboard_id':
        case '':
            SelectDashboardTask(id, prevId);
            break;
              
        case 'treemap_id':
            SelectTreeMapTask(id, prevId);
            break;
    }
        
    if (fromChart == true)
    {
        // Notify the app
        window.chrome.webview.postMessage('SelectTask=' + id);
    }
    
}

function GetBestTextColor(fillColor)
{
    let rgb = fillColor.toRGB();
    let grey = ((rgb[2] + (rgb[1] * 6) + (rgb[0] * 3)) / 10);
    
    return ((grey < 128) ? 'White'.toHexColor() : 'Black'.toHexColor());
}

function GetSelectedTaskId()
{
    var id = sessionStorage.getItem('SelectedId');
    
    if (id == null)
        id = '';
    
    return id;
}

function GetSelectedChartId(chart, row2TaskMapping)
{
    var sel = chart.getSelection()[0];
    
    if (sel == null)
        return GetSelectedTaskId();
    
    return row2TaskMapping[sel.row];
}

function SetSelectedChartRow(id, chart, task2RowMapping)
{
    var row = task2RowMapping[id];
    
    chart.setSelection([{'row': row}]);
}

function CheckUpdateDataValue(dataTable, row, col, newValue)
{
    if (!row || !col || !newValue)
        return false;
    
    if (newValue == dataTable.getValue(row, col))
        return false;
    
    dataTable.setValue(row, col, newValue);
    return true;
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

function UpdateDashboardSelectedTasks(selTasks)
{
    let changed = false;
    
    // Only we've been already populated
    if (dashboardTask2RowMapping)
    {
        for (let i = 0; i < selTasks.length; i++) 
        {
            let selTask = selTasks[i];
            let id = selTask['Task ID'].toString();
            let row = dashboardTask2RowMapping[id];
            
            if (row != null)
            {
                changed |= CheckUpdateDataValue(dashboardDataTable, row, 0, (selTask.Title + ' (' + id + ')'));
                changed |= CheckUpdateDataValue(dashboardDataTable, row, 1, selTask.Priority);
                changed |= CheckUpdateDataValue(dashboardDataTable, row, 2, selTask.Risk);
                
    //            if (selTask.SubTasks != null)
    //                UpdateDashboardSelectedTasks(selTask.SubTasks); // Recursive call
            }
        }
    }
    
    return changed;
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
    let id = GetSelectedChartId(chart, dashboardRow2TaskMapping);
    
    SelectTask(id, true);
}

// Never call this directly; Only via SelectTask()
function SelectDashboardTask(id, prevId)
{
    SetSelectedChartRow(id, dashboardChart11, dashboardTask2RowMapping);
    SetSelectedChartRow(id, dashboardChart12, dashboardTask2RowMapping);
    SetSelectedChartRow(id, dashboardChart21, dashboardTask2RowMapping);
    SetSelectedChartRow(id, dashboardChart22, dashboardTask2RowMapping);
}

function DrawDashboard()
{
    DrawDashboardChart(dashboardChart11, 'Red', 'Blue');
    DrawDashboardChart(dashboardChart12, 'Green', 'Orange');
    DrawDashboardChart(dashboardChart21, 'Yellow', 'Coral');
    DrawDashboardChart(dashboardChart22, 'Purple', 'Turquoise');
}

function DrawDashboardChart(chart, color1, color2) 
{
    var options = 
    {
        animation: {'startup': true, duration: 1000, easing: 'out'},  
        colors: [ color1.toHexColor(), color2.toHexColor() ],
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

        google.visualization.events.addListener(treeMapChart, "ready", OnTreeMapReady);
        google.visualization.events.addListener(treeMapChart, "drilldown", OnTreeMapDrilldown);
        google.visualization.events.addListener(treeMapChart, "rollup", OnTreeMapRollup);
        google.visualization.events.addListener(treeMapChart, "highlight", OnTreeMapHighlight);
        google.visualization.events.addListener(treeMapChart, "unhighlight", OnTreeMapUnhighlight);
       
        PopulateTreeMap();
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
    treeMapDataTable.addColumn('number', 'Done');

    AddTreeMapItem('0', '', false, '#C0C0C0', 'Tasklist');
    
    for (let i = 0; i < tasks.length; i++) 
    {
        AddTaskToTreeMap(tasks[i], '0');
    }
}

function AddTaskToTreeMap(task, parentId)
{
    let id = task['Task ID'].toString();
    AddTreeMapItem(id, parentId, (task['Completion Date'] != ''), task['Colour'], task['Title']);
        
    if (task.Subtasks != null)
    {
        for (let i = 0; i < task.Subtasks.length; i++) 
        {
            AddTaskToTreeMap(task.Subtasks[i], id); // RECURSIVE CALL
        }
    }
}

function AddTreeMapItem(id, parentId, done, color, title)
{
    treeMapDataTable.addRow(
    [
        id,
        parentId, 
        1, // size
        1, // colorVal
        color,
        title,
        done ? 1 : 0,
    ]);
    
    let row = (treeMapDataTable.getNumberOfRows() - 1);
    treeMapRow2TaskMapping[row] = id;
    treeMapTask2RowMapping[id] = row;
}

function UpdateTreeMapSelectedTasks(selTasks)
{
    let changed = false;
        
    // Only if we've been already populated
    if (treeMapTask2RowMapping)
    {
        for (let i = 0; i < selTasks.length; i++) 
        {
            let selTask = selTasks[i];
            let id = selTask['Task ID'].toString();
            let row = treeMapTask2RowMapping[id];
            
            if (row != null)
            {
                changed |= CheckUpdateDataValue(treeMapDataTable, row, 5, selTask.Title);
                changed |= CheckUpdateDataValue(treeMapDataTable, row, 4, selTask.Colour);
                
                if (selTask.SubTasks != null)
                    changed |= UpdateTreeMapSelectedTasks(selTask.SubTasks); // Recursive call
            }
        }
    }
    
    return changed;
}

function DrawTreeMap() 
{
    let options = 
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
//        headerHeight: 50,
        height: 500,
        useWeightedAverageForAggregation: true,
        showTooltips: false,
        
        // Use click to highlight and double-click to drill down.
        eventsConfig: 
        {
          highlight: ['click'],
          rollup: ['contextmenu'], // right-click
          drilldown: ['dblclick'],
        }
    };

    treeMapChart.draw(treeMapDataTable, options);
}

function OnTreeMapReady()
{
    RefreshTreeMapTextAndColors();
}

function OnTreeMapDrilldown()
{
    // This is effectively a double-click handler
    // so select the task just clicked
    let id = GetSelectedChartId(treeMapChart, treeMapRow2TaskMapping);
    let path = GetTreeMapFullPath(id);
    
    SelectTask(id, true);
    RefreshTreeMapTextAndColors();
}

function OnTreeMapRollup(unused)
{
    // If the currently selected item is no longer visible
    // move the selection to its parent
    let selId = GetSelectedTaskId();
    
    if (!IsTreeMapIdVisible(selId))
    {
        let row = treeMapTask2RowMapping[selId];
        let parentId = treeMapDataTable.getValue(row, 1);
        
        SelectTask(parentId, true);
    }

    RefreshTreeMapTextAndColors();
}

// We never call this directly; Only via SelectTask()
// after the selected task id has been saved 
function SelectTreeMapTask(id, prevId)
{
    // Navigate to the task if it's not currently visible
    if (!IsTreeMapIdVisible(id))
    {
        let path = GetTreeMapFullPath(id);
        
        if (path.length == 1)
        {
            SetSelectedChartRow('0', treeMapChart, treeMapTask2RowMapping);
        }
        else
        {
            while (path.length > 1)
                SetSelectedChartRow(path.pop(), treeMapChart, treeMapTask2RowMapping);
        }
        
        RefreshTreeMapTextAndColors();
    }
    else if (prevId == null)
    {
        RefreshTreeMapTextAndColors();
    }
    else
    {
        RefreshTreeMapTextAndColors(prevId);
        RefreshTreeMapTextAndColors(id);
    }
}

function GetTreeMapCellId(cell)
{
    let jCell = $(cell);
    
    if (!jCell.attr('style'))
        return -1;
    
    let id = jCell.find('foreignObject').attr('id');
        
    if (id == null)
        id = jCell.find('text').text(); // default
    
    return id;
}

function IsTreeMapIdVisible(id)
{
    let treechart = $("#treemap_id");
    let svg = treechart.find("svg");
    let cells = svg.find("g");
    
    for (let i = 0; i < cells.length; i++)
    {
        if (GetTreeMapCellId(cells[i]) == id)
            return true;
    };
    
    return false;
}

function GetTreeMapFullPath(id)
{
    let path = [];
    
    while (id && (id != '0'))
    {
        path.push(id);
        
        let row = treeMapTask2RowMapping[id];
        id = treeMapDataTable.getValue(row, 1); // parent id
    }
    
    return path;
}

function OnTreeMapHighlight(item)
{
    let row = item["row"];
    let id = treeMapRow2TaskMapping[row];

    if (id != GetSelectedTaskId())
        SelectTask(id, true);
    else
        RefreshTreeMapTextAndColors(id); // still required because Google!
}

function OnTreeMapUnhighlight(item)
{
    // Even though we have specified 'enableHighlight: false'
    // Google still restores text and colours on 'mouse-leave'
    let row = item["row"];
    let id = treeMapRow2TaskMapping[row];
    
    RefreshTreeMapTextAndColors(id);
}

function RefreshTreeMapTextAndColors(specificId) 
{
    let selId = GetSelectedTaskId();
    
    let treechart = $("#treemap_id");
    let svg = treechart.find("svg");
    let cells = svg.find("g");
    
    let colorTaskBkgnd = (GetPreference('ColorTaskBackground', false) == true);
    let strikethruDone = (GetPreference('StrikethroughDone', true) == true);
    
    for (let i = 0; i < cells.length; i++)
    {
        let cell = cells[i];
        let jCell = $(cell);
        
        if (!jCell.attr('style'))
        {
            // Always hide the default highlight
            jCell.css('display', 'none');
        }
        else
        {
            // If we've been here before then the item text, which
            // was originally the task ID, will have been replaced
            // with the task title and the ID will have been inserted
            // as a 'foreign object' so we look for that first and 
            // add it if it wasn't found
            let fo = jCell.find('foreignObject');
            let id = fo.attr('id');
            
            if (id == null)
            {
                id = jCell.find('text').text(); // default
                
                if (id && (id != ''))
                {
                    fo = document.createElementNS('http://www.w3.org/2000/svg', 'foreignObject' );
                    $(fo).attr('id', id);
                    jCell.append(fo);
                }
            }
                
            if (id && ((specificId == null) || (id == specificId)))
            {
                let row = treeMapTask2RowMapping[id];
                
                if (row == null)
                    row = 0;
             
                let baseColor = treeMapDataTable.getValue(row, 4);
                
                let fillColor = baseColor.lighten(45);
                let borderColor = baseColor;
                let textColor = baseColor;
                
                if (id == selId)
                {
                    fillColor = '#A0D7FF';
                    borderColor = '#5AB4FF';
                    textColor = baseColor.darken(20);
                }
                else if ((id == 0) || colorTaskBkgnd)
                {
                    fillColor = baseColor;
                    borderColor = baseColor;
                    textColor = GetBestTextColor(baseColor);
                }
                
                let rect = jCell.find('rect');
                let text = jCell.find('text');
                
                $(rect).css('fill', fillColor)
                       .css('stroke', borderColor)
                       .css('stroke-width', '1px')
                       .css('cursor', 'default'); // Hide 'hand' cursor because we use double-clicking to drill down
                
                // Put a 1 pixel gap between items       
                if (!$(fo).attr('rectAdjusted'))
                {
                    $(fo).attr('rectAdjusted', true);
                    
                    $(rect).attr('width', $(rect).attr('width') - 3); 
                    $(rect).attr('height', $(rect).attr('height') - 3);
                }

                let title = treeMapDataTable.getValue(row, 5);
                let textDecoration = ((strikethruDone && (treeMapDataTable.getValue(row, 6) == 1)) ? 'line-through' : "");
                
                $(text).css('user-select', 'none') // Prevent double-click from selecting textColor
                       .css('fill', textColor)
                       .css('text-decoration', textDecoration)
                       .text(title);

                // Modify the text to fit the rect width
                let availWidth = $(rect).attr('width') - 10; // add padding
                let actualWidth = $(text)[0].getBBox().width;
                
                while (actualWidth > availWidth)
                {
                    title = title.substring(0, (title.length - 4));
                    $(text).text(title + '...');
                        
                    actualWidth = $(text)[0].getBBox().width;
                }
            }
        }
    }    
}
