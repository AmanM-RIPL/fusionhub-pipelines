import QtQuick
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
//import Gantt 1.0
import QtCharts
import com.fh.controllers




Rectangle {
    id:schedule_root
    width: 1440
    //height: 1024 - (131 + 13)
    height: screen.height-100
    color: "#EDF1F4"
    property string txtProjectName: "NA"

    property date startDate: new Date(2025, 0, 15) // Jan 15, 2025 (months are 0-based)
    property date endDate: new Date(2025, 0, 20)   // Jan 20, 2025
    property date selectedDate: new Date() // Initialize with today's date

    property var monthModel: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'June', 'July', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']

    property var task_month_paramList: []
    property var date_task_num_mapping: ({}) // { date: Date() numOfTasks: number } if numOfTasks > 4 then we show 'More' button

    property var taskModel: []
    property var taskNameModel: []

    property int approvedNo: 0

    property var colorMap: ({})


    TaskController {
        id: taskController
    }


    ListModel {
        id: yearModel
    }


    ListModel {
        id: highlightedDatesModel
        /*
        ListElement { year: 2025; month: 0; day: 3; color: "red" }
        */
    }

    ListModel {
        id: taskToRenderModel
    }

    /**********New Task Popup Start*****************/

    /************end New Task Popup****************/

    Item {
        width: 200
        height: 80
        id:itemId


        Image{
            source: "qrc:/resources/images/backArrow.svg"
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.top: parent.top
            anchors.topMargin: 25

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true
                onClicked: {
                    baseLayout.visible = false
                    WelcomePage.visible = true
                }
            }
        }


        Text{
            id:idText
            text: "Calendar: " + txtProjectName

            color: "#000000"
            font.pixelSize: 44
            font.weight: 700
            anchors.left: parent.left
            anchors.leftMargin: 60
            anchors.top: parent.top
            // anchors.topMargin: 21
        }

        CustomButton {
            color: "#007AFF"
            width: 110
            height: 38
            radius: 4
            btnSource: "qrc:/resources/images/addWhite_icon.png"
            btnName: "New Task"
            btnNameColor: "white"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 340
            visible: false


            MouseArea{
                anchors.fill: parent

                onClicked: {
                   // newVendorPopup.open();
                }
            }
        }

    }//End of Item

    Rectangle{
        id:idApprovalRect
        width: 500
        height: 50
        color: "transparent"
        //anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: itemId.bottom

    Row {
        spacing: 20
        Text{
            id: approvalTypeLabel
            text: "Choose Approval Type"
            color: "#323130"
            font.weight: 700
            font.pixelSize: 14
            font.family: "Segoe UI"
            topPadding: 10
            leftPadding: 35
        }

        CustomComboBox {
            id: approvalTypeComboBox
            model: ["Approved", "Draft", "All"]
            width:200

            onCurrentTextChanged: {
                if(approvalTypeComboBox.currentText === "Approved"){
                    approvedNo = 0;
                }
                else if(approvalTypeComboBox.currentText === "Draft"){
                    approvedNo = 1;
                }
                else
                {
                    approvedNo = 2;
                }

                loadAllTasks();
                showColor(year.currentText, month.currentIndex)
                generateTaskToRenderList(year.currentText, month.currentIndex);
            }
        }
    }
    }


    Rectangle {
        id:idMainRect
        width: 1300//1236
        //height: 599
        height: 400
        radius: 8
        border.color: "#E0E0E0"
        border.width: 2
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: idApprovalRect.bottom


        ScrollView {
            id: horizontalScrollView
            width: parent.width
            height: parent.height
            padding: 1
            clip: true


            ColumnLayout{
                width:parent.width
                height: parent.height
                spacing: 1

                Rectangle {
                    id:idHeader
                    width: parent.width
                    height: 80
                    color: "#E0E0E0" //"#7676801F"
                    radius: 8

                    ColumnLayout{
                        width:parent.width
                        height: parent.height
                        spacing: 0

                        RowLayout {
                            spacing: 5

                            Rectangle {
                                width: idMainRect.width - 350
                                height: parent.height

                                Layout.alignment: Qt.AlignTop
                                Layout.topMargin: 2
                                Layout.leftMargin: 20

                                Row {
                                    spacing: 5

                                    Label {
                                        text: "Select Task: "
                                        //font.bold: true
                                        color: "#000000"
                                        font.pixelSize: 15
                                        font.weight: 700
                                        anchors.verticalCenter: parent.verticalCenter
                                    }

                                    ComboBox {
                                        id: comboTaskId
                                        height: 30
                                        width: 150
                                        model: taskNameModel
                                        font.pixelSize: 15
                                        font.weight: 700
                                        currentIndex: 0

                                        onCurrentIndexChanged:
                                        {
                                            showColor(year.currentText, month.currentIndex)
                                            generateTaskToRenderList(year.currentText, month.currentIndex);
                                        }
                                    }
                                }
                            }

                            // Rectangle {
                            //     //width: idMainRect.width - 320
                            //     width: idMainRect.width - 450
                            //     height: 40
                            //     color:  "transparent"
                            // }

                            Button {
                                id: idPrevMonthButton
                                text: "< "//"Previous Month"
                                font.pixelSize: 20
                                font.weight: 700
                                onClicked: {
                                    if(month.currentIndex > 0){
                                         --month.currentIndex;
                                        idNextMonthButton.enabled = true;
                                        idPrevMonthButton.enabled = true;
                                    }
                                    else{
                                        if(year.currentIndex > 0)
                                        {
                                            --year.currentIndex;
                                            month.currentIndex = 11;
                                            idPrevMonthButton.enabled = true;
                                            idNextMonthButton.enabled = true;
                                        }
                                        else
                                        {
                                            idPrevMonthButton.enabled = false;
                                        }
                                    }

                                    showColor(year.currentText, month.currentIndex);
                                    generateTaskToRenderList(year.currentText, month.currentIndex);
                                }
                            }
                       /*
                        Label {
                            text: Qt.locale("en_US").standaloneMonthName(idMonthGrid.month) + " " + idMonthGrid.year
                            font.bold: true
                        }

                        Button {
                            text: "Next Month"
                            onClicked: {
                                if(month.currentIndex < 11){
                                  ++month.currentIndex;
                                }
                                else{
                                    ++year.currentIndex;
                                    month.currentIndex = 0;
                                }
                            }
                        }*/

                            ComboBox {
                                id: month
                                height: 40
                                width: 150
                                model: monthModel
                                font.pixelSize: 20
                                font.weight: 700
                                currentIndex: 0

                                onCurrentIndexChanged:
                                {
                                     showColor(year.currentText, month.currentIndex)
                                    generateTaskToRenderList(year.currentText, month.currentIndex);
                                }
                            }

                            ComboBox {
                                id: year
                                height: 40
                                width: 150
                                model: yearModel
                                font.pixelSize: 20
                                font.weight: 700
                                currentIndex: 0

                                onCurrentIndexChanged:
                                {
                                     showColor(year.currentText, month.currentIndex)
                                    generateTaskToRenderList(year.currentText, month.currentIndex);
                                }

                            }

                            Button {
                                id:idNextMonthButton
                                text:" >" //"Next Month"
                                font.pixelSize: 20
                                font.weight: 700
                                onClicked: {
                                    if(month.currentIndex < 11){
                                        ++month.currentIndex;
                                        idNextMonthButton.enabled = true;
                                        idPrevMonthButton.enabled = true;
                                    }
                                    else{
                                        if(year.currentIndex < yearModel.count - 1)
                                        {
                                            ++year.currentIndex;
                                            month.currentIndex = 0;
                                            idNextMonthButton.enabled = true;
                                            idPrevMonthButton.enabled = true;
                                        }
                                        else
                                        {
                                            idNextMonthButton.enabled = false;
                                        }
                                    }

                                    showColor(year.currentText, month.currentIndex)
                                    generateTaskToRenderList(year.currentText, month.currentIndex);

                                }
                            }
                        }

                        Rectangle {
                            id:idDayOfWeeksRow
                            width: parent.width
                            height: 40
                            color: "transparent"// "#7676801F"

                            DayOfWeekRow {
                                width: parent.width
                                font.pixelSize: 30
                                font.weight: 700
                                topPadding: 1
                            }
                        }
                    }
                }


                Rectangle {
                    id:idRect
                    width: parent.width
                    height: parent.height - idDayOfWeeksRow
                    color: "#7676801F"

                    MonthGrid {                        
                        id:idMonthGrid
                        width: parent.width
                        height: 400
                        // topPadding: 2
                        month: month.currentIndex
                        year: yearModel.get(year.currentIndex).text
                        locale: Qt.locale("en_US")
                        font.pixelSize: 25
                        font.weight: 700
                        spacing: 0


                        /*onClicked: (date) => {
                            selectedDate = date;
                        }*/

                        delegate:Item {
                            width: idMonthGrid.cellWidth
                            height: idMonthGrid.cellHeight

                            property bool isInCurrentMonth: model.month === idMonthGrid.month

                            Rectangle {                                    
                                width: parent.width
                                height: parent.height

                                // property bool isSelected: (model.year === selectedDate.getFullYear() &&
                                //                             model.month === selectedDate.getMonth() &&
                                //                             model.day === selectedDate.getDate())

                                border.color: "#E0E0E0" //getDateColor(model.date)
                                color: "white" //getDateColor(model.date)
                                Label {
                                    text: model.day.toString()
                                    anchors.centerIn: parent
                                    bottomPadding: 2
                                }
                                visible: parent.isInCurrentMonth
                            }
                        }

                        Component.onCompleted: {
                            var now = new Date ();
                            let currentYear = now.getFullYear().toString();

                            var currentIndex = 0;
                            for(var j = 0; j<yearModel.count; j++)
                            {
                                let yearText = yearModel.get(j).text;

                                if(currentYear.localeCompare(yearText)===0)
                                {
                                    currentIndex = j;
                                }
                            }                            

                            year.currentIndex = currentIndex;
                            month.currentIndex = now.getMonth();


                            showColor(year.currentText, month.currentIndex);
                            generateTaskToRenderList(year.currentText, month.currentIndex);

                        }
                    }

                    Item {
                        id: monthGridOverlay
                        anchors.fill: idMonthGrid
                        z: 10

                        Repeater {
                            model: taskToRenderModel

                            delegate: Rectangle {
                                height: heightModel
                                radius: 3
                                color: "#4285F4"

                                x: xModel
                                y: yModel
                                width: widthModel

                                Text {
                                    anchors.centerIn: parent
                                    text: textModel
                                    color: "white"
                                    font.pixelSize: 12
                                }
                            }

                            Component.onCompleted: {
                                console.log("Renders");
                            }
                        }
                    }
                }
            }
        }

        Component.onCompleted: {
            loadSampleYears();


            if(schedule_root.visible)
            {
                loadAllTasks();
                showColor(year.currentText, month.currentIndex)
                generateTaskToRenderList(year.currentText, month.currentIndex);
            }
        }
        onVisibleChanged: {
            if(schedule_root.visible)
            {
                loadAllTasks();               
                showColor(year.currentText, month.currentIndex);
                generateTaskToRenderList(year.currentText, month.currentIndex);
            }
        }
    }

    function loadAllTasks()
    {
        taskModel = [];
        taskNameModel = [];
        //var paramList = taskController.getTaskList(true);
        var paramList =[];
        if(approvedNo === 0)
        {
            paramList = taskController.getTaskList(true);
        }
        else if(approvedNo === 1)
        {
            paramList = taskController.getTaskList(false);
        }
        else
        {
            paramList = taskController.getTaskList(true);
            var paramListDraft = taskController.getTaskList(false);
            paramList = paramList.concat(paramListDraft);
        }

        for (var i = 0; i < paramList.length; i++)
        {            
            var currentTask = paramList[i];
            taskModel = taskModel.concat(currentTask.id);
            taskNameModel = taskNameModel.concat(currentTask.taskName);
        }
    }

    function loadSampleYears()
    {        
        for (var i = 2010; i < 2081; i++) {
             yearModel.append({"text" : i});
        }
    }

    /*function getDateColor(dateToCheck) {
        for (var i = 0; i < highlightedDatesModel.count; i++) {
            var eventData = highlightedDatesModel.get(i);
            if (eventData.year === dateToCheck.getFullYear() &&
                eventData.month === dateToCheck.getMonth() &&
                eventData.day === dateToCheck.getDate()) {
                return eventData.color;
            }

        }
        return "white";
    }*/

    function updateColorMap() {
        var newMap = {};
        for (var i = 0; i < highlightedDatesModel.count; i++) {
            var event = highlightedDatesModel.get(i);
            //Create a unique key for the date
            var key = event.year + "-" + event.month + "-" + event.day;
            newMap[key] = event.color;
        }
        colorMap = newMap;
    }

    function getDateColor(dateToCheck) {
        var key = dateToCheck.getFullYear() + "-" +
                  dateToCheck.getMonth() + "-" +
                  dateToCheck.getDate();

        return colorMap[key] || "white";
    }


    function showColor(selectedYear, selectedMonth) {        

        highlightedDatesModel.clear();
        if(approvedNo === 0)
        {
            task_month_paramList = taskController.getTaskList(true);
        }
        else if(approvedNo === 1)
        {
            task_month_paramList = taskController.getTaskList(false);
            console.log("approvedNo:", approvedNo)
        }
        else
        {
            task_month_paramList = taskController.getTaskList(true);
            var draftTasklist = taskController.getTaskList(false);
            task_month_paramList = task_month_paramList.concat(draftTasklist);
        }

        for (var i = 0; i < task_month_paramList.length; i++) {
            var currentTask = task_month_paramList[i];

            // Created Date objects for the start, end, and current view
            // Note: selectedMonth is 0-indexed, currentTask.month is 1-indexed
            let startDate = new Date(currentTask.startYear, currentTask.startMonth - 1, currentTask.startDay);
            let endDate = new Date(currentTask.endYear, currentTask.endMonth - 1, currentTask.endDay);

            // Checked if the task ID matches the selected combo box item
            if (currentTask.id === taskModel[comboTaskId.currentIndex])
            {
                //Determined the first and last day of the currently displayed month
                let viewMonthStart = new Date(selectedYear, selectedMonth, 1);
                let viewMonthEnd = new Date(selectedYear, selectedMonth + 1, 0); // Day 0 is last day of prev month

                //Checked if the task overlaps with the selected month
                if (startDate <= viewMonthEnd && endDate >= viewMonthStart) {

                    // Calculated the visual start and end for the current month view
                    let startPrint = (startDate < viewMonthStart) ? 1 : currentTask.startDay;
                    let endPrint = (endDate > viewMonthEnd) ? viewMonthEnd.getDate() : currentTask.endDay;

                    for (let day = startPrint; day <= endPrint; day++) {

                        currentTask.year = selectedYear;
                        currentTask.month = selectedMonth;

                        highlightedDatesModel.append({
                            "year": currentTask.year,
                            "month": currentTask.month,
                            "day": day,
                            "color": "red"
                        });
                    }
                }
            }
        }

        updateColorMap();
    }

    function generateTaskToRenderList(selectedYear, selectedMonth) {
        // initializing
        date_task_num_mapping = {};
        taskToRenderModel.clear();

        // console.log(JSON.stringify(task_month_paramList));

        for (const task of task_month_paramList) {
            const startDate = new Date(task.startYear, task.startMonth - 1, task.startDay);
            const endDate = new Date(task.endYear, task.endMonth - 1, task.endDay);

            const viewMonthStart = new Date(selectedYear, selectedMonth, 1);
            let monthStartOffset = viewMonthStart.getDay();
            const viewMonthEnd = new Date(selectedYear, selectedMonth + 1, 0); // Day 0 is last day of prev month

            if (endDate >= viewMonthStart && startDate <= viewMonthEnd) {
                // update date_task_num_mapping
                const startDateRendering = viewMonthStart > startDate ? viewMonthStart : startDate;
                const endDateRendering = viewMonthEnd < endDate ? viewMonthEnd : endDate;
                const startDateIndex = numOfDaysBetweenDates(startDateRendering, viewMonthStart) + monthStartOffset;
                const endDateIndex = numOfDaysBetweenDates(endDateRendering, viewMonthStart) + monthStartOffset;
                let render_task = false;

                for (let d = new Date(startDateRendering); d <= endDateRendering; d.setDate(d.getDate() + 1)) {
                    const currentDate = new Date(d);

                    if (date_task_num_mapping[currentDate] === undefined) {
                        date_task_num_mapping[currentDate] = 1;
                        render_task = true;
                    } else if (date_task_num_mapping[currentDate] < 3) {
                        date_task_num_mapping[currentDate] = date_task_num_mapping[currentDate] + 1;
                        render_task = true;
                    } else {
                        date_task_num_mapping[currentDate] = date_task_num_mapping[currentDate] + 1;
                        render_task = false;
                    }
                }

                if (render_task) {
                    const startRow = Math.floor(startDateIndex / 7);
                    const endRow = Math.floor(endDateIndex / 7);
                    const startColumn = startDateIndex % 7;
                    const endColumn = endDateIndex % 7;
                    const cellWidth = idMonthGrid.width / 7;
                    const cellHeight = idMonthGrid.height / 6;

                    if (startRow === endRow) {
                        taskToRenderModel.append({
                            textModel: task.taskName,
                            xModel: idRect.x + (startColumn * cellWidth),
                            yModel: idRect.y + (startRow * cellHeight),
                            heightModel: 10,
                            widthModel: (endColumn - startColumn + 1) * cellWidth
                        });
                    } else {
                        const numOfRowsToRender = endRow - startRow + 1; // +1 is required to account for the start row too

                        for (let i = 0; i < numOfRowsToRender; i++) {
                            if (i === 0) {
                                taskToRenderModel.append({
                                    textModel: task.taskName,
                                    xModel: idRect.x + (startColumn * cellWidth),
                                    yModel: idRect.y + (startRow * cellHeight),
                                    heightModel: 10,
                                    widthModel: (7 - startColumn) * cellWidth
                                });
                            }
                            else if (i === numOfRowsToRender - 1) {
                                taskToRenderModel.append({
                                    textModel: task.taskName,
                                    xModel: idRect.x,
                                    yModel: idRect.y + (endRow * cellHeight),
                                    heightModel: 10,
                                    widthModel: (endColumn) * cellWidth
                                });
                            } else {
                                taskToRenderModel.append({
                                    textModel: task.taskName,
                                    xModel: idRect.x,
                                    yModel: idRect.y + (i * cellHeight),
                                    heightModel: 10,
                                    widthModel: 7 * cellWidth
                                });
                            }
                        }
                    }
                }
            }
        }

        // logModel(taskToRenderModel);
    }

    function numOfDaysBetweenDates(date1, date2) {
        const millisecondsDiff = Math.abs(date2.getTime() - date1.getTime());

        const daysDiff = Math.round(
          millisecondsDiff / (1000 * 24 * 60 * 60)
        );

        return daysDiff;
    }

    function logModel(model) {
        for (var i = 0; i < model.count; i++) {
            console.log("Item " + i + ":", JSON.stringify(model.get(i)))
        }
    }
}
