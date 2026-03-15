import QtQuick 2.15

Rectangle {
    width: 70
    height: 70
    radius: 2

    property string btnName: "Text"
    property string btnSource: ""
    property string btnNameColor: "#FFFFFF"

    Column {
        width: btnImg.width + btnText.width + ((btnImg.source === "") ? 0 : 8)
        height: parent.height
        anchors.centerIn: parent
        spacing: 10

        Image{
            id: btnImg
            source: btnSource
            width: 40
            height: 40
            anchors.horizontalCenter: parent.horizontalCenter
            // anchors.left: parent.left
            sourceSize.width: 40
            sourceSize.height: 40
        }

        Text{
            id: btnText
            text: btnName
            color: btnNameColor
            font.weight: 400
            font.pixelSize: 14
            // anchors.right: parent.right
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}

