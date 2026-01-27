import QtQuick
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts 1.15
//import Gantt 1.0
import QtCharts
import com.fh.models 1.0
import com.fh.controllers

Rectangle {
    id:gantt_root
    width: 1440
    //height: 1024 - (131 + 13)
    height: screen.height-100
    color: "#EDF1F4"
    property string txtProjectName: "NA"
    property int febMonthWidth: 28
    property int monthScale: 5
    property int activeTextBoxIndex: -1

    property int approvedNo: 0
    property int startYear: 2080
    property int endYear: 2010
    property date selectedDate: new Date()

    property var monthModel: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'June', 'July', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
    property var  task_month_paramList: []
    property var task_idList: []
    property var valuesToRemove: []

    ListModel {
        id: yearModel
    }


    TaskController{
        id:taskController
    }



    /************Start of Calendar******************/

    Popup {
            id: calendarPopup
            anchors.centerIn: parent
            y: taskStartDateTextBox.y + taskStartDateTextBox.height + 10
            visible: false
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
            parent: Overlay.overlay

            ColumnLayout {

                ///////////////////
                RowLayout {

                    Button {
                        id: idPrevMonthButton
                        text: "< "
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
                        }
                    }


                    ComboBox {
                        id: month
                        height: 40
                        width: 150
                        model: monthModel
                        font.pixelSize: 20
                        font.weight: 700
                        currentIndex: 0
                    }

                    ComboBox {
                        id: year
                        height: 40
                        width: 150
                        model: yearModel
                        font.pixelSize: 20
                        font.weight: 700
                        currentIndex: 0
                    }

                    Button {
                        id:idNextMonthButton
                        text:" >"
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
                        }
                    }
                }

                ///////////////

                DayOfWeekRow {
                    locale: monthGrid_startDate.locale
                    Layout.fillWidth: true
                }

                MonthGrid {
                    id: monthGrid_startDate
                    month: month.currentIndex
                    year: yearModel.get(year.currentIndex).text
                    locale: Qt.locale("en_US")
                    Layout.fillWidth: true

                    onClicked: (date) => {
                        if(activeTextBoxIndex === 1)
                        {
                            taskStartDateTextBox.text = date.toLocaleDateString(Qt.locale(), "dd/MM/yyyy");
                            calendarPopup.visible = false;
                        }

                        if(activeTextBoxIndex === 2)
                        {
                            taskEndDateTextBox.text = date.toLocaleDateString(Qt.locale(), "dd/MM/yyyy");
                            calendarPopup.visible = false;
                        }
                    }

                    Component.onCompleted: {
                         loadSampleYears()
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
                    }
                }
            }
       }

    /********************End of Calendar*****************************/


    FHPopup {
        id: newTaskPopup
        popupWidth: 500
        popupHeight: 520
        title: "New Task"
        parent: Overlay.overlay

        onAcceptCallback: function () {


            taskController.create(taskNameTextBox.text, taskDescriptionTextBox.text,
                                  taskBIMObjectTextBox.text, taskStartDateTextBox.text,
                                  taskEndDateTextBox.text, task_idList[parentIdComboBox.currentIndex], "Upcoming" );


            taskNameTextBox.text = "";
            taskDescriptionTextBox.text = "";
            taskStartDateTextBox.text = "";
            taskEndDateTextBox.text = "";
            taskBIMObjectTextBox.text = "";

            showList();

            calendarPopup.visible = false;
            taskStartDateTextBox.focus = false;
            taskEndDateTextBox.focus = false;
        }

        onCancelCallback: function () {
            taskNameTextBox.text = "";
            taskDescriptionTextBox.text = "";
            taskStartDateTextBox.text = "";
            taskEndDateTextBox.text = "";
            taskBIMObjectTextBox.text = "";

            calendarPopup.visible = false;
            taskStartDateTextBox.focus = false;
            taskEndDateTextBox.focus = false;
        }

        onOpened:
        {
            showList();
        }

        Column {
            width: parent.width
            height: 300//parent.height //30 for each top bottom

            Text{
                id: taskNameLabel
                text: "Task Name"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomTextBox{
                id: taskNameTextBox
                placeholderText: "Task Name"
                text:""
                color: "#323130"
            }

            Text{
                id: taskDescriptionLabel
                text: "Description"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }


            CustomTextBox{
                id: taskDescriptionTextBox
                placeholderText: "Description"
                text:""
                color: "#323130"
            }

            Text{
                id: taskStartDateLabel
                text: "Start Date"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }


            CustomTextBox{
                id: taskStartDateTextBox
                placeholderText: "Start Date"
                text:""
                color: "#323130"

                // onActiveFocusChanged: {
                //     if (activeFocus) {
                //         calendarPopup_startDate.visible = true;

                //     }
                // }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        activeTextBoxIndex = 1;
                        calendarPopup.visible = !calendarPopup.visible;
                    }
                }
            }


            Text{
                id: taskEndDateLabel
                text: "End Date"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox{
                id: taskEndDateTextBox
                placeholderText: "End Date"
                text:""
                color: "#323130"
                // onActiveFocusChanged: {
                //     if (activeFocus) {
                //         calendarPopup_endDate.visible = true;

                //     }
                // }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        activeTextBoxIndex = 2;
                        calendarPopup.visible = !calendarPopup.visible;
                    }
                }
            }


            Text{
                id: taskBIMObjectLabel
                text: "BIM Object"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox{
                id: taskBIMObjectTextBox
                placeholderText: "BIM Object"
                text:""
                color: "#323130"
            }


            Text{
                id: parentIdLabel
                text: "Parent Id"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: parentIdComboBox
                model: task_idList
                width:200
                currentIndex: 0
                onCurrentTextChanged: {

                }
            }
        }
    }


    Column{ //Column end
        width: parent.width
        height:  parent.height
        spacing: 10

   Item {
        width: 100
        height: 100
        id:itemId

        Image{
            source: "qrc:/resources/images/backArrow.svg"
            anchors.left: parent.left
            anchors.leftMargin: 100
            anchors.top: parent.top
            anchors.topMargin: 46

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
            text: txtProjectName
            color: "#000000"
            font.pixelSize: 44
            font.weight: 700
            anchors.left: parent.left
            anchors.leftMargin: 149
            anchors.top: parent.top
            anchors.topMargin: 21
        }
    }//End of Item

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
                showList();
            }
        }
    }

    Rectangle{
        width: 1300//1236
        //height: 599
        height: 300
        radius: 8
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.bottom: parent.bottom
        //anchors.bottomMargin: 138

        Rectangle{
            id:headerId
            width: parent.width
            height: 53
            topLeftRadius: 8
            topRightRadius: 8
            color: "#FAF9F8"
            border.color: "#8A888629"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top            

            CustomButton {
                color: "#007AFF"
                width: 120
                height: 38
                radius: 4
                // border.color: "#007AFF"
                btnSource: "qrc:/resources/images/addWhite_icon.png"
                btnName: "New Task"
                btnNameColor: "white"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                //anchors.top: parent.top
                //anchors.topMargin: 1
                anchors.leftMargin: 5

                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        newTaskPopup.open();
                    }
                }
            }
        }


        Rectangle{
            width: parent.width
            height: parent.height
            color: "#EDF1F4"
            border.color: "#8A888629"
            anchors.top: headerId.bottom
            anchors.topMargin: 2

            RowLayout{
                width: parent.width
                height: parent.height
                spacing: 3

                Rectangle{//LeftSide
                    width: parent.width/2-1
                    height: parent.height
                    color: "#EDF1F4"//"green"
                    Column{
                        //FHTable {
                          GanttTaskListTable{
                            height: 250//300
                            leftPadding: 2                            
                            headerHeight:102
                            headerFontPixelSize:20
                            model:task_month_paramList

                            columns: [
                                { label: "ID", width: 50, key: "id"},
                                { label: "Task", width: 256, key: "taskName" },
                                { label: "Duration\n(In Days)", width: 110, key: "days" },
                                { label: "Start", width: 110, key: "startDate" },
                                { label: "End", width: 110, key: "endDate" },
                            ]
                        }
                    }
                }//End of LeftSide


                Rectangle{//RightSide
                    width: parent.width/2-1
                    height: parent.height
                    //spacing: 3
                    color: "#EDF1F4"//"green"

                    ColumnLayout{
                        width: parent.width
                        height: parent.height
                        spacing: 2

                        Rectangle{
                            id:yearId
                            width: parent.width
                            height: 50//parent.height/2
                            color: "lightgray"//"gray"

                            Text{
                                text: startYear_endYearText(startYear, endYear)//"Year: " + startYear.toString() + " - to - " + endYear.toString()
                                color: "#000000"
                                font.pixelSize: 20
                                font.weight: 700
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.topMargin: 10
                                anchors.leftMargin: 10

                            }
                        }


                        Rectangle{
                            width: parent.width
                            height: parent.height//-51
                            color: "#EDF1F4"//"gray"

                            ScrollView {
                                id: horizontalScrollView
                                width: parent.width
                                height: parent.height
                                padding: 1
                                clip: true

                                ColumnLayout{
                                    width: generatGanttWidth(startYear, endYear, monthScale)//31 * monthScale * totalMonths + 10//TotalMonths
                                    height: parent.height
                                    spacing: 2

                                    onWidthChanged: {
                                            horizontalScrollView.contentWidth = generatGanttWidth(startYear, endYear, monthScale);
                                        }

                                    Rectangle{
                                        width: parent.width
                                        height: parent.height// - 50
                                        color: "#EDF1F4"//"gray"

                                        Column{
                                           // FHTable {
                                            GanttChartTable{
                                                height: 250//300
                                                leftPadding: 2
                                                //removeRow: false
                                                headerHeight:50
                                                headerFontPixelSize:20
                                                model: task_month_paramList //monthParamList

                                                columns: generateGanttColumns(startYear, endYear, monthScale)
                                            }                                            
                                        }                                        
                                    }
                                }
                            }
                        }
                    }//End of RightSide
                }
            }
        }       
    }
    }//ColumnLayout end


    Component.onCompleted: {
        showList();

    }

    onVisibleChanged: {
        showList();
    }

    function showList(){
        if(gantt_root.visible)
        {

            if(approvedNo == 0)
            {
              task_month_paramList = taskController.getTaskList(true);                
            }
            else if(approvedNo == 1)
            {
                task_month_paramList = taskController.getTaskList(false);                
            }
            else
            {
               task_month_paramList = taskController.getTaskList(true);
               var draftTasklist = taskController.getTaskList(false);

               task_month_paramList = task_month_paramList.concat(draftTasklist);
            }

            task_idList = [];

            //here added first element zero for there is no parent id
            task_idList = [0, ...task_month_paramList.map(element => element.id)];

            var tempFilteredArray = task_idList.filter(function(element) {

                        return valuesToRemove.indexOf(element) === -1;
                    });

            task_idList = tempFilteredArray;

            for(var f = 0; f < task_month_paramList.length; f++)
            {
                var task1 =  task_month_paramList[f];

                if(startYear > task1.startYear )
                {
                    startYear = task1.startYear;
                }
                if(endYear < task1.endYear)
                {
                    endYear = task1.endYear;
                }
            }
        }
    }


    function loadSampleYears()
    {
        for (var i = 2010; i < 2081; i++) {
             yearModel.append({"text" : i});
        }
    }


    /**********************Start Dynamic month***********************/

    // Helper to get days in a month
    function getDaysInMonth(year, month) {
        return new Date(year, month + 1, 0).getDate();
    }

    // Generate columns for a range of years
    function generateGanttColumns(startYear, endYear, monthScale) {
        const columns = [];
        const monthNames = ["Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];


        for (let year = startYear; year <= endYear; year++)
        {
            for (let month = 0; month < 12; month++) {
                const daysCount = getDaysInMonth(year, month);
                const monthKey = monthNames[month].toLowerCase();


                columns.push({
                    label: `${monthNames[month]} ${year}`,
                    width: daysCount * monthScale,
                    //key: `${monthKey}_${year}`,
                    key: `${monthKey}`,
                    startx: "startDay",
                    endx: "endDay",
                    days: "days",
                    id: "id",
                    pid: "pid",
                    startYear: "startYear",
                    year: "year",
                    startDate: "startDate",
                    month_year: `${monthKey}_${year}`

                });
            }
        }
        return columns;
    }

    function generatGanttWidth(startYear, endYear, monthScale)
    {
         if(gantt_root.visible)
         {
             if(startYear > endYear)
             {
                var temp = endYear;
                 endYear = startYear;
                 startYear = temp;
             }

            var def = endYear - startYear;
            return  31 * monthScale * (def + 1) * 12  + 10;
         }

        return  0;


    }

    function startYear_endYearText(startYear, endYear)
    {
        if(startYear > endYear)
        {
           var temp = endYear;
            endYear = startYear;
            startYear = temp;
        }

        return "Year: " + startYear.toString() + " - to - " + endYear.toString();
    }

    /*************End of Dynamica month********************************/
}
