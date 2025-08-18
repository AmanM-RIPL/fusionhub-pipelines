import QtQuick
import QtQuick.Controls

Rectangle {
    width: 1440
    height: 1024 - (131 + 13)
    color: "#EDF1F4"


    Image{
        source: "qrc:/resources/images/backArrow.svg"
        anchors.left: parent.left
        anchors.leftMargin: 100
        anchors.top: parent.top
        anchors.topMargin: 46
    }


    Text{
        text: "PROJ-0001: Sample House"
        color: "#000000"
        font.pixelSize: 44
        font.weight: 700
        anchors.left: parent.left
        anchors.leftMargin: 149
        anchors.top: parent.top
        anchors.topMargin: 21
    }



    Rectangle{
        width: 1236
        height: 599
        radius: 8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 138


        Rectangle{
            width: parent.width
            height: 53
            topLeftRadius: 8
            topRightRadius: 8
            color: "#FAF9F8"
            border.color: "#8A888629"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
        }

        Rectangle{
            width: parent.width
            height: 546
            color: "#EDF1F4"
            bottomLeftRadius: 8
            bottomRightRadius: 8
            border.color: "#8A888629"
            anchors.bottom: parent.bottom


            Row{
                spacing: 16
                width: parent.width - 2
                height: parent.height - 2
                anchors.centerIn: parent

                Rectangle{
                    width: 296.5
                    height: parent.height
                    bottomLeftRadius: 8
                    color: "#FAF9F8"

                    UpcomingTaskBoard{
                        anchors.fill: parent
                    }
                }

                Rectangle{
                    width: 296.5
                    height: parent.height
                    color: "#FAF9F8"

                    InprogressTaskBoard{
                        anchors.fill: parent
                    }
                }

                Rectangle{
                    width: 296.5
                    height: parent.height
                    color: "#FAF9F8"

                    MeasurementCompleteTaskBoard{
                        anchors.fill: parent
                    }
                }

                Rectangle{
                    width: 296.5
                    height: parent.height
                    color: "#FAF9F8"
                    bottomRightRadius: 8

                    BilledTaskBoard{
                        anchors.fill: parent
                    }
                }
            }

        }

    }
}
