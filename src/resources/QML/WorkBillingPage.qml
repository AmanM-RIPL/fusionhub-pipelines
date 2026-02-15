import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

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
    property bool canApproveReject: false

    // Properties - Validation errors
    property string workOrderError: ""
    property string descriptionError: ""
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
        id: newWorkBillingPopup
        popupWidth: 900
        popupHeight: 650
        title: "Create Work Billing"
        parent: Overlay.overlay
        buttonEnabled: workBillingRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                workBillingLineController.create(
                            workOrdersFromCtrl[workOrderCombo.currentIndex].id,
                            descriptionTextBox.text,
                            workBillingLineData
                            )
                resetForm(true)
                showWorkBillingList()
                close()
            }
        }

        onCancelCallback: function () {
            resetForm(true)
            close()
        }

        onClosed: {
            showWorkBillingList()
        }

        onOpened: {
            resetForm(true)
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

            // -------- Work Order Selection --------
            Text {
                text: "Select Work Order <span style='color: " + workBillingRoot.mandatoryColor + ";'>*</span>"
                color: workBillingRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomComboBox {
                id: workOrderCombo
                width: parent.width
                model: workOrderList
                currentIndex: -1
                onCurrentIndexChanged: {
                    if (currentIndex !== -1) {
                        workBillingRoot.workOrderError = ""
                    } else {
                        workBillingRoot.workOrderError = "Please select a work order"
                    }
                    checkCreateFormValidity()
                }
            }

            Text {
                text: workBillingRoot.workOrderError
                color: workBillingRoot.mandatoryColor
                font.pixelSize: 12
                visible: workBillingRoot.workOrderError !== ""
            }

            // -------- Description --------
            Text {
                text: "Description <span style='color: " + workBillingRoot.mandatoryColor + ";'>*</span>"
                color: workBillingRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: descriptionTextBox
                placeholderText: "Enter work billing description"
                text: ""
                color: workBillingRoot.labelColor
                width: parent.width
                height: 40
                topPadding: 4
                onTextChanged: {
                    checkCreateFormValidity()
                }
            }

            Text {
                text: workBillingRoot.descriptionError
                color: workBillingRoot.mandatoryColor
                font.pixelSize: 12
                visible: workBillingRoot.descriptionError !== ""
            }

            // -------- Work Billing Line Items --------
            Text {
                id: workBillingLineLabel
                text: "Work Billing Items <span style='color: " + workBillingRoot.mandatoryColor + ";'>*</span>"
                color: workBillingRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            Rectangle {
                width: parent.width
                height: 350
                color: "#EDF1F4"
                border.color: "#D0D0D0"
                border.width: 1

                Column {
                    width: parent.width
                    height: parent.height

                    FHTable{
                        id: popupTable
                        width: parent.width
                        height: 250
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
                                checkCreateFormValidity()
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 70
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

                                CustomComboBox {
                                    id: workOrderLineCombo
                                    width: 200
                                    height: 24
                                    model: workBillingRoot.workOrderLineList
                                    currentIndex: 0
                                }

                                CustomTextBox {
                                    id: dollarValueTextBox
                                    placeholderText: "Amount"
                                    text: ""
                                    color: workBillingRoot.labelColor
                                    width: 140
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: taxAmountTextBox
                                    placeholderText: "Tax Amount"
                                    text: ""
                                    color: workBillingRoot.labelColor
                                    width: 140
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: taxWithholdingTextBox
                                    placeholderText: "Withholding"
                                    text: ""
                                    color: workBillingRoot.labelColor
                                    width: 140
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: retentionAmountTextBox
                                    placeholderText: "Retention"
                                    text: ""
                                    color: workBillingRoot.labelColor
                                    width: 130
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
                                            if (workOrderLineCombo.currentIndex < 0 ||
                                                dollarValueTextBox.text === "") {
                                                workBillingRoot.lineItemsError =
                                                    "Please fill in all required fields: Work Order Line and Amount"
                                                return
                                            }

                                            workBillingRoot.lineItemsError = ""

                                            var newElements = {
                                                "work_order_line_id": String(workOrderLinesFromCtrl[workOrderLineCombo.currentIndex].id),
                                                "work_order_line_name": workOrderLineCombo.currentValue,
                                                "dollar_value": dollarValueTextBox.text,
                                                "tax_amount": taxAmountTextBox.text,
                                                "tax_withholding_amount": taxWithholdingTextBox.text,
                                                "retention_amount": retentionAmountTextBox.text
                                            }

                                            workBillingLineData = workBillingLineData.concat(newElements)

                                            // Clear the inputs
                                            dollarValueTextBox.text = ""
                                            taxAmountTextBox.text = ""
                                            taxWithholdingTextBox.text = ""
                                            retentionAmountTextBox.text = ""

                                            checkCreateFormValidity()
                                        }
                                    }
                                }
                            }

                            Text {
                                text: workBillingRoot.lineItemsError
                                color: workBillingRoot.mandatoryColor
                                font.pixelSize: 11
                                visible: workBillingRoot.lineItemsError !== ""
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
        popupWidth: 900
        popupHeight: 700
        title: popupMode === "view" ? "View Work Billing" : "Edit Work Billing"
        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? workBillingRoot.isEditFormValid : true

        property bool canApproveReject: selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData && validateForm(false)) {
                workBillingLineController.update(
                            selectedData.id,
                            workOrdersFromCtrl[workOrderComboEdit.currentIndex].id,
                            descriptionTextBoxEdit.text,
                            workBillingLineDataEdit
                            )
                resetForm(false)
                showWorkBillingList()
                close()
            } else if (popupMode === "view") {
                if (selectedData) {
                    draftEntityController.approve(selectedData.id)
                    resetForm(false)
                    showWorkBillingList()
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
                workBillingRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)
                fillPopup()
            }
        }

        onClosed: {
            workBillingRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Work Order Selection --------
            Text {
                text: "Select Work Order"
                color: workBillingRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: workOrderComboEdit
                width: parent.width
                model: workOrderList
                currentIndex: 0
                enabled: popupMode === "edit"
                onCurrentIndexChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: workBillingRoot.workOrderError
                color: workBillingRoot.mandatoryColor
                font.pixelSize: 12
                visible: workBillingRoot.workOrderError !== "" && popupMode === "edit"
            }

            // -------- Description --------
            Text {
                text: "Description"
                color: workBillingRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: descriptionTextBoxEdit
                placeholderText: "Enter work billing description"
                text: ""
                color: workBillingRoot.labelColor
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

            Text {
                text: workBillingRoot.descriptionError
                color: workBillingRoot.mandatoryColor
                font.pixelSize: 12
                visible: workBillingRoot.descriptionError !== "" && popupMode === "edit"
            }

            // -------- Work Billing Line Items --------
            Text {
                id: workBillingLineLabelEdit
                text: "Work Billing Items"
                color: workBillingRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            Rectangle {
                width: parent.width
                height: 280
                color: "#EDF1F4"
                border.color: "#D0D0D0"
                border.width: 1

                Column {
                    width: parent.width
                    height: parent.height

                    FHTable {
                        id: popupTableEdit
                        width: parent.width
                        height: 180
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

                                CustomComboBox {
                                    id: workOrderLineComboEdit
                                    width: 200
                                    height: 24
                                    model: workBillingRoot.workOrderLineList
                                    currentIndex: 0
                                }

                                CustomTextBox {
                                    id: dollarValueTextBoxEdit
                                    placeholderText: "Amount"
                                    text: ""
                                    color: workBillingRoot.labelColor
                                    width: 140
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: taxAmountTextBoxEdit
                                    placeholderText: "Tax Amount"
                                    text: ""
                                    color: workBillingRoot.labelColor
                                    width: 140
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: taxWithholdingTextBoxEdit
                                    placeholderText: "Withholding"
                                    text: ""
                                    color: workBillingRoot.labelColor
                                    width: 140
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: retentionAmountTextBoxEdit
                                    placeholderText: "Retention"
                                    text: ""
                                    color: workBillingRoot.labelColor
                                    width: 130
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
                                            if (workOrderLineComboEdit.currentIndex < 0 ||
                                                dollarValueTextBoxEdit.text === "") {
                                                workBillingRoot.lineItemsError =
                                                    "Please fill in all required fields: Work Order Line and Amount"
                                                return
                                            }

                                            workBillingRoot.lineItemsError = ""

                                            var newElements = {
                                                "work_order_line_id": String(workOrderLinesFromCtrl[workOrderLineComboEdit.currentIndex].id),
                                                "work_order_line_name": workOrderLineComboEdit.currentValue,
                                                "dollar_value": dollarValueTextBoxEdit.text,
                                                "tax_amount": taxAmountTextBoxEdit.text,
                                                "tax_withholding_amount": taxWithholdingTextBoxEdit.text,
                                                "retention_amount": retentionAmountTextBoxEdit.text
                                            }

                                            workBillingLineDataEdit = workBillingLineDataEdit.concat(newElements)

                                            dollarValueTextBoxEdit.text = ""
                                            taxAmountTextBoxEdit.text = ""
                                            taxWithholdingTextBoxEdit.text = ""
                                            retentionAmountTextBoxEdit.text = ""

                                            checkEditFormValidity()
                                        }
                                    }
                                }
                            }

                            Text {
                                text: workBillingRoot.lineItemsError
                                color: workBillingRoot.mandatoryColor
                                font.pixelSize: 11
                                visible: workBillingRoot.lineItemsError !== ""
                                leftPadding: 2
                            }
                        }
                    }
                }
            }

            // Cancellation Reason
            Text {
                text: "Cancellation Reason"
                color: workBillingRoot.labelColor
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
                color: workBillingRoot.labelColor
                width: parent.width
                height: 60
                visible: popupMode === "view" && canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        workBillingRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: workBillingRoot.cancellationReasonError
                color: workBillingRoot.mandatoryColor
                font.pixelSize: 12
                visible: workBillingRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
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
                            showWorkBillingList()
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
                        color: workBillingRoot.successColor
                    }
                }

                MouseArea {
                    width: 100
                    height: 34
                    onClicked: {
                        let reason = cancellationReasonTextBox.text.trim()
                        let reasonValidation = validator.validateNotEmpty(reason)
                        if (!reasonValidation.isValid) {
                            workBillingRoot.cancellationReasonError = reasonValidation.message
                            return
                        }

                        if (selectedData) {
                            console.log("Cancelling Work Billing ID: " + selectedData.id + " Reason: " + reason)
                            draftEntityController.cancel(selectedData.id, reason)
                            resetForm(false)
                            showWorkBillingList()
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
                        color: workBillingRoot.dangerColor
                    }
                }
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
                color: workBillingRoot.labelColor
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
                    color: workBillingRoot.labelColor
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
        height: 40
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
                "width": 400,
                "key": "workOrderName"
            },
            {
                "label": "Description",
                "width": 400,
                "key": "workBillingName"
            },
            {
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
    Component.onCompleted: showWorkBillingList()
    onVisibleChanged: showWorkBillingList()

    // ========== VALIDATION FUNCTIONS ==========

    // Check if create form is valid
    function checkCreateFormValidity() {
        let workOrderValidation = validator.validateComboBoxSelection(workOrderCombo.currentIndex, "work order")
        let descriptionValidation = validator.validateNotEmpty(descriptionTextBox.text)
        let lineItemsValidation = workBillingLineData.length > 0

        if (!workOrderValidation.isValid) {
            workBillingRoot.workOrderError = workOrderValidation.message
        } else {
            workBillingRoot.workOrderError = ""
        }

        if (!descriptionValidation.isValid) {
            workBillingRoot.descriptionError = descriptionValidation.message
        } else {
            workBillingRoot.descriptionError = ""
        }

        if (!lineItemsValidation) {
            workBillingRoot.lineItemsError = "At least one line item is required"
        } else {
            workBillingRoot.lineItemsError = ""
        }

        workBillingRoot.isCreateFormValid = workOrderValidation.isValid && descriptionValidation.isValid && lineItemsValidation
    }

    // Check if edit form is valid
    function checkEditFormValidity() {
        let workOrderValidation = validator.validateComboBoxSelection(workOrderComboEdit.currentIndex, "work order")
        let descriptionValidation = validator.validateNotEmpty(descriptionTextBoxEdit.text)
        let lineItemsValidation = workBillingLineDataEdit.length > 0

        if (!workOrderValidation.isValid) {
            workBillingRoot.workOrderError = workOrderValidation.message
        } else {
            workBillingRoot.workOrderError = ""
        }

        if (!descriptionValidation.isValid) {
            workBillingRoot.descriptionError = descriptionValidation.message
        } else {
            workBillingRoot.descriptionError = ""
        }

        if (!lineItemsValidation) {
            workBillingRoot.lineItemsError = "At least one line item is required"
        } else {
            workBillingRoot.lineItemsError = ""
        }

        workBillingRoot.isEditFormValid = workOrderValidation.isValid && descriptionValidation.isValid && lineItemsValidation
    }

    // Main validation function
    function validateForm(isCreate) {
        let workOrderCombo_ref = isCreate ? workOrderCombo : workOrderComboEdit
        let description_ref = isCreate ? descriptionTextBox : descriptionTextBoxEdit

        clearValidationErrors()

        let workOrderValidation = validator.validateComboBoxSelection(workOrderCombo_ref.currentIndex, "work order")
        if (!workOrderValidation.isValid) {
            workBillingRoot.workOrderError = workOrderValidation.message
            return false
        }

        let descriptionValidation = validator.validateNotEmpty(description_ref.text)
        if (!descriptionValidation.isValid) {
            workBillingRoot.descriptionError = "Description is required"
            return false
        }

        let lineItemsData = isCreate ? workBillingLineData : workBillingLineDataEdit
        if (lineItemsData.length === 0) {
            workBillingRoot.lineItemsError = "At least one line item is required"
            return false
        }

        return true
    }

    function clearValidationErrors() {
        workBillingRoot.workOrderError = ""
        workBillingRoot.descriptionError = ""
        workBillingRoot.lineItemsError = ""
        workBillingRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            workOrderCombo.currentIndex = -1
            descriptionTextBox.text = ""
            workBillingLineData = []
            workOrderLineCombo.currentIndex = 0
            dollarValueTextBox.text = ""
            taxAmountTextBox.text = ""
            taxWithholdingTextBox.text = ""
            retentionAmountTextBox.text = ""
            workBillingRoot.isCreateFormValid = false
        } else {
            workOrderComboEdit.currentIndex = 0
            descriptionTextBoxEdit.text = ""
            workBillingLineDataEdit = []
            workOrderLineComboEdit.currentIndex = 0
            dollarValueTextBoxEdit.text = ""
            taxAmountTextBoxEdit.text = ""
            taxWithholdingTextBoxEdit.text = ""
            retentionAmountTextBoxEdit.text = ""
            workBillingRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            workBillingRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
    }

    // ========== DATA LOADING FUNCTIONS ==========

    // Load and display work billings list
    function showWorkBillingList() {
        workBillingRoot.workBillingList = []

        if (!workBillingRoot.visible)
            return

        var wbs = workBillingController.getWorkBillingList(isApproved)

        for (var i = 0; i < wbs.length; i++) {
            if (wbs[i].nextApprovingUser === userController.getCurrentId()) {
                wbs[i].displayStatus = "Pending"
            } else {
                wbs[i].displayStatus = wbs[i].approvalStatus
            }
        }

        workBillingRoot.workBillingList = wbs
    }

    // Fill edit popup with selected data
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

        checkEditFormValidity()
    }
}
