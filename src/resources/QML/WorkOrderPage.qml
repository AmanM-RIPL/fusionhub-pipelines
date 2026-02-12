import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

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
    property bool canApproveReject: false

    // Properties - Validation errors
    property string vendorError: ""
    property string lineItemsError: ""
    property string cancellationReasonError: ""

    // Properties - Form validity
    property bool isCreateFormValid: false
    property bool isEditFormValid: false

    // Color Variables
    property string mandatoryColor: "#D13438"  // Red
    property string labelColor: "#323130"      // Dark Gray
    property string successColor: "#28A745"    // Green
    property string dangerColor: "#DC3545"     // Red

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

    UserController {
        id: userController
    }

    DraftEntityController {
        id: draftEntityController
    }

    // Validation Helper
    ValidationHelper {
        id: validator
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newWorkOrderLinePopup
        popupWidth: 850
        popupHeight: 600
        title: "Create Work Order"
        parent: Overlay.overlay
        buttonEnabled: workOrderLineRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                workOrderLineController.create(
                            vendorsFromCtrl[vendor.currentIndex].id,
                            descriptionTextBox.text,
                            workOrderLineData
                            )
                resetForm(true)
                showWorkOrderList()
                close()
            }
        }

        onCancelCallback: function () {
            resetForm(true)
            close()
        }

        onClosed: {
            showWorkOrderList()
        }

        onOpened: {
            resetForm(true)
            if (workOrderLineRoot.visible) {
                vendorsFromCtrl = []
                workOrdersFromCtrl = []
                taskFromCtrl = []

                vendorsFromCtrl = vendorController.getVendorList(true)
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

            // -------- Vendor Selection --------
            Text {
                text: "Select Vendor <span style='color: " + workOrderLineRoot.mandatoryColor + ";'>*</span>"
                color: workOrderLineRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomComboBox {
                id: vendor
                width: parent.width
                model: vendorList
                currentIndex: -1
                onCurrentIndexChanged: {
                    if (currentIndex !== -1) {
                        workOrderLineRoot.vendorError = ""
                    } else {
                        workOrderLineRoot.vendorError = "Please select a vendor"
                    }
                    checkCreateFormValidity()
                }
            }

            Text {
                text: workOrderLineRoot.vendorError
                color: workOrderLineRoot.mandatoryColor
                font.pixelSize: 12
                visible: workOrderLineRoot.vendorError !== ""
            }

            // -------- Description --------
            Text {
                text: "Description <span style='color: " + workOrderLineRoot.mandatoryColor + ";'>*</span>"
                color: workOrderLineRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: descriptionTextBox
                placeholderText: "Enter work order description"
                text: ""
                color: workOrderLineRoot.labelColor
                width: parent.width
                height: 40
                topPadding: 4
                onTextChanged: {
                    checkCreateFormValidity()
                }
            }

            // -------- Work Order Line Items --------
            Text {
                id: workOrderLineLabel
                text: "Work Order Items <span style='color: " + workOrderLineRoot.mandatoryColor + ";'>*</span>"
                color: workOrderLineRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            Rectangle {
                width: parent.width
                height: 380
                color: "#EDF1F4"
                border.color: "#D0D0D0"
                border.width: 1

                Column {
                    width: parent.width
                    height: parent.height

                    FHTable {
                        id: popupTable
                        width: parent.width
                        height: 250
                        leftPadding: 2
                        removeRow: true
                        model: workOrderLineData
                        columns: [
                            {
                                "label": "Description",
                                "width": 150,
                                "key": "description"
                            },
                            {
                                "label": "QTY",
                                "width": 100,
                                "key": "amount"
                            },
                            {
                                "label": "Task",
                                "width": 120,
                                "key": "task_name"
                            },
                            {
                                "label": "Tax Amount",
                                "width": 120,
                                "key": "tax_amount"
                            },
                            {
                                "label": "Tax Withholding",
                                "width": 120,
                                "key": "tax_with_holding"
                            },
                            {
                                "label": "Retention Amount",
                                "width": 150,
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
                                checkCreateFormValidity()
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 100
                        color: "white"
                        border.color: "#D0D0D0"
                        border.width: 1

                        Column {
                            width: parent.width
                            height: parent.height
                            spacing: 4
                            topPadding: 4

                            // Input fields for new line item
                            Row {
                                width: parent.width - 4
                                height: 28
                                leftPadding: 2
                                spacing: 6

                                CustomTextBox {
                                    id: lineDescriptionTextBox
                                    placeholderText: "Description"
                                    text: ""
                                    color: workOrderLineRoot.labelColor
                                    width: 150
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: amountTextBox
                                    placeholderText: "QTY"
                                    text: ""
                                    color: workOrderLineRoot.labelColor
                                    width: 100
                                    height: 24
                                    topPadding: 1
                                }

                                CustomComboBox {
                                    id: taskId
                                    width: 120
                                    height: 24
                                    model: workOrderLineRoot.taskList
                                    currentIndex: 0
                                }

                                CustomTextBox {
                                    id: taxAmountTextBox
                                    placeholderText: "Tax Amount"
                                    text: ""
                                    color: workOrderLineRoot.labelColor
                                    width: 120
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: taxWithHoldingTextBox
                                    placeholderText: "Withholding"
                                    text: ""
                                    color: workOrderLineRoot.labelColor
                                    width: 120
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: retentionAmountTextBox
                                    placeholderText: "Retention"
                                    text: ""
                                    color: workOrderLineRoot.labelColor
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

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        hoverEnabled: true

                                        onEntered: parent.color = "#f2f2f2"
                                        onExited: parent.color = "transparent"

                                        onClicked: {
                                            // Validate line item
                                            if (lineDescriptionTextBox.text === "" ||
                                                amountTextBox.text === "" ||
                                                taskId.currentIndex < 0) {
                                                workOrderLineRoot.lineItemsError =
                                                    "Please fill in all required fields: Description, QTY, and Task"
                                                return
                                            }

                                            workOrderLineRoot.lineItemsError = ""

                                            var newElements = {
                                                "description": lineDescriptionTextBox.text,
                                                "task_id": String(taskFromCtrl[taskId.currentIndex].id),
                                                "task_name": taskId.currentValue,
                                                "amount": amountTextBox.text,
                                                "tax_amount": taxAmountTextBox.text,
                                                "tax_with_holding": taxWithHoldingTextBox.text,
                                                "retention_amount": retentionAmountTextBox.text
                                            }

                                            workOrderLineData = workOrderLineData.concat(newElements)

                                            // Clear the inputs
                                            lineDescriptionTextBox.text = ""
                                            amountTextBox.text = ""
                                            taxAmountTextBox.text = ""
                                            taxWithHoldingTextBox.text = ""
                                            retentionAmountTextBox.text = ""

                                            checkCreateFormValidity()
                                        }
                                    }
                                }
                            }

                            Text {
                                text: workOrderLineRoot.lineItemsError
                                color: workOrderLineRoot.mandatoryColor
                                font.pixelSize: 11
                                visible: workOrderLineRoot.lineItemsError !== ""
                                leftPadding: 2
                            }
                        }
                    }
                }
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 850
        popupHeight: 650
        title: popupMode === "view" ? "View Work Order" : "Edit Work Order"
        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? workOrderLineRoot.isEditFormValid : true

        property bool canApproveReject: selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData && validateForm(false)) {
                workOrderLineController.update(
                            selectedData.id,
                            vendorsFromCtrl[vendorEdit.currentIndex].id,
                            descriptionTextBoxEdit.text,
                            workOrderLineDataEdit
                            )
                resetForm(false)
                showWorkOrderList()
                close()
            } else if (popupMode === "view") {
                if (selectedData) {
                    draftEntityController.approve(selectedData.id)
                    resetForm(false)
                    showWorkOrderList()
                    close()
                }
            }
        }

        onCancelCallback: function () {
            resetForm(false)
            close()
        }

        onOpened: {
            resetForm(false)
            if (selectedData) {
                workOrderLineRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)
                fillPopup()
            }
        }

        onClosed: {
            workOrderLineRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Vendor Selection --------
            Text {
                text: "Select Vendor"
                color: workOrderLineRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: vendorEdit
                width: parent.width
                model: vendorList
                currentIndex: 0
                enabled: popupMode === "edit"
                onCurrentIndexChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: workOrderLineRoot.vendorError
                color: workOrderLineRoot.mandatoryColor
                font.pixelSize: 12
                visible: workOrderLineRoot.vendorError !== "" && popupMode === "edit"
            }

            // -------- Description --------
            Text {
                text: "Description"
                color: workOrderLineRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: descriptionTextBoxEdit
                placeholderText: "Enter work order description"
                text: ""
                color: workOrderLineRoot.labelColor
                width: parent.width
                height: 40
                enabled: popupMode === "edit"
                topPadding: 4
                onTextChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            // -------- Work Order Line Items --------
            Text {
                id: workOrderLineLabelEdit
                text: "Work Order Items"
                color: workOrderLineRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            Rectangle {
                width: parent.width
                height: 300
                color: "#EDF1F4"
                border.color: "#D0D0D0"
                border.width: 1

                Column {
                    width: parent.width
                    height: parent.height

                    FHTable {
                        id: popupTableEdit
                        width: parent.width
                        height: 200
                        leftPadding: 2
                        removeRow: popupMode === "edit"
                        model: workOrderLineDataEdit
                        columns: [
                            {
                                "label": "Description",
                                "width": 150,
                                "key": "description"
                            },
                            {
                                "label": "QTY",
                                "width": 100,
                                "key": "amount"
                            },
                            {
                                "label": "Task",
                                "width": 120,
                                "key": "task_name"
                            },
                            {
                                "label": "Tax Amount",
                                "width": 120,
                                "key": "tax_amount"
                            },
                            {
                                "label": "Tax Withholding",
                                "width": 120,
                                "key": "tax_with_holding"
                            },
                            {
                                "label": "Retention Amount",
                                "width": 150,
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
                                checkEditFormValidity()
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 70
                        color: "white"
                        border.color: "#D0D0D0"
                        border.width: 1
                        visible: popupMode === "edit"

                        Column {
                            width: parent.width
                            height: parent.height
                            spacing: 4
                            topPadding: 4

                            Row {
                                width: parent.width - 4
                                height: 28
                                leftPadding: 2
                                spacing: 6

                                CustomTextBox {
                                    id: lineDescriptionTextBoxEdit
                                    placeholderText: "Description"
                                    text: ""
                                    color: workOrderLineRoot.labelColor
                                    width: 150
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: amountTextBoxEdit
                                    placeholderText: "QTY"
                                    text: ""
                                    color: workOrderLineRoot.labelColor
                                    width: 100
                                    height: 24
                                    topPadding: 1
                                }

                                CustomComboBox {
                                    id: taskIdEdit
                                    width: 120
                                    height: 24
                                    model: workOrderLineRoot.taskList
                                    currentIndex: 0
                                }

                                CustomTextBox {
                                    id: taxAmountTextBoxEdit
                                    placeholderText: "Tax Amount"
                                    text: ""
                                    color: workOrderLineRoot.labelColor
                                    width: 120
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: taxWithHoldingTextBoxEdit
                                    placeholderText: "Withholding"
                                    text: ""
                                    color: workOrderLineRoot.labelColor
                                    width: 120
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: retentionAmountTextBoxEdit
                                    placeholderText: "Retention"
                                    text: ""
                                    color: workOrderLineRoot.labelColor
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

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        hoverEnabled: true

                                        onEntered: parent.color = "#f2f2f2"
                                        onExited: parent.color = "transparent"

                                        onClicked: {
                                            if (lineDescriptionTextBoxEdit.text === "" ||
                                                amountTextBoxEdit.text === "" ||
                                                taskIdEdit.currentIndex < 0) {
                                                workOrderLineRoot.lineItemsError =
                                                    "Please fill in all required fields: Description, QTY, and Task"
                                                return
                                            }

                                            workOrderLineRoot.lineItemsError = ""

                                            var newElements = {
                                                "description": lineDescriptionTextBoxEdit.text,
                                                "task_id": String(taskFromCtrl[taskIdEdit.currentIndex].id),
                                                "task_name": taskIdEdit.currentValue,
                                                "amount": amountTextBoxEdit.text,
                                                "tax_amount": taxAmountTextBoxEdit.text,
                                                "tax_with_holding": taxWithHoldingTextBoxEdit.text,
                                                "retention_amount": retentionAmountTextBoxEdit.text
                                            }

                                            workOrderLineDataEdit = workOrderLineDataEdit.concat(newElements)

                                            lineDescriptionTextBoxEdit.text = ""
                                            amountTextBoxEdit.text = ""
                                            taxAmountTextBoxEdit.text = ""
                                            taxWithHoldingTextBoxEdit.text = ""
                                            retentionAmountTextBoxEdit.text = ""

                                            checkEditFormValidity()
                                        }
                                    }
                                }
                            }

                            Text {
                                text: workOrderLineRoot.lineItemsError
                                color: workOrderLineRoot.mandatoryColor
                                font.pixelSize: 11
                                visible: workOrderLineRoot.lineItemsError !== ""
                                leftPadding: 2
                            }
                        }
                    }
                }
            }

            // Cancellation Reason
            Text {
                text: "Cancellation Reason"
                color: workOrderLineRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 25
                visible: popupMode === "view" && canApproveReject
            }

            CustomTextBox {
                id: cancellationReasonTextBox
                placeholderText: "Enter cancellation reason"
                text: ""
                color: workOrderLineRoot.labelColor
                width: parent.width
                height: 60
                visible: popupMode === "view" && canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        workOrderLineRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: workOrderLineRoot.cancellationReasonError
                color: workOrderLineRoot.mandatoryColor
                font.pixelSize: 12
                visible: workOrderLineRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
            }

            // Buttons Row - Approve and Reject
            Row {
                width: parent.width
                spacing: 10
                topPadding: 20
                bottomPadding: 20
                visible: popupMode === "view" && canApproveReject
                layoutDirection: Qt.RightToLeft

                MouseArea {
                    width: 100
                    height: 34
                    onClicked: {
                        if (selectedData) {
                            draftEntityController.approve(selectedData.id)
                            resetForm(false)
                            showWorkOrderList()
                            viewEditPopup.close()
                        }
                    }
                    CustomButton {
                        width: parent.width
                        height: parent.height
                        btnName: "Approve"
                        btnNameColor: "#FFFFFF"
                        btnNamePixelSize: 13
                        btnNameFontFamily: "Segoe UI"
                        color: workOrderLineRoot.successColor
                    }
                }

                MouseArea {
                    width: 100
                    height: 34
                    onClicked: {
                        let reason = cancellationReasonTextBox.text.trim()
                        let reasonValidation = validator.validateNotEmpty(reason)
                        if (!reasonValidation.isValid) {
                            workOrderLineRoot.cancellationReasonError = reasonValidation.message
                            return
                        }

                        if (selectedData) {
                            console.log("Cancelling Work Order ID: " + selectedData.id + " Reason: " + reason)
                            draftEntityController.cancel(selectedData.id, reason)
                            resetForm(false)
                            showWorkOrderList()
                            viewEditPopup.close()
                        }
                    }
                    CustomButton {
                        width: parent.width
                        height: parent.height
                        btnName: "Reject"
                        btnNameColor: "#FFFFFF"
                        btnNamePixelSize: 13
                        btnNameFontFamily: "Segoe UI"
                        color: workOrderLineRoot.dangerColor
                    }
                }
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
                color: workOrderLineRoot.labelColor
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
                    color: workOrderLineRoot.labelColor
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
        height: 40
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
                "width": 400,
                "key": "vendorName"
            },
            {
                "label": "Work Order",
                "width": 400,
                "key": "workOrderName"
            },            {
                "label": "Status",
                "width": 200,
                "key": "displayStatus"
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

    // ========== VALIDATION FUNCTIONS ==========

    // Check if create form is valid
    function checkCreateFormValidity() {
        let vendorValidation = validator.validateComboBoxSelection(vendor.currentIndex, "vendor")
        let descriptionValidation = validator.validateNotEmpty(descriptionTextBox.text)
        let lineItemsValidation = workOrderLineData.length > 0

        if (!vendorValidation.isValid) {
            workOrderLineRoot.vendorError = vendorValidation.message
        } else {
            workOrderLineRoot.vendorError = ""
        }

        if (!lineItemsValidation) {
            workOrderLineRoot.lineItemsError = "At least one line item is required"
        } else {
            workOrderLineRoot.lineItemsError = ""
        }

        workOrderLineRoot.isCreateFormValid = vendorValidation.isValid && descriptionValidation.isValid && lineItemsValidation
    }

    // Check if edit form is valid
    function checkEditFormValidity() {
        let vendorValidation = validator.validateComboBoxSelection(vendorEdit.currentIndex, "vendor")
        let descriptionValidation = validator.validateNotEmpty(descriptionTextBoxEdit.text)
        let lineItemsValidation = workOrderLineDataEdit.length > 0

        if (!vendorValidation.isValid) {
            workOrderLineRoot.vendorError = vendorValidation.message
        } else {
            workOrderLineRoot.vendorError = ""
        }

        if (!lineItemsValidation) {
            workOrderLineRoot.lineItemsError = "At least one line item is required"
        } else {
            workOrderLineRoot.lineItemsError = ""
        }

        workOrderLineRoot.isEditFormValid = vendorValidation.isValid && descriptionValidation.isValid && lineItemsValidation
    }

    // Main validation function
    function validateForm(isCreate) {
        let vendorCombo_ref = isCreate ? vendor : vendorEdit
        let description_ref = isCreate ? descriptionTextBox : descriptionTextBoxEdit

        clearValidationErrors()

        let vendorValidation = validator.validateComboBoxSelection(vendorCombo_ref.currentIndex, "vendor")
        if (!vendorValidation.isValid) {
            workOrderLineRoot.vendorError = vendorValidation.message
            return false
        }

        let descriptionValidation = validator.validateNotEmpty(description_ref.text)
        if (!descriptionValidation.isValid) {
            workOrderLineRoot.vendorError = "Description is required"
            return false
        }

        let lineItemsData = isCreate ? workOrderLineData : workOrderLineDataEdit
        if (lineItemsData.length === 0) {
            workOrderLineRoot.lineItemsError = "At least one line item is required"
            return false
        }

        return true
    }

    function clearValidationErrors() {
        workOrderLineRoot.vendorError = ""
        workOrderLineRoot.lineItemsError = ""
        workOrderLineRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            vendor.currentIndex = -1
            descriptionTextBox.text = ""
            workOrderLineData = []
            lineDescriptionTextBox.text = ""
            amountTextBox.text = ""
            taxAmountTextBox.text = ""
            taxWithHoldingTextBox.text = ""
            retentionAmountTextBox.text = ""
            workOrderLineRoot.isCreateFormValid = false
        } else {
            vendorEdit.currentIndex = 0
            descriptionTextBoxEdit.text = ""
            workOrderLineDataEdit = []
            lineDescriptionTextBoxEdit.text = ""
            amountTextBoxEdit.text = ""
            taxAmountTextBoxEdit.text = ""
            taxWithHoldingTextBoxEdit.text = ""
            retentionAmountTextBoxEdit.text = ""
            workOrderLineRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            workOrderLineRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
    }

    // ========== DATA LOADING FUNCTIONS ==========

    // Load and display work orders list
    function showWorkOrderList() {
        workOrderLineRoot.workOrderList = []

        if (!workOrderLineRoot.visible)
            return

        var wos = workOrderController.getWorkOrderList(isApproved)

        for (var i = 0; i < wos.length; i++) {
            if (wos[i].nextApprovingUser === userController.getCurrentId()) {
                wos[i].displayStatus = "Pending"
            } else {
                wos[i].displayStatus = wos[i].approvalStatus
            }
        }

        workOrderLineRoot.workOrderList = wos
    }

    // Fill edit popup with selected data
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

        checkEditFormValidity()
    }
}
