import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: purchaseOrderRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var vendorsFromCtrl: []
    property var materialsFromCtrl: []
    property var unitsOfMeasurementFromCtrl: []

    // Properties - Lists for UI
    property var purchaseOrderList: []
    property var vendorList: []
    property var materialList: []
    property var unitOfMeasurementList: []

    // Properties - Purchase order data
    property var purchaseOrderLineData: []
    property var purchaseOrderLineDataEdit: []

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

    MaterialController {
        id: materialController
    }

    UnitOfMeasurementController {
        id: unitOfMeasurementController
    }

    PurchaseOrderController {
        id: purchaseOrderController
    }

    PurchaseOrderLineController {
        id: purchaseOrderLineController
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
        id: newPurchaseOrderPopup
        popupWidth: 800
        popupHeight: 600
        title: "Create Purchase Order"
        parent: Overlay.overlay
        buttonEnabled: purchaseOrderRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                purchaseOrderLineController.create(
                            vendorsFromCtrl[vendorCombo.currentIndex].id,
                            purchaseOrderLineData
                            )
                resetForm(true)
                showPurchaseOrderList()
                close()
            }
        }

        onCancelCallback: function () {
            resetForm(true)
            close()
        }

        onClosed: {
            showPurchaseOrderList()
        }

        onOpened: {
            resetForm(true)
            if (purchaseOrderRoot.visible) {
                vendorsFromCtrl = []
                materialsFromCtrl = []
                unitsOfMeasurementFromCtrl = []

                vendorsFromCtrl = vendorController.getVendorList(true)
                materialsFromCtrl = materialController.getMaterialList(true)
                unitsOfMeasurementFromCtrl = unitOfMeasurementController.getUOMList(true)

                purchaseOrderRoot.vendorList = []
                purchaseOrderRoot.materialList = []
                purchaseOrderRoot.unitOfMeasurementList = []

                for (var i = 0; i < vendorsFromCtrl.length; i++) {
                    purchaseOrderRoot.vendorList = purchaseOrderRoot.vendorList.concat(
                                vendorsFromCtrl[i].vendorName)
                }
                for (var j = 0; j < materialsFromCtrl.length; j++) {
                    purchaseOrderRoot.materialList = purchaseOrderRoot.materialList.concat(
                                materialsFromCtrl[j].materialName)
                }
                for (var k = 0; k < unitsOfMeasurementFromCtrl.length; k++) {
                    purchaseOrderRoot.unitOfMeasurementList = purchaseOrderRoot.unitOfMeasurementList.concat(
                                unitsOfMeasurementFromCtrl[k].uomName)
                }
            }
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Vendor Selection --------
            Text {
                text: "Select Vendor <span style='color: " + purchaseOrderRoot.mandatoryColor + ";'>*</span>"
                color: purchaseOrderRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomComboBox {
                id: vendorCombo
                width: parent.width
                model: vendorList
                currentIndex: -1
                onCurrentIndexChanged: {
                    if (currentIndex !== -1) {
                        purchaseOrderRoot.vendorError = ""
                    } else {
                        purchaseOrderRoot.vendorError = "Please select a vendor"
                    }
                    checkCreateFormValidity()
                }
            }

            Text {
                text: purchaseOrderRoot.vendorError
                color: purchaseOrderRoot.mandatoryColor
                font.pixelSize: 12
                visible: purchaseOrderRoot.vendorError !== ""
            }

            // -------- Purchase Order Line Items --------
            Text {
                id: purchaseOrderLineLabel
                text: "Purchase Order Items <span style='color: " + purchaseOrderRoot.mandatoryColor + ";'>*</span>"
                color: purchaseOrderRoot.labelColor
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
                        model: purchaseOrderLineData
                        columns: [
                            {
                                "label": "Material",
                                "width": 150,
                                "key": "material_name"
                            },
                            {
                                "label": "Quantity",
                                "width": 100,
                                "key": "quantity"
                            },
                            {
                                "label": "Unit",
                                "width": 120,
                                "key": "unit_name"
                            },
                            {
                                "label": "Dollar Value",
                                "width": 120,
                                "key": "dollar_value"
                            },
                            {
                                "label": "Tax Amount",
                                "width": 120,
                                "key": "tax_amount"
                            },
                            {
                                "label": "Tax Withholding",
                                "width": 120,
                                "key": "tax_withholding"
                            }
                        ]
                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < purchaseOrderLineData.length) {
                                var temp = []
                                for (var i = 0; i < purchaseOrderLineData.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(purchaseOrderLineData[i])
                                    }
                                }
                                purchaseOrderLineData = temp
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
                                    id: materialCombo
                                    width: 140
                                    height: 24
                                    model: purchaseOrderRoot.materialList
                                    currentIndex: 0
                                }

                                CustomTextBox {
                                    id: quantityTextBox
                                    placeholderText: "Qty"
                                    text: ""
                                    color: purchaseOrderRoot.labelColor
                                    width: 100
                                    height: 24
                                    topPadding: 1
                                }

                                CustomComboBox {
                                    id: unitCombo
                                    width: 110
                                    height: 24
                                    model: purchaseOrderRoot.unitOfMeasurementList
                                    currentIndex: 0
                                }

                                CustomTextBox {
                                    id: dollarValueTextBox
                                    placeholderText: "Value"
                                    text: ""
                                    color: purchaseOrderRoot.labelColor
                                    width: 100
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: taxAmountTextBox
                                    placeholderText: "Tax"
                                    text: ""
                                    color: purchaseOrderRoot.labelColor
                                    width: 100
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: taxWithholdingTextBox
                                    placeholderText: "Withholding"
                                    text: ""
                                    color: purchaseOrderRoot.labelColor
                                    width: 100
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
                                            if (materialCombo.currentIndex < 0 ||
                                                quantityTextBox.text === "" ||
                                                dollarValueTextBox.text === "") {
                                                purchaseOrderRoot.lineItemsError =
                                                    "Please fill in all required fields: Material, Quantity, and Dollar Value"
                                                return
                                            }

                                            purchaseOrderRoot.lineItemsError = ""

                                            var newElements = {
                                                "material_id": String(materialsFromCtrl[materialCombo.currentIndex].id),
                                                "material_name": materialCombo.currentValue,
                                                "quantity": quantityTextBox.text,
                                                "unit_of_measurement_id": String(unitsOfMeasurementFromCtrl[unitCombo.currentIndex].id),
                                                "unit_name": unitCombo.currentValue,
                                                "dollar_value": dollarValueTextBox.text,
                                                "tax_amount": taxAmountTextBox.text,
                                                "tax_withholding": taxWithholdingTextBox.text
                                            }

                                            purchaseOrderLineData = purchaseOrderLineData.concat(newElements)

                                            // Clear the inputs
                                            quantityTextBox.text = ""
                                            dollarValueTextBox.text = ""
                                            taxAmountTextBox.text = ""
                                            taxWithholdingTextBox.text = ""

                                            checkCreateFormValidity()
                                        }
                                    }
                                }
                            }

                            Text {
                                text: purchaseOrderRoot.lineItemsError
                                color: purchaseOrderRoot.mandatoryColor
                                font.pixelSize: 11
                                visible: purchaseOrderRoot.lineItemsError !== ""
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
        popupWidth: 800
        popupHeight: 600
        title: popupMode === "view" ? "View Purchase Order" : "Edit Purchase Order"
        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? purchaseOrderRoot.isEditFormValid : true

        property bool canApproveReject: selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData && validateForm(false)) {
                purchaseOrderLineController.update(
                            selectedData.id,
                            vendorsFromCtrl[vendorComboEdit.currentIndex].id,
                            purchaseOrderLineDataEdit
                            )
                resetForm(false)
                showPurchaseOrderList()
                close()
            } else if (popupMode === "view") {
                if (selectedData) {
                    draftEntityController.approve(selectedData.id)
                    resetForm(false)
                    showPurchaseOrderList()
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
                purchaseOrderRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)
                fillPopup()
            }
        }

        onClosed: {
            purchaseOrderRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Vendor Selection --------
            Text {
                text: "Select Vendor"
                color: purchaseOrderRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: vendorComboEdit
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
                text: purchaseOrderRoot.vendorError
                color: purchaseOrderRoot.mandatoryColor
                font.pixelSize: 12
                visible: purchaseOrderRoot.vendorError !== "" && popupMode === "edit"
            }

            // -------- Purchase Order Line Items --------
            Text {
                id: purchaseOrderLineLabelEdit
                text: "Purchase Order Items"
                color: purchaseOrderRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
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

                    FHTable {
                        id: popupTableEdit
                        width: parent.width
                        height: 250
                        leftPadding: 2
                        removeRow: popupMode === "edit"
                        model: purchaseOrderLineDataEdit
                        columns: [
                            {
                                "label": "Material",
                                "width": 150,
                                "key": "material_name"
                            },
                            {
                                "label": "Quantity",
                                "width": 100,
                                "key": "quantity"
                            },
                            {
                                "label": "Unit",
                                "width": 120,
                                "key": "unit_name"
                            },
                            {
                                "label": "Amount",
                                "width": 120,
                                "key": "dollar_value"
                            },
                            {
                                "label": "Tax Amount",
                                "width": 120,
                                "key": "tax_amount"
                            },
                            {
                                "label": "Tax Withholding",
                                "width": 120,
                                "key": "tax_withholding"
                            }
                        ]
                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < purchaseOrderLineDataEdit.length) {
                                var temp = []
                                for (var i = 0; i < purchaseOrderLineDataEdit.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(purchaseOrderLineDataEdit[i])
                                    }
                                }
                                purchaseOrderLineDataEdit = temp
                                checkEditFormValidity()
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 80
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
                                    id: materialComboEdit
                                    width: 140
                                    height: 24
                                    model: purchaseOrderRoot.materialList
                                    currentIndex: 0
                                }

                                CustomTextBox {
                                    id: quantityTextBoxEdit
                                    placeholderText: "Qty"
                                    text: ""
                                    color: purchaseOrderRoot.labelColor
                                    width: 100
                                    height: 24
                                    topPadding: 1
                                }

                                CustomComboBox {
                                    id: unitComboEdit
                                    width: 110
                                    height: 24
                                    model: purchaseOrderRoot.unitOfMeasurementList
                                    currentIndex: 0
                                }

                                CustomTextBox {
                                    id: dollarValueTextBoxEdit
                                    placeholderText: "Value"
                                    text: ""
                                    color: purchaseOrderRoot.labelColor
                                    width: 100
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: taxAmountTextBoxEdit
                                    placeholderText: "Tax"
                                    text: ""
                                    color: purchaseOrderRoot.labelColor
                                    width: 100
                                    height: 24
                                    topPadding: 1
                                }

                                CustomTextBox {
                                    id: taxWithholdingTextBoxEdit
                                    placeholderText: "Withholding"
                                    text: ""
                                    color: purchaseOrderRoot.labelColor
                                    width: 100
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
                                            if (materialComboEdit.currentIndex < 0 ||
                                                quantityTextBoxEdit.text === "" ||
                                                dollarValueTextBoxEdit.text === "") {
                                                purchaseOrderRoot.lineItemsError =
                                                    "Please fill in all required fields: Material, Quantity, and Dollar Value"
                                                return
                                            }

                                            purchaseOrderRoot.lineItemsError = ""

                                            var newElements = {
                                                "material_id": String(materialsFromCtrl[materialComboEdit.currentIndex].id),
                                                "material_name": materialComboEdit.currentValue,
                                                "quantity": quantityTextBoxEdit.text,
                                                "unit_of_measurement_id": String(unitsOfMeasurementFromCtrl[unitComboEdit.currentIndex].id),
                                                "unit_name": unitComboEdit.currentValue,
                                                "dollar_value": dollarValueTextBoxEdit.text,
                                                "tax_amount": taxAmountTextBoxEdit.text,
                                                "tax_withholding": taxWithholdingTextBoxEdit.text
                                            }

                                            purchaseOrderLineDataEdit = purchaseOrderLineDataEdit.concat(newElements)

                                            quantityTextBoxEdit.text = ""
                                            dollarValueTextBoxEdit.text = ""
                                            taxAmountTextBoxEdit.text = ""
                                            taxWithholdingTextBoxEdit.text = ""

                                            checkEditFormValidity()
                                        }
                                    }
                                }
                            }

                            Text {
                                text: purchaseOrderRoot.lineItemsError
                                color: purchaseOrderRoot.mandatoryColor
                                font.pixelSize: 11
                                visible: purchaseOrderRoot.lineItemsError !== ""
                                leftPadding: 2
                            }
                        }
                    }
                }
            }

            // Cancellation Reason
            Text {
                text: "Cancellation Reason"
                color: purchaseOrderRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
                visible: popupMode === "view" && canApproveReject
            }

            CustomTextBox {
                id: cancellationReasonTextBox
                placeholderText: "Enter cancellation reason"
                text: ""
                color: purchaseOrderRoot.labelColor
                width: parent.width
                height: 60
                visible: popupMode === "view" && canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        purchaseOrderRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: purchaseOrderRoot.cancellationReasonError
                color: purchaseOrderRoot.mandatoryColor
                font.pixelSize: 12
                visible: purchaseOrderRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
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
                            showPurchaseOrderList()
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
                        color: purchaseOrderRoot.successColor
                    }
                }

                MouseArea {
                    width: 100
                    height: 34
                    onClicked: {
                        let reason = cancellationReasonTextBox.text.trim()
                        let reasonValidation = validator.validateNotEmpty(reason)
                        if (!reasonValidation.isValid) {
                            purchaseOrderRoot.cancellationReasonError = reasonValidation.message
                            return
                        }

                        if (selectedData) {
                            console.log("Cancelling PO ID: " + selectedData.id + " Reason: " + reason)
                            draftEntityController.cancel(selectedData.id, reason)
                            resetForm(false)
                            showPurchaseOrderList()
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
                        color: purchaseOrderRoot.dangerColor
                    }
                }
            }
        }
    }

    // -------- Header Row --------
    Row {
        spacing: 20
        Text {
            text: "Purchase Order"
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
                onClicked: newPurchaseOrderPopup.open()
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
                color: purchaseOrderRoot.labelColor
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
                    color: purchaseOrderRoot.labelColor
                    font.pixelSize: 14
                }

                onCurrentTextChanged: {
                    isApproved = (approvalTypeComboBox.currentText === "Approved")
                    showPurchaseOrderList()
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
        model: purchaseOrderRoot.purchaseOrderList

        columns: [
            {
                "label": "Id",
                "width": 500,
                "key": "id"
            },
            {
                "label": "Vendor",
                "width": 600,
                "key": "vendorName"
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
    Component.onCompleted: showPurchaseOrderList()
    onVisibleChanged: showPurchaseOrderList()

    // ========== VALIDATION FUNCTIONS ==========

    // Check if create form is valid
    function checkCreateFormValidity() {
        let vendorValidation = validator.validateComboBoxSelection(vendorCombo.currentIndex, "vendor")
        let lineItemsValidation = purchaseOrderLineData.length > 0

        if (!vendorValidation.isValid) {
            purchaseOrderRoot.vendorError = vendorValidation.message
        } else {
            purchaseOrderRoot.vendorError = ""
        }

        if (!lineItemsValidation) {
            purchaseOrderRoot.lineItemsError = "At least one line item is required"
        } else {
            purchaseOrderRoot.lineItemsError = ""
        }

        purchaseOrderRoot.isCreateFormValid = vendorValidation.isValid && lineItemsValidation
    }

    // Check if edit form is valid
    function checkEditFormValidity() {
        let vendorValidation = validator.validateComboBoxSelection(vendorComboEdit.currentIndex, "vendor")
        let lineItemsValidation = purchaseOrderLineDataEdit.length > 0

        if (!vendorValidation.isValid) {
            purchaseOrderRoot.vendorError = vendorValidation.message
        } else {
            purchaseOrderRoot.vendorError = ""
        }

        if (!lineItemsValidation) {
            purchaseOrderRoot.lineItemsError = "At least one line item is required"
        } else {
            purchaseOrderRoot.lineItemsError = ""
        }

        purchaseOrderRoot.isEditFormValid = vendorValidation.isValid && lineItemsValidation
    }

    // Main validation function
    function validateForm(isCreate) {
        let vendorCombo_ref = isCreate ? vendorCombo : vendorComboEdit

        clearValidationErrors()

        let vendorValidation = validator.validateComboBoxSelection(vendorCombo_ref.currentIndex, "vendor")
        if (!vendorValidation.isValid) {
            purchaseOrderRoot.vendorError = vendorValidation.message
            return false
        }

        let lineItemsData = isCreate ? purchaseOrderLineData : purchaseOrderLineDataEdit
        if (lineItemsData.length === 0) {
            purchaseOrderRoot.lineItemsError = "At least one line item is required"
            return false
        }

        return true
    }

    function clearValidationErrors() {
        purchaseOrderRoot.vendorError = ""
        purchaseOrderRoot.lineItemsError = ""
        purchaseOrderRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            vendorCombo.currentIndex = -1
            purchaseOrderLineData = []
            quantityTextBox.text = ""
            dollarValueTextBox.text = ""
            taxAmountTextBox.text = ""
            taxWithholdingTextBox.text = ""
            purchaseOrderRoot.isCreateFormValid = false
        } else {
            vendorComboEdit.currentIndex = 0
            purchaseOrderLineDataEdit = []
            quantityTextBoxEdit.text = ""
            dollarValueTextBoxEdit.text = ""
            taxAmountTextBoxEdit.text = ""
            taxWithholdingTextBoxEdit.text = ""
            purchaseOrderRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            purchaseOrderRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
    }

    // ========== DATA LOADING FUNCTIONS ==========

    // Load and display purchase orders list
    function showPurchaseOrderList() {
        purchaseOrderRoot.purchaseOrderList = []

        if (!purchaseOrderRoot.visible)
            return

        var pos = purchaseOrderController.getPurchaseOrderList(isApproved)

        for (var i = 0; i < pos.length; i++) {
            if (pos[i].nextApprovingUser === userController.getCurrentId()) {
                pos[i].displayStatus = "Pending"
            } else {
                pos[i].displayStatus = pos[i].approvalStatus
            }
        }

        purchaseOrderRoot.purchaseOrderList = pos
    }

    // Fill edit popup with selected data
    function fillPopup() {
        if (!selectedData)
            return

        // Find and set vendor
        for (var i = 0; i < vendorsFromCtrl.length; i++) {
            if (vendorsFromCtrl[i].id === selectedData.vendorId) {
                vendorComboEdit.currentIndex = i
                break
            }
        }

        // Load line items
        var tempLines = []
        var allLines = purchaseOrderLineController.getPurchaseOrderLineList(isApproved)

        for (var j = 0; j < allLines.length; j++) {
            if (allLines[j].id === selectedData.id) {
                var lineItem = {
                    "material_id": String(allLines[j].materialId || ""),
                    "material_name": allLines[j].materialName || "",
                    "quantity": String(allLines[j].quantity || "0"),
                    "unit_of_measurement_id": String(allLines[j].unitOfMeasurementId || ""),
                    "unit_name": allLines[j].unitOfMeasurementName || "",
                    "dollar_value": String(allLines[j].amount || "0"),
                    "tax_amount": String(allLines[j].taxAmount || "0"),
                    "tax_withholding": String(allLines[j].taxWithHolding || "0")
                }
                tempLines.push(lineItem)
            }
        }

        purchaseOrderLineDataEdit = tempLines

        checkEditFormValidity()
    }
}
