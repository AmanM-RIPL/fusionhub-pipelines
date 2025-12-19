import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 1.15

Rectangle {
    width: 1440
   // height: 1024 - (131 + 13)
     height: screen.height-100
    color: "#EDF1F4"
    property string txtProjectName: "NA"

    Item {
        id:itemId
        width: 100
        height: 100


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
        //text: "PROJ-0001: Sample House"
        text: txtProjectName
        color: "#000000"
        font.pixelSize: 44
        font.weight: 700
        anchors.left: parent.left
        anchors.leftMargin: 149
        anchors.top: parent.top
        anchors.topMargin: 21
    }
   }



    Rectangle{
        id:mainRectId
        width: 1236
        height: 450//599
        radius: 8
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.bottom: parent.bottom
        //anchors.bottomMargin: 138
         anchors.top: itemId.bottom


        Rectangle{
            id:idrect
            width: parent.width
            height: 53
            topLeftRadius: 8
            topRightRadius: 8
            color: "#FAF9F8"
            border.color: "#8A888629"
            anchors.horizontalCenter: parent.horizontalCenter
           // anchors.top: parent.top
           anchors.top: mainRectId.top
        }


        Rectangle{
            width: parent.width
            height: 400//parent.height - 53  //546
            color: "#EDF1F4"
            bottomLeftRadius: 8
            bottomRightRadius: 8
            border.color: "#8A888629"
            //anchors.bottom: parent.bottom
            anchors.top: idrect.bottom
             clip: true

            RowLayout{
                spacing: 16
                width: parent.width - 2
                height: parent.height - 2
                anchors.centerIn: parent


               Rectangle{
                    width: 296.5
                    height: parent.height
                    bottomLeftRadius: 8
                    color: "#FAF9F8"
                    clip: true

                    UpcomingTaskBoard{
                        anchors.fill: parent

                    }
                }

                Rectangle{
                    width: 296.5
                    height: parent.height
                    color: "#FAF9F8"
                    clip: true

                    InprogressTaskBoard{
                        anchors.fill: parent
                    }
                }

                Rectangle{
                    width: 296.5
                    height: parent.height
                    color: "#FAF9F8"
                   clip: true

                    MeasurementCompleteTaskBoard{
                        anchors.fill: parent
                    }
                }

                Rectangle{
                    width: 296.5
                    height: parent.height
                    color: "#FAF9F8"
                    clip: true

                    bottomRightRadius: 8

                    BilledTaskBoard{
                        anchors.fill: parent
                    }
                }
            }

        }

    }
}
