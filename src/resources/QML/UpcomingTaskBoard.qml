import QtQuick
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts 1.15
import com.fh.models 1.0
import com.fh.controllers

Rectangle {
    id:upcomingTaskBoard_root
    width: 296.5
    height: 544

    color: "#FAF9F8"

    property var monthModel: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'June', 'July', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
    property var  task_month_paramList: []
    property var task_idList: []
    property var valuesToRemove: []

    property bool isApproved: true
    property int startYear: 2080
    property int endYear: 2010

    property date selectedDate: new Date()

    property string txtTotalUpcomingTask: "0"

    ListModel {
        id: yearModel
    }



    TaskController{
        id:taskController
    }

    WorkBillingLineController{
      id:workBillingLineController
    }

    /************Start of Calendar******************/

    Popup {
            id: calendarPopup_startDate
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
                        taskStartDateTextBox.text = date.toLocaleDateString(Qt.locale(), "dd/MM/yyyy");
                        calendarPopup_startDate.visible = false;
                        //taskStartDateTextBox.activeFocus = false;
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


    Popup {
            id: calendarPopup_endDate
            anchors.centerIn: parent
            y: taskEndDateTextBox.y + taskEndDateTextBox.height + 10
            visible: false
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
            parent: Overlay.overlay

            ColumnLayout {
                ///////////////////
                RowLayout {

                    Button {
                        id: idPrevMonthButtonNew
                        text: "< "//"Previous Month"
                        font.pixelSize: 20
                        font.weight: 700
                        onClicked: {
                            if(monthNew.currentIndex > 0){
                                 --monthNew.currentIndex;
                                idNextMonthButtonNew.enabled = true;
                                idPrevMonthButtonNew.enabled = true;
                            }
                            else{
                                if(yearNew.currentIndex > 0)
                                {
                                    --yearNew.currentIndex;
                                    monthNew.currentIndex = 11;
                                    idPrevMonthButtonNew.enabled = true;
                                    idNextMonthButtonNew.enabled = true;
                                }
                                else
                                {
                                    idPrevMonthButtonNew.enabled = false;
                                }
                            }
                        }
                    }

                    ComboBox {
                        id: monthNew
                        height: 40
                        width: 150
                        model: monthModel
                        font.pixelSize: 20
                        font.weight: 700
                        currentIndex: 0
                    }

                    ComboBox {
                        id: yearNew
                        height: 40
                        width: 150
                        model: yearModel
                        font.pixelSize: 20
                        font.weight: 700
                        currentIndex: 0
                    }

                    Button {
                        id:idNextMonthButtonNew
                        text:" >" //"Next Month"
                        font.pixelSize: 20
                        font.weight: 700
                        onClicked: {
                            if(monthNew.currentIndex < 11){
                                ++monthNew.currentIndex;
                                idNextMonthButtonNew.enabled = true;
                                idPrevMonthButtonNew.enabled = true;
                            }
                            else{
                                if(yearNew.currentIndex < yearModel.count - 1)
                                {
                                    ++yearNew.currentIndex;
                                    monthNew.currentIndex = 0;
                                    idNextMonthButtonNew.enabled = true;
                                    idPrevMonthButtonNew.enabled = true;
                                }
                                else
                                {
                                    idNextMonthButtonNew.enabled = false;
                                }
                            }
                        }
                    }
                }

                ///////////////

                DayOfWeekRow {
                    locale: monthGrid_endDate.locale
                    Layout.fillWidth: true
                }

                MonthGrid {
                    id: monthGrid_endDate
                    month: monthNew.currentIndex
                    year: yearModel.get(yearNew.currentIndex).text
                    locale: Qt.locale("en_US")
                    Layout.fillWidth: true

                    onClicked: (date) => {
                        taskEndDateTextBox.text = date.toLocaleDateString(Qt.locale(), "dd/MM/yyyy");
                        calendarPopup_endDate.visible = false;
                        //taskEndDateTextBox.activeFocus = false;
                    }

                    Component.onCompleted: {
                         loadSampleYears()
                        var now = new Date ();
                        let currentYear = now.getFullYear().toString();

                        var currentIndex = 0;
                        for(var j = 0; j < yearModel.count; j++)
                        {
                            let yearText = yearModel.get(j).text;

                            if(currentYear.localeCompare(yearText)===0)
                            {
                                currentIndex = j;
                            }
                        }
                        yearNew.currentIndex =  currentIndex;
                        monthNew.currentIndex = now.getMonth();
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
                                  taskEndDateTextBox.text, task_idList[parentIdComboBox.currentIndex]);


            taskNameTextBox.text = "";
            taskDescriptionTextBox.text = "";
            taskStartDateTextBox.text = "";
            taskEndDateTextBox.text = "";
            taskBIMObjectTextBox.text = "";

            showList();

            calendarPopup_startDate.visible = false;
            calendarPopup_endDate.visible = false;
            taskStartDateTextBox.focus = false;
            taskEndDateTextBox.focus = false;


        }

        onCancelCallback: function () {
            taskNameTextBox.text = "";
            taskDescriptionTextBox.text = "";
            taskStartDateTextBox.text = "";
            taskEndDateTextBox.text = "";
            taskBIMObjectTextBox.text = "";


            calendarPopup_startDate.visible = false;
            calendarPopup_endDate.visible = false;
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

                onActiveFocusChanged: {
                    if (activeFocus) {
                        calendarPopup_startDate.visible = true;

                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        taskStartDateTextBox.forceActiveFocus();
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
                onActiveFocusChanged: {
                    if (activeFocus) {
                        calendarPopup_endDate.visible = true;

                    }
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        taskEndDateTextBox.forceActiveFocus();
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



    Rectangle{
        id: statusRect
        width: 20
        height: width
        radius: width/2
        color: "#0078D4"
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 8

        Text{
            text: txtTotalUpcomingTask
            color: "#FFFFFF"
            font.pixelSize: 10
            font.weight: 400
            anchors.centerIn: parent
        }
    }


    Text{
        text: "Upcoming"
        color: "#323130"
        font.pixelSize: 14
        font.weight: 400
        anchors.verticalCenter: statusRect.verticalCenter
        anchors.left: statusRect.right
        anchors.leftMargin: 10
    }



    Rectangle{
        width: 16
        height: 16
        color: parent.color
        anchors.right: parent.right
        anchors.rightMargin: 59
        anchors.top: parent.top
        anchors.topMargin: 17        
        visible: false

        Image{
            source: "qrc:/resources/images/addBlack.svg"
            anchors.centerIn: parent

            MouseArea{
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: {
                    newTaskPopup.open();
                }
            }
        }
    }

    Rectangle{
        width: 16
        height: 16
        color: parent.color
        anchors.right: parent.right
        anchors.rightMargin: 17
        anchors.top: parent.top
        anchors.topMargin: 17

        /*Image{
            source: "qrc:/resources/images/close.svg"
            anchors.centerIn: parent            
        }*/

        Image{
            source: "qrc:/resources/images/addBlack.svg"
            anchors.centerIn: parent

            MouseArea{
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                hoverEnabled: true

                onClicked: {
                    newTaskPopup.open();
                }
            }
        }
    }


    Rectangle{
        id: listViewRect
        width: 276.5
        height: parent.height-50// 480
        color: parent.color
        //anchors.bottom: parent.bottom
        //anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter


        ListModel{
            id: listModel            
        }

        ListView{
            anchors.fill: parent
            spacing: 8
            clip: true
            model: listModel
            delegate: listDelegate
        }

        Component{
            id: listDelegate

            Rectangle{
                width: 276.5
                height: 114
                color: "#FFFFFF"

                Text{
                    text: title
                    color: "#323130"
                    font.pixelSize: 18
                    font.weight: 400
                    anchors.top: parent.top
                    anchors.topMargin: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                }

                Rectangle{
                    width: 16
                    height: 16
                    color: parent.color
                    anchors.right: parent.right
                    anchors.rightMargin: 21
                    anchors.top: parent.top
                    anchors.topMargin: 21

                    Image {
                        source: "qrc:/resources/images/dotMenu.svg"
                        anchors.centerIn: parent
                    }
                }

                Text{
                    text: desc
                    width: 252.5
                    height: 32
                    color: "#323130"
                    wrapMode: Text.Wrap
                    font.pixelSize: 12
                    font.weight: 400
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                }
            }
        }
    }

    Component.onCompleted: {
        showList();

    }

    onVisibleChanged: {
        showList();
    }

    function showList(){
        if(upcomingTaskBoard_root.visible){
            task_idList = [];
            listModel.clear();


            task_month_paramList = taskController.getTaskList(isApproved);
            /*var allTaskList = taskController.getTaskList(isApproved);
            var billedTaskList = workBillingLineController.getBilledTaskList(isApproved);

            for(var f =0; f<allTaskList.length; f++)
            {
                console.log("f:-", f, "id:", allTaskList[f].id);
            }

            for(var g =0; g<billedTaskList.length; g++)
            {
                console.log("g:-", g, "id:", billedTaskList[g].id);
            }


            task_month_paramList = allTaskList.filter(task => {
                return !billedTaskList.some(billedTask =>
                    parseInt(billedTask.id) === parseInt(task.id)
                );
            });
             txtTotalUpcomingTask = String(task_month_paramList.length - billedTaskList.length);
           */

            txtTotalUpcomingTask = task_month_paramList.length;

            //here added first element zero for there is no parent id
            task_idList = [0, ...task_month_paramList.map(element => element.id)];

            var tempFilteredArray = task_idList.filter(function(element) {
                        return valuesToRemove.indexOf(element) === -1;
            });

            task_idList = tempFilteredArray;

            for(var i = 0; i < task_month_paramList.length; i++)
            {
               var task =  task_month_paramList[i];

                if(startYear > task.startYear )
                {
                    startYear = task.startYear;
                }
                if(endYear < task.endYear)
                {
                    endYear = task.endYear;
                }

                listModel.append({
                    "title": task.id + "_" + task.taskName,
                    "desc": task.description
                });
            }
        }
    }

    function loadSampleYears()
    {
        for (var i = 2010; i < 2081; i++) {
             yearModel.append({"text" : i});
        }
    }

    function refreshData() {

       // upcomingTaskBoard_root.showList();
         //Component.statusChanged()
    }
}
