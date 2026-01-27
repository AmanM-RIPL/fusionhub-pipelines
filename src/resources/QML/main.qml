import QtQuick 2.15
import QtQuick.Controls
//import QtQuick.Window
//import QtQuick.Controls 2.15
//import QtQuick.Layouts 1.3

import com.fh.controllers;




ApplicationWindow {

    id: main
    // width: 1440
    // height: 1024
    visibility: Window.Maximized
    visible: true


    AddUserPage{
        id: addUserPage
        anchors.fill: parent

        onLogOutClicked: {
            loginPage.visible = true
            welcomePage.visible = false
            addUserPage.visible = false
        }
    }

    OrganizationSettingsPage{
        id: organizationSettingsPage
        anchors.fill: parent

        onLogOutClicked: {
            loginPage.visible = true
            welcomePage.visible = false
            organizationSettingsPage.visible = false
        }

        onUserSettingsClicked:{
            userSettingsPopup.open()
        }
    }

    LoginPage{
        id: loginPage
        anchors.fill: parent

        onLoginClicked: {
            welcomePage.visible = true
            organizationSettingsPage.visible = false
            if(userControllerUpdate.getCurrentUserName() === "admin")
            {
                welcomePage.showOrgSettingsButton = true
                welcomePage.showNewProjectButton = true
                welcomePage.showButtonColor = "#7676801F"
            }
            else
            {
                welcomePage.showOrgSettingsButton = false
                welcomePage.showNewProjectButton = false
                welcomePage.showButtonColor = "transparent"
            }
        }
    }

    WelcomePage{
        id: welcomePage
        anchors.fill: parent
        visible: false

        onLogOutClicked: {
            loginPage.visible = true
            welcomePage.visible = false
            organizationSettingsPage.visible = false
        }

        onNewProjectClicked: {
            //baseLayout.visible = true
            baseLayout.visible = false
            idCreateNewProjectDialog.open()
            organizationSettingsPage.visible = false
        }


        onUserSettingsClicked:{
            userSettingsPopup.open()
        }

        onOrganizationSettingsClicked:{
            baseLayout.visible = false
            loginPage.visible = false
            welcomePage.visible = false
            organizationSettingsPage.visible = true
        }
   }

    BaseLayout{
        id: baseLayout
        anchors.fill: parent
        visible: false

        onLogOutClicked: {
            loginPage.visible = true
            welcomePage.visible = false
            baseLayout.visible = false
        }
    }




    /***********This is common Popup Dialog only for UserSettings******************************/

    UserController {
        id: userControllerUpdate
    }

    FHPopup {
        id: userSettingsPopup
        popupWidth: 500
        popupHeight: 500
        title: "User Settings"
        parent: Overlay.overlay
        buttonName: "Update"
        buttonSource: ""

        onOpened: {
            tokenTextBox.text = userControllerUpdate.getToken();
            userFullNameTextBox.text = userControllerUpdate.getCurrentUserFullName();
            userNameTextBox.text = userControllerUpdate.getCurrentUserName();
            userMobile1TextBox.text = userControllerUpdate.getCurrentUserMobile1();
            userMobile2TextBox.text = userControllerUpdate.getCurrentUserMobile2();
            userEmail1TextBox.text = userControllerUpdate.getCurrentUserEmail1();
            userEmail2TextBox.text = userControllerUpdate.getCurrentUserEmail2();
            userJobTitleTextBox.text = userControllerUpdate.getCurrentUserJobTitle();
            userStartDateTextBox.text = userControllerUpdate.getCurrentUserStartDate();
            userEndDateTextBox.text = userControllerUpdate.getCurrentUserEndDate();
            //userMonthlyDeskCostTextBox.text = userControllerUpdate.getCurrentUserMonthlyDeskCostValue();
            userPasswordTextBox.text = userControllerUpdate.getCurrentUserPassword();
                }


        onAcceptCallback: function () {
            console.log("token: " + tokenTextBox.text);
            userControllerUpdate.update(userControllerUpdate.getCurrentUserId(),
                                  userFullNameTextBox.text,
                                  userNameTextBox.text,
                                  userMobile1TextBox.text,
                                  userMobile2TextBox.text,
                                  userEmail1TextBox.text,
                                  userEmail2TextBox.text,
                                  userJobTitleTextBox.text,
                                  userStartDateTextBox.text,
                                  userEndDateTextBox.text,
                                 // userMonthlyDeskCostTextBox.text,
                                  userControllerUpdate.getCurrentUserMonthlyDeskCostValue(),
                                  userPasswordTextBox.text);

            userFullNameTextBox.text = "";
            userNameTextBox.text = "";
            userMobile1TextBox.text = "";
            userMobile2TextBox.text = "";
            userEmail1TextBox.text = "";
            userEmail2TextBox.text = "";
            userJobTitleTextBox.text = "";
            userStartDateTextBox.text = "";
            userEndDateTextBox.text = "";
            //userMonthlyDeskCostTextBox.text = "";
            userPasswordTextBox.text = "";

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
            //userMonthlyDeskCostTextBox.text = "";
            userPasswordTextBox.text = ""
        }

         content: Column{
            width: parent.width
            height:650;//parent.height //30 for each top bottom
            Text{
                id: tokenLabel
                text: "Token:"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: tokenTextBox
                placeholderText: "token"
                text:""
                color: "#323130"
            }
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
                text:""
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
                readOnly: true

                MouseArea {
                    id: mouseArea
                    hoverEnabled: true
                    anchors.fill: parent
                }
                ToolTip.visible: mouseArea.containsMouse;
                ToolTip.text: "User name is not editable"
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


            /*Text{
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
            }*/

            Text{
                id: userPassword
                text: "Password:"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: userPasswordTextBox
                placeholderText: "Password"
                text: ""
                color: "#323130"
            }
        }
    }
    /*******************End of UserSetting PopupDialog*******************************************/
}
