import QtQuick 2.15
import QtQuick.Controls
import com.fh.controllers;
import QtQuick.Window 2.15
import QtQuick.Dialogs


Rectangle {
    id: root
    anchors.fill: parent
    color: "#EDF1F4"

    signal logOutClicked()
    signal newAddUserClicked()
    signal backClicked()
    signal showAddUserPermission()


    property string mandatoryColor: "#D13438"
    property string labelColor: "#323130"

    property var selectedUserData: null
    property int currentUserId: 0
    property int currentProjectId: 0

    property var userlist:[]

   // PermissionController {
   // id:permissionController
   // }

    // ========== MAIN UI ==========
    Column {
        anchors.fill: parent
        padding: 2

        // -------- Tool Bar --------
        Rectangle {
            id: toolbar
            width: parent.width - 20
            height: 75
            radius: 8
            border.width: 1
            border.color: "#7676801F"
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                spacing: 7
                padding: 10


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
                            //newProjectClicked();
                        }
                    }
                }

                CustomButton {
                    color: "transparent"
                    width: 186
                    height: 38
                    radius: 4
                    border.color: "#8080808C"
                    btnSource: "qrc:/resources/images/profile.svg"
                    btnName: "User Permission Setting"
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
                            userSettingsClicked();
                        }
                    }
                }

                Rectangle { width: 1; height: 60; color: "transparent" }
            }

            CustomButton {
                color: "#007AFF"
                width: 89
                height: 38
                radius: 4
                btnSource: "qrc:/resources/images/logout.svg"
                btnName: "Logout"
                btnNameColor: "white"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 13

                MouseArea {
                    anchors.fill: parent
                    onClicked: logOutClicked()
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
                topPadding: 50
                leftPadding: 50
                rightPadding: 50
                bottomPadding: 25

                Item {
                    width: 30
                    height: 38

                    Image {
                        id: backbuttonUsers
                        source: "qrc:/resources/images/backArrow.svg"
                        anchors.fill: parent
                        anchors.verticalCenter: parent.verticalCenter
                        fillMode: Image.PreserveAspectFit

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true
                            onClicked: {
                                addUserPage.visible = false
                                organizationSettingsPage.visible = true
                            }
                        }
                    }

                    Rectangle {
                        id: blankUsers
                        width: 10
                        height: 38
                        color: "transparent"
                        anchors.left: backbuttonUsers.right
                    }

                    Text {
                        text: "Users Permission"
                        color: "Black"
                        font.weight: 700
                        font.pixelSize: 40
                        font.family: "Segoe UI"
                        anchors.left: blankUsers.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }


            Column {
                id: userRoot
                width: parent.width
                padding: 10

                UserController { id: userController }

                    FHTable {
                        id: userTable
                        height: 400
                        leftPadding: 20
                        anchors {
                            top: toolbar.bottom
                           // bottom: parent.bottom
                            left: parent.left
                            right: parent.right
                            margins: 20
                        }
                        model: userlist
                        property bool allowPermissionView: false
                        columns: [
                            { label: "Id",         width: 250, key: "id"        },
                            { label: "Project",    width: 250, key: "project"   },
                            { label: "First Name", width: 200, key: "firstName" },
                            { label: "Last Name",  width: 200, key: "lastName"  },
                            { label: "User Name",  width: 200, key: "username"  },
                            { label: "Email",      width: 250, key: "email"     }
                        ]

                        // onViewRequested: function(row) {
                        //     root.popupMode = "View Permissions"
                        //     root.selectedUserData = row
                        //     fillEditPopup()
                        //     viewEditPopup.open()
                        // }

                        onEditRequested: function(row) {
                               console.log("row", JSON.stringify(row));
                               addUserPermissionPage.currentUserId = row.id
                               addUserPermissionPage.currentProjectId = 0
                               showAddUserPermission();
                        }

                    }
                    Component.onCompleted: showUserList()
                    onVisibleChanged: showUserList()
            }
        }
    }
// Strt Fetch User List Data From APi
    Timer {
        id: retryTimer
        interval: 250
        repeat: false
        onTriggered: {
            userController.getUserList()
        }
    }
    Connections {
        target: userController

        function onUserListReceived(data) {
            root.userlist = data
        }

        function onUserListFailed(error) {
            if (error.includes("login") && !retryTimer.running) {
              //  console.log("QML: Token might not be ready, starting retry timer...")
                retryTimer.start()
            }
        }
        function onUserDetailsReceived(details) {
            root.selectedUserData = details
                    fillEditPopup()
                    viewEditPopup.open()
            }

            function onUserDetailsFailed(error) {
                console.log("Error fetching user:", error)
            }
    }
    function showUserList() {
      //  console.log("QML: Requesting data...")
        userController.getUserList()
    }
// End Fetch User List Data From APi

}
