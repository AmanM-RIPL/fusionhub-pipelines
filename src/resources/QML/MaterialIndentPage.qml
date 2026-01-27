import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: materialIndentRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var materialsFromCtrl: []
    property var tasksFromCtrl: []

    // Properties - Lists for UI
    property var materialIndentList: []
    property var materialList: []
    property var taskList: []

    // Properties - State management
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false

    // Controllers
    MaterialIndentController {
        id: materialIndentController
    }

    MaterialController {
        id: materialController
    }

    TaskController {
        id: taskController
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newMaterialIndentPopup
        popupWidth: 600
        popupHeight: 450
        title: "Create Material Indent"
        parent: Overlay.overlay

        onAcceptCallback: function () {
            if (materialComboBox.currentIndex === -1 || taskComboBox.currentIndex === -1 || quantityTextBox.text === "") {
                console.log("Validation Error: Please select material, task and enter quantity")
                return
            }

            materialIndentController.create(
                        quantityTextBox.text,
                        materialsFromCtrl[materialComboBox.currentIndex].id,
                        tasksFromCtrl[taskComboBox.currentIndex].id
                        )

            // Reset
            quantityTextBox.text = ""
            materialComboBox.currentIndex = -1
            taskComboBox.currentIndex = -1

            showMaterialIndentList()

            close()
        }

        onCancelCallback: function () {
            quantityTextBox.text = ""
            materialComboBox.currentIndex = -1
            taskComboBox.currentIndex = -1
            close()
        }

        onClosed: {
            showMaterialIndentList()
        }

        onOpened: {
            // Load lists
            quantityTextBox.text = ""
            materialComboBox.currentIndex = -1
            taskComboBox.currentIndex = -1

            if (materialIndentRoot.visible) {
                materialsFromCtrl = []
                tasksFromCtrl = []

                materialsFromCtrl = materialController.getMaterialList(true)
                tasksFromCtrl = taskController.getTaskList(true)

                // Clear current lists then populate
                materialIndentRoot.materialList = []
                materialIndentRoot.taskList = []

                for (var i = 0; i < materialsFromCtrl.length; i++) {
                    materialIndentRoot.materialList = materialIndentRoot.materialList.concat(
                                materialsFromCtrl[i].materialName)
                }
                for (var j = 0; j < tasksFromCtrl.length; j++) {
                    materialIndentRoot.taskList = materialIndentRoot.taskList.concat(
                                tasksFromCtrl[j].taskName)
                }
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
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
                width: parent.width
                model: materialIndentRoot.materialList
                currentIndex: -1
            }

            Text {
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
                width: parent.width
                model: materialIndentRoot.taskList
                currentIndex: -1
            }

            Text {
                id: quantityLabel
                text: "Quantity"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: quantityTextBox
                placeholderText: "Quantity"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 600
        popupHeight: 450
        title: popupMode === "view" ? "View Material Indent" : "Edit Material Indent"

        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData) {
                if (materialComboBoxEdit.currentIndex === -1 || taskComboBoxEdit.currentIndex === -1 || quantityTextBoxEdit.text === "") {
                    console.log("Validation Error: Please select material, task and enter quantity")
                    return
                }

                materialIndentController.update(
                            selectedData.id,
                            quantityTextBoxEdit.text,
                            materialsFromCtrl[materialComboBoxEdit.currentIndex].id,
                            tasksFromCtrl[taskComboBoxEdit.currentIndex].id
                            )

                // Reset
                quantityTextBoxEdit.text = ""
                materialComboBoxEdit.currentIndex = -1
                taskComboBoxEdit.currentIndex = -1

                showMaterialIndentList()
                close()
            }
        }

        onCancelCallback: function () {
            quantityTextBoxEdit.text = ""
            materialComboBoxEdit.currentIndex = -1
            taskComboBoxEdit.currentIndex = -1
            close()
        }

        onOpened: {
            // Load lists from controllers
            quantityTextBoxEdit.text = ""
            materialComboBoxEdit.currentIndex = -1
            taskComboBoxEdit.currentIndex = -1

            materialsFromCtrl = []
            tasksFromCtrl = []

            materialsFromCtrl = materialController.getMaterialList(true)
            tasksFromCtrl = taskController.getTaskList(true)

            // Clear and populate material and task lists
            var tempMaterialList = []
            var tempTaskList = []

            for (var i = 0; i < materialsFromCtrl.length; i++) {
                tempMaterialList = tempMaterialList.concat(materialsFromCtrl[i].materialName)
            }

            for (var k = 0; k < tasksFromCtrl.length; k++) {
                tempTaskList = tempTaskList.concat(tasksFromCtrl[k].taskName)
            }

            materialIndentRoot.materialList = tempMaterialList
            materialIndentRoot.taskList = tempTaskList

            // Fill popup with selected data
            if (selectedData) {
                fillPopup()
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                id: materialLabelEdit
                text: "Material"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: materialComboBoxEdit
                width: parent.width
                model: materialIndentRoot.materialList
                currentIndex: -1
                enabled: popupMode === "edit"
            }

            Text {
                id: taskLabelEdit
                text: "Task"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: taskComboBoxEdit
                width: parent.width
                model: materialIndentRoot.taskList
                currentIndex: -1
                enabled: popupMode === "edit"
            }

            Text {
                id: quantityLabelEdit
                text: "Quantity"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: quantityTextBoxEdit
                placeholderText: "Quantity"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
            }
        }
    }

    // -------- Header Row --------
    Row {
        spacing: 20
        Text {
            text: "Material Indent"
            color: "#000000"
            font.family: "Segoe UI"
            font.weight: 700
            font.pixelSize: 44
            leftPadding: 20
        }

        CustomButton {
            color: "#007AFF"
            width: 85
            height: 38
            radius: 4
            btnSource: "qrc:/resources/images/addWhite_icon.png"
            btnName: "New"
            btnNameColor: "white"
            anchors.verticalCenter: parent.verticalCenter

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: newMaterialIndentPopup.open()
            }
        }
    }

    Rectangle {
        width: 100
        height: 40
        color: "#EDF1F4"
    }

    // -------- Approval Type Selection --------
    Column {
        spacing: 20
        Row {
            spacing: 20
            anchors.left: parent.left

            Text {
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
                width: 200
                currentIndex: 0

                contentItem: Text {
                    text: approvalTypeComboBox.displayText
                    leftPadding: 10
                    verticalAlignment: Text.AlignVCenter
                    color: "#323130"
                    font.pixelSize: 14
                }

                onCurrentTextChanged: {
                    isApproved = (approvalTypeComboBox.currentText === "Approved")
                    showMaterialIndentList()
                }
            }
        }
    }

    Rectangle {
        width: 100
        height: 40
        color: "#EDF1F4"
    }

    // -------- Main Table --------
    FHTable {
        height: 200
        leftPadding: 20
        model: materialIndentRoot.materialIndentList

        columns: [
            {
                "label": "Id",
                "width": 200,
                "key": "id"
            },
            {
                "label": "Quantity",
                "width": 400,
                "key": "quantity"
            },
            {
                "label": "Material",
                "width": 350,
                "key": "materialName"
            },
            {
                "label": "Task",
                "width": 350,
                "key": "taskName"
            }
        ]

        onViewRequested: function(row) {
            popupMode = "view"
            selectedData = row
            fillPopup()
            viewEditPopup.open()
        }

        onEditRequested: function(row) {
            popupMode = "edit"
            selectedData = row
            fillPopup()
            viewEditPopup.open()
        }
    }

    // -------- Component Initialization --------
    Component.onCompleted: showMaterialIndentList()
    onVisibleChanged: showMaterialIndentList()

    // -------- Functions --------
    function showMaterialIndentList() {
        materialIndentRoot.materialIndentList = []

        if (!materialIndentRoot.visible)
            return

        materialIndentRoot.materialIndentList = materialIndentController.getMaterialIndentList(isApproved)
    }

    function fillPopup() {
        if (!selectedData)
            return

        // Find and set material
        for (var i = 0; i < materialsFromCtrl.length; i++) {
            if (materialsFromCtrl[i].id === selectedData.materialId) {
                materialComboBoxEdit.currentIndex = i
                break
            }
        }

        // Find and set task
        for (var j = 0; j < tasksFromCtrl.length; j++) {
            if (tasksFromCtrl[j].id === selectedData.taskId) {
                taskComboBoxEdit.currentIndex = j
                break
            }
        }

        quantityTextBoxEdit.text = selectedData.quantity || ""
    }
}
