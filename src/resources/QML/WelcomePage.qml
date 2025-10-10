import QtQuick 2.15
import QtQuick.Controls
import com.fh.models 1.0
import QtQuick.Layouts 1.3
import com.fh.controllers;
import QtQuick.Window 2.15

Rectangle {
    // width: 1440
    // height: 1024
    anchors.fill: parent
    color: "#EDF1F4"

    signal logOutClicked()
    signal newProjectClicked()
    signal userSettingsClicked()
    signal organizationSettingsClicked()

    property bool showOrgSettingsButton: true
    property bool showNewProjectButton: true
    property color showButtonColor: "#7676801F"

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
                    width: 123
                    height: 38
                    radius: 4
                    border.color: "#007AFF"
                    btnSource: "qrc:/resources/images/add.svg"
                    btnName: "New Project"
                    btnNameColor: "#007AFF"
                    anchors.verticalCenter: parent.verticalCenter
                    visible:showNewProjectButton

                    MouseArea{
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true

                        onEntered: {
                            parent.color = "#f0f0f5";
                        }

                        onExited: {
                            parent.color = "transparent";
                        }

                        onClicked: {                           
                            idCreateNewProjectDialog.open()
                        }
                    }
                }

                /*****Start of New Project Dialog********************/

                ProjectController {
                    id: projectController
                }

                FHPopup {
                    id: idCreateNewProjectDialog
                    popupWidth: 500
                    popupHeight: 500
                    title: "New Project"
                    parent: Overlay.overlay                    

                    onAcceptCallback: function () {
                       projectController.create(projectNameTextBox.text, customerNameTextBox.text, contactNameTextBox.text,
                                                 phoneNumberTextBox.text, emailTextBox.text, totalDollarValueTextBox.text,
                                                 descriptionTextBox.text);


                        projectNameTextBox.text = "";
                        customerNameTextBox.text = "";
                        contactNameTextBox.text = "";
                        phoneNumberTextBox.text = "";
                        emailTextBox.text = "";
                        totalDollarValueTextBox.text = "";
                        descriptionTextBox.text = "";                        

                        pageLoader.active = false
                        pageLoader.active = true                       

                    }

                    onCancelCallback: function () {
                        projectNameTextBox.text = "";
                        customerNameTextBox.text = "";
                        contactNameTextBox.text = "";
                        phoneNumberTextBox.text = "";
                        emailTextBox.text = "";
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
                            id: totalDollarValueLabel
                            text: "Total Dolar Value:"
                            color: "#323130"
                            font.weight: 700
                            font.pixelSize: 14
                            font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox{
                            id: totalDollarValueTextBox
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
                /**********End of AddedDialog************/

                Rectangle {
                    width: 1
                    height: 60
                   // color: "#7676801F"
                    color:showButtonColor
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
                    border.color: "#8080808C"
                    btnSource: "qrc:/resources/images/settings.svg"
                    btnName: "Organization Settings"
                    btnNameColor: "black"
                    anchors.verticalCenter: parent.verticalCenter
                    visible: showOrgSettingsButton

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
                            organizationSettingsClicked();
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
            id: projectColumn
            height: parent.height
            width: parent.width
            padding: 10

            Text {
                text: "Welcome to Dashboard"
                color: "#000000"
                font.family: "Segoe UI"
                font.weight: 700
                font.pixelSize: 44
                leftPadding: 10
                topPadding: 20
                bottomPadding: 10
            }

            Row {
                width: parent.width
                spacing: 10
                leftPadding: 10
                // topPadding: 40

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
                    anchors.verticalCenter: parent.verticalCenter
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

            ScrollView{
                id: scrollviewProject
                height: 190
                width: parent.width - 40
                ScrollBar.horizontal.policy: scrollviewProject.contentWidth > scrollviewProject.width ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AlwaysOff

            Loader {
                    id: pageLoader
                    source: "ActiveProjectList.qml" //The QML page to be reloaded

                }
            }


            Rectangle {
                width: 100
                height: 20
                color: "#EDF1F4"
            }

            Row {
                width: parent.width
                spacing: 10
                leftPadding: 10
                // topPadding: 40

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
                    anchors.verticalCenter: parent.verticalCenter
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
