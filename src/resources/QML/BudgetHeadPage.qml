import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: budgetHeadRoot
    width: parent.width
    padding: 10

    property var budgetHeadList: []
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false
    property bool canApproveReject: false

    // Properties - Validation errors
    property string descriptionError: ""
    property string cancellationReasonError: ""

    // Properties - Form validity
    property bool isCreateFormValid: false
    property bool isEditFormValid: false

    // Color Variables
    property string mandatoryColor: "#D13438"  // Red
    property string labelColor: "#323130"      // Dark Gray

    // Validation Helper
    ValidationHelper {
        id: validator
    }

    BudgetHeadController {
        id: budgetheadController
    }

    UserController {
        id: userController
    }

    DraftEntityController {
        id: draftEntityController
    }

    Timer {
        id: descriptionValidationTimer
        interval: 500
        repeat: false
        onTriggered: validateDescription()
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newBudgetHeadPopup
        popupWidth: 500
        popupHeight: 280
        title: "New Budget Head"
        parent: Overlay.overlay
        buttonEnabled: budgetHeadRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                budgetheadController.create(descriptionTextBox.text)
                resetForm(true)
                showList()
                close()
            }
        }

        onCancelCallback: function () {
            resetForm(true)
            close()
        }

        onClosed: showList()

        onOpened: {
            resetForm(true)
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Description <span style='color: " + budgetHeadRoot.mandatoryColor + ";'>*</span>"
                color: budgetHeadRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: descriptionTextBox
                placeholderText: "Description"
                text: ""
                color: budgetHeadRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    descriptionValidationTimer.stop()
                    descriptionValidationTimer.start()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: budgetHeadRoot.descriptionError
                color: budgetHeadRoot.mandatoryColor
                font.pixelSize: 12
                visible: budgetHeadRoot.descriptionError !== ""
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 700
        popupHeight: 500
        title: popupMode === "view" ? "View Budget Head" : "Edit Budget Head"

        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? budgetHeadRoot.isEditFormValid : true

        // Property to check if user can approve/reject
        property bool canApproveReject: selectedData && (selectedData.nextApprovingUser === userController.getCurrentId())

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData && validateForm(false)) {
                budgetheadController.update(
                    selectedData.id,
                    descriptionTextBoxEdit.text
                )

                resetForm(false)
                showList()
                close()
            } else if (popupMode === "view") {
                if (selectedData) {
                    draftEntityController.approve(selectedData.id)
                    resetForm(false)
                    showList()
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
                budgetHeadRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === userController.getCurrentId())
                fillPopup()
            }
        }

        onClosed: {
            budgetHeadRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            spacing: 10
            bottomPadding: 20

            Text {
                text: "Description"
                color: budgetHeadRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: descriptionTextBoxEdit
                placeholderText: "Description"
                text: ""
                color: budgetHeadRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        descriptionValidationTimer.stop()
                        descriptionValidationTimer.start()
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: budgetHeadRoot.descriptionError
                color: budgetHeadRoot.mandatoryColor
                font.pixelSize: 12
                visible: budgetHeadRoot.descriptionError !== ""
            }

            // Cancellation Reason
            Text {
                text: "Cancellation Reason"
                color: budgetHeadRoot.labelColor
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
                color: budgetHeadRoot.labelColor
                width: parent.width
                height: 60
                visible: popupMode === "view" && canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        budgetHeadRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: budgetHeadRoot.cancellationReasonError
                color: budgetHeadRoot.mandatoryColor
                font.pixelSize: 12
                visible: budgetHeadRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
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
                        // Approve button
                        if (selectedData) {
                            draftEntityController.approve(selectedData.id)
                            resetForm(false)
                            showList()
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
                        // Reject button
                        let reason = cancellationReasonTextBox.text.trim()

                        if (reason === "") {
                            budgetHeadRoot.cancellationReasonError = "Please provide cancellation reason"
                            return
                        }

                        if (selectedData) {
                            console.log("Cancelling budget head ID: " + selectedData.id + " Reason: " + reason)
                            draftEntityController.cancel(selectedData.id, reason)
                            resetForm(false)
                            showList()
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
            text: "Budget Head"
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
                onClicked: newBudgetHeadPopup.open()
            }
        }
    }

    Rectangle {
        width: 100
        height: 40
        color: "#EDF1F4"
    }

    // -------- Approval Type Selection --------
    Row {
        spacing: 20
        anchors.left: parent.left
        Text {
            id: approvalTypeLabel
            text: "Choose Approval Type"
            color: budgetHeadRoot.labelColor
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
                color: budgetHeadRoot.labelColor
                font.pixelSize: 14
            }

            onCurrentTextChanged: {
                isApproved = (approvalTypeComboBox.currentText === "Approved")
                showList()
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
        id: ftabl
        height: 200
        leftPadding: 20
        model: budgetHeadRoot.budgetHeadList

        columns: [
            { label: "Id", width: 325, key: "id" },
            { label: "Description", width: 650, key: "description" },
            { label: "Status", width: 325, key: "displayStatus" }
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
    Component.onCompleted: showList()
    onVisibleChanged: showList()

    // ========== VALIDATION FUNCTIONS ==========

    // Check if create form is valid
    function checkCreateFormValidity() {
        let descriptionValid = validateField(descriptionTextBox.text)
        budgetHeadRoot.isCreateFormValid = descriptionValid
    }

    // Check if edit form is valid
    function checkEditFormValidity() {
        let descriptionValid = validateField(descriptionTextBoxEdit.text)
        budgetHeadRoot.isEditFormValid = descriptionValid
    }

    // Main validation function
    function validateForm(isCreate) {
        let descriptionInput = isCreate ? descriptionTextBox : descriptionTextBoxEdit

        clearValidationErrors()

        if (!validateField(descriptionInput.text)) {
            return false
        }

        return true
    }

    function validateField(text) {
        let result = validator.validateNotEmpty(text)

        if (result.isValid) {
            result = validator.validateAlphanumeric(text)
        }

        if (!result.isValid) {
            budgetHeadRoot.descriptionError = result.message
            return false
        } else {
            budgetHeadRoot.descriptionError = ""
            return true
        }
    }

    function validateDescription() {
        let text = newBudgetHeadPopup.visible ? descriptionTextBox.text : descriptionTextBoxEdit.text
        validateField(text)
    }

    function clearValidationErrors() {
        budgetHeadRoot.descriptionError = ""
        budgetHeadRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            descriptionTextBox.text = ""
            budgetHeadRoot.isCreateFormValid = false
        } else {
            descriptionTextBoxEdit.text = ""
            budgetHeadRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            budgetHeadRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
        descriptionValidationTimer.stop()
    }

    // Load and display budget head list
    function showList() {
        budgetHeadRoot.budgetHeadList = []
        var budgetHeads = budgetheadController.getBudgetHeadList(isApproved)
        for (var i = 0; i < budgetHeads.length; i++) {
            if (budgetHeads[i].nextApprovingUser === userController.getCurrentId()) {
                budgetHeads[i].displayStatus = "Pending"
            } else {
                budgetHeads[i].displayStatus = budgetHeads[i].approvalStatus
            }
        }

        budgetHeadRoot.budgetHeadList = budgetHeads
    }

    // Fill edit popup with selected data
    function fillPopup() {
        if (!selectedData)
            return

        descriptionTextBoxEdit.text = selectedData.description || ""
        checkEditFormValidity()
    }
}
