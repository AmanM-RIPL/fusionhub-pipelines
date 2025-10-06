import QtQuick 2.15
import QtQuick.Controls
import com.fh.controllers;
import QtQuick.Window 2.15
import QtQuick.Dialogs


Rectangle{
    anchors.fill: parent
    color: "#EDF1F4"    


    signal logOutClicked()
    signal newAddUserClicked()   


    Column {
        anchors.fill: parent
        padding: 10

        /*
            Tool Bar
        */
        Rectangle {
            id:toolbar
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
                    width: 129
                    height: 38
                    radius: 4
                    border.color: "#8080808C"
                    btnSource: "qrc:/resources/images/add.svg"
                    btnName: "Add User"
                    btnNameColor: "blue"
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
                           //newAddUserClicked();
                            newUserPopup.open();
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    //color: "#7676801F"
                    color: "transparent"
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

                    Rectangle{
                        id: blankUsers
                        width: 10
                        height: 38
                        color:"transparent"
                        anchors.left: backbuttonUsers.right
                    }

                    Text{
                        text: "Users"
                        color:"Black"
                        font.weight: 700
                        font.pixelSize: 40
                        font.family: "Segoe UI"
                        anchors.left: blankUsers.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }          


            Column {
                // anchors.fill: parent               
                id: userRoot
                width: parent.width
                padding: 10                
                property var userList: []

                UserController {
                    id: userController
                }

                MessageDialog {
                        id: alertMessageDialog
                        title: "Alert!"
                        text: "Input text should not be blank."
                        buttons: MessageDialog.Ok
                        modality: Qt.ApplicationModal
                        onAccepted: {
                            //console.log("OK button clicked!");
                            // Perform actions after the dialog is accepted
                        }
                    }



                FHPopup {
                    id: newUserPopup
                    popupWidth: 500
                    popupHeight: 500
                    title: "New User"
                    parent: Overlay.overlay                    

                    onAcceptCallback: function () {
                        if(userFullNameTextBox.text === "" || userNameTextBox.text === "" || userMobile1TextBox.text === ""
                            || userMobile2TextBox.text === "" || userEmail1TextBox.text === "" || userEmail2TextBox.text === ""
                            || userJobTitleTextBox.text === "" || userStartDateTextBox.text === "" || userEndDateTextBox.text === ""
                            || userMonthlyDeskCostTextBox.text === "")
                        {
                            alertMessageDialog.open()

                        }
                        else{
                            userController.create(userFullNameTextBox.text,
                                                  userNameTextBox.text,
                                                  userMobile1TextBox.text,
                                                  userMobile2TextBox.text,
                                                  userEmail1TextBox.text,
                                                  userEmail2TextBox.text,
                                                  userJobTitleTextBox.text,
                                                  userStartDateTextBox.text,
                                                  userEndDateTextBox.text,
                                                  userMonthlyDeskCostTextBox.text);

                            userFullNameTextBox.text = "";
                            userNameTextBox.text = "";
                            userMobile1TextBox.text = "";
                            userMobile2TextBox.text = "";
                            userEmail1TextBox.text = "";
                            userEmail2TextBox.text = "";
                            userJobTitleTextBox.text = "";
                            userStartDateTextBox.text = "";
                            userEndDateTextBox.text = "";
                            userMonthlyDeskCostTextBox.text = "";

                            userRoot.userList = userController.getUserList();
                        }
                    }

                    onCancelCallback: function () {
                        userFullNameTextBox.text = "";
                        userNameTextBox.text = "";
                        userMobile1TextBox.text = "";
                        userMobile2TextBox.text = "";
                        userEmail1TextBox.text = "";
                        userEmail2TextBox.text = "";
                        userJobTitleTextBox.text = "";
                        userStartDateTextBox.text = "";
                        userEndDateTextBox.text = "";
                        userMonthlyDeskCostTextBox.text = "";
                    }

                     content: Column{
                        width: parent.width
                        height:650;//parent.height //30 for each top bottom

                        Text{
                            id: userFullNameLabel
                            text: "FullName:"
                            color: "#323130"
                            font.weight: 700
                            font.pixelSize: 14
                            font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox{
                            id: userFullNameTextBox
                            placeholderText: "User Full Name"
                            text: ""
                            color: "#323130"
                        }


                        Text{
                            id: userNameLabel
                            text: "UserName:"
                            color: "#323130"
                            font.weight: 700
                            font.pixelSize: 14
                            font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox{
                            id: userNameTextBox
                            placeholderText: "User Name"
                            text: ""
                            color: "#323130"
                        }


                        Text{
                            id: userMobile1Label
                            text: "User Mobile1"
                            color: "#323130"
                            font.weight: 700
                            font.pixelSize: 14
                            font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox{
                            id: userMobile1TextBox
                            placeholderText: "User Mobile1"
                            text: ""
                            color: "#323130"
                        }

                        Text{
                            id: userMobile2Label
                            text: "User Mobile2"
                            color: "#323130"
                            font.weight: 700
                            font.pixelSize: 14
                            font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox{
                            id: userMobile2TextBox
                            placeholderText: "User Mobile2"
                            text: ""
                            color: "#323130"
                        }


                        Text{
                            id: userEmail1Label
                            text: "User Email1"
                            color: "#323130"
                            font.weight: 700
                            font.pixelSize: 14
                            font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox{
                            id: userEmail1TextBox
                            placeholderText: "User Email1"
                            text: ""
                            color: "#323130"
                        }


                        Text{
                            id: userEmail2Label
                            text: "User Email2"
                            color: "#323130"
                            font.weight: 700
                            font.pixelSize: 14
                            font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox{
                            id: userEmail2TextBox
                            placeholderText: "User Email2"
                            text: ""
                            color: "#323130"
                        }


                        Text{
                            id: userJobTitle
                            text: "Job Tile:"
                            color: "#323130"
                            font.weight: 700
                            font.pixelSize: 14
                            font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox{
                            id: userJobTitleTextBox
                            placeholderText: "Job Title"
                            text: ""
                            color: "#323130"
                        }


                        Text{
                            id: userStartDate
                            text: "Start Date:"
                            color: "#323130"
                            font.weight: 700
                            font.pixelSize: 14
                            font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox{
                            id: userStartDateTextBox
                            placeholderText: "Start Date"
                            text: ""
                            color: "#323130"
                        }


                        Text{
                            id: userEndDate
                            text: "End Date:"
                            color: "#323130"
                            font.weight: 700
                            font.pixelSize: 14
                            font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox{
                            id: userEndDateTextBox
                            placeholderText: "End Date"
                            text: ""
                            color: "#323130"
                        }


                        Text{
                            id: userMonthlyDeskCost
                            text: "Monthly Desk Cost:"
                            color: "#323130"
                            font.weight: 700
                            font.pixelSize: 14
                            font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox{
                            id: userMonthlyDeskCostTextBox
                            placeholderText: "Monthly Desk Cost Value in Dollar"
                            text: ""
                            color: "#323130"
                        }
                    }
                }

                ScrollView{
                    //id: horizontalScrollView
                    //anchors.fill: parent
                    //clip: true
                    //height: 230
                    height: Screen.height - backbuttonUsers.y - backbuttonUsers.height
                            - toolbar.y - toolbar.height - blankUsers.y - blankUsers.height - 170
                    width: Screen.width - 60
                    ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
                    ScrollBar.vertical.policy: ScrollBar.AlwaysOff

                    FHTable {
                        //height: 200
                        height: Screen.height - backbuttonUsers.y - backbuttonUsers.height
                                - toolbar.y - toolbar.height - blankUsers.y - blankUsers.height - 200
                        leftPadding: 20
                        model: userRoot.userList


                        columns: [
                            { label: "Full Name", width: 250, key: "user_fullname" },
                            { label: "User Name", width: 250, key: "user_name" },
                            { label: "Mobile1", width: 100, key: "mobile1" },
                            { label: "Mobile2", width: 100, key: "mobile2" },
                            { label: "Email1", width: 250, key: "email1" },
                            { label: "Email2", width: 250, key: "email2" },
                            { label: "Job Title", width: 250, key: "jobTitle" },
                            { label: "Start Date", width: 150, key: "startDate" },
                            { label: "End Date", width: 150, key: "endDate" },
                            { label: "Monthly Desk Cost(In Dollar)", width: 200, key: "monthlyDeskCostValue" },
                        ]
                    }
                }

                Component.onCompleted: {                   
                    userRoot.userList  = userController.getUserList();
                }                
            }
        }
    }
}

