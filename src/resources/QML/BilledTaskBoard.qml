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

    property int activeRowIndex: -1

    // Signals
    signal viewTask(var row)
    //signal editTask(var row)


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
                required property var id
                required property string title
                required property string desc
                required property int index

                required property string taskName
                required property string startDate
                required property string endDate
                required property string bimElement
                required property string status
                required property var pid
                required property int draftId

                width: 276.5 + 15
                height: 114
                color: "#FFFFFF"

                //property var rowData: modelData
                property int rowIndex: index

                property var rowData: {"id": id, "taskName": taskName, "title": title, "desc": desc, "startDate": startDate, "endDate": endDate, "bimElement": bimElement, "pid":pid, "status": status, "draftId":draftId };



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
                        id: dots
                        source: "qrc:/resources/images/dotMenu.svg"
                        anchors.centerIn: parent

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                activeRowIndex = activeRowIndex === rowIndex ? -1 : rowIndex
                            }
                        }
                    }

                    // Action buttons
                    Column {
                        spacing: 6
                        anchors.right: dots.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 6
                        visible: activeRowIndex === rowIndex

                        Rectangle {
                            width: 35
                            height: 24
                            radius: 4
                            color: viewMouseArea.pressed ? "#0056b3" : (viewMouseArea.containsMouse ? "#0069d9" : "#007AFF")

                            Text {
                                anchors.centerIn: parent
                                text: "View"
                                color: "white"
                                font.pixelSize: 12
                            }

                            MouseArea {
                                id: viewMouseArea
                                anchors.fill: parent
                                hoverEnabled: true
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                   // var rowData = {"id": id, "title": title, "desc": desc };
                                    viewTask(rowData)
                                    activeRowIndex = -1
                                }
                            }
                        }
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

        if(!isApproved)
        {
            return;
        }

       var billedTaskList = workBillingLineController.getBilledTaskList(isApproved);
       // console.log("billedTaskList:", JSON.stringify(billedTaskList))

        txtTotalBilledTask = String(billedTaskList.length);

        for(var i = 0; i < billedTaskList.length; i++ )
        {
            var task =  billedTaskList[i];
            //console.log("Billed_task_billedTaskList[i].id:", billedTaskList[i].id)
            listModel.append({
                            "title": String(task.id) + "_" + task.taskName,
                            "desc": task.description,
                            "id":task.id,
                            "taskName": task.taskName,
                            "startDate": task.startDate,
                            "endDate": task.endDate,
                            "bimElement": task.bimElement,
                            "status": task.status,
                            "pid": task.pid,
                            "draftId": task.draftId
                        });
        }
    }
}
