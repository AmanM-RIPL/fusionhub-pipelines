import QtQuick
import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts 1.15
//import Gantt 1.0
import QtCharts

Rectangle {
    width: 1440
    //height: 1024 - (131 + 13)
    height: screen.height-100
    color: "#EDF1F4"
    property string txtProjectName: "NA"
    property int febMonthWidth: 28
    property int monthScale: 5

    /*
    property var  taskParamList: [
        {"id":"1", "task":"Cutting", "duration":"20days", "start":"2/01/2025", "end":"22/01/2025"},
        {"id":"2", "task":"Bending", "duration":"10days", "start":"20/02/2025", "end":"30/02/2025"},
        {"id":"3", "task":"Clustering", "duration":"25days", "start":"5/03/2025", "end":"30/03/2025"}
    ]



    property var  monthParamList: [
        {"duration":"20days", "id":"1", "pid":"1", "jan":"20", "feb":"0", "mar":"0", "apr":"0", "may":"0", "jun":"0",
                                 "jul":"0", "aug":"0", "sep":"0", "oct":"0", "nov":"0", "dec":"0"},

        {"duration":"10days", "id":"2", "pid":"3", "jan":"0", "feb":"10", "mar":"0", "apr":"0", "may":"0", "jun":"0",
                                "jul":"0", "aug":"0", "sep":"0", "oct":"0", "nov":"0", "dec":"0"},

        {"duration":"25days", "id":"3", "pid":"1", "jan":"0", "feb":"0", "mar":"20", "apr":"0", "may":"0", "jun":"0",
                            "jul":"0", "aug":"0", "sep":"0", "oct":"0", "nov":"0", "dec":"0"}
    ]
    */


    property var  task_month_paramList: [
        {"id":"1", "pid":"1", "task":"Cutting", "duration":"20days", "days":"20", "startx":"2", "endx":"22", "start":"2/01/2025", "end":"22/01/2025",  "jan":"20", "feb":" ", "mar":" ", "apr":" ", "may":" ", "jun":" ",
                                 "jul":" ", "aug":" ", "sep":" ", "oct":" ", "nov":" ", "dec":" "},

        {"id":"2", "pid":"3", "task":"Bending", "duration":"10days", "days":"10", "startx":"20", "endx":"30", "start":"20/02/2025", "end":"30/02/2025",  "jan":" ", "feb":"10", "mar":" ", "apr":" ", "may":" ", "jun":" ",
                                "jul":" ", "aug":" ", "sep":" ", "oct":" ", "nov":" ", "dec":" "},

        {"id":"3", "pid":"1", "task":"Clustering", "duration":"25days", "days":"25", "startx":"5", "endx":"30", "start":"5/03/2025", "end":"30/03/2025", "jan":" ", "feb":" ", "mar":"25", "apr":" ", "may":" ", "jun":" ",
                            "jul":" ", "aug":" ", "sep":" ", "oct":" ", "nov":" ", "dec":" "}
    ]



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
        width: 1300//1236
        //height: 599
        height: 400
        radius: 8
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.bottom: parent.bottom
        //anchors.bottomMargin: 138
        anchors.top: itemId.bottom

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

            Text{
                text:"New Task"
                color: "#000000"
                font.pixelSize: 20
                font.weight: 700
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: 21
                anchors.leftMargin: 50
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
                    color: "green"
                    Column{
                        FHTable {
                            height: 300
                            leftPadding: 2
                            removeRow: false
                            headerHeight:102
                            headerFontPixelSize:20
                            model:task_month_paramList//taskParamList// costParameterList
                            columns: [
                                { label: "ID", width: 50, key: "id"},
                                { label: "Task", width: 256, key: "task" },
                                { label: "Duration", width: 110, key: "duration" },
                                { label: "Start", width: 110, key: "start" },
                                { label: "End", width: 110, key: "end" },
                            ]

                        }
                    }
                }//End of LeftSide


                Rectangle{//RightSide
                    width: parent.width/2-1
                    height: parent.height
                    //spacing: 3
                    color: "green"

                    ColumnLayout{
                        width: parent.width
                        height: parent.height
                        spacing: 2

                        Rectangle{
                            id:yearId
                            width: parent.width
                            height: 50//parent.height/2
                            color: "gray"

                            Text{
                                text:"Year"
                                color: "#000000"
                                font.pixelSize: 20
                                font.weight: 700
                                anchors.left: parent.left
                                anchors.top: parent.top
                                anchors.topMargin: 21
                                anchors.leftMargin: 50
                            }
                        }


                        Rectangle{
                            width: parent.width
                            height: parent.height-51
                            color: "gray"

                            ScrollView {
                                id: horizontalScrollView
                                width: parent.width
                                height: parent.height
                                padding: 1
                                clip: true

                                ColumnLayout{
                                    width: 31 * monthScale * 12 + 10//TotalMonths
                                    height: parent.height
                                    spacing: 2

                                    Rectangle{
                                        width: parent.width
                                        height: parent.height - 50
                                        color: "gray"

                                        Column{
                                            FHTable {
                                                height: 300
                                                leftPadding: 2
                                                removeRow: false
                                                headerHeight:50
                                                headerFontPixelSize:20
                                                model: task_month_paramList //monthParamList
                                                columns: [
                                                    { label: "Jan", width: 31 * monthScale, key: "jan", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                    { label: "Feb", width: febMonthWidth * monthScale, key: "feb", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                    { label: "Mar", width: 31  * monthScale, key: "mar", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                    { label: "Apr", width: 30 * monthScale, key: "apr", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                    { label: "May", width: 31 * monthScale, key: "may", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                    { label: "June", width: 30 * monthScale, key: "jun", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                    { label: "July", width: 31 * monthScale, key: "jul", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                    { label: "Aug", width: 31 * monthScale, key: "aug", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                    { label: "Sep", width: 30 * monthScale, key: "sep", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                    { label: "Oct", width: 31 * monthScale, key: "oct", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                    { label: "Nov", width: 30 * monthScale, key: "nov", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                    { label: "Dec", width: 31 * monthScale, key: "dec", startx: "startx", endx: "endx", days: "days", id: "id", pid: "pid" },
                                                ]
                                            }                                            
                                        }


                                        /*ChartView {
                                            //title: "Horizontal Percent Bar Chart"
                                            anchors.fill: parent
                                            legend.alignment: Qt.AlignBottom
                                            antialiasing: true

                                            HorizontalPercentBarSeries {
                                                axisY: BarCategoryAxis { categories: ["Jan", "Feb", "Mar", "Apr", "May", "June", "July", "Aug", "Sep", "Oct", "Nov", "Dec"] }
                                                BarSet { label: "Bob"; values: [2, 2, 3, 4, 5, 6] }
                                                BarSet { label: "Susan"; values: [5, 1, 2, 4, 1, 7] }
                                                BarSet { label: "James"; values: [3, 5, 8, 13, 5, 8] }
                                            }
                                        }*/
                                    }
                                }
                            }
                        }
                    }//End of RightSide
                }
            }
        }       
    }
}
