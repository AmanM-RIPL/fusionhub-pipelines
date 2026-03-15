import QtQuick 2.15
import QtQuick.Controls

Rectangle {
    anchors.fill: parent
    color: "#EDF1F4"

    signal logOutClicked()
    signal backClicked()
    signal organizationSettingsClicked()
    signal userSettingsClicked()
    signal userPermissionSettingsClicked()

    Column {
        anchors.fill: parent
        padding: 10

        /*
            Tool Bar
        */
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
                Rectangle {
                    width: 1
                    height: 60
                    //color: "#7676801F"
                    color: "transparent"

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

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true

                        onEntered: {
                            parent.color = "#f2f2f2";
                        }

                        onExited: {
                            parent.color = "transparent";
                        }

                        onClicked: {
                            userSettingsClicked();
                        }
                    }
                }


                CustomButton {
                    color: "transparent"
                    width: 186
                    height: 38
                    radius: 4
                    //border.color: "#8080808C"
                    border.color: "#007AFF"
                    btnSource: "qrc:/resources/images/settings.svg"
                    btnName: "Organization Settings"
                    //btnNameColor: "black"
                    btnNameColor: "#007AFF"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true

                        onEntered: {
                            parent.color = "#f2f2f2";
                        }

                        onExited: {
                            parent.color = "transparent";
                        }

                        onClicked: {
                            //newProjectClicked();
                        }
                    }
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

        Column {
            id: users
            height: parent.height
            width: parent.width
            padding: 10

            Row {
                spacing: 7
                padding: 50

                Item {
                    width: 30
                    height: 38
                    Image {
                        id: backbuttonAdmin
                        source: "qrc:/resources/images/backArrow.svg"
                        anchors.fill: parent
                        anchors.verticalCenter: parent.verticalCenter
                        fillMode: Image.PreserveAspectFit
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true
                            onClicked: {
                                welcomePage.visible = true
                                organizationSettingsPage.visible = false
                            }
                        }
                    }

                    Rectangle{
                        id: blankAdmin
                        width: 10
                        height: 38
                        color:"transparent"
                        anchors.left: backbuttonAdmin.right
                    }

                    Text{
                        text: "Admin"
                        color:"Black"
                        font.weight: 700
                        font.pixelSize: 40
                        font.family: "Segoe UI"
                        anchors.left: blankAdmin.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            //Users
            //{
                Rectangle{
                    width: 300
                    height: 164
                    radius: 8

                    //signal addUserClicked()

                    Image {
                        source: "qrc:/resources/images/profile.svg"
                        anchors.left: parent.left
                        anchors.leftMargin: 12
                        anchors.top: parent.top
                        anchors.topMargin: 13
                    }

                    Text{
                        text: "Users"
                        color: "#323130"
                        font.family: "Segoe UI"
                        font.weight: 700
                        font.pixelSize: 20
                        anchors.left: parent.left
                        anchors.leftMargin: 30
                        anchors.top: parent.top
                        anchors.topMargin: 6

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                userSettingsClicked()
                            }
                        }
                    }

                    Text{
                        width: 275
                        height: 42
                        text:"description"
                        color: "#323130"
                        font.family: "Segoe UI"
                        font.weight: 400
                        font.pixelSize: 16
                        wrapMode: Text.Wrap
                        anchors.centerIn: parent
                    }

                    Text {
                        id: viewAllUsersText
                        text: "View All Users"
                        font.bold: true
                        color: "blue"
                        font.family: "Segoe UI"
                        font.weight: 400
                        font.pixelSize: 16
                        anchors.left: parent.left
                        anchors.leftMargin: 12
                        anchors.top: parent.top
                        anchors.topMargin: 131

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor

                            onClicked: {
                                loginPage.visible = false
                                welcomePage.visible = false
                                organizationSettingsPage.visible = false
                                addUserPage.visible = true
                                addUserPermissionPage.visible = false
                            }
                        }
                    }
               //      Image {
               //          source: "qrc:/resources/images/addBlack.svg"
               //          anchors.left: parent.left
               //          anchors.leftMargin: 45
               //          anchors.top: parent.top
               //          anchors.topMargin: 138

               //          MouseArea {
               //              anchors.fill: parent
               //              cursorShape: Qt.PointingHandCursor
               //              hoverEnabled: true

               //              onClicked: {
               //                  loginPage.visible = false
               //                  welcomePage.visible = false
               //                  organizationSettingsPage.visible = false
               //                  addUserPage.visible = true
               //              }
               //          }
               //      }

                }

            //}
     //Space
            Rectangle {
                width: 100
                height: 20
                color: "#EDF1F4"
            }

    //Permission
            Rectangle{
                width: 300
                height: 164
                radius: 8

                //signal addUserClicked()

                Image {
                    source: "qrc:/resources/images/profile.svg"
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.top: parent.top
                    anchors.topMargin: 13
                }

                Text{
                    text: "Permission"
                    color: "#323130"
                    font.family: "Segoe UI"
                    font.weight: 700
                    font.pixelSize: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 30
                    anchors.top: parent.top
                    anchors.topMargin: 6

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            //projectClicked();
                        }
                    }
                }

                Text{
                    width: 275
                    height: 42
                    text:"User Permission description"
                    color: "#323130"
                    font.family: "Segoe UI"
                    font.weight: 400
                    font.pixelSize: 16
                    wrapMode: Text.Wrap
                    anchors.centerIn: parent
                }

                Text{
                    text: "View All Permissions"
                    font.bold: true
                    color: "blue"
                    font.family: "Segoe UI"
                    font.weight: 400
                    font.pixelSize: 16
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.top: parent.top
                    anchors.topMargin: 131

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            userPermissionSettingsClicked()
                        }
                    }
                }

               /* Image {
                    source: "qrc:/resources/images/addBlack.svg"
                    anchors.left: parent.left
                    anchors.leftMargin: 45
                    anchors.top: parent.top
                    anchors.topMargin: 138

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true

                        onClicked: {
                            loginPage.visible = false
                            welcomePage.visible = false
                            organizationSettingsPage.visible = false
                            addUserPage.visible = true
                        }
                    }
                } */
            }
    //space
            Rectangle {
                width: 100
                height: 20
                color: "#EDF1F4"
            }
        }
    }
}
