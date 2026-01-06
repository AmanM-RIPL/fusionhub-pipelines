import QtQuick
import QtQuick.Controls
import QtQuick.Layouts 1.15
import com.fh.models 1.0
import com.fh.controllers

Rectangle {
    id:root
    width: 1440
   // height: 1024 - (131 + 13)
     height: screen.height-100
    color: "#EDF1F4"
    property string txtProjectName: "NA"
    property bool isApproved: true

    property var selectedData: null
    property string popupMode: "view"


    property var  taskList: []
    //property var task_idList: []

    property var  task: {"taskName" : "", "description" : "", "startDate" : "", "endDate" : "", "bimElement" : "", "pid" : "" }

    TaskController{
        id:taskController
    }

    /****************ViewEditPopupStart***********************/

    FHPopup {
        id: viewEditPopup
        popupWidth: 500
        popupHeight: 550
        title: popupMode === "view" ? "View Task" : "Edit Task"

        parent: Overlay.overlay

        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        //buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData) {
              //console.log("task:", JSON.stringify(task))

                taskController.update(taskNameTextBox.text, taskDescriptionTextBox.text,
                                taskBIMObjectTextBox.text, taskStartDateTextBox.text,
                                taskEndDateTextBox.text, task.pid, task.id, task.draftId);

                upcomingtaskBoardLoader.sourceComponent = undefined;
                upcomingtaskBoardLoader.sourceComponent = upcomingTaskComponent;
            }
        }

        onCancelCallback: function () {

        }

        onOpened: {
            // Fill popup with selected data
            if (selectedData) {
                fillPopup()
            }
        }

        Column {
            width: parent.width
            height: 300//parent.height //30 for each top bottom


            Text{
                id: taskNameLabel
                text: "Task Name"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }


            CustomTextBox{
                id: taskNameTextBox
                placeholderText: "Task Name"
                text:task.taskName
                color: "#323130"
                readOnly: popupMode === "view" ? true : false
            }


            Text{
                id: taskDescriptionLabel
                text: "Description"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }


            CustomTextBox{
                id: taskDescriptionTextBox
                placeholderText: "Description"
                text:task.description
                color: "#323130"
                readOnly: popupMode === "view" ? true : false
            }

            Text{
                id: taskStartDateLabel
                text: "Start Date"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox{
                id: taskStartDateTextBox
                placeholderText: "Start Date"
                text:task.startDate
                color: "#323130"
                readOnly: popupMode === "view" ? true : false

                // onActiveFocusChanged: {
                //     if (activeFocus) {
                //         calendarPopup_startDate.visible = true;

                //     }
                // }

                // MouseArea {
                //     anchors.fill: parent
                //     onClicked: {
                //         taskStartDateTextBox.forceActiveFocus();
                //     }
                // }
            }


            Text{
                id: taskEndDateLabel
                text: "End Date"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox{
                id: taskEndDateTextBox
                placeholderText: "End Date"
                text:task.endDate
                color: "#323130"
                readOnly: popupMode === "view" ? true : false
                // onActiveFocusChanged: {
                //     if (activeFocus) {
                //         calendarPopup_endDate.visible = true;

                //     }
                // }

                // MouseArea {
                //     anchors.fill: parent
                //     onClicked: {
                //         taskEndDateTextBox.forceActiveFocus();
                //     }
                // }
            }


            Text{
                id: taskBIMObjectLabel
                text: "BIM Object"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox{
                id: taskBIMObjectTextBox
                placeholderText: "BIM Object"
                text:task.bimElement
                color: "#323130"
                readOnly: popupMode === "view" ? true : false
            }


            Text{
                id: parentIdLabel
                text: "Parent Id"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            // CustomComboBox {
            //     id: parentIdComboBox
            //     model: task_idList
            //     width:200
            //     currentIndex: 0
            //     onCurrentTextChanged: {

            //     }
            // }

            CustomTextBox{
                id: parentIdComboBox
                text:task.pid
                color: "#323130"
                readOnly:true
            }
        }
    }

    /****************ViewEditPopupEnd*************************/

    Item {
        id:itemId
        width: 100
        height: 100


    Image{
        source: "qrc:/resources/images/backArrow.svg"
        anchors.left: parent.left
        anchors.leftMargin: 100
        anchors.top: parent.top
        anchors.topMargin: 46

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            hoverEnabled: true
            onClicked: {
                baseLayout.visible = false
                WelcomePage.visible = true
            }
        }
    }

    Text{
        //text: "PROJ-0001: Sample House"
        text: txtProjectName
        color: "#000000"
        font.pixelSize: 44
        font.weight: 700
        anchors.left: parent.left
        anchors.leftMargin: 149
        anchors.top: parent.top
        anchors.topMargin: 21
    }
   }


    Rectangle{
        id:idApprovalRect
        width: 500
        height: 50
        color: "transparent"
        //anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: itemId.bottom
        anchors.left: mainRectId.left

    Row {
        spacing: 20
        Text{
            id: approvalTypeLabel
            text: "Choose Approval Type"
            color: "#323130"
            font.weight: 700
            font.pixelSize: 14
            font.family: "Segoe UI"
            topPadding: 10
            //leftPadding: 20
        }


        CustomComboBox {
            id: approvalTypeComboBox
            model: ["Approved", "Draft"]
            width:200

            onCurrentTextChanged: {                
                isApproved = (approvalTypeComboBox.currentText === "Approved");
                // Force reset the loader
                upcomingtaskBoardLoader.sourceComponent = undefined;
                upcomingtaskBoardLoader.sourceComponent = upcomingTaskComponent;

                billedtaskBoardLoader.sourceComponent = undefined;
                billedtaskBoardLoader.sourceComponent = billedTaskComponent;
            }
        }       
    }
    }

    Component {
        id: upcomingTaskComponent
        UpcomingTaskBoard {
            // Use the ID of the parent/root variable so it's always accessible
            isApproved: root.isApproved

            onViewTask: function(row) {
                popupMode = "view"
                selectedData = row
                 //console.log("selectedData1:", JSON.stringify(selectedData))
                fillPopup()
                viewEditPopup.open()
            }

            onEditTask: function(row) {
                popupMode = "edit"
                selectedData = row
                //console.log("selectedData2:", JSON.stringify(selectedData))
                fillPopup()
                viewEditPopup.open()
            }
        }

    }


    Component {
        id: billedTaskComponent
        BilledTaskBoard {
            // Use the ID of the parent/root variable so it's always accessible
            isApproved: root.isApproved

            onViewTask: function(row) {
                popupMode = "view"
                selectedData = row
                 //console.log("selectedData1:", JSON.stringify(selectedData))
                fillPopup()
                viewEditPopup.open()
            }

        }
    }

    Rectangle{
        id:mainRectId
        width: 1236
        //height: 450//599
        height: 400//599
        radius: 8
        anchors.horizontalCenter: parent.horizontalCenter
        //anchors.bottom: parent.bottom
        //anchors.bottomMargin: 138
         anchors.top: idApprovalRect.bottom


        Rectangle{
            id:idrect
            width: parent.width
            height: 53
            topLeftRadius: 8
            topRightRadius: 8
            color: "#FAF9F8"
            border.color: "#8A888629"
            anchors.horizontalCenter: parent.horizontalCenter
           // anchors.top: parent.top
           anchors.top: mainRectId.top
        }


        Rectangle{
            width: parent.width
            height: 400//parent.height - 53  //546
            color: "#EDF1F4"
            bottomLeftRadius: 8
            bottomRightRadius: 8
            border.color: "#8A888629"
            //anchors.bottom: parent.bottom
            anchors.top: idrect.bottom
             clip: true

            RowLayout{
                spacing: 16
                width: parent.width - 2
                height: parent.height - 2
                anchors.centerIn: parent



               Rectangle{
                    width: 296.5
                    height: parent.height
                    bottomLeftRadius: 8
                    color: "#FAF9F8"
                    clip: true

                    /*UpcomingTaskBoard{
                        id:upcomingTaskboard
                        isApproved: isApproved
                        anchors.fill: parent

                    }*/

                    Loader {
                        id: upcomingtaskBoardLoader
                        anchors.fill: parent
                        sourceComponent: upcomingTaskComponent
                    }
                }

                Rectangle{
                    width: 296.5
                    height: parent.height
                    color: "#FAF9F8"
                    clip: true

                    InprogressTaskBoard{
                        anchors.fill: parent
                    }
                }

                Rectangle{
                    width: 296.5
                    height: parent.height
                    color: "#FAF9F8"
                   clip: true

                    MeasurementCompleteTaskBoard{
                        anchors.fill: parent
                    }
                }

                Rectangle{
                    width: 296.5
                    height: parent.height
                    color: "#FAF9F8"
                    clip: true

                    bottomRightRadius: 8

                    /*BilledTaskBoard{
                        id:billedTaskBoard
                        isApproved: isApproved
                        anchors.fill: parent
                    }*/

                    Loader {
                        id: billedtaskBoardLoader
                        anchors.fill: parent
                        sourceComponent: billedTaskComponent
                    }
                }
            }
        }
    }

    function fillPopup() {

        //task_idList = [];



        if (!selectedData){
            return
        }

        taskList = taskController.getTaskList(isApproved);
        for(var i = 0; i < taskList.length; i++)
        {
           var taskTemp =  taskList[i];
            if(selectedData.id === taskTemp.id)
            {
                task = taskTemp;
                //task_idList.push(String(task.pid));
                //console.log("fillPopup()::taskidList:",JSON.stringify(task_idList))
            }
        }

    }
}
