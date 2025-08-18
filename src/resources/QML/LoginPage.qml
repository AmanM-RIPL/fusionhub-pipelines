import QtQuick 2.15
import QtQuick.Controls

Rectangle {
    // width: 1440
    // height: 1024
    // anchors.fill: parent

    signal loginClicked()

    Row {
        spacing: 10
        anchors.fill: parent

        Column {
            width: parent.width / 2
            height: parent.height
            spacing: 20
            padding: 20

            Image {
                source: "qrc:/resources/images/FusionHubLogo.png"
            }

            Rectangle {
                width: 100
                height: 100
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
            }


            Text{
                id: forgotPasswordText
                text: "Forgot Password"
                color: "#0078D4"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomButton{
                btnName: "Login"

                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        loginClicked()
                    }
                }
            }
        }

        Column {
            width: parent.width / 2
            height: parent.height

            Image {
                source: "qrc:/resources/images/LoginPoster.png"
            }
        }

    }
}
