import QtQuick 2.15
import QtQuick.Controls
import com.fh.controllers;

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

                CustomButton {
                    color: "transparent"
                    width: 85
                    height: 38
                    btnSource: "qrc:/resources/images/backArrow.svg"
                    btnName: "Users"
                    btnNameColor: "black"
                    btnNamePixelSize: 40
                    //anchors.verticalCenter: parent.verticalCenter
                    anchors.centerIn: parent.Center

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {                            
                            addUserPage.visible = false
                            organizationSettingsPage.visible = true
                        }
                    }
                }               
            }


            Rectangle {
                width: 100
                height: 20
                color: "#EDF1F4"
            }

            //UserList{

            Column {
                // anchors.fill: parent
                //id: vendorRoot
                id: userRoot
                width: parent.width
                padding: 10

                //property var vendorList: []
                property var userList: []

                UserController {
                    id: userController
                }

                FHPopup {
                    id: newUserPopup
                    popupWidth: 500
                    popupHeight: 500
                    title: "New User"

                    onAcceptCallback: function () {
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


                   // Column {
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


                ///

                Rectangle {
                    width: 100
                    height: 40
                    color: "#EDF1F4"
                }

                FHTable {
                    height: 200
                    leftPadding: 20
                    model: userRoot.userList
                    columns: [
                        { label: "Full Name", width: 150, key: "user_fullname" },
                        { label: "User Name", width: 150, key: "user_name" },
                        { label: "Mobile1", width: 90, key: "mobile1" },
                        { label: "Mobile2", width: 90, key: "mobile2" },
                        { label: "Email1", width: 150, key: "email1" },
                        { label: "Email2", width: 150, key: "email2" },
                        { label: "Job Title", width: 100, key: "jobTitle" },
                        { label: "Start Date", width: 75, key: "startDate" },
                        { label: "End Date", width: 75, key: "endDate" },
                        { label: "Monthly Desk Cost(In Dollar)", width: 200, key: "monthlyDeskCostValue" },
                    ]
                }

                Component.onCompleted: {
                    //userList = userRepository.findAllQML();
                   userList  = userController.getUserList();
                }
            }

           // }

            Rectangle {
                width: 100
                height: 20
                color: "#EDF1F4"
            }
        }
    }
}

