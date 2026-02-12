import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import QtQuick.Controls 2.15
import QtQuick.Dialogs
import "utils"
Column {
    id: scheduleOfRatesRoot
    width: parent.width
    padding: 10

    property var scheduleOfRatesList: []
    property bool isApproved: false

    property var scheduleSetupList: ["--", "scheduleSetupList"]
    property var resourceList: ["--", "resourceList"]
    property var costList: ["--", "costList"]
    property var dataList: []

    property var dataMap: {"key": "", "value": ""}

    // Properties - State management
    property var selectedData: null
    property string popupMode: "view"
    property bool canApproveReject: false

    // Properties - Validation errors
    property string scheduleNameError: ""
    property string scheduleSetupError: ""
    property string costValueError: ""
    property string cancellationReasonError: ""

    // Properties - Form validity
    property bool isCreateFormValid: false
    property bool isEditFormValid: false

    // Color Variables
    property string mandatoryColor: "#D13438"  // Red
    property string labelColor: "#323130"      // Dark Gray
    property string successColor: "#28A745"    // Green
    property string dangerColor: "#DC3545"     // Red

    ScheduleSetupController {
        id: scheduleSetupController
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
        id: costValueValidationTimer
        interval: 500
        repeat: false
        onTriggered: validateCostValue()
    }

    MessageDialog {
        id: confirmDialog
        title: "Confirm Action"
        text: "Do you want to save as a draft?"
        buttons: MessageDialog.Yes | MessageDialog.No
        onAccepted: {
            newScheduleOfRatesPopup.close()
        }
        onRejected: {
            scheduleNameTextBox.text = ""
            dataList = []
            dataMap = ({})
        }
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newScheduleOfRatesPopup
        popupWidth: 700
        popupHeight: 650
        title: "Create Schedule Of Rate"
        parent: Overlay.overlay
        buttonEnabled: scheduleOfRatesRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                if (dataList.length > 0) {
                    scheduleOfRatesController.create(scheduleNameTextBox.text, dataMap)
                    resetForm(true)
                    showList()
                    close()
                } else {
                    projectBudgetRoot.scheduleNameError = "Please add at least one cost entry"
                }
            }
        }

        onCancelCallback: function () {
            let size = (dataMap) ? Object.keys(dataMap).length : 0
            if (dataList.length > 0) {
                confirmDialog.open()
            } else {
                resetForm(true)
                close()
            }
        }

        onOpened: {
            resetForm(true)
            if (scheduleOfRatesRoot.visible) {
                scheduleSetupList = scheduleSetupController.getSetupList(true)
                if (scheduleSetupList.length > 0) {
                    var scheduleSetup = scheduleSetupList[0]
                    costList = scheduleSetup.vecCostParamDataDetails
                    resourceList = scheduleSetup.vecResourceParamDataDetails
                    dataMap.key = scheduleSetup.id
                    if (dataMap[dataMap.key]) {
                        dataList = dataMap[dataMap.key]
                    } else {
                        dataList = []
                    }
                }
            }
        }

        onClosed: {
            scheduleNameError = ""
            scheduleSetupError = ""
            costValueError = ""
        }

        Column {
            width: parent.width
            height: 300
            spacing: 5

            Text {
                id: scheduleNameLabel
                text: "Schedule Of Rate <span style='color: " + scheduleOfRatesRoot.mandatoryColor + ";'>*</span>"
                color: scheduleOfRatesRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: scheduleNameTextBox
                placeholderText: "SOR Name"
                text: ""
                color: scheduleOfRatesRoot.labelColor
                width: parent.width - 8
                onTextChanged: {
                    if (text.trim() !== "") {
                        scheduleOfRatesRoot.scheduleNameError = ""
                    }
                    checkCreateFormValidity()
                }
            }

            Text {
                text: scheduleOfRatesRoot.scheduleNameError
                color: scheduleOfRatesRoot.mandatoryColor
                font.pixelSize: 12
                visible: scheduleOfRatesRoot.scheduleNameError !== ""
            }

            Rectangle {
                width: parent.width - 6
                height: 20
                color: "white"
            }

            Row {
                width: parent.width - 8
                height: 30
                leftPadding: 2

                Text {
                    id: scheduleLabel
                    text: "Select Schedule <span style='color: " + scheduleOfRatesRoot.mandatoryColor + ";'>*</span>"
                    color: scheduleOfRatesRoot.labelColor
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                    textFormat: Text.RichText
                }

                CustomComboBox {
                    id: scheduleNameComboBox
                    width: 215
                    height: 22

                    model: scheduleSetupList
                    textRole: "scheduleSetupName"
                    currentIndex: 0

                    onCurrentIndexChanged: {
                        if (currentIndex >= 0) {
                            var scheduleSetup = scheduleSetupList[currentIndex]
                            dataMap.key = scheduleSetup.id

                            if (dataMap[dataMap.key]) {
                                dataList = dataMap[dataMap.key]
                            } else {
                                dataList = []
                            }

                            costList = scheduleSetup.vecCostParamDataDetails
                            resourceList = scheduleSetup.vecResourceParamDataDetails

                            if (currentIndex !== -1) {
                                scheduleOfRatesRoot.scheduleSetupError = ""
                            }
                            checkCreateFormValidity()
                        }
                    }
                }
            }

            Text {
                text: scheduleOfRatesRoot.scheduleSetupError
                color: scheduleOfRatesRoot.mandatoryColor
                font.pixelSize: 12
                visible: scheduleOfRatesRoot.scheduleSetupError !== ""
            }

            Rectangle {
                width: parent.width - 6
                height: parent.height - 40
                color: "#EDF1F4"
                Column {

                    FHTable {
                        width: 644
                        height: 130
                        leftPadding: 2
                        model: dataList
                        columns: [
                            {
                                "label": "Cost",
                                "width": 322,
                                "key": "cost"
                            },
                            {
                                "label": "Value",
                                "width": 322,
                                "key": "value"
                            }
                        ]

                        onRemoveRowChanged: {
                            dataList.splice(removedIndex, 1)
                            checkCreateFormValidity()
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width - 8
                height: 20
                color: "white"

                Row {
                    width: parent.width - 8
                    height: 20
                    leftPadding: 2

                    CustomComboBox {
                        id: costComboBox
                        width: 322
                        height: 22
                        model: costList
                        textRole: "cost_param_name"
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: costValueTextBox
                        placeholderText: "value"
                        text: ""
                        color: scheduleOfRatesRoot.labelColor
                        width: 305
                        height: 22
                        font.weight: 700
                        font.pixelSize: 10
                        font.family: "Segoe UI"
                        onTextChanged: {
                            costValueValidationTimer.stop()
                            costValueValidationTimer.start()
                            checkCreateFormValidity()
                        }
                    }

                    CustomButton {
                        color: "transparent"
                        width: 20
                        height: 22
                        border.color: "#8080808C"
                        btnSource: "qrc:/resources/images/add.svg"
                        btnName: ""
                        btnNameColor: "blue"
                        anchors.verticalCenter: parent.verticalCenter

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true

                            onEntered: {
                                parent.color = "#f2f2f2"
                            }

                            onExited: {
                                parent.color = "transparent"
                            }

                            onClicked: {
                                if (validateCostEntry()) {
                                    var newElements = {
                                        "scheduleSetup": scheduleNameComboBox.currentText,
                                        "cost": costComboBox.currentText,
                                        "value": costValueTextBox.text,
                                        "uom": String(costList[costComboBox.currentIndex].type_of_bim_dimension)
                                    }

                                    dataList = dataList.concat(newElements)
                                    dataMap[dataMap.key] = dataList

                                    costValueTextBox.text = ""
                                    costComboBox.currentIndex = 0
                                    checkCreateFormValidity()
                                }
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
        popupWidth: 700
        popupHeight: 650
        title: popupMode === "view" ? "View Schedule Of Rate" : "Edit Schedule Of Rate"
        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? scheduleOfRatesRoot.isEditFormValid : true

        property bool canApproveReject: selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData && validateForm(false)) {
                scheduleOfRatesController.update(selectedData.id, scheduleNameTextBoxEdit.text, dataMap)
                resetForm(false)
                showList()
                close()
            } else if (popupMode === "view") {
                if (selectedData) {
                    console.log("Approving Schedule Of Rates ID: " + selectedData.id)
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
                scheduleOfRatesRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)
                fillPopup()
            }
        }

        onClosed: {
            scheduleOfRatesRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            height: 300
            spacing: 5

            Text {
                id: scheduleNameLabelEdit
                text: "Schedule Of Rate"
                color: scheduleOfRatesRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: scheduleNameTextBoxEdit
                placeholderText: "SOR Name"
                text: ""
                color: scheduleOfRatesRoot.labelColor
                width: parent.width - 8
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: scheduleOfRatesRoot.scheduleNameError
                color: scheduleOfRatesRoot.mandatoryColor
                font.pixelSize: 12
                visible: scheduleOfRatesRoot.scheduleNameError !== "" && popupMode === "edit"
            }

            Rectangle {
                width: parent.width - 6
                height: 20
                color: "white"
            }

            Row {
                width: parent.width - 8
                height: 30
                leftPadding: 2

                Text {
                    id: scheduleLabelEdit
                    text: "Select Schedule"
                    color: scheduleOfRatesRoot.labelColor
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                }

                CustomComboBox {
                    id: scheduleNameComboBoxEdit
                    width: 215
                    height: 22
                    model: scheduleSetupList
                    textRole: "scheduleSetupName"
                    currentIndex: 0
                    enabled: popupMode === "edit"

                    onCurrentIndexChanged: {
                        if (popupMode === "edit" && currentIndex >= 0) {
                            var scheduleSetup = scheduleSetupList[currentIndex]
                            dataMap.key = scheduleSetup.id

                            if (dataMap[dataMap.key]) {
                                dataList = dataMap[dataMap.key]
                            } else {
                                dataList = []
                            }

                            costList = scheduleSetup.vecCostParamDataDetails
                            resourceList = scheduleSetup.vecResourceParamDataDetails
                            checkEditFormValidity()
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width - 6
                height: parent.height - 40
                color: "#EDF1F4"
                Column {

                    FHTable {
                        width: 644
                        height: 130
                        leftPadding: 2
                        model: dataList
                        columns: [
                            {
                                "label": "Cost",
                                "width": 322,
                                "key": "cost"
                            },
                            {
                                "label": "Value",
                                "width": 322,
                                "key": "value"
                            }
                        ]

                        onRemoveRowChanged: {
                            dataList.splice(removedIndex, 1)
                            if (popupMode === "edit") {
                                checkEditFormValidity()
                            }
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width - 8
                height: 20
                color: "white"
                visible: popupMode === "edit"

                Row {
                    width: parent.width - 8
                    height: 20
                    leftPadding: 2

                    CustomComboBox {
                        id: costComboBoxEdit
                        width: 322
                        height: 22
                        model: costList
                        textRole: "cost_param_name"
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: costValueTextBoxEdit
                        placeholderText: "value"
                        text: ""
                        color: scheduleOfRatesRoot.labelColor
                        width: 305
                        height: 22
                        font.weight: 700
                        font.pixelSize: 10
                        font.family: "Segoe UI"
                        onTextChanged: {
                            if (popupMode === "edit") {
                                costValueValidationTimer.stop()
                                costValueValidationTimer.start()
                                checkEditFormValidity()
                            }
                        }
                    }

                    CustomButton {
                        color: "transparent"
                        width: 20
                        height: 22
                        border.color: "#8080808C"
                        btnSource: "qrc:/resources/images/add.svg"
                        btnName: ""
                        btnNameColor: "blue"
                        anchors.verticalCenter: parent.verticalCenter

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true

                            onEntered: {
                                parent.color = "#f2f2f2"
                            }

                            onExited: {
                                parent.color = "transparent"
                            }

                            onClicked: {
                                if (validateCostEntryEdit()) {
                                    var newElements = {
                                        "scheduleSetup": scheduleNameComboBoxEdit.currentText,
                                        "cost": costComboBoxEdit.currentText,
                                        "value": costValueTextBoxEdit.text,
                                        "uom": String(costList[costComboBoxEdit.currentIndex].type_of_bim_dimension)
                                    }

                                    dataList = dataList.concat(newElements)
                                    dataMap[dataMap.key] = dataList

                                    costValueTextBoxEdit.text = ""
                                    costComboBoxEdit.currentIndex = 0
                                    checkEditFormValidity()
                                }
                            }
                        }
                    }
                }
            }

            // Cancellation Reason
            Text {
                text: "Cancellation Reason <span style='color: " + scheduleOfRatesRoot.mandatoryColor + ";'>*</span>"
                color: scheduleOfRatesRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
                visible: popupMode === "view" && canApproveReject
                textFormat: Text.RichText
            }

            CustomTextBox {
                id: cancellationReasonTextBox
                placeholderText: "Enter cancellation reason"
                text: ""
                color: scheduleOfRatesRoot.labelColor
                width: parent.width - 8
                height: 60
                visible: popupMode === "view" && canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        scheduleOfRatesRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: scheduleOfRatesRoot.cancellationReasonError
                color: scheduleOfRatesRoot.mandatoryColor
                font.pixelSize: 12
                visible: scheduleOfRatesRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
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
                            console.log("Approving Schedule Of Rates ID: " + selectedData.id)
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
                        color: scheduleOfRatesRoot.successColor
                    }
                }

                MouseArea {
                    width: 100
                    height: 34
                    onClicked: {
                        let reason = cancellationReasonTextBox.text.trim()
                        let reasonValidation = validator.validateNotEmpty(reason)
                        if (!reasonValidation.isValid) {
                            scheduleOfRatesRoot.cancellationReasonError = reasonValidation.message
                            return
                        }

                        if (selectedData) {
                            console.log("Cancelling Schedule Of Rates ID: " + selectedData.id + " Reason: " + reason)
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
                        color: scheduleOfRatesRoot.dangerColor
                    }
                }
            }
        }
    }

    // -------- Header Row --------
    Row {
        spacing: 20
        Text {
            text: "Schedule of Rate"
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
                onClicked: newScheduleOfRatesPopup.open()
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
                color: scheduleOfRatesRoot.labelColor
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
                    color: scheduleOfRatesRoot.labelColor
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
        model: scheduleOfRatesRoot.scheduleOfRatesList

        columns: [
            {
                "label": "ID",
                "width": 500,
                "key": "id"
            },
            {
                "label": "SOR Name",
                "width": 500,
                "key": "scheduleOfRatesName"
            },
            {
                "label": "Status",
                "width": 300,
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
    Component.onCompleted: showList()
    onVisibleChanged: showList()

    // ========== VALIDATION FUNCTIONS ==========

    // Check if create form is valid
    function checkCreateFormValidity() {
        let scheduleNameValidation = validator.validateNotEmpty(scheduleNameTextBox.text)
        let scheduleSetupValidation = validator.validateComboBoxSelection(scheduleNameComboBox.currentIndex, "schedule")
        let dataListValidation = dataList.length > 0

        if (!scheduleNameValidation.isValid) {
            scheduleOfRatesRoot.scheduleNameError = scheduleNameValidation.message
        } else {
            scheduleOfRatesRoot.scheduleNameError = ""
        }

        if (!scheduleSetupValidation.isValid) {
            scheduleOfRatesRoot.scheduleSetupError = scheduleSetupValidation.message
        } else {
            scheduleOfRatesRoot.scheduleSetupError = ""
        }

        scheduleOfRatesRoot.isCreateFormValid = scheduleNameValidation.isValid && scheduleSetupValidation.isValid && dataListValidation
    }

    // Check if edit form is valid
    function checkEditFormValidity() {
        let scheduleNameValidation = validator.validateNotEmpty(scheduleNameTextBoxEdit.text)
        let dataListValidation = dataList.length > 0

        if (!scheduleNameValidation.isValid) {
            scheduleOfRatesRoot.scheduleNameError = scheduleNameValidation.message
        } else {
            scheduleOfRatesRoot.scheduleNameError = ""
        }

        scheduleOfRatesRoot.isEditFormValid = scheduleNameValidation.isValid && dataListValidation
    }

    // Main validation function
    function validateForm(isCreate) {
        clearValidationErrors()

        let scheduleNameValidation = isCreate ?
            validator.validateNotEmpty(scheduleNameTextBox.text) :
            validator.validateNotEmpty(scheduleNameTextBoxEdit.text)

        if (!scheduleNameValidation.isValid) {
            scheduleOfRatesRoot.scheduleNameError = scheduleNameValidation.message
            return false
        }

        if (dataList.length === 0) {
            scheduleOfRatesRoot.scheduleNameError = "Please add at least one cost entry"
            return false
        }

        return true
    }

    // Validate cost entry in create form
    function validateCostEntry() {
        let costValidation = validator.validateComboBoxSelection(costComboBox.currentIndex, "cost")
        let valueValidation = validator.validatePositiveDouble(costValueTextBox.text)

        if (!costValidation.isValid) {
            scheduleOfRatesRoot.costValueError = costValidation.message
            return false
        }

        if (!valueValidation.isValid) {
            scheduleOfRatesRoot.costValueError = valueValidation.message
            return false
        }

        scheduleOfRatesRoot.costValueError = ""
        return true
    }

    // Validate cost entry in edit form
    function validateCostEntryEdit() {
        let costValidation = validator.validateComboBoxSelection(costComboBoxEdit.currentIndex, "cost")
        let valueValidation = validator.validatePositiveDouble(costValueTextBoxEdit.text)

        if (!costValidation.isValid) {
            scheduleOfRatesRoot.costValueError = costValidation.message
            return false
        }

        if (!valueValidation.isValid) {
            scheduleOfRatesRoot.costValueError = valueValidation.message
            return false
        }

        scheduleOfRatesRoot.costValueError = ""
        return true
    }

    function validateCostValue() {
        let text = newScheduleOfRatesPopup.visible ? costValueTextBox.text : costValueTextBoxEdit.text
        let validation = validator.validatePositiveDouble(text)

        if (!validation.isValid) {
            scheduleOfRatesRoot.costValueError = validation.message
        } else {
            scheduleOfRatesRoot.costValueError = ""
        }
    }

    function clearValidationErrors() {
        scheduleOfRatesRoot.scheduleNameError = ""
        scheduleOfRatesRoot.scheduleSetupError = ""
        scheduleOfRatesRoot.costValueError = ""
        scheduleOfRatesRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            scheduleNameTextBox.text = ""
            costValueTextBox.text = ""
            scheduleNameComboBox.currentIndex = 0
            costComboBox.currentIndex = 0
            dataList = []
            dataMap = {"key": "", "value": ""}
            scheduleOfRatesRoot.isCreateFormValid = false
        } else {
            scheduleNameTextBoxEdit.text = ""
            costValueTextBoxEdit.text = ""
            scheduleNameComboBoxEdit.currentIndex = 0
            costComboBoxEdit.currentIndex = 0
            dataList = []
            dataMap = {"key": "", "value": ""}
            scheduleOfRatesRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            scheduleOfRatesRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
        costValueValidationTimer.stop()
    }

    // ========== DATA LOADING FUNCTIONS ==========

    // Load and display schedule of rates list
    function showList() {
        scheduleOfRatesRoot.scheduleOfRatesList = []

        if (!scheduleOfRatesRoot.visible)
            return

        var scheduleOfRates = scheduleOfRatesController.getScheduleOfRatesList(isApproved)

        for (var i = 0; i < scheduleOfRates.length; i++) {
            // Add status display logic
            if (scheduleOfRates[i].nextApprovingUser === userController.getCurrentId()) {
                scheduleOfRates[i].displayStatus = "Pending"
            } else {
                scheduleOfRates[i].displayStatus = scheduleOfRates[i].approvalStatus
            }
        }

        scheduleOfRatesRoot.scheduleOfRatesList = scheduleOfRates
    }

    // Fill edit popup with selected data
    function fillPopup() {
        if (!selectedData)
            return

        scheduleNameTextBoxEdit.text = selectedData.scheduleOfRatesName || ""

        // Load schedule setups from controller if not already loaded
        if (scheduleSetupList.length === 0 || scheduleSetupList[0] === "--") {
            scheduleSetupList = scheduleSetupController.getSetupList(true)
        }

        // Load data map for the selected schedule of rates
        if (selectedData.dataMap) {
            dataMap = selectedData.dataMap
            var key = Object.keys(dataMap)[0]
            if (key) {
                dataList = dataMap[key] || []
            }
        }

        checkEditFormValidity()
    }
}
