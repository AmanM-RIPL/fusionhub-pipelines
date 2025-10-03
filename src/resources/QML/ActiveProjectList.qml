import QtQuick 2.15
import QtQuick.Controls
import com.fh.controllers;

Rectangle {
    width: 300 *(activeProjectsView.count) + 13 * (activeProjectsView.count - 1)
    height: 164
    color: "#EDF1F4"

    readonly property string jsonData: projectController.getProjectList().toString();
    property string txtDefault: "There is no active project at this moment."

    Text{
         id:defaultActiveText
         text: txtDefault
         color: "#323130"
         font.family: "Segoe UI"
         font.weight: 700
         font.pixelSize: 20
         anchors.left: parent.left
         anchors.leftMargin: 0
         anchors.top: parent.top
         anchors.topMargin: 16 - 4
     }


    ListModel{
        id: activeProjectsModel
    }


    ListView{
        id: activeProjectsView
        anchors.fill: parent
        model: activeProjectsModel
        spacing: 13
        clip: true
        orientation: ListView.Horizontal
        delegate: activeProjectDelegate

        ProjectController {
            id: projectController
        }
    }

    Component{
        id: activeProjectDelegate        

        Rectangle{
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

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true

                    onClicked: {
                       // projectClicked();
                         projectController.openDatabase(projectName, id)
                         baseLayout.visible = true
                         baseLayout.projectName = projectName
                    }
                }
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
                text:description
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
        }
    }

    Component.onCompleted: {
        parseAndPopulate(jsonData);
    }

    function parseAndPopulate(jsonString) {
        try {
            var dataArray = JSON.parse(jsonString);
            if (dataArray && Array.isArray(dataArray)) {
                activeProjectsModel.clear();
                for (var i = 0; i < dataArray.length; i++) {
                    activeProjectsModel.append(dataArray[i]);
                }
                txtDefault="";
            }
        } catch (e) {
            console.error("Error parsing JSON:", e);
            txtDefault = "There is no active project at this moment."
        }
    }
}
