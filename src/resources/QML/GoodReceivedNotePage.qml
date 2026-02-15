import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: goodReceivedNoteRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var purchaseOrderLinesFromCtrl: []
    property var purchaseOrdersFromCtrl: []

    // Properties - Lists for UI
    property var goodReceivedNoteList: []
    property var purchaseOrderLineList: []

    // Properties - State management
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false
    property bool canApproveReject: false

    // Properties - Validation errors
    property string poLineError: ""
    property string amountReceivedError: ""
    property string cancellationReasonError: ""

    // Properties - Form validity
    property bool isCreateFormValid: false
    property bool isEditFormValid: false

    // Color Variables
    property string mandatoryColor: "#D13438"  // Red
    property string labelColor: "#323130"      // Dark Gray

    // Controllers
    GoodReceivedNoteController {
        id: goodReceivedNoteController
    }

    PurchaseOrderLineController {
        id: purchaseOrderLineController
    }

    PurchaseOrderController {
        id: purchaseOrderController
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

    Timer {
        id: amountValidationTimer
        interval: 500
        repeat: false
        onTriggered: validateAmount()
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newGoodReceivedNotePopup
        popupWidth: 600
        popupHeight: 400
        title: "Create Good Received Note"
        parent: Overlay.overlay
        buttonEnabled: goodReceivedNoteRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                goodReceivedNoteController.create(
                            purchaseOrderLinesFromCtrl[purchaseOrderLineCombo.currentIndex].id,
                            amountReceivedTextBox.text
                            )
                resetForm(true)
                showGoodReceivedNoteList()
                close()
            }
        }

        onCancelCallback: function () {
            resetForm(true)
            close()
        }

        onClosed: {
            showGoodReceivedNoteList()
        }

        onOpened: {
            resetForm(true)
            if (goodReceivedNoteRoot.visible) {
                purchaseOrderLinesFromCtrl = []
                purchaseOrdersFromCtrl = []

                purchaseOrderLinesFromCtrl = purchaseOrderLineController.getPurchaseOrderLineList(true)
                purchaseOrdersFromCtrl = purchaseOrderController.getPurchaseOrderList(true)

                goodReceivedNoteRoot.purchaseOrderLineList = []

                for (var i = 0; i < purchaseOrderLinesFromCtrl.length; i++) {
                    goodReceivedNoteRoot.purchaseOrderLineList = goodReceivedNoteRoot.purchaseOrderLineList.concat(
                                purchaseOrderLinesFromCtrl[i].id)
                }
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Purchase Order Line <span style='color: " + goodReceivedNoteRoot.mandatoryColor + ";'>*</span>"
                color: goodReceivedNoteRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomComboBox {
                id: purchaseOrderLineCombo
                width: parent.width
                model: purchaseOrderLineList
                currentIndex: -1
                onCurrentIndexChanged: {
                    if (currentIndex !== -1) {
                        goodReceivedNoteRoot.poLineError = ""
                    } else {
                        goodReceivedNoteRoot.poLineError = "Please select a purchase order line"
                    }
                    checkCreateFormValidity()
                }
            }

            Text {
                text: goodReceivedNoteRoot.poLineError
                color: goodReceivedNoteRoot.mandatoryColor
                font.pixelSize: 12
                visible: goodReceivedNoteRoot.poLineError !== ""
            }

            Text {
                text: "Amount of Material Received <span style='color: " + goodReceivedNoteRoot.mandatoryColor + ";'>*</span>"
                color: goodReceivedNoteRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: amountReceivedTextBox
                placeholderText: "Amount of Material Received"
                text: ""
                color: goodReceivedNoteRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    amountValidationTimer.stop()
                    amountValidationTimer.start()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: goodReceivedNoteRoot.amountReceivedError
                color: goodReceivedNoteRoot.mandatoryColor
                font.pixelSize: 12
                visible: goodReceivedNoteRoot.amountReceivedError !== ""
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 600
        popupHeight: 550
        title: popupMode === "view" ? "View Good Received Note" : "Edit Good Received Note"
        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? goodReceivedNoteRoot.isEditFormValid : true

        property bool canApproveReject: selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData && validateForm(false)) {
                goodReceivedNoteController.update(
                            selectedData.id,
                            purchaseOrderLinesFromCtrl[poLineComboEdit.currentIndex].id,
                            amountReceivedFieldEdit.text
                            )
                resetForm(false)
                showGoodReceivedNoteList()
                close()
            } else if (popupMode === "view") {
                if (selectedData) {
                    console.log("Approving GRN ID: " + selectedData.id)
                    draftEntityController.approve(selectedData.id)
                    resetForm(false)
                    showGoodReceivedNoteList()
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
                goodReceivedNoteRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)
                fillPopup()
            }
        }

        onClosed: {
            goodReceivedNoteRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Purchase Order Line"
                color: goodReceivedNoteRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: poLineComboEdit
                width: parent.width
                model: purchaseOrderLineList
                currentIndex: 0
                enabled: popupMode === "edit"
                onCurrentIndexChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: goodReceivedNoteRoot.poLineError
                color: goodReceivedNoteRoot.mandatoryColor
                font.pixelSize: 12
                visible: goodReceivedNoteRoot.poLineError !== "" && popupMode === "edit"
            }

            Text {
                text: "Amount of Material Received"
                color: goodReceivedNoteRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: amountReceivedFieldEdit
                placeholderText: "Amount of Material Received"
                text: ""
                color: goodReceivedNoteRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        amountValidationTimer.stop()
                        amountValidationTimer.start()
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: goodReceivedNoteRoot.amountReceivedError
                color: goodReceivedNoteRoot.mandatoryColor
                font.pixelSize: 12
                visible: goodReceivedNoteRoot.amountReceivedError !== "" && popupMode === "edit"
            }

            // Cancellation Reason
            Text {
                text: "Cancellation Reason"
                color: goodReceivedNoteRoot.labelColor
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
                color: goodReceivedNoteRoot.labelColor
                width: parent.width
                height: 60
                visible: popupMode === "view" && canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        goodReceivedNoteRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: goodReceivedNoteRoot.cancellationReasonError
                color: goodReceivedNoteRoot.mandatoryColor
                font.pixelSize: 12
                visible: goodReceivedNoteRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
            }

            // Buttons Row - Approve and Reject
            Row {
                width: parent.width
                spacing: 10
                topPadding: 20
                visible: popupMode === "view" && canApproveReject
                layoutDirection: Qt.RightToLeft

                MouseArea {
                    width: 100
                    height: 34
                    onClicked: {
                        if (selectedData) {
                            console.log("Approving GRN ID: " + selectedData.id)
                            draftEntityController.approve(selectedData.id)
                            resetForm(false)
                            showGoodReceivedNoteList()
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
                        color: "#28A745"
                    }
                }

                MouseArea {
                    width: 100
                    height: 34
                    onClicked: {
                        let reason = cancellationReasonTextBox.text.trim()
                        let reasonValidation = validator.validateNotEmpty(reason)
                        if (!reasonValidation.isValid) {
                            goodReceivedNoteRoot.cancellationReasonError = reasonValidation.message
                            return
                        }

                        if (selectedData) {
                            console.log("Cancelling GRN ID: " + selectedData.id + " Reason: " + reason)
                            draftEntityController.cancel(selectedData.id, reason)
                            resetForm(false)
                            showGoodReceivedNoteList()
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
                        color: "#DC3545"
                    }
                }
            }
        }
    }

    // -------- Header Row --------
    Row {
        spacing: 20
        Text {
            text: "Goods Received Note"
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
                onClicked: newGoodReceivedNotePopup.open()
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
                color: goodReceivedNoteRoot.labelColor
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
                    color: goodReceivedNoteRoot.labelColor
                    font.pixelSize: 14
                }

                onCurrentTextChanged: {
                    isApproved = (approvalTypeComboBox.currentText === "Approved")
                    showGoodReceivedNoteList()
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
        height: 300
        leftPadding: 20
        model: goodReceivedNoteRoot.goodReceivedNoteList

        columns: [
            {
                "label": "Id",
                "width": 300,
                "key": "id"
            },
            {
                "label": "PO Line ID",
                "width": 400,
                "key": "purchaseOrderLineId"
            },
            {
                "label": "Amount Received",
                "width": 400,
                "key": "quantity"
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
    Component.onCompleted: showGoodReceivedNoteList()
    onVisibleChanged: showGoodReceivedNoteList()

    // ========== VALIDATION FUNCTIONS ==========

    // Check if create form is valid
    function checkCreateFormValidity() {
        let poLineValidation = validator.validateComboBoxSelection(purchaseOrderLineCombo.currentIndex, "purchase order line")
        let amountValidation = validator.validatePositiveDouble(amountReceivedTextBox.text)

        if (!poLineValidation.isValid) {
            goodReceivedNoteRoot.poLineError = poLineValidation.message
        } else {
            goodReceivedNoteRoot.poLineError = ""
        }

        if (!amountValidation.isValid) {
            goodReceivedNoteRoot.amountReceivedError = amountValidation.message
        } else {
            goodReceivedNoteRoot.amountReceivedError = ""
        }

        goodReceivedNoteRoot.isCreateFormValid = poLineValidation.isValid && amountValidation.isValid
    }

    // Check if edit form is valid
    function checkEditFormValidity() {
        let poLineValidation = validator.validateComboBoxSelection(poLineComboEdit.currentIndex, "purchase order line")
        let amountValidation = validator.validatePositiveDouble(amountReceivedFieldEdit.text)

        if (!poLineValidation.isValid) {
            goodReceivedNoteRoot.poLineError = poLineValidation.message
        } else {
            goodReceivedNoteRoot.poLineError = ""
        }

        if (!amountValidation.isValid) {
            goodReceivedNoteRoot.amountReceivedError = amountValidation.message
        } else {
            goodReceivedNoteRoot.amountReceivedError = ""
        }

        goodReceivedNoteRoot.isEditFormValid = poLineValidation.isValid && amountValidation.isValid
    }

    // Main validation function
    function validateForm(isCreate) {
        let poLineCombo = isCreate ? purchaseOrderLineCombo : poLineComboEdit
        let amountField = isCreate ? amountReceivedTextBox : amountReceivedFieldEdit

        clearValidationErrors()

        let poLineValidation = validator.validateComboBoxSelection(poLineCombo.currentIndex, "purchase order line")
        if (!poLineValidation.isValid) {
            goodReceivedNoteRoot.poLineError = poLineValidation.message
            return false
        }

        let amountValidation = validator.validatePositiveDouble(amountField.text)
        if (!amountValidation.isValid) {
            goodReceivedNoteRoot.amountReceivedError = amountValidation.message
            return false
        }

        return true
    }

    function validateAmount() {
        let text = newGoodReceivedNotePopup.visible ? amountReceivedTextBox.text : amountReceivedFieldEdit.text
        let validation = validator.validatePositiveDouble(text)

        if (!validation.isValid) {
            goodReceivedNoteRoot.amountReceivedError = validation.message
        } else {
            goodReceivedNoteRoot.amountReceivedError = ""
        }
    }

    function clearValidationErrors() {
        goodReceivedNoteRoot.poLineError = ""
        goodReceivedNoteRoot.amountReceivedError = ""
        goodReceivedNoteRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            amountReceivedTextBox.text = ""
            purchaseOrderLineCombo.currentIndex = -1
            goodReceivedNoteRoot.isCreateFormValid = false
        } else {
            amountReceivedFieldEdit.text = ""
            poLineComboEdit.currentIndex = 0
            goodReceivedNoteRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            goodReceivedNoteRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
        amountValidationTimer.stop()
    }

    // ========== DATA LOADING FUNCTIONS ==========

    // Load and display GRN list
    function showGoodReceivedNoteList() {
        goodReceivedNoteRoot.goodReceivedNoteList = []

        if (!goodReceivedNoteRoot.visible)
            return

        var grns = goodReceivedNoteController.getGoodReceivedNoteList(isApproved)

        for (var i = 0; i < grns.length; i++) {
            if (grns[i].nextApprovingUser === userController.getCurrentId()) {
                grns[i].displayStatus = "Pending"
            } else {
                grns[i].displayStatus = grns[i].approvalStatus
            }
        }

        goodReceivedNoteRoot.goodReceivedNoteList = grns
    }

    // Fill edit popup with selected data
    function fillPopup() {
        if (!selectedData)
            return

        amountReceivedFieldEdit.text = selectedData.quantity !== null
                ? String(selectedData.quantity)
                : ""

        for (var i = 0; i < purchaseOrderLinesFromCtrl.length; i++) {
            if (purchaseOrderLinesFromCtrl[i].id === selectedData.purchaseOrderLineId) {
                poLineComboEdit.currentIndex = i
                break
            }
        }

        checkEditFormValidity()
    }
}
