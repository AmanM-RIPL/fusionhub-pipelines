import QtQuick 2.15
import QtQuick.Controls

Rectangle {
   // width: 300 *(inActiveProjectsView.count) + 13 * (inActiveProjectsView.count - 1)
    width: parent.width
    height: 164
    color: "#EDF1F4"

    ListModel{
        id: activeProjectsModel


        ListElement{
            inactiveText: "There is no inactive project at this moment."
        }

       /* ListElement{
            projectName: "Project Name"
            dateData: "June 05"
            customerName: "Customer Name"
        }

        ListElement{
            projectName: "Project Name"
            dateData: "June 05"
            customerName: "Customer Name"
        }

        ListElement{
            projectName: "Project Name"
            dateData: "June 05"
            customerName: "Customer Name"
        }
        */
    }


    ListView{
        id: inActiveProjectsView
        anchors.fill: parent
        model: activeProjectsModel
        spacing: 13
        clip: true
        orientation: ListView.Horizontal
        delegate: activeProjectDelegate
    }

    Component{
        id: activeProjectDelegate

        Rectangle {
            width: parent.width
            height: 164
            radius: 0
            color: "#EDF1F4"


            Text{
                text: inactiveText
                color: "#323130"
                font.family: "Segoe UI"
                font.weight: 700
                font.pixelSize: 40
                anchors.left: parent.left
                //anchors.leftMargin: 51
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 16 - 4
            }
        }

       /* Rectangle{
            width: 300
            height: 164
            radius: 8

            Image {
                source: "qrc:/resources/images/activeIcon.svg"
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.top: parent.top
                anchors.topMargin: 13
            }

            Image {
                source: "qrc:/resources/images/dotMenu.svg"
                anchors.right: parent.right
                anchors.rightMargin: 13
                anchors.top: parent.top
                anchors.topMargin: 13
            }

            Text{
                text: projectName
                color: "#323130"
                font.family: "Segoe UI"
                font.weight: 700
                font.pixelSize: 16
                anchors.left: parent.left
                anchors.leftMargin: 51
                anchors.top: parent.top
                anchors.topMargin: 16 - 4
            }


            Text{
                text: dateData
                color: "#8A8886"
                font.family: "Segoe UI"
                font.weight: 400
                font.pixelSize: 12
                anchors.left: parent.left
                anchors.leftMargin: 51
                anchors.top: parent.top
                anchors.topMargin: 34 - 4
            }

            Text{
                width: 275
                height: 42
                text: "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
                color: "#323130"
                font.family: "Segoe UI"
                font.weight: 400
                font.pixelSize: 12
                wrapMode: Text.Wrap
                anchors.centerIn: parent
            }

            Text{
                text: customerName
                color: "#323130"
                font.family: "Segoe UI"
                font.weight: 400
                font.pixelSize: 12
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.top: parent.top
                anchors.topMargin: 141
            }

            Image {
                source: "qrc:/resources/images/customerIcons.svg"
                anchors.right: parent.right
                anchors.rightMargin: 13
                anchors.top: parent.top
                anchors.topMargin: 131
            }

        }*/
    }

}
