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
const TreeMapViewId   = "treemap_id";

// DashboardView class //////////////////////////////////////////////////////////////////////////

class DashboardView
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
    
    static Id = "dashboard_id";

    // -----------------------------------------
    
    constructor()
    {
        // Do nothing for cost-less instantiation
    }

    /* void */ #Initialise() 
    {
        if (this.#dashboardChart11 == null)
        {
            this.#dashboardChart11 = new google.visualization.BarChart    (document.getElementById('dashboard_chart11'));
            this.#dashboardChart12 = new google.visualization.ScatterChart(document.getElementById('dashboard_chart12'));
            this.#dashboardChart21 = new google.visualization.AreaChart   (document.getElementById('dashboard_chart21'));
            this.#dashboardChart22 = new google.visualization.ColumnChart (document.getElementById('dashboard_chart22'));
            
            google.visualization.events.addListener(this.#dashboardChart11, 'select', this.#On11Select.bind(this));
            google.visualization.events.addListener(this.#dashboardChart12, 'select', this.#On12Select.bind(this));
            google.visualization.events.addListener(this.#dashboardChart21, 'select', this.#On21Select.bind(this));
            google.visualization.events.addListener(this.#dashboardChart22, 'select', this.#On22Select.bind(this));
            
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

    /* bool */ UpdateSelectedTasks(selTasks, redraw)
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
            
            if (changed && redraw)
                this.#Draw();
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

    /* void */ #On11Select(e)
    {
        this.#OnSelectTask(this.#dashboardChart11);
    }

    /* void */ #On12Select(e)
    {
        this.#OnSelectTask(this.#dashboardChart12);
    }

    /* void */ #On21Select(e)
    {
        this.#OnSelectTask(this.#dashboardChart21);
    }

    /* void */ #On22Select(e)
    {
        this.#OnSelectTask(this.#dashboardChart22);
    }

    /* void */ #OnSelectTask(chart)
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
        this.#Initialise();
        this.#Draw();
        
        RestoreSelectedTask();
    }

    /* void */ OnResize()
    {
        this.#Draw();
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
                  
    /* void */ #Draw()
    {
        this.#DrawChart(this.#dashboardChart11, 'Red', 'Blue');
        this.#DrawChart(this.#dashboardChart12, 'Green', 'Orange');
        this.#DrawChart(this.#dashboardChart21, 'Yellow', 'Coral');
        this.#DrawChart(this.#dashboardChart22, 'Purple', 'Turquoise');
    }

    /* void */ #DrawChart(chart, color1, color2) 
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

} // DashboardView class

// TreeMapView classes //////////////////////////////////////////////////////////////////

const TreeMapDepthKey = "TreeMapDepth"; // Session state

// -------------------------------------------------------------------------------------

class TreeMapCellRect extends DOMRect
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
    
    get centreX() { return (this.left + (this.width / 2.0)); };
    get centreY() { return (this.top + (this.height / 2.0)); };
}

// -------------------------------------------------------------------------------------

class TreeMapView
{
    // All data is private
    #chart = null;              // google.visualization.TreeMap
    #dataTable = null;          // google.visualization.DataTable
    
    #row2TaskMapping = null;    // Map<row, id>
    #task2RowMapping = null;    // Map<id, row>

    // -------------------------------------------------

    static Id = 'treemap_id';
    
    // -------------------------------------------------
    
    constructor()
    {
        // Do nothing for cost-less instantiation
    }

    /* void */ #Initialise()
    {
        if (this.#chart == null)
        {
            this.#chart = new google.visualization.TreeMap(document.getElementById('treemap_chart'));

            google.visualization.events.addListener(this.#chart, "ready",       this.#OnReady.bind(this));
            google.visualization.events.addListener(this.#chart, "drilldown",   this.#OnDrilldown.bind(this));
            google.visualization.events.addListener(this.#chart, "rollup",      this.#OnRollup.bind(this));
            google.visualization.events.addListener(this.#chart, "highlight",   this.#OnHighlight.bind(this));
            google.visualization.events.addListener(this.#chart, "unhighlight", this.#OnUnhighlight.bind(this));

            this.#Populate();
        }
    }

    /* void */ #Populate()
    {
        this.#dataTable = new google.visualization.DataTable();

        this.#row2TaskMapping = new Map();
        this.#task2RowMapping = new Map();

        this.#dataTable.addColumn('string', 'TaskId');
        this.#dataTable.addColumn('string', 'ParentId');
        this.#dataTable.addColumn('number', 'Size');
        this.#dataTable.addColumn('number', 'ColorVal');
        this.#dataTable.addColumn('string', 'WebColor');
        this.#dataTable.addColumn('string', 'Title');
        this.#dataTable.addColumn('boolean', 'Done');

        // Root
        this.#AddRow('0', 
                     '', 
                     false, 
                     '#C0C0C0', 
                     'Tasklist');
        
        // Top-level tasks
        for (let i = 0; i < tasks.length; i++) 
        {
            this.#AddTask(tasks[i], '0');
        }
    }

    /* void */ #AddTask(task, parentId)
    {
        let id = task['Task ID'].toString();
        
        this.#AddRow(id, 
                     parentId, 
                     (task['Completion Date'] != ''), 
                     task['Colour'], 
                     ('[' + id + '] ' + task['Title']));
            
        if (task.Subtasks != null)
        {
            for (let i = 0; i < task.Subtasks.length; i++) 
            {
                this.#AddTask(task.Subtasks[i], id); // RECURSIVE CALL
            }
        }
    }

    /* void */ #AddRow(id, parentId, done, color, title)
    {
        this.#dataTable.addRow(
        [
            id,
            parentId, 
            1, // size
            1, // colorVal
            color,
            title,
            done,
        ]);
        
        let row = (this.#dataTable.getNumberOfRows() - 1);
        this.#row2TaskMapping[row] = id;
        this.#task2RowMapping[id] = row;
    }

    /* void */ UpdateSelectedTasks(selTasks)
    {
        let changed = false;
            
        // Only if we've been already populated
        if (this.#task2RowMapping)
        {
            for (let i = 0; i < selTasks.length; i++) 
            {
                let selTask = selTasks[i];
                let id = selTask['Task ID'].toString();
                let row = this.#task2RowMapping[id];
                
                if (row != null)
                {
                    changed |= CheckUpdateDataValue(this.#dataTable, row, 5, selTask.Title);
                    changed |= CheckUpdateDataValue(this.#dataTable, row, 4, selTask.Colour);
                    
                    if (selTask.SubTasks != null)
                        changed |= this.UpdateSelectedTasks(selTask.SubTasks); // Recursive call
                }
            }
        }
        
        return changed;
    }

    /* void */ #Draw() 
    {
        const HeaderHeight = 30;

        let options = 
        {
            enableHighlight: false,
            fontSize:        0.1, // to prevent Google replacing task IDs with '…'
            generateTooltip: this.#OnGetTooltip.bind(this),
            headerHeight:    HeaderHeight,
            height:          500,
            maxDepth:        1,
            maxPostDepth:    this.GetSubtaskDepth(),
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

        this.#chart.draw(this.#dataTable, options);
    }

    /* void */ Refresh()
    {
        // Refreshing will return the map to the top level
        // so we'll need to drill back down to where we were
        let headerId = GetStorage('HeaderId');
                    
        this.#Initialise();
        this.#Draw();
        
        RestoreSelectedTask();
        this.#DrilldownTo(headerId, true);
    }

    /* void */ OnResize()
    {
        // Drawing will return the map to the top level
        // so we'll need to drill back down to where we were
        let headerId = this.#GetHeaderId();
                    
        this.#Draw();
        this.#DrilldownTo(headerId, false);
    }

    /* void */ #DrilldownTo(id, ensureVisible = true)
    {
        if (id)
        {
            SetSelectedChartRow(id, this.#chart, this.#task2RowMapping);
            this.#RefreshTextAndColors();
            
            if (ensureVisible)
                this.#EnsureSelectionVisible();
        }
    }

    /* void */ OnFocusChanged(unused)
    {
        this.#RefreshTextAndColors(GetSelectedTaskId());
    }

    /* void */ OnKeyDown(event)
    {
        switch (event.code)
        {
        case 'Enter':
            {
                let headerId = this.#GetHeaderId();
                let selId = GetSelectedTaskId();
                
                if (selId == headerId)
                {
                    this.#chart.goUpAndDraw();
                    this.#OnRollup();
                }
                else
                {
                    this.#DrilldownTo(selId, true);
                }
            }
            break;
            
        case 'Escape':
            if (IsSelectableTask(this.#GetHeaderId()))
            {
                this.#chart.goUpAndDraw();
                this.#OnRollup();
            }
            break;
            
        case 'ArrowLeft':
        case 'ArrowRight':
        case 'ArrowUp':
        case 'ArrowDown':
            if (this.#SelectFirstAdjacentTask(event.code))
                event.preventDefault();
            break;
            
        case 'Home':
            if (this.#SelectLastAdjacentTask('ArrowLeft'))
                event.preventDefault();
            break;
            
        case 'End':
            if (this.#SelectLastAdjacentTask('ArrowRight'))
                event.preventDefault();
            break;
            
        case 'PageUp':
            if (this.#SelectLastAdjacentTask('ArrowUp'))
                event.preventDefault();
            break;
            
        case 'PageDown':
            if (this.#SelectLastAdjacentTask('ArrowDown'))
                event.preventDefault();
            break;
        }
    }

    /* void */ #SelectFirstAdjacentTask(keyCode, ensureVisible = true)
    {
        let selId = GetSelectedTaskId();
        
        if (!selId)
            return false;
        
        let idCells = this.#InitialiseCellIds();
        let selCell = this.#GetCell(selId), nextCell = null;
     
        let closestoffset = 1000000000.0;
        
        for (let i = 0; i < idCells.length; i++)
        {
            let cell = idCells[i];
            
            // Root node is not selectable
            if (!IsSelectableTask(this.#GetCellId(cell)))
                continue;
            
            let offset = {};
            
            if (this.#IsCellAdjacentTo(cell, selCell, keyCode, offset))
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
            let nextId = this.#GetCellId(nextCell);
            SelectTask(nextId, true);
            
            if (ensureVisible)
                this.#EnsureSelectionVisible();
            
            return true;
        }
        
        // all else
        return false;
    }

    /* void */ #SelectLastAdjacentTask(keyCode)
    {
        let selChange = false;
        
        while (this.#SelectFirstAdjacentTask(keyCode, false))
            selChange = true;
        
        if (selChange)
            this.#EnsureSelectionVisible();
        
        return selChange;
    }

    /* void */ #IsCellAdjacentTo(cell, otherCell, keyCode, /*out*/ centreOffset)
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

    /* void */ #OnReady()
    {
        this.#RefreshTextAndColors();
    }

    /* void */ RestoreSessionState(state)
    {
        if (state)
        {
            let depthState = state[TreeMapDepthKey];
        
            if (depthState && (depthState != this.GetSubtaskDepth()))
                SetStorage(TreeMapDepthKey, depthState);
        }
    }

    /* string */ GetSessionState()
    {
        return { [TreeMapDepthKey]: this.GetSubtaskDepth() };
    }

    /* int */ GetSubtaskDepth()
    {
        let depth = GetStorage(TreeMapDepthKey);
        
        if (depth == null)
            return 0;
        
        return Number(depth);
    }

    /* void */ SetSubtaskDepth(depth)
    {
        if (depth != this.GetSubtaskDepth())
        {
            SetStorage(TreeMapDepthKey, depth);
            this.#Draw();
        }
    }

    /* string */ #OnGetTooltip(row, size, value)
    {
        return '<div class="tooltip">' + 
               this.#dataTable.getValue(row, 5) + 
               '</div>';
    }

    /* void */ #OnDrilldown()
    {
        // This is effectively a double-click handler
        // so select the task just clicked so that it
        // becomes the selected parent in the next level
        let id = GetSelectedIdFromChart(this.#chart, this.#row2TaskMapping);
        
        if (IsSelectableTask(id))
        {
            SelectTask(id, true);
            this.#EnsureSelectionVisible();
            
            // Save the id as the new 'header id'
            SetStorage('HeaderId', id);
        }
        
        // Still need this because Google will overwrite our values
        this.#RefreshTextAndColors();
    }

    /* void */ #OnRollup()
    {
        // If the currently selected item is no longer visible
        // move the selection to its parent
        let selId = GetSelectedTaskId();
        
        if (!this.#IsCellVisible(selId))
        {
            let row = this.#task2RowMapping[selId];
            selId = this.#dataTable.getValue(row, 1);
            
            SelectTask(selId, true);
        }
        
        // Save the new 'header id'
        SetStorage('HeaderId', this.#GetHeaderId());

        this.#EnsureSelectionVisible();
        this.#RefreshTextAndColors();
    }

    // We never call this directly; Only via SelectTask()
    // after the selected task id has been saved 
    /* void */ SelectTask(id, prevId = null)
    {
        // Navigate to the task if it's not currently visible
        if (!this.#IsCellVisible(id))
        {
            this.#DrilldownTo(id, true); // Will also refresh text and colors
        }
        else if (prevId == null)
        {
            this.#RefreshTextAndColors(); // All cells
        }
        else
        {
            this.#RefreshTextAndColors(prevId);
            this.#RefreshTextAndColors(id);
        }
        
        this.#EnsureSelectionVisible();
    }

    /* DOMRect */ GetSelectedTaskLabelRect()
    {
        this.#EnsureSelectionVisible(true);
        
        let selId = GetSelectedTaskId();
        let cell = this.#GetCell(selId);
        
        return this.#GetCellLabelRect(cell);
    }

    /* DOMRect */ #GetCellLabelRect(cell)
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

    /* void */ #EnsureSelectionVisible(labelOnly = false)
    {
        let selId = GetSelectedTaskId();
        let cell = this.#GetCell(selId);
        
        if (cell)
        {
            // Since we have largely eliminated the horizontal
            // scrollbar it's only necessary to scroll in 'y'
            // And we only scroll if the item is not entirely visible
            let clientHeight = document.documentElement.clientHeight;
            let cellRect = (labelOnly ? this.#GetCellLabelRect(cell) : cell.getBoundingClientRect());
            
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
            {
                window.scroll({
                    top: [yScroll], 
                    left: [yScroll], 
                    //behavior: 'smooth' 
                    });
            }
        }
    }

    /* void */ #IsCellVisible(id)
    {
        return (this.#GetCell(id) != null);
    }

    /* void */ #GetCellId(cell)
    {
        if ($(cell).attr('style') == null)
            return '-1';
        
        let id = $(cell).find('foreignObject').attr('id');
            
        if (id == null)
            id = $(cell).find('text').text(); // default
        
        return id;
    }

    /* void */ #GetCell(id)
    {
        let treechart = $("#treemap_id");
        let svg = treechart.find("svg");
        let cells = svg.find("g");
        
        for (let i = 0; i < cells.length; i++)
        {
            if (this.#GetCellId(cells[i]) == id)
                return cells[i];
        }
        
        return null;
    }

    /* void */ #GetFullPath(id)
    {
        let idPath = this.#GetFullIdPath(id);
        let fullPath = new Array();
        
        while (idPath.length > 0)
        {
            let id = idPath.pop();
            let row = this.#task2RowMapping[id];
            
            fullPath.push(this.#dataTable.getValue(row, 5));
        }
        
        return fullPath.join(" \\ ");
    }

    /* void */ #GetFullIdPath(id)
    {
        let idPath = [];
        
        while (id && !Number.isNaN(id))
        {
            idPath.push(id);
            
            let row = this.#task2RowMapping[id];
            id = this.#dataTable.getValue(row, 1); // parent id
        }
        
        return idPath;
    }

    // Effectively the 'OnMouseClick' handler
    /* void */ #OnHighlight(item)
    {
        let row = item["row"];
        let id = this.#row2TaskMapping[row];

        if (IsSelectableTask(id) && (id != GetSelectedTaskId()))
        {
            SelectTask(id, true);
            this.#EnsureSelectionVisible(true);
        }
        else
        {
            this.#RefreshTextAndColors(id); // still required because Google!
        }
    }

    /* void */ #OnUnhighlight(item)
    {
        // Even though we have specified 'enableHighlight: false'
        // Google still restores text and colours on 'mouse-leave'
        let row = item["row"];
        let id = this.#row2TaskMapping[row];
        
        this.#RefreshTextAndColors(id);
    }

    /* void */ #RefreshTextAndColors(specificId) 
    {
        let colorTaskBkgnd = (GetPreference('ColorTaskBackground', false) == true);
        let strikethruDone = (GetPreference('StrikethroughDone', true) == true);
        
        let headerId = this.#GetHeaderId(); // will also initialise cell Ids
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
            let id = this.#GetCellId(cell);
                
            if (id && (!specificId || (id == specificId)))
            {
                let row = this.#task2RowMapping[id];
                
                if (row == null)
                    row = 0;
             
                // Render task 'box'
                let baseColor = this.#dataTable.getValue(row, 4);
                let fillColor = "";
                let fillOpacity = Number($(rect).css('fill-opacity'));
                let borderColor = "";
                
                const SelectionOpacity = 0.85;
                const NonSelectionOpacity = 0.5;
                
                const FocusedSelectionFillColor     = '#A0D7FF';
                const FocusedSelectionBorderColor   = '#5AB4FF';

                const UnfocusedSelectionFillColor   = '#CCE8FF';
                const UnfocusedSelectionBorderColor = FocusedSelectionFillColor;

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
                    let title = ((id == headerId) ? this.#GetFullPath(id) : this.#dataTable.getValue(row, 5));
                    let textDecoration = ((strikethruDone && (this.#dataTable.getValue(row, 6) == 1)) ? 'line-through' : "");
                    let textColor = this.#GetTextColor(baseColor, (id == selId), colorTaskBkgnd);
                    
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

    /* void */ #InitialiseCellIds()
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

    /* void */ #GetHeaderId() 
    {
        let colorTaskBkgnd = (GetPreference('ColorTaskBackground', false) == true);
        let strikethruDone = (GetPreference('StrikethroughDone', true) == true);
        
        let idCells = this.#InitialiseCellIds();
        
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
                
                let id = this.#GetCellId(cell);
                let nextId = this.#GetCellId(nextCell);
                
                let row = this.#task2RowMapping[id];
                let nextRow = this.#task2RowMapping[nextId];
                
                let pid = this.#dataTable.getValue(row, 1);
                let nextPid = this.#dataTable.getValue(nextRow, 1);
                
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

    /* void */ #GetTextColor(baseColor, selected, colorTaskBkgnd)
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
} // TreeMapView class

// Global instances -----------------------------------------------------------------------

let dashboard = new DashboardView();
let treemap   = new TreeMapView();

// External functions called by the app ---------------------------------------------------
function GetSelectedTaskLabelRect()
{
    let viewId = GetSelectedViewId();
    let rect = null;

    switch (viewId)
    {
        case DashboardView.Id:
            rect = dashboard.GetSelectedTaskLabelRect();
            break;
              
        case TreeMapView.Id:
            rect = treemap.GetSelectedTaskLabelRect();
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
    return { [SelectedViewKey] : GetSelectedViewId(), 
             [DashboardView.Id]: dashboard.GetSessionState(),
             [TreeMapView.Id]  : treemap.GetSessionState() };
}

// General data and functions -------------------------------------------------------------

function OnLoad()
{
    if (!SupportsHTML5Storage())
        alert('local storage not supported');

    if (GetPreference('BackColor', null))
        document.body.style.backgroundColor = GetPreference('BackColor', null);
  
    chrome.webview.addEventListener('message', OnAppMessage);
    
    let viewId = GetSelectedViewId();
    OnChangeView(viewId);
}

function OnChangeView(viewId) 
{
    SetStorage(SelectedViewKey, viewId);
    
    ShowHideView(DashboardView.Id, viewId);
    ShowHideView(TreeMapView.Id, viewId);
    // New views here
    
    // Dependent UI
    switch (viewId)
    {
        case TreeMapView.Id:
            document.getElementById('treemapdepth_combo').style.display = 'inline-block';
            document.getElementById('treemapdepth').value = treemap.GetSubtaskDepth().toString();
            break;
            
        default:
            document.getElementById('treemapdepth_combo').style.display = 'none';
            break;
    }
    
    RefreshSelectedView();
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

function ShowHideView(divName, viewId)
{
    let show = (viewId == divName);
    let display = (show ? 'block' : 'none');
    
    document.getElementById(divName).style.display = display;
}
        
function GetSelectedViewId()
{
    let viewId = GetStorage(SelectedViewKey);
    
    if ((viewId == null) || (viewId == "undefined"))
        viewId = DashboardView.Id;
    
    return viewId;
}

function RefreshSelectedView()
{
    let viewId = GetSelectedViewId();
    document.getElementById('views').value = viewId;

    switch (viewId)
    {
        case DashboardView.Id:
            dashboard.Refresh();
            break;
              
        case TreeMapView.Id:
            treemap.Refresh();
            break;
    }
}

function OnChangeSubtaskDepth(depth)
{
    treemap.SetSubtaskDepth(Number(depth))
}

function OnResize(event)
{
    let viewId = GetSelectedViewId();

    switch (viewId)
    {
        case DashboardView.Id:
            dashboard.OnResize();
            break;
              
        case TreeMapView.Id:
            treemap.OnResize();
            break;
    }
}

function OnKeyDown(event)
{
    let viewId = GetSelectedViewId();

    switch (viewId)
    {
        case DashboardView.Id:
            dashboard.OnKeyDown(event);
            break;
              
        case TreeMapView.Id:
            treemap.OnKeyDown(event);
            break;
    }
}

function OnFocusChanged(event)
{
    let hasFocus = (event.type == 'focus');
    let viewId = GetSelectedViewId();

    switch (viewId)
    {
        case DashboardView.Id:
            dashboard.OnFocusChanged(hasFocus);
            break;
              
        case TreeMapView.Id:
            treemap.OnFocusChanged(hasFocus);
            break;
    }
}

function MergeSelectedTaskAttributes(selTasks)
{
    // Keep all views up to date but only redraw the active view
    let viewId = GetSelectedViewId();
    
    treemap.UpdateSelectedTasks(selTasks, (viewId == TreeMapView.Id));
    dashboard.UpdateSelectedTasks(selTasks, (viewId == DashboardView.Id));
    
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
    dashboard.RestoreSessionState(state[DashboardView.Id]);
    treemap.RestoreSessionState(state[TreeMapView.Id]);

    let viewState = state[SelectedViewKey];
    
    if (viewState && (viewState != GetSelectedViewId()))
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

    switch (GetSelectedViewId())
    {
        case DashboardView.Id:
        case '':
            dashboard.SelectTask(id, prevId);
            break;
              
        case TreeMapView.Id:
            treemap.SelectTask(id, prevId);
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
