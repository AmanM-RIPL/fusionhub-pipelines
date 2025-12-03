import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers;

Column {
    // anchors.fill: parent
    id: materialIndentRoot
    width: parent.width
    padding: 10

    property var materialIndentList: []
    property var materialIndentListForTable: []
    property var materialText: []
    property var materialList: []

    property var taskText: []
    property var taskList: []

    property bool isApproved:false


    MaterialIndentController {
        id: materialIndentController
    }

    MaterialController
    {
        id:materialController
    }

    TaskController
    {
        id:taskController
    }

    FHPopup {
        id: newMaterialIndentPopup
        popupWidth: 500
        popupHeight: 350
        title: "New Material Indent"

        onAcceptCallback: function () {
            if (materialComboBox.currentIndex === -1 || taskComboBox.currentIndex === -1 || quantityTextBox.text === "") {
                console.log("Validation Error: Please select material, task and enter quantity");
                return;
            }

            materialIndentController.create(
                quantityTextBox.text,
                materialIndentRoot.materialList[materialComboBox.currentIndex].id,
                materialIndentRoot.taskList[taskComboBox.currentIndex].id
            );

            showList();

            // reset fields
            quantityTextBox.text = "";
            materialComboBox.currentIndex = -1;
            taskComboBox.currentIndex = -1;
        }


        onCancelCallback: function () {
            quantityTextBox.text = "";
            materialComboBox.currentIndex = -1;
            taskComboBox.currentIndex = -1;
        }

        Column {
            width: parent.width
            height: 200//parent.height //30 for each top bottom

            Text{
                id: materialLabel
                text: "Material"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: materialComboBox
                model: materialIndentRoot.materialText
            }

            Text{
                id: taskLabel
                text: "Task"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: taskComboBox
                model: materialIndentRoot.taskText
            }

            Text{
                id: quantityLabel
                text: "Quantity"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomTextBox{
                id: quantityTextBox
                placeholderText: "Quantity"
                text:""
                color: "#323130"
            }

        }

    }

    Row {
        spacing: 20
        Text {
            text: "Material Indent"
            color: "#000000"
            font.family: "Segoe UI"
            font.weight: 700
            font.pixelSize: 44
            leftPadding: 20
            // topPadding: 20
        }

        CustomButton {
            color: "#007AFF"
            width: 85
            height: 38
            radius: 4
            // border.color: "#007AFF"
            btnSource: "qrc:/resources/images/addWhite_icon.png"
            btnName: "New"
            btnNameColor: "white"
            anchors.verticalCenter: parent.verticalCenter

            MouseArea{
                anchors.fill: parent

                onClicked: {
                    newMaterialIndentPopup.open();
                }
            }
        }
    }

    Rectangle {
        width: 100
        height: 40
        color: "#EDF1F4"
    }

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
            leftPadding: 20
        }


        CustomComboBox {
            id: approvalTypeComboBox
            model: ["Approved", "Draft"]
            width:200

            onCurrentTextChanged: {
                if(approvalTypeComboBox.currentText === "Approved"){
                    isApproved = true
                }
                else{
                    isApproved = false;
                }
                showList();
            }
        }
    }

    Rectangle {
        width: 100
        height: 40
        color: "#EDF1F4"
    }



    FHTable {
        height: 200
        leftPadding: 20
        model: materialIndentRoot.materialIndentListForTable
        columns: [
            { label: "Id", width: 100, key: "id" },
            { label: "Quantity", width: 200, key: "quantity" },
            { label: "Material", width: 200, key: "material" },
            { label: "Task", width: 200, key: "task" }
        ]
    }

    Component.onCompleted: {
        showList();
    }

    onVisibleChanged: {
         showList();
    }

    function showList() {
        if (materialIndentRoot.visible) {

            // get data
            materialIndentList = materialIndentController.getMaterialIndentList(isApproved);
            materialList = materialController.getMaterialList(true);
            taskList = taskController.getTaskList(true);

            // Material list for dropdown
            materialText = materialList.map(m => m.materialName);

            // Task list for dropdown
            taskText = taskList.map(t => t.taskName);

            // Table binding
            materialIndentListForTable = materialIndentList.map(materialIndent => {

            console.log("Raw MaterialIndent:", JSON.stringify(materialIndent, null, 2));
                const material = materialList.find(x => x.id === materialIndent.materialId);
                const task = taskList.find(t => t.id === materialIndent.taskId);

                return {
                    id: materialIndent.id,
                    quantity: materialIndent.quantity,
                    material: material ? material.materialName : "Unknown",
                    task: task ? task.taskName : "Unknown"
                };
            });

          //  console.log("Raw MaterialIndent List:", JSON.stringify(materialIndentList, null, 2));

        }
    }

}

