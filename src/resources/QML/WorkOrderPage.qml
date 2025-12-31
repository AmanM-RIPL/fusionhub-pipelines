import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: workOrderLineRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var workOrdersFromCtrl: []
    property var taskFromCtrl: []
    property var vendorsFromCtrl: []

    // Properties - Lists for UI
    property var workOrderLineList: []
    property var vendorList: []
    property var taskList: []
    property var workOrderList: []

    // Properties - Work order line data
    property var workOrderLineData: []
    property var workOrderLineDataEdit: []

    // Properties - State management
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false

    // Controllers
    VendorController {
        id: vendorController
    }

    WorkOrderController {
        id: workOrderController
    }

    WorkOrderLineController {
        id: workOrderLineController
    }

    TaskController {
        id: taskController
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newWorkOrderLinePopup
        popupWidth: 900
        popupHeight: 600
        title: "Create Work Order"
        parent: Overlay.overlay

        onAcceptCallback: function () {
            if (workOrderLineData.length > 0) {
                workOrderLineController.create(
                            vendorsFromCtrl[vendor.currentIndex].id,
                            descriptionTextBox.text,
                            workOrderLineData
                            )
                // Reset
                workOrderLineData = []
                descriptionTextBox.text = ""
            }
            close()
        }

        onCancelCallback: function () {
            workOrderLineData = []
            descriptionTextBox.text = ""
            close()
        }

        onClosed: {
            showWorkOrderList()
        }

        onOpened: {
            // Load lists
            workOrderLineData = []
            descriptionTextBox.text = ""

            if (workOrderLineRoot.visible) {
                vendorsFromCtrl = vendorController.getVendorList(true)
                workOrdersFromCtrl = []
                taskFromCtrl = []

                workOrdersFromCtrl = workOrderController.getWorkOrderList(true)
                taskFromCtrl = taskController.getTaskList(true)

                // Clear current lists then populate
                workOrderLineRoot.vendorList = []
                workOrderLineRoot.workOrderList = []
                workOrderLineRoot.taskList = []

                for (var i = 0; i < vendorsFromCtrl.length; i++) {
                    workOrderLineRoot.vendorList = workOrderLineRoot.vendorList.concat(
                                vendorsFromCtrl[i].vendorName)
                }
                for (var j = 0; j < workOrdersFromCtrl.length; j++) {
                    workOrderLineRoot.workOrderList = workOrderLineRoot.workOrderList.concat(
                                workOrdersFromCtrl[j].description)
                }
                for (var k = 0; k < taskFromCtrl.length; k++) {
                    workOrderLineRoot.taskList = workOrderLineRoot.taskList.concat(
                                taskFromCtrl[k].taskName)
                }
            }
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Vendor + Description Row --------
            Row {
                width: parent.width
                spacing: 40

                Column {
                    spacing: 4
                    Text {
                        text: "Select Vendor"
                        color: "#323130"
                        font.weight: 700
                        font.pixelSize: 14
                    }
                    CustomComboBox {
                        id: vendor
                        width: 300
                        model: vendorList
                        currentIndex: 0
                    }
                }

                Column {
                    spacing: 4
                    Text {
                        text: "Description"
                        color: "#323130"
                        font.weight: 700
                        font.pixelSize: 14
                    }
                    CustomTextBox {
                        id: descriptionTextBox
                        placeholderText: "Work Order"
                        color: "#323130"
                        width: 500
                        height: 30
                    }
                }
            }

            // -------- Work Order Line Item --------
            Text {
                id: workOrderLineLabel
                text: "Work Order Item:"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            Rectangle {
                width: parent.width - 6
                height: 420
                color: "#EDF1F4"

                Column {
                    width: parent.width

                    FHTable {
                        id: popupTable
                        width: parent.width - 4
                        height: 300
                        leftPadding: 2
                        removeRow: true
                        model: workOrderLineData
                        columns: [
                            {
                                "label": "QTY",
                                "width": 120,
                                "key": "amount"
                            },
                            {
                                "label": "Description",
                                "width": 120,
                                "key": "description"
                            },
                            {
                                "label": "Tax Amount",
                                "width": 120,
                                "key": "tax_amount"
                            },
                            {
                                "label": "Tax With Holding",
                                "width": 120,
                                "key": "tax_with_holding"
                            },
                            {
                                "label": "Task",
                                "width": 184,
                                "key": "task_name"
                            },
                            {
                                "label": "Retention Amount",
                                "width": 175,
                                "key": "retention_amount"
                            }
                        ]
                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < workOrderLineData.length) {
                                var temp = []
                                for (var i = 0; i < workOrderLineData.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(workOrderLineData[i])
                                    }
                                }
                                workOrderLineData = temp
                            }
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width - 8
                height: 28
                color: "white"

                Row {
                    width: parent.width - 8
                    height: 28
                    leftPadding: 2
                    spacing: 6

                    CustomTextBox {
                        id: amountTextBox
                        placeholderText: "QTY"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: lineDescriptionTextBox
                        placeholderText: "Description"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxAmountTextBox
                        placeholderText: "Tax Amount"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxWithHoldingTextBox
                        placeholderText: "Tax With Holding"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomComboBox {
                        id: taskId
                        width: 184
                        height: 24
                        model: workOrderLineRoot.taskList
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: retentionAmountTextBox
                        placeholderText: "Retention Amount"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomButton {
                        color: "transparent"
                        width: 24
                        height: 24
                        border.color: "#8080808C"
                        btnSource: "qrc:/resources/images/add.svg"
                        btnName: ""
                        btnNameColor: "blue"

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true

                            onEntered: parent.color = "#f2f2f2"
                            onExited: parent.color = "transparent"

                            onClicked: {
                                var newElements = {
                                    "description": lineDescriptionTextBox.text,
                                    "task_id": String(taskFromCtrl[taskId.currentIndex].id),
                                    "task_name": taskId.currentValue,
                                    "amount": amountTextBox.text,
                                    "tax_amount": taxAmountTextBox.text,
                                    "tax_with_holding": taxWithHoldingTextBox.text,
                                    "retention_amount": retentionAmountTextBox.text
                                }

                                // Append to workOrderLineData
                                workOrderLineData = workOrderLineData.concat(newElements)

                                // Clear the inputs
                                lineDescriptionTextBox.text = ""
                                amountTextBox.text = ""
                                taxAmountTextBox.text = ""
                                taxWithHoldingTextBox.text = ""
                                retentionAmountTextBox.text = ""
                            }
                        }
                    }
                }
            }

            Item {
                width: 1
                height: 20
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 900
        popupHeight: 600
        title: popupMode === "view" ? "View Work Order" : "Edit Work Order"

        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData) {
                if (workOrderLineDataEdit.length > 0) {
                    workOrderLineController.update(
                                selectedData.id,
                                vendorsFromCtrl[vendorEdit.currentIndex].id,
                                descriptionTextBoxEdit.text,
                                workOrderLineDataEdit
                                )
                    // Reset
                    workOrderLineDataEdit = []
                    descriptionTextBoxEdit.text = ""
                    vendorEdit.currentIndex = -1
                }
                showWorkOrderList()
            }
        }

        onCancelCallback: function () {
            workOrderLineDataEdit = []
            descriptionTextBoxEdit.text = ""
        }

        onOpened: {
            // Load lists from controllers
            workOrderLineDataEdit = []
            descriptionTextBoxEdit.text = ""

            vendorsFromCtrl = vendorController.getVendorList(true)
            workOrdersFromCtrl = []
            taskFromCtrl = []

            workOrdersFromCtrl = workOrderController.getWorkOrderList(true)
            taskFromCtrl = taskController.getTaskList(true)

            // Clear and populate vendor and task lists
            var tempVendorList = []
            var tempTaskList = []

            for (var i = 0; i < vendorsFromCtrl.length; i++) {
                tempVendorList = tempVendorList.concat(vendorsFromCtrl[i].vendorName)
            }

            for (var k = 0; k < taskFromCtrl.length; k++) {
                tempTaskList = tempTaskList.concat(taskFromCtrl[k].taskName)
            }

            workOrderLineRoot.vendorList = tempVendorList
            workOrderLineRoot.taskList = tempTaskList

            // Fill popup with selected data
            if (selectedData) {
                fillPopup()
            }
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Vendor + Description Row --------
            Row {
                width: parent.width
                spacing: 40

                Column {
                    spacing: 4
                    Text {
                        text: "Select Vendor"
                        color: "#323130"
                        font.weight: 700
                        font.pixelSize: 14
                    }
                    CustomComboBox {
                        id: vendorEdit
                        width: 300
                        model: vendorList
                        currentIndex: 0
                        enabled: popupMode === "edit"
                    }
                }

                Column {
                    spacing: 4
                    Text {
                        text: "Description"
                        color: "#323130"
                        font.weight: 700
                        font.pixelSize: 14
                    }
                    CustomTextBox {
                        id: descriptionTextBoxEdit
                        placeholderText: "Work Order"
                        color: "#323130"
                        width: 500
                        height: 30
                        enabled: popupMode === "edit"
                    }
                }
            }

            // -------- WorkOrderLine Row --------
            Text {
                id: workOrderLineLabelEdit
                text: "Work Order Items:"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            Rectangle {
                width: parent.width - 6
                height: 420
                color: "#EDF1F4"

                Column {
                    width: parent.width

                    FHTable {
                        id: popupTableEdit
                        width: parent.width - 4
                        height: 300
                        leftPadding: 2
                        removeRow: popupMode === "edit"
                        model: workOrderLineDataEdit
                        columns: [
                            {
                                "label": "QTY",
                                "width": 120,
                                "key": "amount"
                            },
                            {
                                "label": "Description",
                                "width": 120,
                                "key": "description"
                            },
                            {
                                "label": "Tax Amount",
                                "width": 120,
                                "key": "tax_amount"
                            },
                            {
                                "label": "Tax With Holding",
                                "width": 120,
                                "key": "tax_with_holding"
                            },
                            {
                                "label": "Task",
                                "width": 184,
                                "key": "task_name"
                            },
                            {
                                "label": "Retention Amount",
                                "width": 175,
                                "key": "retention_amount"
                            }
                        ]
                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < workOrderLineDataEdit.length) {
                                var temp = []
                                for (var i = 0; i < workOrderLineDataEdit.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(workOrderLineDataEdit[i])
                                    }
                                }
                                workOrderLineDataEdit = temp
                            }
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width - 8
                height: 28
                color: "white"
                visible: popupMode === "edit"

                Row {
                    width: parent.width - 8
                    height: 28
                    leftPadding: 2
                    spacing: 6

                    CustomTextBox {
                        id: amountTextBoxEdit
                        placeholderText: "QTY"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: lineDescriptionTextBoxEdit
                        placeholderText: "Description"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxAmountTextBoxEdit
                        placeholderText: "Tax Amount"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxWithHoldingTextBoxEdit
                        placeholderText: "Tax With Holding"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomComboBox {
                        id: taskIdEdit
                        width: 184
                        height: 24
                        model: workOrderLineRoot.taskList
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: retentionAmountTextBoxEdit
                        placeholderText: "Retention Amount"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomButton {
                        color: "transparent"
                        width: 24
                        height: 24
                        border.color: "#8080808C"
                        btnSource: "qrc:/resources/images/add.svg"
                        btnName: ""
                        btnNameColor: "blue"

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true

                            onEntered: parent.color = "#f2f2f2"
                            onExited: parent.color = "transparent"

                            onClicked: {
                                var newElements = {
                                    "description": lineDescriptionTextBoxEdit.text,
                                    "task_id": String(taskFromCtrl[taskIdEdit.currentIndex].id),
                                    "task_name": taskIdEdit.currentValue,
                                    "amount": amountTextBoxEdit.text,
                                    "tax_amount": taxAmountTextBoxEdit.text,
                                    "tax_with_holding": taxWithHoldingTextBoxEdit.text,
                                    "retention_amount": retentionAmountTextBoxEdit.text
                                }

                                // Append to workOrderLineDataEdit
                                workOrderLineDataEdit = workOrderLineDataEdit.concat(newElements)

                                // Clear inputs
                                lineDescriptionTextBoxEdit.text = ""
                                amountTextBoxEdit.text = ""
                                taxAmountTextBoxEdit.text = ""
                                taxWithHoldingTextBoxEdit.text = ""
                                retentionAmountTextBoxEdit.text = ""
                            }
                        }
                    }
                }
            }

            Item {
                width: 1
                height: 20
            }
        }
    }

    // -------- Header Row --------
    Row {
        spacing: 20
        Text {
            text: "Work Order"
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
                onClicked: newWorkOrderLinePopup.open()
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
                    showWorkOrderList()
                }
            }
        }
    }

    Rectangle {
        width: 100
        height: 5
        color: "#EDF1F4"
    }

    // -------- Main Table --------
    FHTable {
        height: 200
        leftPadding: 20
        model: workOrderLineRoot.workOrderList

        columns: [
            {
                "label": "Id",
                "width": 300,
                "key": "id"
            },
            {
                "label": "Vendor",
                "width": 500,
                "key": "vendorName"
            },
            {
                "label": "Work Order",
                "width": 500,
                "key": "workOrderName"
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
    Component.onCompleted: showWorkOrderList()
    onVisibleChanged: showWorkOrderList()

    // -------- Functions --------
    function showWorkOrderList() {
        workOrderLineRoot.workOrderList = []

        if (!workOrderLineRoot.visible)
            return

        workOrderLineRoot.workOrderList = workOrderController.getWorkOrderList(isApproved)
        //  console.log("workOrderList", JSON.stringify(workOrderLineRoot.workOrderList))
    }

    function fillPopup() {
        if (!selectedData)
            return

        // Find and set vendor
        for (var i = 0; i < vendorsFromCtrl.length; i++) {
            if (vendorsFromCtrl[i].id === selectedData.vendorId) {
                vendorEdit.currentIndex = i
                break
            }
        }
        descriptionTextBoxEdit.text = selectedData.workOrderName || ""

        // Load line items
        var tempLines = []
        var allLines = workOrderLineController.getWorkOrderLineList(isApproved)
        for (var j = 0; j < allLines.length; j++) {
            if (allLines[j].id === selectedData.id) {
                var lineItem = {
                    "description": allLines[j].descriptionLine || "",
                    "task_id": String(allLines[j].taskId || ""),
                    "task_name": allLines[j].taskName || "",
                    "amount": String(allLines[j].amount || "0"),
                    "tax_amount": String(allLines[j].taxAmount || "0"),
                    "tax_with_holding": String(allLines[j].taxWithHolding || "0"),
                    "retention_amount": String(allLines[j].retentionAmount || "0")
                }
                tempLines.push(lineItem)
            }
        }

        workOrderLineDataEdit = tempLines
    }
}
