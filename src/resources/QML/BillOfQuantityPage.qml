import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: billOfQuantityRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var billOfQuantityList: []
    property var billOfQuantityListForTable: []
    property var scheduleOfRatesList: []
    property var scheduleText: []

    // Properties - State management
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false
    property bool canApproveReject: false

    // Properties - Validation errors
    property string billNameError: ""
    property string scheduleError: ""
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
    BillOfQuantityController {
        id: billOfQuantityController
    }

    ScheduleOfRatesController {
        id: scheduleOfRatesController
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
        id: billNameValidationTimer
        interval: 500
        repeat: false
        onTriggered: validateBillName()
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newBillOfQuantityPopup
        popupWidth: 600
        popupHeight: 400
        title: "Create Bill of Quantity"
        parent: Overlay.overlay
        buttonEnabled: billOfQuantityRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                billOfQuantityController.create(
                            billOfQuantityNameTextBox.text,
                            billOfQuantityRoot.scheduleOfRatesList[scheduleOfRatesComboBox.currentIndex].id
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
            if (billOfQuantityRoot.visible) {
                // Load schedule of rates
                billOfQuantityRoot.scheduleOfRatesList = billOfQuantityController.getScheduleOfRatesList()

                var tempScheduleText = []
                for (var j = 0; j < billOfQuantityRoot.scheduleOfRatesList.length; j++) {
                    tempScheduleText.push(billOfQuantityRoot.scheduleOfRatesList[j].scheduleOfRatesName)
                }
                billOfQuantityRoot.scheduleText = tempScheduleText
                scheduleOfRatesComboBox.model = billOfQuantityRoot.scheduleText
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Bill of Quantity Name <span style='color: " + billOfQuantityRoot.mandatoryColor + ";'>*</span>"
                color: billOfQuantityRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: billOfQuantityNameTextBox
                placeholderText: "Enter bill of quantity name"
                text: ""
                color: billOfQuantityRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    billNameValidationTimer.stop()
                    billNameValidationTimer.start()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: billOfQuantityRoot.billNameError
                color: billOfQuantityRoot.mandatoryColor
                font.pixelSize: 12
                visible: billOfQuantityRoot.billNameError !== ""
            }

            Text {
                text: "Schedule Rate <span style='color: " + billOfQuantityRoot.mandatoryColor + ";'>*</span>"
                color: billOfQuantityRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomComboBox {
                id: scheduleOfRatesComboBox
                model: billOfQuantityRoot.scheduleText
                width: parent.width
                currentIndex: -1
                onCurrentIndexChanged: {
                    if (currentIndex !== -1) {
                        billOfQuantityRoot.scheduleError = ""
                    } else {
                        billOfQuantityRoot.scheduleError = "Please select a schedule rate"
                    }
                    checkCreateFormValidity()
                }
            }

            Text {
                text: billOfQuantityRoot.scheduleError
                color: billOfQuantityRoot.mandatoryColor
                font.pixelSize: 12
                visible: billOfQuantityRoot.scheduleError !== ""
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 600
        popupHeight: 550
        title: popupMode === "view" ? "View Bill of Quantity" : "Edit Bill of Quantity"
        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? billOfQuantityRoot.isEditFormValid : true

        property bool canApproveReject: selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData && validateForm(false)) {
                billOfQuantityController.update(
                            selectedData.id,
                            billOfQuantityNameTextBoxEdit.text,
                            billOfQuantityRoot.scheduleOfRatesList[scheduleOfRatesComboBoxEdit.currentIndex].id
                            )
                resetForm(false)
                showList()
                close()
            } else if (popupMode === "view") {
                if (selectedData) {
                    console.log("Approving Bill of Quantity ID: " + selectedData.id)
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
                billOfQuantityRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)
                fillPopup()
            }
        }

        onClosed: {
            billOfQuantityRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Bill of Quantity Name"
                color: billOfQuantityRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: billOfQuantityNameTextBoxEdit
                placeholderText: "Bill of quantity name"
                text: ""
                color: billOfQuantityRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        billNameValidationTimer.stop()
                        billNameValidationTimer.start()
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: billOfQuantityRoot.billNameError
                color: billOfQuantityRoot.mandatoryColor
                font.pixelSize: 12
                visible: billOfQuantityRoot.billNameError !== "" && popupMode === "edit"
            }

            Text {
                text: "Schedule Rate"
                color: billOfQuantityRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: scheduleOfRatesComboBoxEdit
                model: billOfQuantityRoot.scheduleText
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
                text: billOfQuantityRoot.scheduleError
                color: billOfQuantityRoot.mandatoryColor
                font.pixelSize: 12
                visible: billOfQuantityRoot.scheduleError !== "" && popupMode === "edit"
            }

            // Cancellation Reason
            Text {
                text: "Cancellation Reason"
                color: billOfQuantityRoot.labelColor
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
                color: billOfQuantityRoot.labelColor
                width: parent.width
                height: 60
                visible: popupMode === "view" && canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        billOfQuantityRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: billOfQuantityRoot.cancellationReasonError
                color: billOfQuantityRoot.mandatoryColor
                font.pixelSize: 12
                visible: billOfQuantityRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
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
                            console.log("Approving Bill of Quantity ID: " + selectedData.id)
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
                        color: billOfQuantityRoot.successColor
                    }
                }

                MouseArea {
                    width: 100
                    height: 34
                    onClicked: {
                        let reason = cancellationReasonTextBox.text.trim()
                        let reasonValidation = validator.validateNotEmpty(reason)
                        if (!reasonValidation.isValid) {
                            billOfQuantityRoot.cancellationReasonError = reasonValidation.message
                            return
                        }

                        if (selectedData) {
                            console.log("Cancelling Bill of Quantity ID: " + selectedData.id + " Reason: " + reason)
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
                        color: billOfQuantityRoot.dangerColor
                    }
                }
            }
        }
    }

    // -------- Header Row --------
    Row {
        spacing: 20
        Text {
            text: "Bill of Quantity"
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
                onClicked: newBillOfQuantityPopup.open()
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
                color: billOfQuantityRoot.labelColor
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
                    color: billOfQuantityRoot.labelColor
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
        model: billOfQuantityRoot.billOfQuantityListForTable

        columns: [
            { label: "Id", width: 300, key: "id" },
            { label: "Name", width: 400, key: "description" },
            { label: "Schedule Rate", width: 400, key: "schedule_of_rates_id" },
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
        let nameValidation = validator.validateNotEmpty(billOfQuantityNameTextBox.text)
        let scheduleValidation = validator.validateComboBoxSelection(scheduleOfRatesComboBox.currentIndex, "schedule rate")

        if (!nameValidation.isValid) {
            billOfQuantityRoot.billNameError = nameValidation.message
        } else {
            billOfQuantityRoot.billNameError = ""
        }

        if (!scheduleValidation.isValid) {
            billOfQuantityRoot.scheduleError = scheduleValidation.message
        } else {
            billOfQuantityRoot.scheduleError = ""
        }

        billOfQuantityRoot.isCreateFormValid = nameValidation.isValid && scheduleValidation.isValid
    }

    // Check if edit form is valid
    function checkEditFormValidity() {
        let nameValidation = validator.validateNotEmpty(billOfQuantityNameTextBoxEdit.text)
        let scheduleValidation = validator.validateComboBoxSelection(scheduleOfRatesComboBoxEdit.currentIndex, "schedule rate")

        if (!nameValidation.isValid) {
            billOfQuantityRoot.billNameError = nameValidation.message
        } else {
            billOfQuantityRoot.billNameError = ""
        }

        if (!scheduleValidation.isValid) {
            billOfQuantityRoot.scheduleError = scheduleValidation.message
        } else {
            billOfQuantityRoot.scheduleError = ""
        }

        billOfQuantityRoot.isEditFormValid = nameValidation.isValid && scheduleValidation.isValid
    }

    // Main validation function
    function validateForm(isCreate) {
        let nameField = isCreate ? billOfQuantityNameTextBox : billOfQuantityNameTextBoxEdit
        let scheduleCombo = isCreate ? scheduleOfRatesComboBox : scheduleOfRatesComboBoxEdit

        clearValidationErrors()

        let nameValidation = validator.validateNotEmpty(nameField.text)
        if (!nameValidation.isValid) {
            billOfQuantityRoot.billNameError = nameValidation.message
            return false
        }

        let scheduleValidation = validator.validateComboBoxSelection(scheduleCombo.currentIndex, "schedule rate")
        if (!scheduleValidation.isValid) {
            billOfQuantityRoot.scheduleError = scheduleValidation.message
            return false
        }

        return true
    }

    function validateBillName() {
        let text = newBillOfQuantityPopup.visible ? billOfQuantityNameTextBox.text : billOfQuantityNameTextBoxEdit.text
        let validation = validator.validateNotEmpty(text)

        if (!validation.isValid) {
            billOfQuantityRoot.billNameError = validation.message
        } else {
            billOfQuantityRoot.billNameError = ""
        }
    }

    function clearValidationErrors() {
        billOfQuantityRoot.billNameError = ""
        billOfQuantityRoot.scheduleError = ""
        billOfQuantityRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            billOfQuantityNameTextBox.text = ""
            scheduleOfRatesComboBox.currentIndex = -1
            billOfQuantityRoot.isCreateFormValid = false
        } else {
            billOfQuantityNameTextBoxEdit.text = ""
            scheduleOfRatesComboBoxEdit.currentIndex = -1
            billOfQuantityRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            billOfQuantityRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
        billNameValidationTimer.stop()
    }

    // ========== DATA LOADING FUNCTIONS ==========

    // Load and display bill of quantity list
    function showList() {
        billOfQuantityRoot.billOfQuantityList = []
        billOfQuantityRoot.billOfQuantityListForTable = []

        if (!billOfQuantityRoot.visible)
            return

        var billOfQuantities = billOfQuantityController.getBillOfQuantityList(isApproved)
        billOfQuantityRoot.billOfQuantityList = billOfQuantities

        // Fetch schedule of rates list
        billOfQuantityRoot.scheduleOfRatesList = billOfQuantityController.getScheduleOfRatesList()

        // Prepare text list for combo box
        const scheduleNames = billOfQuantityRoot.scheduleOfRatesList.map(
            schedule => schedule.scheduleOfRatesName
        )
        billOfQuantityRoot.scheduleText = scheduleNames

        // Prepare table data with status

        billOfQuantityRoot.billOfQuantityListForTable = billOfQuantityRoot.billOfQuantityList.map(
            bill => {
                const matchedSchedule = billOfQuantityRoot.scheduleOfRatesList.find(
                    s => Number(s.id) === Number(bill.scheduleOfRatesId)
                )

                let displayStatus = bill.approvalStatus
                    if (bill.nextApprovingUser === userController.getCurrentId()) {
                        displayStatus = "Pending"
                    }
                    else {
                        displayStatus = "Approved"
                    }
                return {
                    id: bill.id,
                    description: bill.description,
                    schedule_of_rates_id: matchedSchedule ? matchedSchedule.scheduleOfRatesName : "Unknown",
                    scheduleOfRatesId: bill.scheduleOfRatesId,
                    displayStatus: displayStatus,
                    nextApprovingUser: bill.nextApprovingUser,
                    createdByUser: bill.createdByUser
                }
            }
        )
    }

    // Fill edit popup with selected data
    function fillPopup() {
        if (!selectedData)
            return

        billOfQuantityNameTextBoxEdit.text = selectedData.description || ""

        // Load schedule of rates if not already loaded
        if (billOfQuantityRoot.scheduleOfRatesList.length === 0) {
            billOfQuantityRoot.scheduleOfRatesList = billOfQuantityController.getScheduleOfRatesList()

            var tempScheduleText = []
            for (var k = 0; k < billOfQuantityRoot.scheduleOfRatesList.length; k++) {
                tempScheduleText.push(billOfQuantityRoot.scheduleOfRatesList[k].scheduleOfRatesName)
            }
            billOfQuantityRoot.scheduleText = tempScheduleText
            scheduleOfRatesComboBoxEdit.model = billOfQuantityRoot.scheduleText
        }

        // Find and set schedule rate
        for (var i = 0; i < billOfQuantityRoot.scheduleOfRatesList.length; i++) {
            if (billOfQuantityRoot.scheduleOfRatesList[i].id === selectedData.scheduleOfRatesId) {
                scheduleOfRatesComboBoxEdit.currentIndex = i
                break
            }
        }

        checkEditFormValidity()
    }
}
