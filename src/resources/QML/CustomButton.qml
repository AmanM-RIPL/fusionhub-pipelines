import QtQuick

Rectangle {
    width: 79
    height: 34
    radius: 2
    color: "#0078D4"

    property string btnName: "Text"
    property string btnSource: ""
    property string btnNameColor: "#FFFFFF"

    Rectangle{
        width: btnImg.width + btnText.width + ((btnImg.source === "") ? 0 : 8)
        height: parent.height
        color: "transparent"
        anchors.centerIn: parent
        Image{
            id: btnImg
            source: btnSource
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
        }
        Text{
            id: btnText
            text: btnName
            color: btnNameColor
            font.weight: 700
            font.pixelSize: 14
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
