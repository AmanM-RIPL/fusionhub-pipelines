import QtQuick 2.15
import QtQuick.Controls.Basic

TextField {
    id: control
    width: 450
    height: 34
    placeholderText: qsTr("Enter description")
    placeholderTextColor: "#A4A4A4"
    color: "#A4A4A4"
    font.weight: 400
    font.pixelSize: 14

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        radius: 2
        color: control.enabled ? "transparent" : "transparent"//"#353637"
        border.color: control.enabled ? "#8A888629" : "#8A888629"
    }
}
