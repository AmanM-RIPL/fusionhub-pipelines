import QtQuick 2.15
import QtQuick.Controls
import com.fh.controllers;

Rectangle {
    // width: 1440
    // height: 1024
    // anchors.fill: parent

    signal loginClicked()

    UserController {
        id: userController
    }


    Row {
        spacing: 10
        anchors.fill: parent

        Column {
            width: parent.width / 2
            height: parent.height
            spacing: 20
            padding: 20

            Rectangle {
                width: 100
                height: 200
                visible: Screen.height > 1000 ? true : false //only when the screen is too large we show this to bring more white space
            }

            Image {
                source: "qrc:/resources/images/FusionHubLogo.png"
                anchors.left: parent.left
                anchors.margins: 0
            }

            Rectangle {
                width: 100
                height: 100
                visible: Screen.height < 1000 ? true : false // for large screens we don't need this
            }

            Row {
                spacing: 10

                Text{
                    id: loginText
                    text: "Log In"
                    color: "#605E5C"
                    font.family: "Segoe UI"
                    font.pixelSize: 34
                }

                Image{
                    source: "qrc:/resources/images/StopConstructionBoard.png"
                    height: 30
                    width: 30
                    anchors.top: parent.top
                    anchors.topMargin: 10
                }
            }

            Rectangle{
                width: parent.width
                height: 1
                color: "#8A888629"
                opacity: 0.16
            }

            Rectangle {
                id: loginErrorBox
                visible: false
                width: 450
                height: 25
                radius: 4
                color: "#fa2d2d"

                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    spacing: 10

                    Image{
                        source: "qrc:/resources/images/addWhite_icon.png"
                        height: 20
                        width: 20
                    }

                    Text {
                        text: "Invalid Username/Password!!"
                        font.pixelSize: 14
                        font.weight: 700
                        color: "#ffffff"
                    }
                }
            }

            Text{
                id: usernameLabel
                text: "Username"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }


            CustomTextBox{
                id: usernameTextBox
                placeholderText: "Username"
                color: "#323130"
            }


            Text{
                id: passwordLabel
                text: "Password"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }


            CustomTextBox {
                id: passwordTextBox
                placeholderText: "Password"
                echoMode: TextInput.Password
                color: "#323130"
            }


            Text{
                id: forgotPasswordText
                text: "Forgot Password"
                color: "#0078D4"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    hoverEnabled: true

                    onEntered: {
                        parent.font.underline = true;
                    }

                    onExited: {
                        parent.font.underline = false;
                    }

                    onClicked: {
                        // to be implemented later
                    }
                }
            }

            CustomButton{
                btnName: "Login"

                MouseArea{
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {

                        loginErrorBox.visible = false;
                        const loginResult = userController.login(usernameTextBox.text, passwordTextBox.text);

                        if (loginResult) {
                            loginClicked();
                        } else {
                            loginErrorBox.visible = true;
                        }
                    }
                }
            }
        }

        Column {
            width: parent.width / 2
            height: parent.height

            Image {
                source: "qrc:/resources/images/LoginPoster.png"
                width: parent.width
                height: parent.height
            }
        }

    }
}
