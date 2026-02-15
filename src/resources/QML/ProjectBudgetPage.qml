import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: projectBudgetRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var budgetHeadsFromCtrl: []

    // Properties - Lists for UI
    property var budgetHeadText: []
    property var budgetHeadList: []
    property var projectBudgetList: []

    // Properties - State management
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false
    property bool canApproveReject: false

    // Properties - Validation errors
    property string budgetHeadError: ""
    property string dollarAmountError: ""
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
    ProjectBudgetController {
        id: projectBudgetController
    }

    BudgetHeadController {
        id: budgetHeadController
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
        id: dollarAmountValidationTimer
        interval: 500
        repeat: false
        onTriggered: validateDollarAmount()
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newProjectBudgetPopup
        popupWidth: 600
        popupHeight: 350
        title: "Create Project Budget"
        parent: Overlay.overlay
        buttonEnabled: projectBudgetRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                projectBudgetController.create(
                            projectBudgetRoot.budgetHeadList[budgetHeadComboBox.currentIndex].id,
                            dollarAmountTextBox.text
                            )
                resetForm(true)
                showList()
                close()
            }
        }

        onCancelCallback: function () {
            resetForm(true)
            close()
        }

        onClosed: {
            showList()
        }

        onOpened: {
            resetForm(true)
            if (projectBudgetRoot.visible) {
                // Load budget heads from controller
                budgetHeadsFromCtrl = budgetHeadController.getBudgetHeadList(true)
                projectBudgetRoot.budgetHeadList = budgetHeadsFromCtrl

                var tempBudgetHeadText = []
                for (var j = 0; j < budgetHeadsFromCtrl.length; j++) {
                    tempBudgetHeadText.push(budgetHeadsFromCtrl[j].description)
                }
                projectBudgetRoot.budgetHeadText = tempBudgetHeadText
                budgetHeadComboBox.model = projectBudgetRoot.budgetHeadText
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Budget Head <span style='color: " + projectBudgetRoot.mandatoryColor + ";'>*</span>"
                color: projectBudgetRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomComboBox {
                id: budgetHeadComboBox
                model: projectBudgetRoot.budgetHeadText
                width: parent.width
                currentIndex: -1
                onCurrentIndexChanged: {
                    if (currentIndex !== -1) {
                        projectBudgetRoot.budgetHeadError = ""
                    } else {
                        projectBudgetRoot.budgetHeadError = "Please select a budget head"
                    }
                    checkCreateFormValidity()
                }
            }

            Text {
                text: projectBudgetRoot.budgetHeadError
                color: projectBudgetRoot.mandatoryColor
                font.pixelSize: 12
                visible: projectBudgetRoot.budgetHeadError !== ""
            }

            Text {
                text: "Dollar Amount <span style='color: " + projectBudgetRoot.mandatoryColor + ";'>*</span>"
                color: projectBudgetRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: dollarAmountTextBox
                placeholderText: "Enter dollar amount"
                text: ""
                color: projectBudgetRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    dollarAmountValidationTimer.stop()
                    dollarAmountValidationTimer.start()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: projectBudgetRoot.dollarAmountError
                color: projectBudgetRoot.mandatoryColor
                font.pixelSize: 12
                visible: projectBudgetRoot.dollarAmountError !== ""
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 600
        popupHeight: 600
        title: popupMode === "view" ? "View Project Budget" : "Edit Project Budget"
        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? projectBudgetRoot.isEditFormValid : true

        property bool canApproveReject: selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData && validateForm(false)) {
                projectBudgetController.update(
                            selectedData.id,
                            projectBudgetRoot.budgetHeadList[budgetHeadComboBoxEdit.currentIndex].id,
                            dollarAmountTextBoxEdit.text
                            )
                resetForm(false)
                showList()
                close()
            } else if (popupMode === "view") {
                if (selectedData) {
                    console.log("Approving Project Budget ID: " + selectedData.id)
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
                projectBudgetRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)
                fillPopup()
            }
        }

        onClosed: {
            projectBudgetRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Budget Head"
                color: projectBudgetRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: budgetHeadComboBoxEdit
                model: projectBudgetRoot.budgetHeadText
                width: parent.width
                currentIndex: -1
                enabled: popupMode === "edit"
                onCurrentIndexChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: projectBudgetRoot.budgetHeadError
                color: projectBudgetRoot.mandatoryColor
                font.pixelSize: 12
                visible: projectBudgetRoot.budgetHeadError !== "" && popupMode === "edit"
            }

            Text {
                text: "Dollar Amount"
                color: projectBudgetRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: dollarAmountTextBoxEdit
                placeholderText: "Dollar amount"
                text: ""
                color: projectBudgetRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        dollarAmountValidationTimer.stop()
                        dollarAmountValidationTimer.start()
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: projectBudgetRoot.dollarAmountError
                color: projectBudgetRoot.mandatoryColor
                font.pixelSize: 12
                visible: projectBudgetRoot.dollarAmountError !== "" && popupMode === "edit"
            }

            // Cancellation Reason
            Text {
                text: "Cancellation Reason"
                color: projectBudgetRoot.labelColor
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
                color: projectBudgetRoot.labelColor
                width: parent.width
                height: 60
                visible: popupMode === "view" && canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        projectBudgetRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: projectBudgetRoot.cancellationReasonError
                color: projectBudgetRoot.mandatoryColor
                font.pixelSize: 12
                visible: projectBudgetRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
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
                            console.log("Approving Project Budget ID: " + selectedData.id)
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
                        color: projectBudgetRoot.successColor
                    }
                }

                MouseArea {
                    width: 100
                    height: 34
                    onClicked: {
                        let reason = cancellationReasonTextBox.text.trim()
                        let reasonValidation = validator.validateNotEmpty(reason)
                        if (!reasonValidation.isValid) {
                            projectBudgetRoot.cancellationReasonError = reasonValidation.message
                            return
                        }

                        if (selectedData) {
                            console.log("Cancelling Project Budget ID: " + selectedData.id + " Reason: " + reason)
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
                        color: projectBudgetRoot.dangerColor
                    }
                }
            }
        }
    }

    // -------- Header Row --------
    Row {
        spacing: 20
        Text {
            text: "Project Budget"
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
                onClicked: newProjectBudgetPopup.open()
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
                color: projectBudgetRoot.labelColor
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
                    color: projectBudgetRoot.labelColor
                    font.pixelSize: 14
                }

                onCurrentTextChanged: {
                    isApproved = (approvalTypeComboBox.currentText === "Approved")
                    showList()
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
        model: projectBudgetRoot.projectBudgetList

        columns: [
            { label: "Id", width: 300, key: "id" },
            { label: "Budget Head", width: 400, key: "budgetHeadName" },
            { label: "Project Budget Amount", width: 400, key: "dollarValue" },
            { label: "Status", width: 200, key: "displayStatus" }
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
        let budgetHeadValidation = validator.validateComboBoxSelection(budgetHeadComboBox.currentIndex, "budget head")
        let amountValidation = validator.validatePositiveDouble(dollarAmountTextBox.text)

        if (!budgetHeadValidation.isValid) {
            projectBudgetRoot.budgetHeadError = budgetHeadValidation.message
        } else {
            projectBudgetRoot.budgetHeadError = ""
        }

        if (!amountValidation.isValid) {
            projectBudgetRoot.dollarAmountError = amountValidation.message
        } else {
            projectBudgetRoot.dollarAmountError = ""
        }

        projectBudgetRoot.isCreateFormValid = budgetHeadValidation.isValid && amountValidation.isValid
    }

    // Check if edit form is valid
    function checkEditFormValidity() {
        let budgetHeadValidation = validator.validateComboBoxSelection(budgetHeadComboBoxEdit.currentIndex, "budget head")
        let amountValidation = validator.validatePositiveDouble(dollarAmountTextBoxEdit.text)

        if (!budgetHeadValidation.isValid) {
            projectBudgetRoot.budgetHeadError = budgetHeadValidation.message
        } else {
            projectBudgetRoot.budgetHeadError = ""
        }

        if (!amountValidation.isValid) {
            projectBudgetRoot.dollarAmountError = amountValidation.message
        } else {
            projectBudgetRoot.dollarAmountError = ""
        }

        projectBudgetRoot.isEditFormValid = budgetHeadValidation.isValid && amountValidation.isValid
    }

    // Main validation function
    function validateForm(isCreate) {
        let budgetHeadCombo = isCreate ? budgetHeadComboBox : budgetHeadComboBoxEdit
        let amountField = isCreate ? dollarAmountTextBox : dollarAmountTextBoxEdit

        clearValidationErrors()

        let budgetHeadValidation = validator.validateComboBoxSelection(budgetHeadCombo.currentIndex, "budget head")
        if (!budgetHeadValidation.isValid) {
            projectBudgetRoot.budgetHeadError = budgetHeadValidation.message
            return false
        }

        let amountValidation = validator.validatePositiveDouble(amountField.text)
        if (!amountValidation.isValid) {
            projectBudgetRoot.dollarAmountError = amountValidation.message
            return false
        }

        return true
    }

    function validateDollarAmount() {
        let text = newProjectBudgetPopup.visible ? dollarAmountTextBox.text : dollarAmountTextBoxEdit.text
        let validation = validator.validatePositiveDouble(text)

        if (!validation.isValid) {
            projectBudgetRoot.dollarAmountError = validation.message
        } else {
            projectBudgetRoot.dollarAmountError = ""
        }
    }

    function clearValidationErrors() {
        projectBudgetRoot.budgetHeadError = ""
        projectBudgetRoot.dollarAmountError = ""
        projectBudgetRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            budgetHeadComboBox.currentIndex = -1
            dollarAmountTextBox.text = ""
            projectBudgetRoot.isCreateFormValid = false
        } else {
            budgetHeadComboBoxEdit.currentIndex = -1
            dollarAmountTextBoxEdit.text = ""
            projectBudgetRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            projectBudgetRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
        dollarAmountValidationTimer.stop()
    }

    // ========== DATA LOADING FUNCTIONS ==========

    // Load and display project budget list
    function showList() {
        projectBudgetRoot.projectBudgetList = []

        if (!projectBudgetRoot.visible)
            return

        var projectBudgets = projectBudgetController.getProjectBudgetList(isApproved)
        var budgetHeads = budgetHeadController.getBudgetHeadList(true)

        // Store budget heads for later use in fillPopup
        projectBudgetRoot.budgetHeadList = budgetHeads

        for (var i = 0; i < projectBudgets.length; i++) {
            var budgetHeadName = ""
            for (var j = 0; j < budgetHeads.length; j++) {
                if (budgetHeads[j].id === projectBudgets[i].budgetHeadId) {
                    budgetHeadName = budgetHeads[j].description
                    break
                }
            }
            projectBudgets[i].budgetHeadName = budgetHeadName

            // Add status display logic
            if (projectBudgets[i].nextApprovingUser === userController.getCurrentId()) {
                projectBudgets[i].displayStatus = "Pending"
            } else {
                projectBudgets[i].displayStatus = projectBudgets[i].approvalStatus
            }
        }

        projectBudgetRoot.projectBudgetList = projectBudgets
    }

    // Fill edit popup with selected data
    function fillPopup() {
        if (!selectedData)
            return

        dollarAmountTextBoxEdit.text = selectedData.dollarValue || ""

        // Load budget heads from controller if not already loaded
        if (projectBudgetRoot.budgetHeadList.length === 0) {
            budgetHeadsFromCtrl = budgetHeadController.getBudgetHeadList(true)
            projectBudgetRoot.budgetHeadList = budgetHeadsFromCtrl

            var tempBudgetHeadText = []
            for (var k = 0; k < budgetHeadsFromCtrl.length; k++) {
                tempBudgetHeadText.push(budgetHeadsFromCtrl[k].description)
            }
            projectBudgetRoot.budgetHeadText = tempBudgetHeadText
            budgetHeadComboBoxEdit.model = projectBudgetRoot.budgetHeadText
        }

        // Find and set budget head
        for (var i = 0; i < projectBudgetRoot.budgetHeadList.length; i++) {
            if (projectBudgetRoot.budgetHeadList[i].id === selectedData.budgetHeadId) {
                budgetHeadComboBoxEdit.currentIndex = i
                break
            }
        }

        checkEditFormValidity()
    }
}
