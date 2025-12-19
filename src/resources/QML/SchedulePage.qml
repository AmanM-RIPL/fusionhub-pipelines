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

    property var  task_month_paramList: []

    TaskController{
        id:taskController
    }


    ListModel {
            id: yearModel
        }


    ListModel {
            id: highlightedDatesModel            
            // Adjust these years/months to match your current view
            /*
            ListElement { year: 2025; month: 0; day: 3; color: "red" }
            */

        }


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


    Rectangle{
        id:idMainRect
        width: 1300//1236
        //height: 599
        height: 400
        radius: 8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: itemId.bottom



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
                    color:  "#7676801F"

                    ColumnLayout{
                        width:parent.width
                        height: parent.height
                        spacing: 0

                    RowLayout {
                        spacing: 5
                        Rectangle {
                            width: idMainRect.width - 320
                            height: 40
                            color:  "transparent"
                        }

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

                                showColor(year.currentText, month.currentIndex)

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
                        height:300
                        topPadding: 2
                        month: month.currentIndex
                        year: yearModel.get(year.currentIndex).text
                        locale: Qt.locale("en_US")
                        font.pixelSize: 25
                        font.weight: 700

                        /*onClicked: (date) => {
                            selectedDate = date;
                        }*/

                        delegate:Item {
                            width: idMonthGrid.cellWidth
                            height: idMonthGrid.cellHeight
                            property bool isInCurrentMonth: model.month === idMonthGrid.month
                            Rectangle {                                    
                                width: parent.width + 8
                                height: parent.height/2

                                property bool isSelected: (model.year === selectedDate.getFullYear() &&
                                                            model.month === selectedDate.getMonth() &&
                                                            model.day === selectedDate.getDate())

                                border.color: getDateColor(model.date)
                                color: getDateColor(model.date)
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

                        }
                    }                    
                }
            }
        }

        Component.onCompleted: {
             loadSampleYears();
            if(schedule_root.visible)
            {
                //task_month_paramList = taskController.getTaskList(isApproved);
                task_month_paramList = taskController.getTaskList(true);
            }
        }
        onVisibleChanged: {
            if(schedule_root.visible)
            {
                //task_month_paramList = taskController.getTaskList(isApproved);
                 task_month_paramList = taskController.getTaskList(true);

                /*for (var i = 0; i < task_month_paramList.length; i++)
                {
                    var currentTask = task_month_paramList[i];
                    for(let day = currentTask.startDay; day <= currentTask.endDay; day++)
                    {
                        highlightedDatesModel.append({
                                        "year": currentTask.year,
                                        "month": currentTask.month - 1,
                                        "day": day,
                                        "color": "red"
                                    });
                    }
                }*/

                showColor(year.currentText, month.currentIndex)

            }
        }
    }

    function loadSampleYears()
    {        
        for (var i = 2010; i < 2081; i++) {
             yearModel.append({"text" : i});
        }
    }

    function getDateColor(dateToCheck) {
        for (var i = 0; i < highlightedDatesModel.count; i++) {
            var eventData = highlightedDatesModel.get(i);
            if (eventData.year === dateToCheck.getFullYear() &&
                eventData.month === dateToCheck.getMonth() &&
                eventData.day === dateToCheck.getDate()) {
                return eventData.color;
            }
        }
        return "white";
    }

    function showColor(selectedYear, selectedMonth)
    {
        for (var i = 0; i < task_month_paramList.length; i++)
        {
            var currentTask = task_month_paramList[i];

            let result = selectedYear.localeCompare(currentTask.year);
            if(result === 0  && currentTask.month - 1 === selectedMonth)
            {
                for(let day = currentTask.startDay; day <= currentTask.endDay; day++)
                {
                    highlightedDatesModel.append({
                        "year": currentTask.year,
                        "month": currentTask.month - 1,
                        "day": day,
                        "color": "red"
                    });
                }
            }
        }
    }
}
