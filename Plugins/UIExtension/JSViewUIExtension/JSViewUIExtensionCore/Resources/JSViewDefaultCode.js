google.charts.load('current', {'packages':['corechart']});
google.charts.load('current', {'packages':['treemap']});

google.charts.setOnLoadCallback(OnLoad);

window.onresize = OnResize;
window.onkeydown = OnKeyDown;
window.onblur = OnFocusChanged;
window.onfocus = OnFocusChanged;

// ----------------------------------------------------------------------------------------

// App Messages
const SetSelectedTaskMsg     = "SetSelectedTask";
const SetPreferencesMsg      = "SetPreferences";
const RefreshContentMsg      = "RefreshContent";
const RestoreSessionStateMsg = "RestoreSessionState";

// Session Storage Keys
const SelectedViewKey = "SelectedView";
const SelectedTaskKey = "SelectedTask";
const PreferencesKey  = "Preferences";
const TreeMapDepthKey = "TreeMapDepth";

// View IDs
const _DashboardViewId = "dashboard_id";
const TreeMapViewId   = "treemap_id";

// Dashboard class //////////////////////////////////////////////////////////////////////////

class Dashboard
{
    // All data is private
    #dashboardDataTable = null;
    #dashboardRow2TaskMapping = null;
    #dashboardTask2RowMapping = null;

    #dashboardChart11 = null;
    #dashboardChart12 = null;
    #dashboardChart21 = null;
    #dashboardChart22 = null;

    // -----------------------------------------

