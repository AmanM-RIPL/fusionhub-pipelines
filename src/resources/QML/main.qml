import QtQuick 2.15
import QtQuick.Controls

ApplicationWindow {
    id: main
    width: 1440
    height: 1024
    visible: true

    LoginPage{
        id: loginPage
        anchors.fill: parent

        onLoginClicked: {
            welcomePage.visible = true
        }
    }

    WelcomePage{
        id: welcomePage
        anchors.fill: parent
        visible: false

        onLogOutClicked: {
            loginPage.visible = true
            welcomePage.visible = false
        }

        onNewProjectClicked: {
            baseLayout.visible = true
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
}
