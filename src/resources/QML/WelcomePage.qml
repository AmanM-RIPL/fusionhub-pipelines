import QtQuick 2.15
import QtQuick.Controls

Rectangle {
    // width: 1440
    // height: 1024
    anchors.fill: parent
    color: "#EDF1F4"

    signal logOutClicked()
    signal newProjectClicked()

    Column {
        anchors.fill: parent
        padding: 10

        Rectangle {
            width: parent.width - 20
            height: 75
            radius: 8
            border.width: 1
            border.color: "#7676801F"//"#8A888629"
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter


            Row {
                spacing: 7
                padding: 10

                CustomButton {
                    color: "transparent"
                    width: 123
                    height: 38
                    radius: 4
                    border.color: "#007AFF"
                    btnSource: "qrc:/resources/images/add.svg"
                    btnName: "New Project"
                    btnNameColor: "#007AFF"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea{
                        anchors.fill: parent

                        onClicked: {
                            newProjectClicked()
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }

                CustomButton {
                    color: "transparent"
                    width: 129
                    height: 38
                    radius: 4
                    border.color: "#8080808C"
                    btnSource: "qrc:/resources/images/profile.svg"
                    btnName: "User Settings"
                    btnNameColor: "black"
                    anchors.verticalCenter: parent.verticalCenter
                }

                CustomButton {
                    color: "transparent"
                    width: 186
                    height: 38
                    radius: 4
                    border.color: "#8080808C"
                    btnSource: "qrc:/resources/images/settings.svg"
                    btnName: "Organization Settings"
                    btnNameColor: "black"
                    anchors.verticalCenter: parent.verticalCenter
                }
            }


            CustomButton {
                color: "#007AFF"
                width: 89
                height: 38
                radius: 4
                // border.color: "#007AFF"
                btnSource: "qrc:/resources/images/logout.svg"
                btnName: "Logout"
                btnNameColor: "white"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 13

                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        logOutClicked()
                    }
                }
            }


        }

        Rectangle {
            width: 100
            height: 40
            color: "#EDF1F4"
        }

        Column {
            id: projectColumn
            anchors.fill: parent
            padding: 10

            Text {
                text: "Welcome to Dashboard"
                color: "#000000"
                font.family: "Segoe UI"
                font.weight: 700
                font.pixelSize: 44
                leftPadding: 50
                topPadding: 100
            }

            Row {
                width: parent.width
                spacing: 10
                leftPadding: 70
                topPadding: 40

                Text{
                    id: activeText
                    text: "Active"
                    color: "#000000"
                    font.family: "Segoe UI"
                    font.weight: 700
                    font.pixelSize: 18
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    opacity: 0.44
                    color: "#8A888670"
                }
            }

            Rectangle {
                width: 100
                height: 20
                color: "#EDF1F4"
            }

            ActiveProjectList {
                onProjectClicked: {
                    newProjectClicked();
                }
            }

            Row {
                width: parent.width
                spacing: 10
                leftPadding: 70
                topPadding: 40

                Text{
                    id: inActiveText
                    text: "In-Active"
                    color: "#000000"
                    font.family: "Segoe UI"
                    font.weight: 700
                    font.pixelSize: 18
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    opacity: 0.44
                    color: "#8A888670"
                }
            }

            Rectangle {
                width: 100
                height: 20
                color: "#EDF1F4"
            }

            InActiveProjectList {
            }
        }
    }
}
