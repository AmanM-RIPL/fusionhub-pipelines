import QtQuick
import QtQuick.Controls

Rectangle {
    width: 1440
    height: 1024 - (131 + 13)
    color: "#EDF1F4"


    Text{
        text: "Work Order"
        color: "#000000"
        font.pixelSize: 44
        font.weight: 700
        anchors.left: parent.left
        anchors.leftMargin: 100
        anchors.top: parent.top
        anchors.topMargin: 21
    }



    Text{
        text: "Choose Approval Type"
        color: "#000000"
        font.pixelSize: 14
        font.weight: 400
        anchors.left: parent.left
        anchors.leftMargin: 111
        anchors.top: parent.top
        anchors.topMargin: 134
    }


    CustomComboBox{
        width: 174
        height: 34
        anchors.left: parent.left
        anchors.leftMargin: 269
        anchors.top: parent.top
        anchors.topMargin: 127
    }


    Rectangle{
        width: 1280 + 7
        height: 547
        radius: 8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 105


        Rectangle{
            width: parent.width
            height: 45
            topLeftRadius: 8
            topRightRadius: 8


            Row {
                // height: parent.height
                anchors.fill: parent
                spacing: 0
                  Rectangle { width: 121;  height: parent.height; color: "#FFFFFF"; Text { anchors.verticalCenter: parent.verticalCenter; anchors.left: parent.left; anchors.leftMargin: 13; text: "Order Number"; color: "#323130"; font.weight: 700; font.pixelSize: 14 } topLeftRadius: 8 }
                  Rectangle { width: 1  ;  height: parent.height; color: "#888629"; opacity: 0.16}
                  Rectangle { width: 202;  height: parent.height; color: "#FFFFFF"; Text { anchors.verticalCenter: parent.verticalCenter; anchors.left: parent.left; anchors.leftMargin: 13; text: "Description"; color: "#323130"; font.weight: 700; font.pixelSize: 14 } }
                  Rectangle { width: 1  ;  height: parent.height; color: "#888629"; opacity: 0.16}
                  Rectangle { width: 172;  height: parent.height; color: "#FFFFFF"; Text { anchors.verticalCenter: parent.verticalCenter; anchors.left: parent.left; anchors.leftMargin: 13; text: "Vendor"; color: "#323130"; font.weight: 700; font.pixelSize: 14 } }
                  Rectangle { width: 1  ;  height: parent.height; color: "#888629"; opacity: 0.16}
                  Rectangle { width: 127;  height: parent.height; color: "#FFFFFF"; Text { anchors.verticalCenter: parent.verticalCenter; anchors.left: parent.left; anchors.leftMargin: 13; text: "Dollar value"; color: "#323130"; font.weight: 700; font.pixelSize: 14 } }
                  Rectangle { width: 1  ;  height: parent.height; color: "#888629"; opacity: 0.16}
                  Rectangle { width: 127;  height: parent.height; color: "#FFFFFF"; Text { anchors.verticalCenter: parent.verticalCenter; anchors.left: parent.left; anchors.leftMargin: 13; text: "Tax amount"; color: "#323130"; font.weight: 700; font.pixelSize: 14 } }
                  Rectangle { width: 1  ;  height: parent.height; color: "#888629"; opacity: 0.16}
                  Rectangle { width: 185;  height: parent.height; color: "#FFFFFF"; Text { anchors.verticalCenter: parent.verticalCenter; anchors.left: parent.left; anchors.leftMargin: 13; text: "Tax withholding amount"; color: "#323130"; font.weight: 700; font.pixelSize: 14 } }
                  Rectangle { width: 1  ;  height: parent.height; color: "#888629"; opacity: 0.16}
                  Rectangle { width: 172;  height: parent.height; color: "#FFFFFF"; Text { anchors.verticalCenter: parent.verticalCenter; anchors.left: parent.left; anchors.leftMargin: 13; text: "Link to a task"; color: "#323130"; font.weight: 700; font.pixelSize: 14 } }
                  Rectangle { width: 1  ;  height: parent.height; color: "#888629"; opacity: 0.16}
                  Rectangle { width: 172;  height: parent.height; color: "#FFFFFF"; Text { anchors.verticalCenter: parent.verticalCenter; anchors.left: parent.left; anchors.leftMargin: 13; text: "Retention Amount"; color: "#323130"; font.weight: 700; font.pixelSize: 14 } topRightRadius: 8 }
            }
        }

    }

}
