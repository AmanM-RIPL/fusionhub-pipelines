import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Window
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

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

        /*onNewAddUserClicked:{
            idCreateNewUserDialog.open()
        }*/

    }

    OrganizationSettingsPage{
        id: organizationSettingsPage
        anchors.fill: parent
        onLogOutClicked: {
            loginPage.visible = true
            welcomePage.visible = false
            organizationSettingsPage.visible = false
        }       
    }

    LoginPage{
        id: loginPage
        anchors.fill: parent

        onLoginClicked: {
            welcomePage.visible = true
            organizationSettingsPage.visible = false
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
            //baseLayout.visible = false
            //loginPage.visible = false
            //welcomePage.visible = false
            //userSettingsPage.visible = true
        }

        onOrganizationSettingsClicked:{
            baseLayout.visible = false
            loginPage.visible = false
            welcomePage.visible = false
            organizationSettingsPage.visible = true
        }


        /*******************CreateNewProjectDialog*******************/


        Dialog {
            ProjectController {
                id: projectController
            }
            id: idCreateNewProjectDialog
            title: "<font color=\"blue\">New Project</font>"
            width: 400
            height: 600
            font.weight: 700
            font.pixelSize: 18
            font.family: "Segoe UI"
            anchors.centerIn: parent
            modal: true
            closePolicy: Dialog.NoAutoClose
            standardButtons: Dialog.Ok | Dialog.Cancel

            contentItem:Rectangle {
                color: "transparent"
                radius: 0
                border.color: "transparent"
                border.width: 0
                anchors.topMargin: 1
                ColumnLayout{
                    anchors.fill: parent
                    anchors.margins: 15
                    anchors.topMargin: 1
                    spacing: 10
                    Rectangle{
                        width: parent.width
                        anchors.topMargin: 10
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        height: 15
                        color: "transparent"
                        radius: 0
                        border.color: "transparent"
                        Column{
                            anchors.fill: parent
                            spacing:5
                            Text {
                                id: idProjectNameLabal
                                width: parent.width
                                height: 15
                                text: "Project Name:"
                                font.weight: 700
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                            }
                            CustomTextBox{
                                id: idProjectNameValue
                                placeholderText: "Example: A00001"
                                color: "#323130"
                                anchors.leftMargin: 10
                                anchors.rightMargin: 10
                                width: parent.width-20
                                height: 40
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                            }
                        }
                    }

                    Rectangle{
                        width: parent.width
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        height: 15
                        color: "transparent"
                        radius: 0
                        border.color: "transparent"
                        Column{
                            anchors.fill: parent
                            spacing: 5
                            Text {
                                id: idCustomerNameLabel
                                anchors.leftMargin: 10
                                text: "Customer Name:"
                                width: 100
                                height: 15
                                font.weight: 700
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                            }
                            CustomTextBox{
                                id: idCustomerNameValue
                                anchors.leftMargin: 10
                                placeholderText: "Example: XYZ"
                                color: "#323130"
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                                width: parent.width - 20
                                height: 40
                            }
                        }
                    }

                    Rectangle{
                        width: parent.width
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        height: 15
                        color: "transparent"
                        radius: 0
                        border.color: "transparent"
                        Column{
                            anchors.fill: parent
                            spacing: 5
                            Text {
                                id: idContactNameLabel
                                anchors.leftMargin: 10
                                text: "Contact Name:"
                                width: 100
                                height: 15
                                font.weight: 700
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                            }
                            CustomTextBox{
                                id: idContactNameValue
                                anchors.leftMargin: 10
                                placeholderText: "Example: XYZ"
                                color: "#323130"
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                                width: parent.width - 20
                                height: 40
                            }
                        }
                    }

                    Rectangle{
                        width: parent.width
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        height: 15
                        color: "transparent"
                        radius: 0
                        border.color: "transparent"
                        Column{
                            anchors.fill: parent
                            spacing: 5
                            Text {
                                id:idPhoneNumberLabel
                                anchors.leftMargin: 10
                                text: "Customer Phone Number:"
                                width: 100
                                height: 15
                                font.weight: 700
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                            }
                            CustomTextBox{
                                id: idPhoneNumberValue
                                anchors.leftMargin: 10
                                placeholderText: "Example: +91-9999999999"
                                color: "#323130"
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                                width: parent.width - 20
                                height: 40
                            }
                        }
                    }

                    Rectangle{
                        width: parent.width
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        height: 15
                        color: "transparent"
                        radius: 0
                        border.color: "transparent"
                        Column{
                            anchors.fill: parent
                            spacing: 5
                            Text {
                                id:idEmailLabel
                                anchors.leftMargin: 10
                                text: "Customer Email:"
                                width: 100
                                height: 15
                                font.weight: 700
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                            }
                            CustomTextBox{
                                id: idEmailValue
                                anchors.leftMargin: 10
                                placeholderText: "Example: xyz@gmail.com"
                                color: "#323130"
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                                width: parent.width - 20
                                height: 40
                            }
                        }
                    }

                    Rectangle{
                        width: parent.width
                        height: 15
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        color: "transparent"
                        radius: 0
                        border.color: "transparent"
                        Column{
                            anchors.fill: parent
                            spacing: 5
                            Text {
                                id:idTotalDollarLabel
                                anchors.leftMargin: 10
                                text: "Total Dollar Value Of Project:"
                                width: 100
                                height: 15
                                font.weight: 700
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                            }
                            CustomTextBox{
                                id: idTotalDollarValue
                                anchors.leftMargin: 10
                                placeholderText: "Example: $500000"
                                color: "#323130"
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                                width: parent.width - 20
                                height: 40
                            }
                        }
                    }

                    Rectangle{
                        width: parent.width
                        height: 15
                        anchors.leftMargin: 10
                        anchors.rightMargin: 10
                        color: "transparent"
                        radius: 0
                        border.color: "transparent"
                        Column{
                            anchors.fill: parent
                            spacing: 5
                            Text {
                                id:idDescriptionLabel
                                anchors.leftMargin: 10
                                text: "Description:"
                                width: 100
                                height: 15
                                font.weight: 700
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                            }
                            CustomTextBox{
                                id: idDescriptionValue
                                anchors.leftMargin: 10
                                placeholderText: "Example: Write brief description"
                                color: "#323130"
                                font.pixelSize: 14
                                font.family: "Segoe UI"
                                width: parent.width - 20
                                height: 40
                            }
                        }
                    }
                }
            }
            onAccepted:
            {
                projectController.create(idProjectNameValue.text, idCustomerNameValue.text, idContactNameValue.text,
                                         idPhoneNumberValue.text, idEmailValue.text, idTotalDollarValue.text, idDescriptionValue.text);
            }
            onRejected:
            {
                console.log("Cancel button clicked")
            }
        }


        ///////////////
        /*
        FHPopup {
            id: idCreateNewProjectDialog
            popupWidth: 500
            popupHeight: 500
            title: "New Project"

            ProjectController {
                id: projectController
            }

            onAcceptCallback: function () {
                projectController.create(projectNameTextBox.text, customerNameTextBox.text, contactNameTextBox.text,
                                         phoneNumberTextBox.text, emaiTextBox.text, totalDollarValueTextBox.text,
                                         descriptionTextBox.text);


                projectNameTextBox.text = "";
                customerNameTextBox.text = "";
                contactNameTextBox.text = "";
                phoneNumberTextBox.text = "";
                emaiTextBox.text = "";
                totalDollarValueTextBox.text = "";
                descriptionTextBox.text = "";

                userRoot.userList = userController.getUserList();
            }

            onCancelCallback: function () {
                projectNameTextBox.text = "";
                customerNameTextBox.text = "";
                contactNameTextBox.text = "";
                phoneNumberTextBox.text = "";
                emaiTextBox.text = "";
                totalDollarValueTextBox.text = "";
                descriptionTextBox.text = "";
            }


           // Column {
             content: Column{
                width: parent.width
                height:500;//parent.height //30 for each top bottom

                Text{
                    id: projectNameLabel
                    text: "Proejct Name:"
                    color: "#323130"
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                    topPadding: 10
                }
                CustomTextBox{
                    id: projectNameTextBox
                    placeholderText: "Proejct Name"
                    text: ""
                    color: "#323130"
                }


                Text{
                    id: customerNameLabel
                    text: "Customer Name:"
                    color: "#323130"
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                    topPadding: 10
                }
                CustomTextBox{
                    id: customerNameTextBox
                    placeholderText: "Customer Name"
                    text: ""
                    color: "#323130"
                }


                Text{
                    id: contactNameLabel
                    text: "Contact Name:"
                    color: "#323130"
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                    topPadding: 10
                }
                CustomTextBox{
                    id: contactNameTextBox
                    placeholderText: "Contact Name"
                    text: ""
                    color: "#323130"
                }


                Text{
                    id: phoneNumberLabel
                    text: "Phone Number:"
                    color: "#323130"
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                    topPadding: 10
                }
                CustomTextBox{
                    id: phoneNumberTextBox
                    placeholderText: "phone number"
                    text: ""
                    color: "#323130"
                }


                Text{
                    id: emailLabel
                    text: "Email:"
                    color: "#323130"
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                    topPadding: 10
                }
                CustomTextBox{
                    id: emailTextBox
                    placeholderText: "Email"
                    text: ""
                    color: "#323130"
                }


                Text{
                    id: totalDollarLabel
                    text: "Total Dolar Value:"
                    color: "#323130"
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                    topPadding: 10
                }
                CustomTextBox{
                    id: totalDollarTextBox
                    placeholderText: "Total Dollar Value"
                    text: ""
                    color: "#323130"
                }


                Text{
                    id: descriptionLabel
                    text: "Description:"
                    color: "#323130"
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                    topPadding: 10
                }
                CustomTextBox{
                    id: descriptionTextBox
                    placeholderText: "Description"
                    text: ""
                    color: "#323130"
                }
            }
        }
        */
   }
/////

    /*******************End of CreateNewProjectDialog*******************/    



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
}
