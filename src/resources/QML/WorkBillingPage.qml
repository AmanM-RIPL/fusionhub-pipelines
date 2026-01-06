import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: workBillingRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var workOrdersFromCtrl: []
    property var workOrderLinesFromCtrl: []

    // Properties - Lists for UI
    property var workBillingList: []
    property var workOrderList: []
    property var workOrderLineList: []

    // Properties - Work billing data
    property var workBillingLineData: []
    property var workBillingLineDataEdit: []

    // Properties - State management
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false

    // Controllers
    WorkOrderController {
        id: workOrderController
    }

    WorkOrderLineController {
        id: workOrderLineController
    }

    WorkBillingController {
        id: workBillingController
    }

    WorkBillingLineController {
        id: workBillingLineController
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newWorkBillingPopup
        popupWidth: 900
        popupHeight: 600
        title: "Create Work Billing"
        parent: Overlay.overlay

        onAcceptCallback: function () {
            if (workBillingLineData.length > 0) {
                workBillingLineController.create(
                            workOrdersFromCtrl[workOrderCombo.currentIndex].id,
                            descriptionTextBox.text,
                            workBillingLineData
                            )
                // Reset
                workBillingLineData = []
                descriptionTextBox.text = ""
            }
            close()
        }

        onCancelCallback: function () {
            workBillingLineData = []
            descriptionTextBox.text = ""
            close()
        }

        onClosed: {
            showWorkBillingList()
        }

        onOpened: {
            // Load lists
            workBillingLineData = []
            descriptionTextBox.text = ""

            if (workBillingRoot.visible) {
                workOrdersFromCtrl = []
                workOrderLinesFromCtrl = []

                workOrdersFromCtrl = workOrderController.getWorkOrderList(true)
                workOrderLinesFromCtrl = workOrderLineController.getWorkOrderLineList(true)

                // Clear current lists then populate
                workBillingRoot.workOrderList = []
                workBillingRoot.workOrderLineList = []

                for (var i = 0; i < workOrdersFromCtrl.length; i++) {
                    workBillingRoot.workOrderList = workBillingRoot.workOrderList.concat(
                                workOrdersFromCtrl[i].workOrderName)
                }
                for (var j = 0; j < workOrderLinesFromCtrl.length; j++) {
                    workBillingRoot.workOrderLineList = workBillingRoot.workOrderLineList.concat(
                                workOrderLinesFromCtrl[j].description)
                }
            }
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Work Order + Description Row --------
            Row {
                width: parent.width
                spacing: 40

                Column {
                    spacing: 4
                    Text {
                        text: "Select Work Order"
                        color: "#323130"
                        font.weight: 700
                        font.pixelSize: 14
                    }
                    CustomComboBox {
                        id: workOrderCombo
                        width: 300
                        model: workOrderList
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
                        placeholderText: "Work Billing Description"
                        color: "#323130"
                        width: 500
                        height: 30
                    }
                }
            }

            // -------- Work Billing Line Item --------
            Text {
                id: workBillingLineLabel
                text: "Work Billing Items:"
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

                    FHTable{
                        id: popupTable
                        width: parent.width - 4
                        height: 300
                        leftPadding: 2
                        removeRow: true
                        model: workBillingLineData
                        columns: [
                            {
                                "label": "Work Order Line",
                                "width": 200,
                                "key": "work_order_line_name"
                            },
                            {
                                "label": "Amount",
                                "width": 150,
                                "key": "dollar_value"
                            },
                            {
                                "label": "Tax Amount",
                                "width": 150,
                                "key": "tax_amount"
                            },
                            {
                                "label": "Tax Withholding",
                                "width": 150,
                                "key": "tax_withholding_amount"
                            },
                            {
                                "label": "Retention Amount",
                                "width": 170,
                                "key": "retention_amount"
                            }
                        ]
                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < workBillingLineData.length) {
                                var temp = []
                                for (var i = 0; i < workBillingLineData.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(workBillingLineData[i])
                                    }
                                }
                                workBillingLineData = temp
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

                    CustomComboBox {
                        id: workOrderLineCombo
                        width: 180
                        height: 24
                        model: workBillingRoot.workOrderLineList
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: dollarValueTextBox
                        placeholderText: "Amount"
                        text: ""
                        color: "#323130"
                        width: 150
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxAmountTextBox
                        placeholderText: "Tax Amount"
                        text: ""
                        color: "#323130"
                        width: 150
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxWithholdingTextBox
                        placeholderText: "Tax Withholding"
                        text: ""
                        color: "#323130"
                        width: 140
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: retentionAmountTextBox
                        placeholderText: "Retention Amount"
                        text: ""
                        color: "#323130"
                        width: 150
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
                                    "work_order_line_id": String(workOrderLinesFromCtrl[workOrderLineCombo.currentIndex].id),
                                    "work_order_line_name": workOrderLineCombo.currentValue,
                                    "dollar_value": dollarValueTextBox.text,
                                    "tax_amount": taxAmountTextBox.text,
                                    "tax_withholding_amount": taxWithholdingTextBox.text,
                                    "retention_amount": retentionAmountTextBox.text
                                }

                                // Append to workBillingLineData
                                workBillingLineData = workBillingLineData.concat(newElements)

                                // Clear the inputs
                                dollarValueTextBox.text = ""
                                taxAmountTextBox.text = ""
                                taxWithholdingTextBox.text = ""
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
        title: popupMode === "view" ? "View Work Billing" : "Edit Work Billing"

        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData) {
                if (workBillingLineDataEdit.length > 0) {
                    workBillingLineController.update(
                                selectedData.id,
                                workOrdersFromCtrl[workOrderComboEdit.currentIndex].id,
                                descriptionTextBoxEdit.text,
                                workBillingLineDataEdit
                                )
                    // Reset
                    workBillingLineDataEdit = []
                    descriptionTextBoxEdit.text = ""
                    workOrderComboEdit.currentIndex = -1
                }
                showWorkBillingList()
            }
        }

        onCancelCallback: function () {
            workBillingLineDataEdit = []
            descriptionTextBoxEdit.text = ""
        }

        onOpened: {
            // Load lists from controllers
            workBillingLineDataEdit = []
            descriptionTextBoxEdit.text = ""

            workOrdersFromCtrl = []
            workOrderLinesFromCtrl = []

            workOrdersFromCtrl = workOrderController.getWorkOrderList(true)
            workOrderLinesFromCtrl = workOrderLineController.getWorkOrderLineList(true)

            // Clear and populate work order and work order line lists
            var tempWorkOrderList = []
            var tempWorkOrderLineList = []

            for (var i = 0; i < workOrdersFromCtrl.length; i++) {
                tempWorkOrderList = tempWorkOrderList.concat(workOrdersFromCtrl[i].workOrderName)
            }

            for (var k = 0; k < workOrderLinesFromCtrl.length; k++) {
                tempWorkOrderLineList = tempWorkOrderLineList.concat(workOrderLinesFromCtrl[k].description)
            }

            workBillingRoot.workOrderList = tempWorkOrderList
            workBillingRoot.workOrderLineList = tempWorkOrderLineList

            // Fill popup with selected data
            if (selectedData) {
                fillPopup()
            }
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Work Order + Description Row --------
            Row {
                width: parent.width
                spacing: 40

                Column {
                    spacing: 4
                    Text {
                        text: "Select Work Order"
                        color: "#323130"
                        font.weight: 700
                        font.pixelSize: 14
                    }
                    CustomComboBox {
                        id: workOrderComboEdit
                        width: 300
                        model: workOrderList
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
                        placeholderText: "Work Billing Description"
                        color: "#323130"
                        width: 500
                        height: 30
                        enabled: popupMode === "edit"
                    }
                }
            }

            // -------- Work Billing Line Row --------
            Text {
                id: workBillingLineLabelEdit
                text: "Work Billing Items:"
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
                        model: workBillingLineDataEdit
                        columns: [
                            {
                                "label": "Work Order Line",
                                "width": 200,
                                "key": "work_order_line_name"
                            },
                            {
                                "label": "Amount",
                                "width": 150,
                                "key": "dollar_value"
                            },
                            {
                                "label": "Tax Amount",
                                "width": 150,
                                "key": "tax_amount"
                            },
                            {
                                "label": "Tax Withholding",
                                "width": 150,
                                "key": "tax_withholding_amount"
                            },
                            {
                                "label": "Retention Amount",
                                "width": 170,
                                "key": "retention_amount"
                            }
                        ]
                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < workBillingLineDataEdit.length) {
                                var temp = []
                                for (var i = 0; i < workBillingLineDataEdit.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(workBillingLineDataEdit[i])
                                    }
                                }
                                workBillingLineDataEdit = temp
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

                    CustomComboBox {
                        id: workOrderLineComboEdit
                        width: 180
                        height: 24
                        model: workBillingRoot.workOrderLineList
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: dollarValueTextBoxEdit
                        placeholderText: "Amount"
                        text: ""
                        color: "#323130"
                        width: 150
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxAmountTextBoxEdit
                        placeholderText: "Tax Amount"
                        text: ""
                        color: "#323130"
                        width: 150
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxWithholdingTextBoxEdit
                        placeholderText: "Tax Withholding"
                        text: ""
                        color: "#323130"
                        width: 140
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: retentionAmountTextBoxEdit
                        placeholderText: "Retention Amount"
                        text: ""
                        color: "#323130"
                        width: 140
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
                                    "work_order_line_id": String(workOrderLinesFromCtrl[workOrderLineComboEdit.currentIndex].id),
                                    "work_order_line_name": workOrderLineComboEdit.currentValue,
                                    "dollar_value": dollarValueTextBoxEdit.text,
                                    "tax_amount": taxAmountTextBoxEdit.text,
                                    "tax_withholding_amount": taxWithholdingTextBoxEdit.text,
                                    "retention_amount": retentionAmountTextBoxEdit.text
                                }

                                // Append to workBillingLineDataEdit
                                workBillingLineDataEdit = workBillingLineDataEdit.concat(newElements)

                                // Clear inputs
                                dollarValueTextBoxEdit.text = ""
                                taxAmountTextBoxEdit.text = ""
                                taxWithholdingTextBoxEdit.text = ""
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
            text: "Work Billing"
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
                onClicked: newWorkBillingPopup.open()
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
                    showWorkBillingList()
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
        model: workBillingRoot.workBillingList

        columns: [
            {
                "label": "Id",
                "width": 300,
                "key": "id"
            },
            {
                "label": "Work Order",
                "width": 500,
                "key": "workOrderName"
            },
            {
                "label": "Description",
                "width": 500,
                "key": "workBillingName"
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
    Component.onCompleted: showWorkBillingList()
    onVisibleChanged: showWorkBillingList()

    // -------- Functions --------
    function showWorkBillingList() {
        workBillingRoot.workBillingList = []

        if (!workBillingRoot.visible)
            return

        workBillingRoot.workBillingList = workBillingController.getWorkBillingList(isApproved)
    }

    function fillPopup() {
        if (!selectedData)
            return

        // Find and set work order
        for (var i = 0; i < workOrdersFromCtrl.length; i++) {
            if (workOrdersFromCtrl[i].id === selectedData.workOrderId) {
                workOrderComboEdit.currentIndex = i
                break
            }
        }
        descriptionTextBoxEdit.text = selectedData.workBillingName || ""

        // Load line items
        var tempLines = []
        var allLines = workBillingLineController.getWorkBillingLineList(isApproved)
       // console.log("allLines==",JSON.stringify(allLines))

        for (var j = 0; j < allLines.length; j++) {
            if (allLines[j].id === selectedData.id) {
                var lineItem = {
                    "work_order_line_id": String(allLines[j].workOrderLineId || ""),
                    "work_order_line_name": allLines[j].workOrderLineName || "",
                    "dollar_value": String(allLines[j].amount || "0"),
                    "tax_amount": String(allLines[j].taxAmount || "0"),
                    "tax_withholding_amount": String(allLines[j].taxWithHolding || "0"),
                    "retention_amount": String(allLines[j].retentionAmount || "0")
                }
                tempLines.push(lineItem)
            }
        }

        workBillingLineDataEdit = tempLines
    }
}
