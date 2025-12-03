import QtQuick
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
//import Gantt 1.0
import QtCharts



Rectangle {
    width: 1440
    //height: 1024 - (131 + 13)
    height: screen.height-100
    color: "#EDF1F4"
    property string txtProjectName: "NA"

    property date startDate: new Date(2025, 0, 15) // Jan 15, 2025 (months are 0-based)
    property date endDate: new Date(2025, 0, 20)   // Jan 20, 2025
    property date selectedDate: new Date() // Initialize with today's date

    property var monthModel: ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'June', 'July', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']

    ListModel {
            id: yearModel
        }


    ListModel {
            id: highlightedDatesModel            
            // Adjust these years/months to match your current view
            ListElement { year: 2025; month: 0; day: 3; color: "red" }
            ListElement { year: 2025; month: 0; day: 4; color: "red" }
            ListElement { year: 2025; month: 0; day: 5; color: "red" }
            ListElement { year: 2025; month: 0; day: 6; color: "red" }
            ListElement { year: 2025; month: 0; day: 7; color: "red" }
            ListElement { year: 2025; month: 0; day: 8; color: "red" }
            ListElement { year: 2025; month: 0; day: 9; color: "red" }
            ListElement { year: 2025; month: 0; day: 10; color: "red" }
            ListElement { year: 2025; month: 0; day: 11; color: "red" }
            ListElement { year: 2025; month: 0; day: 12; color: "red" }
            ListElement { year: 2025; month: 0; day: 13; color: "red" }
            ListElement { year: 2025; month: 0; day: 14; color: "red" }
            ListElement { year: 2025; month: 0; day: 15; color: "red" }
            ListElement { year: 2025; month: 0; day: 16; color: "red" }
            ListElement { year: 2025; month: 0; day: 17; color: "red" }
            ListElement { year: 2025; month: 0; day: 18; color: "red" }
            ListElement { year: 2025; month: 0; day: 19; color: "red" }
            ListElement { year: 2025; month: 0; day: 20; color: "red" }
            ListElement { year: 2025; month: 0; day: 21; color: "red" }
            ListElement { year: 2025; month: 0; day: 22; color: "red" }

            ListElement { year: 2025; month: 1; day: 19; color: "red" }
            ListElement { year: 2025; month: 1; day: 20; color: "red" }
            ListElement { year: 2025; month: 1; day: 21; color: "red" }
            ListElement { year: 2025; month: 1; day: 22; color: "red" }
            ListElement { year: 2025; month: 1; day: 23; color: "red" }
            ListElement { year: 2025; month: 1; day: 24; color: "red" }
            ListElement { year: 2025; month: 1; day: 25; color: "red" }
            ListElement { year: 2025; month: 1; day: 26; color: "red" }
            ListElement { year: 2025; month: 1; day: 27; color: "red" }
            ListElement { year: 2025; month: 1; day: 28; color: "red" }



            ListElement { year: 2025; month: 2; day: 5; color: "red" }
            ListElement { year: 2025; month: 2; day: 6; color: "red" }
            ListElement { year: 2025; month: 2; day: 7; color: "red" }
            ListElement { year: 2025; month: 2; day: 8; color: "red" }
            ListElement { year: 2025; month: 2; day: 9; color: "red" }
            ListElement { year: 2025; month: 2; day: 10; color: "red" }
            ListElement { year: 2025; month: 2; day: 11; color: "red" }
            ListElement { year: 2025; month: 2; day: 12; color: "red" }
            ListElement { year: 2025; month: 2; day: 13; color: "red" }
            ListElement { year: 2025; month: 2; day: 14; color: "red" }
            ListElement { year: 2025; month: 2; day: 15; color: "red" }
            ListElement { year: 2025; month: 2; day: 16; color: "red" }
            ListElement { year: 2025; month: 2; day: 17; color: "red" }
            ListElement { year: 2025; month: 2; day: 18; color: "red" }
            ListElement { year: 2025; month: 2; day: 19; color: "red" }
            ListElement { year: 2025; month: 2; day: 20; color: "red" }
            ListElement { year: 2025; month: 2; day: 21; color: "red" }
            ListElement { year: 2025; month: 2; day: 22; color: "red" }
            ListElement { year: 2025; month: 2; day: 23; color: "red" }
            ListElement { year: 2025; month: 2; day: 24; color: "red" }
            ListElement { year: 2025; month: 2; day: 25; color: "red" }
            ListElement { year: 2025; month: 2; day: 26; color: "red" }
            ListElement { year: 2025; month: 2; day: 27; color: "red" }
            ListElement { year: 2025; month: 2; day: 28; color: "red" }
            ListElement { year: 2025; month: 2; day: 29; color: "red" }
            ListElement { year: 2025; month: 2; day: 30; color: "red" }
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
                        }

                        ComboBox {
                            id: year
                            height: 40
                            width: 150
                            model: yearModel
                            font.pixelSize: 20
                            font.weight: 700
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
                        topPadding: 0                        
                        month: month.currentIndex
                        year: yearModel.get(year.currentIndex).text
                        locale: Qt.locale("en_US")
                        font.pixelSize: 30
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
                                height: parent.height

                                property bool isSelected: (model.year === selectedDate.getFullYear() &&
                                                            model.month === selectedDate.getMonth() &&
                                                            model.day === selectedDate.getDate())

                                border.color: getDateColor(model.date)
                                color: getDateColor(model.date)
                                Label {
                                    text: model.day.toString()
                                    anchors.centerIn: parent
                                }
                                visible: parent.isInCurrentMonth
                            }
                        }

                        Component.onCompleted: {
                            var now = new Date ();
                            let currentYear = now.getFullYear().toString();

                            var currentIndex = -1;
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
}
