import QtQuick
import QtQuick.Controls

Rectangle {
    width: 296.5
    height: 544
    color: "#FAF9F8"


    Rectangle{
        id: statusRect
        width: 20
        height: width
        radius: width/2
        color: "#0078D4"
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.left: parent.left
        anchors.leftMargin: 8

        Text{
            text: "4"
            color: "#FFFFFF"
            font.pixelSize: 10
            font.weight: 400
            anchors.centerIn: parent
        }
    }


    Text{
        text: "Upcoming"
        color: "#323130"
        font.pixelSize: 14
        font.weight: 400
        anchors.verticalCenter: statusRect.verticalCenter
        anchors.left: statusRect.right
        anchors.leftMargin: 10
    }



    Rectangle{
        width: 16
        height: 16
        color: parent.color
        anchors.right: parent.right
        anchors.rightMargin: 59
        anchors.top: parent.top
        anchors.topMargin: 17

        Image{
            source: "qrc:/resources/images/addBlack.svg"
            anchors.centerIn: parent
        }
    }

    Rectangle{
        width: 16
        height: 16
        color: parent.color
        anchors.right: parent.right
        anchors.rightMargin: 17
        anchors.top: parent.top
        anchors.topMargin: 17

        Image{
            source: "qrc:/resources/images/close.svg"
            anchors.centerIn: parent
        }
    }


    Rectangle{
        id: listViewRect
        width: 276.5
        height: 480
        color: parent.color
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter


        ListModel{
            id: listModel

            ListElement{
                title: "Termite Treatment"
                desc: "Providing pre-constrution anti-termite treatment at plinth level"
            }


            ListElement{
                title: "Concrete Laying"
                desc: "Providing and laying in position plain cement concrete of specified grade cement with trap/granite/quartzite/gneiss"
            }


            ListElement{
                title: "RMC Work"
                desc: "Lintels, beams, plinth beams, girders, floor"
            }


            ListElement{
                title: "RMC Work"
                desc: "Extra for providing two stage centering for height more than 4.5m and upto 9m height in terms of floor slabs including finishing"
            }
        }


        ListView{
            anchors.fill: parent
            spacing: 8
            clip: true
            model: listModel
            delegate: listDelegate
        }


        Component{
            id: listDelegate

            Rectangle{
                width: 276.5
                height: 114
                color: "#FFFFFF"

                Text{
                    text: title
                    color: "#323130"
                    font.pixelSize: 16
                    font.weight: 400
                    anchors.top: parent.top
                    anchors.topMargin: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                }

                Rectangle{
                    width: 16
                    height: 16
                    color: parent.color
                    anchors.right: parent.right
                    anchors.rightMargin: 21
                    anchors.top: parent.top
                    anchors.topMargin: 21

                    Image {
                        source: "qrc:/resources/images/dotMenu.svg"
                        anchors.centerIn: parent
                    }
                }


                Text{
                    text: desc
                    width: 252.5
                    height: 32
                    color: "#323130"
                    wrapMode: Text.Wrap
                    font.pixelSize: 12
                    font.weight: 400
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 12
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                }
            }
        }

    }


}
