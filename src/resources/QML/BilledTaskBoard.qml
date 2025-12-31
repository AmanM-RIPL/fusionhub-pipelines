import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 1.15
import com.fh.models 1.0
import com.fh.controllers


Rectangle {
    id: billedTaskBoardRoot
    width: 296.5
    height: 544
    color: "#FAF9F8"

    WorkBillingLineController{
      id:workBillingLineController
    }

    property string txtTotalBilledTask: "0"
    property bool isApproved: true


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
            text: txtTotalBilledTask
            color: "#FFFFFF"
            font.pixelSize: 10
            font.weight: 400
            anchors.centerIn: parent
        }
    }


    Text{
        text: "Billed"
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
        visible: false

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
        visible: false

        Image{
            source: "qrc:/resources/images/close.svg"
            anchors.centerIn: parent
        }
    }


    Rectangle{
        id: listViewRect
        width: 276.5
        height: parent.height-50// 480
        color: parent.color
        //anchors.bottom: parent.bottom
        //anchors.bottomMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.horizontalCenter: parent.horizontalCenter


        ListModel{
            id: listModel

            /*ListElement{
                title: "Project Initiation"
                desc: "Project Initiation documents submitted to respective clients"
            }*/
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

    Component.onCompleted: {
        if(billedTaskBoardRoot.visible)
        {
            showList();
        }

    }

    onVisibleChanged: {
        if(billedTaskBoardRoot.visible)
        {
            showList();
        }
    }

    function showList()
    {
        listModel.clear();
        //console.log("Billed_task_isApproved:", isApproved)
       var billedTaskList = workBillingLineController.getBilledTaskList(isApproved);

        txtTotalBilledTask = String(billedTaskList.length);

        for(var i = 0; i < billedTaskList.length; i++ )
        {
            console.log("Billed_task_billedTaskList[i].id:", billedTaskList[i].id)
            listModel.append({
                             "title": String(billedTaskList[i].id) + "_" + billedTaskList[i].taskName,
                             "desc": billedTaskList[i].description
                        });
        }
    }
}
