import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Popup {
    id: popupRoot
    width: popupWidth
    height: popupHeight
    modal: true
    focus: true
    dim: true
    anchors.centerIn: parent
    background: Rectangle {
        width: popupWidth
        height: popupHeight
        color: "white"
    }

    property alias title: titleText.text
    property int popupWidth: 500
    property int popupHeight: 200
    property var onAcceptCallback: null
    property var onCancelCallback: null
    property var onApproveCallback: null

    property bool showAcceptButton: true
    property bool showApproveButton: false

    default property alias content: contentItem.children

    property string buttonName: "Add"
    property string buttonSource: "qrc:/resources/images/addWhite_icon.png"
    property bool buttonEnabled: true


    Rectangle {
        anchors.fill: parent
        color: "white"
        anchors.margins: 10

        Column {
            anchors.fill: parent
            spacing: 10

            Item {
                width: parent.width
                height: 30

                Text {
                    id: titleText
                    text: "New Budget Head"
                    color: "#007AFF"
                    font.family: "Segoe UI"
                    font.weight: 700
                    font.pixelSize: 15
                }

                CustomButton {
                    color: "#FFFFFF"
                    width: 20
                    height: 20
                    radius: 4
                    btnSource: "qrc:/resources/images/close.svg"
                    btnName: " "
                    btnNameColor: "white"
                    anchors.right: parent.right

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            popupRoot.close()
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#EDF1F4"
            }

            ScrollView {
                width: parent.width
                height: parent.height - 100

                Column {
                    id: contentItem
                    width: parent.width
                    spacing: 10
                }
            }

            Rectangle {
                width: parent.width
                height: 1
                color: "#EDF1F4"
            }

            Row {
                width: parent.width
                height: 30
                spacing: 20
                layoutDirection: Qt.RightToLeft

                // Accept Button
                CustomButton {
                    visible: popupRoot.showAcceptButton
                    enabled: popupRoot.buttonEnabled
                    color: popupRoot.buttonEnabled ? "#007AFF" : "#D0D0D0"
                    width: 84
                    height: 30
                    radius: 4
                    btnSource: buttonSource
                    btnName: buttonName
                    btnNameColor: popupRoot.buttonEnabled ? "white" : "#808080"

                    MouseArea{
                        anchors.fill: parent
                        enabled: popupRoot.buttonEnabled

                        onClicked: {
                            if (popupRoot.buttonEnabled) {
                                popupRoot.close()
                                if (popupRoot.onAcceptCallback) popupRoot.onAcceptCallback()
                            }
                        }
                    }
                }

                // Cancel Button
                CustomButton {
                    color: "#FFFFFF"
                    width: 84
                    height: 30
                    radius: 4
                    border.color: "#007AFF"
                    btnSource: "qrc:/resources/images/close.svg"
                    btnName: "Cancel"
                    btnNameColor: "grey"

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            popupRoot.close()
                            if (popupRoot.onCancelCallback) popupRoot.onCancelCallback()
                        }
                    }
                }

                // Approve Button
                CustomButton {
                    visible: popupRoot.showApproveButton
                    color: "#4CAF50"  // Green color
                    width: 84
                    height: 30
                    radius: 4
                    btnSource: "qrc:/resources/images/addWhite_icon.png"
                    btnName: "Approve"
                    btnNameColor: "white"

                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            popupRoot.close()
                            if (popupRoot.onApproveCallback) popupRoot.onApproveCallback()
                        }
                    }
                }
            }
        }
    }
}