    /* void */ Initialise() 
    {
        if (this.#dashboardChart11 == null)
        {
            this.#dashboardChart11 = new google.visualization.BarChart    (document.getElementById('dashboard_chart11'));
            this.#dashboardChart12 = new google.visualization.ScatterChart(document.getElementById('dashboard_chart12'));
            this.#dashboardChart21 = new google.visualization.AreaChart   (document.getElementById('dashboard_chart21'));
            this.#dashboardChart22 = new google.visualization.ColumnChart (document.getElementById('dashboard_chart22'));
            
            // Listerners are typically 'static' in nature so we need 
            // some trickery to be able to use a instance method
            this.On11SelectHandler = this.On11Select.bind(this);
            google.visualization.events.addListener(this.#dashboardChart11, 'select', this.On11SelectHandler);

            this.On12SelectHandler = this.On12Select.bind(this);
            google.visualization.events.addListener(this.#dashboardChart12, 'select', this.On12SelectHandler);

            this.On21SelectHandler = this.On21Select.bind(this);
            google.visualization.events.addListener(this.#dashboardChart21, 'select', this.On21SelectHandler);

            this.On22SelectHandler = this.On22Select.bind(this);
            google.visualization.events.addListener(this.#dashboardChart22, 'select', this.On22SelectHandler);
            
            this.#Populate()
        }
    }

    /* void */ #Populate()
    {
        this.#dashboardDataTable = new google.visualization.DataTable();
        
        this.#dashboardRow2TaskMapping = new Map();
        this.#dashboardTask2RowMapping = new Map();

        this.#dashboardDataTable.addColumn('string', 'Task');
        this.#dashboardDataTable.addColumn('number', 'Priority');
        this.#dashboardDataTable.addColumn('number', 'Risk');

        for (let i = 0; i < tasks.length; i++) 
        {
            let id = tasks[i]['Task ID'];
            let title = tasks[i].Title + ' (' + id + ')';
            
            this.#dashboardDataTable.addRow([title, tasks[i].Priority, tasks[i].Risk]);
            this.#dashboardRow2TaskMapping[i] = id;
            this.#dashboardTask2RowMapping[id] = i;
        }
    }

    /* bool */ UpdateSelectedTasks(selTasks)
    {
        let changed = false;
        
        // Only we've been already populated
        if (this.#dashboardTask2RowMapping)
        {
            for (let i = 0; i < selTasks.length; i++) 
            {
                let selTask = selTasks[i];
                let id = selTask['Task ID'].toString();
                let row = this.#dashboardTask2RowMapping[id];
                
                if (row != null)
                {
                    changed |= CheckUpdateDataValue(dashboardDataTable, row, 0, (selTask.Title + ' (' + id + ')'));
                    changed |= CheckUpdateDataValue(dashboardDataTable, row, 1, selTask.Priority);
                    changed |= CheckUpdateDataValue(dashboardDataTable, row, 2, selTask.Risk);
                    
        //            if (selTask.SubTasks != null)
        //                UpdateSelectedTasks(selTask.SubTasks); // Recursive call
                }
            }
        }
        
        return changed;
    }

    /* void */ RestoreSessionState(state)
    {
        // Nothing to do
    }

    /* string */ GetSessionState()
    {
        return {};
    }

    /* void */ On11Select(e)
    {
        this.OnSelectTask(this.#dashboardChart11);
    }

    /* void */ On12Select(e)
    {
        this.OnSelectTask(this.#dashboardChart12);
    }

    /* void */ On21Select(e)
    {
        this.OnSelectTask(this.#dashboardChart21);
    }

    /* void */ On22Select(e)
    {
        this.OnSelectTask(this.#dashboardChart22);
    }

    /* void */ OnSelectTask(chart)
    {
        let id = GetSelectedIdFromChart(chart, this.#dashboardRow2TaskMapping);
        
        if (IsSelectableTask(id))
            SelectTask(id, true);
    }

    /* void */ OnKeyDown(unused)
    {
        // Do nothing
    }

    /* void */ OnFocusChanged(hasFocus)
    {
        // Do nothing
    }

    /* void */ Refresh()
    {
        this.Initialise();
        this.Draw();
        
        RestoreSelectedTask();
    }

    /* void */ OnResize()
    {
        this.Draw();
    }

    // Never call this directly; Only via SelectTask()
    /* void */ SelectTask(id, prevId)
    {
        SetSelectedChartRow(id, this.#dashboardChart11, this.#dashboardTask2RowMapping);
        SetSelectedChartRow(id, this.#dashboardChart12, this.#dashboardTask2RowMapping);
        SetSelectedChartRow(id, this.#dashboardChart21, this.#dashboardTask2RowMapping);
        SetSelectedChartRow(id, this.#dashboardChart22, this.#dashboardTask2RowMapping);
    }

    /* DOMRect */ GetSelectedTaskLabelRect()
    {
        return null;
    }
                  
    /* void */ Draw()
    {
        this.DrawChart(this.#dashboardChart11, 'Red', 'Blue');
        this.DrawChart(this.#dashboardChart12, 'Green', 'Orange');
        this.DrawChart(this.#dashboardChart21, 'Yellow', 'Coral');
        this.DrawChart(this.#dashboardChart22, 'Purple', 'Turquoise');
    }

    /* void */ DrawChart(chart, color1, color2) 
    {
        let options = 
        {
            colors: [ color1.toHexColor(), color2.toHexColor() ],
            curveType: 'function',
            legend: { position: 'bottom' },
            title: 'Priority & Risk',
            
            animation: 
            {
                'startup': true, 
                duration: 1000, 
                easing: 'out'
            },  
        };

        chart.draw(this.#dashboardDataTable, options);
    }

} // class Dashboard

// Global instances -----------------------------------------------------------------------

let dashboard = new Dashboard();



// External functions called by the app ---------------------------------------------------
function GetSelectedTaskLabelRect()
{
    let view = GetSelectedView();
    let rect = null;

    switch (view)
    {
        case _DashboardViewId:
            rect = dashboard.GetSelectedTaskLabelRect();
            break;
              
        case TreeMapViewId:
            rect = GetSelectedTreeMapTaskLabelRect();
            break;
    }
    
    return { x:      rect.x, 
             y:      rect.y, 
             width:  rect.width, 
             height: rect.height };
}

// Called by the app only
function GetSessionState()
{
    return { [SelectedViewKey]: GetSelectedView(), 
             [_DashboardViewId]: dashboard.GetSessionState(),
             [TreeMapViewId]  : GetTreeMapSessionState() };
}

// General data and functions -------------------------------------------------------------

function OnLoad()
{
    if (!SupportsHTML5Storage())
        alert('local storage not supported');

    if (GetPreference('BackColor', null))
        document.body.style.backgroundColor = GetPreference('BackColor', null);
  
    chrome.webview.addEventListener('message', OnAppMessage);
    
    OnChangeView(GetSelectedView());
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

function OnAppMessage(event)
{  
    let msg = JSON.parse(event.data);

    if (msg.msg == RefreshContentMsg)
    {
        if (msg.tasks)
            MergeSelectedTaskAttributes(msg.tasks);
        else
            location.reload(location.href);
    }
    else if (msg.msg == SetSelectedTaskMsg)
    {
        SelectTask(msg.id, false);
    }
    else if (msg.msg == SetPreferencesMsg)
    {
        let colorTaskBkgnd = GetPreference('ColorTaskBackground', false);
        let strikethruDone = GetPreference('StrikethroughDone', true);
        let backColor = GetPreference('BackColor', null);
        
        SetStorage(PreferencesKey, JSON.stringify(msg.prefs).toString());
        
        if (backColor != GetPreference('BackColor', null))
            document.body.style.backgroundColor = GetPreference('BackColor', null);
        
        if ((GetPreference('ColorTaskBackground', false) != colorTaskBkgnd) ||
            (GetPreference('StrikethroughDone', true) != strikethruDone))
        {
            RefreshSelectedView();
        }
    }
    else if (msg.msg == RestoreSessionStateMsg)
    {
        RestoreSessionState(JSON.parse(msg.state));
    }
}

function GetPreference(pref, defValue)
{
    let prefs = JSON.parse(GetStorage('Preferences'));
    
    if (!prefs || (prefs[pref] == null))
        return defValue;
    
    return prefs[pref];
}

function GetStorage(key)
{
    return sessionStorage.getItem(key);
}

function SetStorage(key, value)
{
    sessionStorage.setItem(key, value);
}

function OnChangeView(view) 
{
    SetStorage(SelectedViewKey, view);
    
    ShowHideView(_DashboardViewId, view);
    ShowHideView(TreeMapViewId, view);
    // New views here
    
    // Dependent UI
    if (view == TreeMapViewId)
    {
        document.getElementById('treemapdepth_combo').style.display = 'inline-block';
        document.getElementById('treemapdepth').value = GetTreeMapDepth().toString();
    }
    else
    {
        document.getElementById('treemapdepth_combo').style.display = 'none';
    }
    
    RefreshSelectedView();
}

function ShowHideView(divName, viewId)
{
    let show = (viewId == divName);
    let display = (show ? 'block' : 'none');
    
    document.getElementById(divName).style.display = display;
}
        
function GetSelectedView()
{
    let view = GetStorage(SelectedViewKey);
    
    if (view == null)
        view = _DashboardViewId;
    
    return view;
}

function RefreshSelectedView()
{
    let view = GetSelectedView();
    document.getElementById('views').value = view;

    switch (view)
    {
        case _DashboardViewId:
            dashboard.Refresh();
            break;
              
        case TreeMapViewId:
            RefreshTreeMap();
            break;
    }
}

function OnResize(event)
{
    let view = GetSelectedView();

    switch (view)
    {
        case _DashboardViewId:
            dashboard.OnResize();
            break;
              
        case TreeMapViewId:
            OnResizeTreeMap();
            break;
    }
}

function OnKeyDown(event)
{
    let view = GetSelectedView();

    switch (view)
    {
        case _DashboardViewId:
            dashboard.OnKeyDown(event);
            break;
              
        case TreeMapViewId:
            OnKeyDownTreeMap(event);
            break;
    }
}

function OnFocusChanged(event)
{
    let hasFocus = (event.type == 'focus');
    let view = GetSelectedView();

    switch (view)
    {
        case _DashboardViewId:
            dashboard.OnFocusChanged(hasFocus);
            break;
              
        case TreeMapViewId:
            OnFocusChangedTreeMap(hasFocus);
            break;
    }
}

function MergeSelectedTaskAttributes(selTasks)
{
    switch (GetSelectedView())
    {
        case _DashboardViewId:
            {
                // Keep all views up to date but we only redraw if the dashboard changed
                UpdateTreeMapSelectedTasks(selTasks);
                
                if (dashboard.UpdateSelectedTasks(selTasks))
                    dashboard.Draw();
            }
            break;
              
        case TreeMapViewId:
            {
                // Keep all views up to date but we only redraw if the treemap changed
                UpdateSelectedTasks(selTasks);
                
                if (UpdateTreeMapSelectedTasks(selTasks))
                    DrawTreeMap();
            }
            break;
    }
    
    RestoreSelectedTask();
}

function RestoreSelectedTask()
{
    let id = GetSelectedTaskId();
    
    if (IsSelectableTask(id))
        SelectTask(id, false);
}

function RestoreSessionState(state)
{
    dashboard.RestoreSessionState(state[_DashboardViewId]);
    RestoreTreeMapSessionState(state[TreeMapViewId]);

    let viewState = state[SelectedViewKey];
    
    if (viewState && (viewState != GetSelectedView()))
        OnChangeView(viewState);
}

function NotifyApp(msgKey, value1Key, value1, value2Key = null, value2 = null, value3Key = null, value3 = null)
{
    let msg = { };
    
    msg['msg'] = msgKey;
    
    if (value1Key)
    {
        msg[value1Key] = value1;
        
        if (value2Key)
        {
            msg[value2Key] = value2;
            
            if (value3Key)
                msg[value3Key] = value3;
        }
    }
    
    window.chrome.webview.postMessage(JSON.stringify(msg));
}

function IsSelectableTask(id)
{
    return (id && !Number.isNaN(id) && (Number(id) > 0));
}

function SelectTask(id, fromChart)
{
    // Prevent selection of the 'Tasklist' root
    if (!IsSelectableTask(id))
    {
        alert('Task not selectable. ID: ' + id);
        return;
    }
    
    let prevId = GetSelectedTaskId();
    SetStorage(SelectedTaskKey, id);

    switch (GetSelectedView())
    {
        case _DashboardViewId:
        case '':
            dashboard.SelectTask(id, prevId);
            break;
              
        case TreeMapViewId:
            SelectTreeMapTask(id, prevId);
            break;
    }
        
    if (fromChart)
        NotifyApp(SetSelectedTaskMsg, "id", id);
}

function GetSelectedTaskId()
{
    let id = GetStorage(SelectedTaskKey);
    
    if (id == null)
        id = '';
    
    return id;
}

function GetSelectedIdFromChart(chart, row2TaskMapping)
{
    let sel = chart.getSelection()[0];
    
    if (sel == null)
        return GetSelectedTaskId();
    
    return row2TaskMapping[sel.row];
}

function SetSelectedChartRow(id, chart, task2RowMapping)
{
    let row = task2RowMapping[id];
    
    chart.setSelection([{'row': row}]);
}

function CheckUpdateDataValue(dataTable, row, col, newValue)
{
    if ((row == null) || (col == null) || (newValue == null))
        return false;
    
    if (newValue == dataTable.getValue(row, col))
        return false;
    
    dataTable.setValue(row, col, newValue);
    return true;
}

// TreeMap data and functions---------------------------------------------------------------

var treeMapDataTable = null;
var treeMapRow2TaskMapping = null;
var treeMapTask2RowMapping = null;

var treeMapChart = null;

// -------------------------------------------------

const HeaderHeight = 30;

const FocusedSelectionFillColor     = '#A0D7FF';
const FocusedSelectionBorderColor   = '#5AB4FF';

const UnfocusedSelectionFillColor   = '#CCE8FF';
const UnfocusedSelectionBorderColor = FocusedSelectionFillColor;

// -------------------------------------------------

class TreeMapRect extends DOMRect
{
    get centreX() { return (this.left + (this.width / 2.0)); };
    get centreY() { return (this.top + (this.height / 2.0)); };
}

// -------------------------------------------------

class TreeMapCellRect extends TreeMapRect
{
    constructor(cell) 
    {
        super();
        let rect = $($(cell).find('rect'));
        
        this.x = Number(rect.attr('x'));
        this.y = Number(rect.attr('y'));
        this.width = Number(rect.attr('width'));
        this.height = Number(rect.attr('height'));
    }
}

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
    treeMapDataTable.addColumn('boolean', 'Done');

    AddTreeMapItem('0', 
                   '', 
                   false, 
                   '#C0C0C0', 
                   'Tasklist');
    
    for (let i = 0; i < tasks.length; i++) 
    {
        AddTaskToTreeMap(tasks[i], '0');
    }
}

function AddTaskToTreeMap(task, parentId)
{
    let id = task['Task ID'].toString();
    
    AddTreeMapItem(id, 
                   parentId, 
                   (task['Completion Date'] != ''), 
                   task['Colour'], 
                   task['Title']);
        
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
        done,
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
        fontSize:        0.1, // to prevent Google replacing task IDs with '…'
        generateTooltip: OnTreeMapGetTooltip,
        headerHeight:    HeaderHeight,
        height:          500,
        maxDepth:        1,
        maxPostDepth:    GetTreeMapDepth(),
        midColor:        '#808080',
        showTooltips:    true,
        
        // Use click to highlight and double-click to drill down.
        eventsConfig: 
        {
          highlight: ['click'],
          rollup:    ['contextmenu'], // right-click
          drilldown: ['dblclick'],
        }
    };

    treeMapChart.draw(treeMapDataTable, options);
}

function RefreshTreeMap()
{
    // Refreshing will return the map to the top level
    // so we'll need to drill back down to where we were
    let headerId = GetStorage('HeaderId');
                
    InitTreeMap();
    DrawTreeMap();
    
    RestoreSelectedTask();
    TreeMapDrilldownTo(headerId, true);
}

function OnResizeTreeMap()
{
    // Drawing will return the map to the top level
    // so we'll need to drill back down to where we were
    let headerId = GetTreeMapHeaderId();
                
    DrawTreeMap();
    TreeMapDrilldownTo(headerId, false);
}

function TreeMapDrilldownTo(id, ensureVisible = true)
{
    if (id)
    {
        SetSelectedChartRow(id, treeMapChart, treeMapTask2RowMapping);
        RefreshTreeMapTextAndColors();
        
        if (ensureVisible)
            EnsureTreeMapSelectionVisible();
    }
}

function OnFocusChangedTreeMap(unused)
{
    RefreshTreeMapTextAndColors(GetSelectedTaskId());
}

function OnKeyDownTreeMap(event)
{
    switch (event.code)
    {
    case 'Enter':
        {
            let headerId = GetTreeMapHeaderId();
            let selId = GetSelectedTaskId();
            
            if (selId == headerId)
            {
                treeMapChart.goUpAndDraw();
                OnTreeMapRollup();
            }
            else
            {
                TreeMapDrilldownTo(selId, true);
            }
        }
        break;
        
    case 'Escape':
        if (IsSelectableTask(GetTreeMapHeaderId()))
        {
            treeMapChart.goUpAndDraw();
            OnTreeMapRollup();
        }
        break;
        
    case 'ArrowLeft':
    case 'ArrowRight':
    case 'ArrowUp':
    case 'ArrowDown':
        if (SelectFirstAdjacentTreeMapTask(event.code))
            event.preventDefault();
        break;
        
    case 'Home':
        if (SelectLastAdjacentTreeMapTask('ArrowLeft'))
            event.preventDefault();
        break;
        
    case 'End':
        if (SelectLastAdjacentTreeMapTask('ArrowRight'))
            event.preventDefault();
        break;
        
    case 'PageUp':
        if (SelectLastAdjacentTreeMapTask('ArrowUp'))
            event.preventDefault();
        break;
        
    case 'PageDown':
        if (SelectLastAdjacentTreeMapTask('ArrowDown'))
            event.preventDefault();
        break;
    }
}

function SelectFirstAdjacentTreeMapTask(keyCode, ensureVisible = true)
{
    let selId = GetSelectedTaskId();
    
    if (!selId)
        return false;
    
    let idCells = InitialiseTreeMapCellIds();
    let selCell = GetTreeMapCell(selId), nextCell = null;
 
    let closestoffset = 1000000000.0;
    
    for (let i = 0; i < idCells.length; i++)
    {
        let cell = idCells[i];
        
        // Root node is not selectable
        if (!IsSelectableTask(GetTreeMapCellId(cell)))
            continue;
        
        let offset = {};
        
        if (IsTreeMapCellAdjacentTo(cell, selCell, keyCode, offset))
        {            
            if (offset.value < closestoffset)
            {
                nextCell = cell;
                closestoffset = offset.value;
            }
        }                        
    }
    
    if (nextCell)
    {
        let nextId = GetTreeMapCellId(nextCell);
        SelectTask(nextId, true);
        
        if (ensureVisible)
            EnsureTreeMapSelectionVisible();
        
        return true;
    }
    
    // all else
    return false;
}

function SelectLastAdjacentTreeMapTask(keyCode)
{
    let selChange = false;
    
    while (SelectFirstAdjacentTreeMapTask(keyCode, false))
        selChange = true;
    
    if (selChange)
        EnsureTreeMapSelectionVisible();
    
    return selChange;
}

function IsTreeMapCellAdjacentTo(cell, otherCell, keyCode, /*out*/ centreOffset)
{
    let rect = new TreeMapCellRect(cell);
    let otherRect = new TreeMapCellRect(otherCell);
    
    switch (keyCode)
    {
    case 'ArrowLeft': // Is 'cell' to left of 'otherCell'?
        if ((rect.right < otherRect.left) && (rect.right > (otherRect.left - 4)))
        {
            centreOffset.value = Math.abs(rect.centreY - otherRect.centreY);
            return true;
        }
        break;
        
    case 'ArrowRight': // Is 'cell' to right of 'otherCell'?
        if ((rect.left > otherRect.right) && (rect.left < (otherRect.right + 4)))
        {
            centreOffset.value = Math.abs(rect.centreY - otherRect.centreY);
            return true;
        }
        break;
        
    case 'ArrowUp': // Is 'cell' above 'otherCell'?
        if ((rect.bottom < otherRect.top) && (rect.bottom > (otherRect.top - 4)))
        {
            centreOffset.value = Math.abs(rect.centreX - otherRect.centreX);
            return true;
        }
        break;
        
    case 'ArrowDown': // Is 'cell' below 'otherCell'?
        if ((rect.top > otherRect.bottom) && (rect.top < (otherRect.bottom + 4)))
        {
            centreOffset.value = Math.abs(rect.centreX - otherRect.centreX);
            return true;
        }
        break;
    }
    
    return false;
}

function OnTreeMapReady()
{
    RefreshTreeMapTextAndColors();
}

function RestoreTreeMapSessionState(state)
{
    if (state)
    {
        let depthState = state[TreeMapDepthKey];
    
        if (depthState && (depthState != GetTreeMapDepth()))
            SetStorage(TreeMapDepthKey, depthState);
    }
}

function GetTreeMapSessionState()
{
    return { [TreeMapDepthKey]: GetTreeMapDepth() };
}

function GetTreeMapDepth()
{
    let depth = GetStorage(TreeMapDepthKey);
    
    if (depth == null)
        return 0;
    
    return Number(depth);
}

function OnChangeTreeMapDepth(depth)
{
    if (Number(depth) != GetTreeMapDepth())
    {
        SetStorage(TreeMapDepthKey, depth);
        DrawTreeMap();
    }
}

function OnTreeMapGetTooltip(row, size, value)
{
    return '<div class="tooltip">' + 
           treeMapDataTable.getValue(row, 5) + 
           '</div>';
}

function OnTreeMapDrilldown()
{
    // This is effectively a double-click handler
    // so select the task just clicked so that it
    // becomes the selected parent in the next level
    let id = GetSelectedIdFromChart(treeMapChart, treeMapRow2TaskMapping);
    
    if (IsSelectableTask(id))
    {
        SelectTask(id, true);
        EnsureTreeMapSelectionVisible();
        
        // Save the id as the new 'header id'
        SetStorage('HeaderId', id);
    }
    
    // Still need this because Google will overwrite our values
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
        selId = treeMapDataTable.getValue(row, 1);
        
        SelectTask(selId, true);
    }
    
    // Save the new 'header id'
    SetStorage('HeaderId', GetTreeMapHeaderId());

    EnsureTreeMapSelectionVisible();
    RefreshTreeMapTextAndColors();
}

// We never call this directly; Only via SelectTask()
// after the selected task id has been saved 
function SelectTreeMapTask(id, prevId = null)
{
    // Navigate to the task if it's not currently visible
    if (!IsTreeMapIdVisible(id))
    {
        TreeMapDrilldownTo(id, true); // Will also refresh text and colors
    }
    else if (prevId == null)
    {
        RefreshTreeMapTextAndColors(); // All cells
    }
    else
    {
        RefreshTreeMapTextAndColors(prevId);
        RefreshTreeMapTextAndColors(id);
    }
    
    EnsureTreeMapSelectionVisible();
}

function GetSelectedTreeMapTaskLabelRect()
{
    EnsureTreeMapSelectionVisible(true);
    
    let selId = GetSelectedTaskId();
    let cell = GetTreeMapCell(selId);
    
    return GetTreeMapCellLabelRect(cell);
}

function GetTreeMapCellLabelRect(cell)
{
    if (cell)
    {
        let fo = $(cell).find('foreignObject');
        
        if (fo)
        {
            let labelRect = new DOMRect;
            
            labelRect.x = $(fo).attr('labelRect_x');
            labelRect.y = $(fo).attr('labelRect_y');
            labelRect.width = $(fo).attr('labelRect_width');
            labelRect.height = $(fo).attr('labelRect_height');
            
            // Convert to viewport coords
            // Note: This takes the scrolled state into account
            let bbox = cell.getBoundingClientRect();
            let rect = $(cell).find('rect');
            
            const padding = 2;
                    
            labelRect.x += (bbox.x - $(rect).attr('x') - padding);
            labelRect.y += (bbox.y - $(rect).attr('y') - padding);
            labelRect.width += (2 * padding);
            labelRect.height += (2 * padding);
        
            return labelRect;
        }
    }
    
    return null;
}

function EnsureTreeMapSelectionVisible(labelOnly = false)
{
    let selId = GetSelectedTaskId();
    let cell = GetTreeMapCell(selId);
    
    if (cell)
    {
        // Since we have largely eliminated the horizontal
        // scrollbar it's only necessary to scroll in 'y'
        // And we only scroll if the item is not entirely visible
        let clientHeight = document.documentElement.clientHeight;
        let cellRect = (labelOnly ? GetTreeMapCellLabelRect(cell) : cell.getBoundingClientRect());
        
        const xScroll = document.documentElement.scrollLeft;
        let yScroll = document.documentElement.scrollTop;
        
        if (cellRect.top < 0) // partly above the client rect
        {
            yScroll += cellRect.top;
        }
        else if (cellRect.bottom > clientHeight) // partly below the client rect 
        {
            yScroll += (cellRect.bottom - clientHeight);
        }
        
        if (yScroll != document.documentElement.scrollTop)
            window.scroll(xScroll, yScroll);
    }
}

function IsTreeMapIdVisible(id)
{
    return (GetTreeMapCell(id) != null);
}

function GetTreeMapCellId(cell)
{
    if ($(cell).attr('style') == null)
        return '-1';
    
    let id = $(cell).find('foreignObject').attr('id');
        
    if (id == null)
        id = $(cell).find('text').text(); // default
    
    return id;
}

function GetTreeMapCell(id)
{
    let treechart = $("#treemap_id");
    let svg = treechart.find("svg");
    let cells = svg.find("g");
    
    for (let i = 0; i < cells.length; i++)
    {
        if (GetTreeMapCellId(cells[i]) == id)
            return cells[i];
    }
    
    return null;
}

function GetTreeMapFullPath(id)
{
    let idPath = GetTreeMapFullIdPath(id);
    let fullPath = new Array();
    
    while (idPath.length > 0)
    {
        let id = idPath.pop();
        let row = treeMapTask2RowMapping[id];
        
        fullPath.push(treeMapDataTable.getValue(row, 5));
    }
    
    return fullPath.join(" \\ ");
}

function GetTreeMapFullIdPath(id)
{
    let idPath = [];
    
    while (id && !Number.isNaN(id))
    {
        idPath.push(id);
        
        let row = treeMapTask2RowMapping[id];
        id = treeMapDataTable.getValue(row, 1); // parent id
    }
    
    return idPath;
}

// Effectively the 'OnMouseClick' handler
function OnTreeMapHighlight(item)
{
    let row = item["row"];
    let id = treeMapRow2TaskMapping[row];

    if (IsSelectableTask(id) && (id != GetSelectedTaskId()))
    {
        SelectTask(id, true);
        EnsureTreeMapSelectionVisible(true);
    }
    else
    {
        RefreshTreeMapTextAndColors(id); // still required because Google!
    }
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
    let colorTaskBkgnd = (GetPreference('ColorTaskBackground', false) == true);
    let strikethruDone = (GetPreference('StrikethroughDone', true) == true);
    
    let headerId = GetTreeMapHeaderId(); // will also initialise cell Ids
    let selId = GetSelectedTaskId();
    
    let treechart = $("#treemap_id");
    let svg = treechart.find("svg");
    let cells = svg.find("g");
    
    for (let i = 0; i < cells.length; i++)
    {
        let cell = cells[i];

        // prevent selection border when tooltip are active
        $(cell).css('outline', 'none'); 
      
        // Reduce width/height to create a gap between cells
        let rect = $(cell).find('rect');
        let fo = $(cell).find('foreignObject');
            
        if (!fo.attr('rectAdjusted'))
        {
            let width = ($(rect).attr('width') - 2); // 2 => 1 pixel
            let height = ($(rect).attr('height') - 2); // 2 => 1 pixel
            
            // Clip rects to 'y = 0'
            let y = $(rect).attr('y');
            
            if (y < 0)
            {
                height += y;
                y = 0;
            }
            
            $(rect).attr('width', width) 
                   .attr('height', height)
                   .attr('y', y)
                   .attr('shape-rendering', 'crispEdges');
                   
            $(rect).css('stroke-width', '1px')
                   .css('cursor', 'default'); // Hide 'hand' cursor because we use double-clicking to drill down
            
            $(fo).attr('rectAdjusted', true);
        }
        
        // Hide the default highlight Element
        if ($(cell).children().length > 3)
        {
            $(cell).css('display', 'none');
        }
        
        // Process only those cells having a task Id
        let id = GetTreeMapCellId(cell);
            
        if (id && (!specificId || (id == specificId)))
        {
            let row = treeMapTask2RowMapping[id];
            
            if (row == null)
                row = 0;
         
            // Render task 'box'
            let baseColor = treeMapDataTable.getValue(row, 4);
            let fillColor = "";
            let fillOpacity = Number($(rect).css('fill-opacity'));
            let borderColor = "";
            
            const SelectionOpacity = 0.85;
            const NonSelectionOpacity = 0.5;
            
            if (id == selId)
            {
                if (document.hasFocus())
                {
                    fillColor = FocusedSelectionFillColor;
                    borderColor = FocusedSelectionBorderColor;
                }
                else
                {
                    fillColor = UnfocusedSelectionFillColor;
                    borderColor = UnfocusedSelectionBorderColor;
                }
                
                // Increase opacity of selected task color
                // because it's easily lost
                fillOpacity = Math.max(fillOpacity, SelectionOpacity);
            }
            else
            {
                if (fillOpacity == SelectionOpacity)
                    fillOpacity = NonSelectionOpacity;
                
                if (baseColor == '')
                {
                    fillColor = 'White'.toHexColor();
                    borderColor = 'Gray'.toHexColor();
                }
                else if ((id == 0) || colorTaskBkgnd)
                {
                    fillColor = baseColor;
                    borderColor = baseColor;
                }
                else
                {
                    fillColor = baseColor.lighten(45);
                    borderColor = baseColor;
                }
            }
            
            $(rect).css('fill', fillColor)
                   .css('fill-opacity', fillOpacity)
                   .css('stroke', borderColor)
                   .css('stroke-opacity', fillOpacity);

            // Render task text
            let text = $(cell).find('text');
            
            if ($(text)[0])
            {
                let title = ((id == headerId) ? GetTreeMapFullPath(id) : treeMapDataTable.getValue(row, 5));
                let textDecoration = ((strikethruDone && (treeMapDataTable.getValue(row, 6) == 1)) ? 'line-through' : "");
                let textColor = GetTreeMapTextColor(baseColor, (id == selId), colorTaskBkgnd);
                
                $(text).css('user-select', 'none') // Prevent double-click from selecting textColor
                       .css('fill', textColor)
                       .css('text-decoration', textDecoration)
                       .text(title);

                // Modify the text to fit the rect width
                let availWidth = ($(rect).attr('width') - 4); // add padding
                let labelRect = $(text)[0].getBBox();
                
                while (labelRect.width > availWidth)
                {
                    title = title.substring(0, (title.length - 4));
                    $(text).text(title + '...');
                    
                    if (title.length == 0)
                        break;
                        
                    labelRect = $(text)[0].getBBox();
                }
                
                // Save the label rect so it's available when the app
                // queries for it via GetSelectedTaskLabelRect
                if (!fo.attr('labelRect_x'))
                {
                    $(fo).attr('labelRect_x', labelRect.x);
                    $(fo).attr('labelRect_y', labelRect.y);
                    $(fo).attr('labelRect_width', labelRect.width);
                    $(fo).attr('labelRect_height', labelRect.height);
                }
                       
                // Because we set the font size to 0.1 to get around
                // the problem where Google auto-added ellipses, the
                // header text is mis-aligned vertically
                if ((id == headerId) && ($(rect).attr('y') == '0'))
                {
                    let textHeight = labelRect.height;
                    let rectHeight = Number($(rect).attr('height'));
                    
                    let y = (((rectHeight - textHeight) / 2) + textHeight);
                    $(text).attr('y', y);
                }
            }
        }
    }    
}

function InitialiseTreeMapCellIds()
{
    let treechart = $("#treemap_id");
    let svg = treechart.find("svg");
    let cells = svg.find("g");
    let idCells = new Array();
    
    for (let i = 0; i < cells.length; i++)
    {
        let cell = cells[i];
        let fo = $(cell).find('foreignObject');
        let id = $(fo).attr('id');
        
        if (id == null) // First time ONLY
        {
            fo = document.createElementNS('http://www.w3.org/2000/svg', 'foreignObject' );
            
            id = $(cell).find('text').text(); // default
        
            if (id == '…')
            {
                // This means that Google has overwritten the 
                // task ID and our whole solution fails for this
                // cell. 
                // I fixed it by reducing the font size to zero but
                // will leave this in to catch any future occurrence
                alert('Bad Task ID');
            }
            
            if (Number.isNaN(id))
                $(fo).attr('id', ''); // avoid further processing
            else
                $(fo).attr('id', id);
            
            $(cell).append(fo);
        }        
        
        if (id != '')
            idCells.push(cell);
    }
    
    return idCells;
}

function GetTreeMapHeaderId() 
{
    let colorTaskBkgnd = (GetPreference('ColorTaskBackground', false) == true);
    let strikethruDone = (GetPreference('StrikethroughDone', true) == true);
    
    let idCells = InitialiseTreeMapCellIds();
    
    if (idCells.length == 1)
    {
        // Must exist
        return $(idCells[0]).find('foreignObject').attr('id');
    }
    
    if (idCells.length > 1)
    {
        // Note: We work in reverse because observation suggests that
        //       the header task (always) comes last
        idCells = idCells.reverse();
        let lastIndex = (idCells.length - 1);
        
        for (let i = 0; i < idCells.length; i++)
        {
            let cell = idCells[i];
            let nextCell = ((i == lastIndex) ? idCells[0] : idCells[i + 1]);
            
            let id = GetTreeMapCellId(cell);
            let nextId = GetTreeMapCellId(nextCell);
            
            let row = treeMapTask2RowMapping[id];
            let nextRow = treeMapTask2RowMapping[nextId];
            
            let pid = treeMapDataTable.getValue(row, 1);
            let nextPid = treeMapDataTable.getValue(nextRow, 1);
            
            if (pid != nextPid)
            {
                if (pid == nextId)
                    return nextId;
                
                // else
                return id;
            }
        }
    }
    
    return '';
}

function GetTreeMapTextColor(baseColor, selected, colorTaskBkgnd)
{
    if (baseColor == "")
        return 'Black'.toHexColor();
    
    if (selected)
    {
        // match core app by setting luminance 
        // to a maximum value of 30%
        let hsl = baseColor.toHSL();
        hsl[2] = Math.min(30, hsl[2]);
        
        return ColorHelper.hslToHexColor(hsl);
    }
    
    if (colorTaskBkgnd)
    {
        let rgb = baseColor.toRGB();
        let grey = ((rgb[2] + (rgb[1] * 6) + (rgb[0] * 3)) / 10);
        
        return ((grey < 128) ? 'White'.toHexColor() : 'Black'.toHexColor());
    }
    
    // else
    return baseColor;
}
