import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: unitOfMeasurementRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var unitOfMeasurementsFromCtrl: []

    // Properties - Lists for UI
    property var unitOfMeasurementList: []
    property var unitTypeList: ["Weight", "Volume", "Count", "Length", "Area", "Time", "Temperature"]

    // Properties - State management
    property var selectedUom: null
    property string popupMode: "view"
    property bool isApproved: false
    property bool canApproveReject: false

    // Properties - Validation errors
    property string uomNameError: ""
    property string unitTypeError: ""
    property string conversionError: ""
    property string cancellationReasonError: ""

    // Properties - Form validity
    property bool isCreateFormValid: false
    property bool isEditFormValid: false

    // Color Variables
    property string mandatoryColor: "#D13438"  // Red
    property string labelColor: "#323130"      // Dark Gray

    // Controllers
    UnitOfMeasurementController {
        id: unitOfMeasurementController
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
        id: nameValidationTimer
        interval: 500
        repeat: false
        onTriggered: validateUOMName()
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newUnitOfMeasurementPopup
        popupWidth: 600
        popupHeight: 650
        title: "Create Unit Of Measurement"
        parent: Overlay.overlay
        buttonEnabled: unitOfMeasurementRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                unitOfMeasurementController.create(
                            uomNameTextBox.text,
                            unitOfMeasurementRoot.unitTypeList[unitTypeComboBox.currentIndex],
                            conToSqmTextBox.text,
                            conToCubicMeterTextBox.text,
                            conToMeterTextBox.text,
                            conToKgTextBox.text
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

        onClosed: showList()

        onOpened: {
            resetForm(true)
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "UOM Name <span style='color: " + unitOfMeasurementRoot.mandatoryColor + ";'>*</span>"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: uomNameTextBox
                placeholderText: "UOM Name"
                text: ""
                color: unitOfMeasurementRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    nameValidationTimer.stop()
                    nameValidationTimer.start()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: unitOfMeasurementRoot.uomNameError
                color: unitOfMeasurementRoot.mandatoryColor
                font.pixelSize: 12
                visible: unitOfMeasurementRoot.uomNameError !== ""
            }

            Text {
                text: "Unit Type <span style='color: " + unitOfMeasurementRoot.mandatoryColor + ";'>*</span>"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomComboBox {
                id: unitTypeComboBox
                width: parent.width
                model: unitOfMeasurementRoot.unitTypeList
                currentIndex: -1
                onCurrentIndexChanged: {
                    if (currentIndex !== -1) {
                        unitOfMeasurementRoot.unitTypeError = ""
                    } else {
                        unitOfMeasurementRoot.unitTypeError = "Please select unit type"
                    }
                    checkCreateFormValidity()
                }
            }

            Text {
                text: unitOfMeasurementRoot.unitTypeError
                color: unitOfMeasurementRoot.mandatoryColor
                font.pixelSize: 12
                visible: unitOfMeasurementRoot.unitTypeError !== ""
            }

            Text {
                text: "Conversion to sqm"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: conToSqmTextBox
                placeholderText: "Conversion to sqm"
                text: ""
                color: unitOfMeasurementRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: checkCreateFormValidity()
            }

            Text {
                text: "Conversion to Cubic Meter"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: conToCubicMeterTextBox
                placeholderText: "Conversion to Cubic Meter"
                text: ""
                color: unitOfMeasurementRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: checkCreateFormValidity()
            }

            Text {
                text: "Conversion to Meter"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: conToMeterTextBox
                placeholderText: "Conversion to Meter"
                text: ""
                color: unitOfMeasurementRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: checkCreateFormValidity()
            }

            Text {
                text: "Conversion to kg"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: conToKgTextBox
                placeholderText: "Conversion to kg"
                text: ""
                color: unitOfMeasurementRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: checkCreateFormValidity()
            }

            Text {
                text: unitOfMeasurementRoot.conversionError
                color: unitOfMeasurementRoot.mandatoryColor
                font.pixelSize: 12
                visible: unitOfMeasurementRoot.conversionError !== ""
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 600
        popupHeight: 700
        title: popupMode === "view" ? "View Unit Of Measurement" : "Edit Unit Of Measurement"
        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? unitOfMeasurementRoot.isEditFormValid : true

        // Property to check if user can approve/reject
        property bool canApproveReject: selectedUom && (selectedUom.nextApprovingUser === selectedUom.createdByUser)

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedUom && validateForm(false)) {
                unitOfMeasurementController.update(
                            selectedUom.id,
                            uomNameTextBoxEdit.text,
                            unitOfMeasurementRoot.unitTypeList[unitTypeComboBoxEdit.currentIndex],
                            conToSqmTextBoxEdit.text,
                            conToCubicMeterTextBoxEdit.text,
                            conToMeterTextBoxEdit.text,
                            conToKgTextBoxEdit.text
                            )
                resetForm(false)
                showList()
                close()
            } else if (popupMode === "view") {
                // Approve when in view mode
                if (selectedUom) {
                    console.log("Approving UOM ID: " + selectedUom.id)
                    draftEntityController.approve(selectedUom.id)
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
            if (selectedUom) {
                unitOfMeasurementRoot.canApproveReject = selectedUom && (selectedUom.nextApprovingUser === selectedUom.createdByUser)
                fillPopup()
            }
        }

        onClosed: {
            unitOfMeasurementRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "UOM Name"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: uomNameTextBoxEdit
                placeholderText: "UOM Name"
                text: ""
                color: unitOfMeasurementRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        nameValidationTimer.stop()
                        nameValidationTimer.start()
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: unitOfMeasurementRoot.uomNameError
                color: unitOfMeasurementRoot.mandatoryColor
                font.pixelSize: 12
                visible: unitOfMeasurementRoot.uomNameError !== ""
            }

            Text {
                text: "Unit Type"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: unitTypeComboBoxEdit
                width: parent.width
                model: unitOfMeasurementRoot.unitTypeList
                currentIndex: -1
                enabled: popupMode === "edit"
                onCurrentIndexChanged: {
                    unitOfMeasurementRoot.unitTypeError = (currentIndex !== -1) ? "" : "Please select unit type"
                    checkEditFormValidity()
                }
            }

            Text {
                text: unitOfMeasurementRoot.unitTypeError
                color: unitOfMeasurementRoot.mandatoryColor
                font.pixelSize: 12
                visible: unitOfMeasurementRoot.unitTypeError !== ""
            }

            Text {
                text: "Conversion to sqm"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: conToSqmTextBoxEdit
                placeholderText: "Conversion to sqm"
                text: ""
                color: unitOfMeasurementRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: "Conversion to Cubic Meter"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: conToCubicMeterTextBoxEdit
                placeholderText: "Conversion to Cubic Meter"
                text: ""
                color: unitOfMeasurementRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: "Conversion to Meter"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: conToMeterTextBoxEdit
                placeholderText: "Conversion to Meter"
                text: ""
                color: unitOfMeasurementRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: "Conversion to kg"
                color: unitOfMeasurementRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: conToKgTextBoxEdit
                placeholderText: "Conversion to kg"
                text: ""
                color: unitOfMeasurementRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: unitOfMeasurementRoot.conversionError
                color: unitOfMeasurementRoot.mandatoryColor
                font.pixelSize: 12
                visible: unitOfMeasurementRoot.conversionError !== "" && popupMode === "edit"
            }

            // Cancellation Reason
            Text {
                text: "Cancellation Reason"
                color: unitOfMeasurementRoot.labelColor
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
                color: unitOfMeasurementRoot.labelColor
                width: parent.width
                height: 60
                visible: popupMode === "view" && canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        unitOfMeasurementRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: unitOfMeasurementRoot.cancellationReasonError
                color: unitOfMeasurementRoot.mandatoryColor
                font.pixelSize: 12
                visible: unitOfMeasurementRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
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
                        if (selectedUom) {
                            console.log("Approving UOM ID: " + selectedUom.id)
                            draftEntityController.approve(selectedUom.id)
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

                        // Validate cancellation reason using validator
                        let reasonValidation = validator.validateNotEmpty(reason)
                        if (!reasonValidation.isValid) {
                            unitOfMeasurementRoot.cancellationReasonError = reasonValidation.message
                            return
                        }

                        if (selectedUom) {
                            console.log("Cancelling UOM ID: " + selectedUom.id + " Reason: " + reason)
                            draftEntityController.cancel(selectedUom.id, reason)
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
            text: "Unit Of Measurement"
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
                onClicked: newUnitOfMeasurementPopup.open()
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
            text: "Choose Approval Type"
            color: unitOfMeasurementRoot.labelColor
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
                color: unitOfMeasurementRoot.labelColor
                font.pixelSize: 14
            }
            onCurrentTextChanged: {
                isApproved = (currentText === "Approved")
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
        height: 300
        leftPadding: 20
        model: unitOfMeasurementRoot.unitOfMeasurementList

        columns: [
            {"label": "Id", "width": 150, "key": "id"},
            {"label": "Unit Name", "width": 180, "key": "uomName"},
            {"label": "Unit Type", "width": 200, "key": "unitType"},
            {"label": "Conversion to sqm", "width": 200, "key": "conversionToSqm"},
            {"label": "Conversion to cubic meter", "width": 200, "key": "conversionToCubicMeter"},
            {"label": "Conversion to meter", "width": 150, "key": "conversionToMeter"},
            {"label": "Conversion to kilogram", "width": 150, "key": "conversionToKilogram"},
            {"label": "Status", "width": 80, "key": "displayStatus"}
        ]

        onViewRequested: function(row) {
            popupMode = "view"
            selectedUom = row
            fillPopup()
            viewEditPopup.open()
        }

        onEditRequested: function(row) {
            popupMode = "edit"
            selectedUom = row
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
        let nameValidation = validator.validateNotEmpty(uomNameTextBox.text)
        let typeValidation = validator.validateComboBoxSelection(unitTypeComboBox.currentIndex, "unit type")

        // Validate conversion fields
        let sqmValidation = validator.validatePositiveDouble(conToSqmTextBox.text)
        let cubicValidation = validator.validatePositiveDouble(conToCubicMeterTextBox.text)
        let meterValidation = validator.validatePositiveDouble(conToMeterTextBox.text)
        let kgValidation = validator.validatePositiveDouble(conToKgTextBox.text)

        if (!nameValidation.isValid) {
            unitOfMeasurementRoot.uomNameError = nameValidation.message
        } else {
            unitOfMeasurementRoot.uomNameError = ""
        }

        if (!typeValidation.isValid) {
            unitOfMeasurementRoot.unitTypeError = typeValidation.message
        } else {
            unitOfMeasurementRoot.unitTypeError = ""
        }

        if (!sqmValidation.isValid || !cubicValidation.isValid || !meterValidation.isValid || !kgValidation.isValid) {
            unitOfMeasurementRoot.conversionError = "Conversion values must be valid positive numbers"
        } else {
            unitOfMeasurementRoot.conversionError = ""
        }

        unitOfMeasurementRoot.isCreateFormValid = nameValidation.isValid && typeValidation.isValid &&
                                                   sqmValidation.isValid && cubicValidation.isValid &&
                                                   meterValidation.isValid && kgValidation.isValid
    }

    // Check if edit form is valid
    function checkEditFormValidity() {
        let nameValidation = validator.validateNotEmpty(uomNameTextBoxEdit.text)
        let typeValidation = validator.validateComboBoxSelection(unitTypeComboBoxEdit.currentIndex, "unit type")

        // Validate conversion fields
        let sqmValidation = validator.validatePositiveDouble(conToSqmTextBoxEdit.text)
        let cubicValidation = validator.validatePositiveDouble(conToCubicMeterTextBoxEdit.text)
        let meterValidation = validator.validatePositiveDouble(conToMeterTextBoxEdit.text)
        let kgValidation = validator.validatePositiveDouble(conToKgTextBoxEdit.text)

        if (!nameValidation.isValid) {
            unitOfMeasurementRoot.uomNameError = nameValidation.message
        } else {
            unitOfMeasurementRoot.uomNameError = ""
        }

        if (!typeValidation.isValid) {
            unitOfMeasurementRoot.unitTypeError = typeValidation.message
        } else {
            unitOfMeasurementRoot.unitTypeError = ""
        }

        if (!sqmValidation.isValid || !cubicValidation.isValid || !meterValidation.isValid || !kgValidation.isValid) {
            unitOfMeasurementRoot.conversionError = "Conversion values must be valid positive numbers"
        } else {
            unitOfMeasurementRoot.conversionError = ""
        }

        unitOfMeasurementRoot.isEditFormValid = nameValidation.isValid && typeValidation.isValid &&
                                                 sqmValidation.isValid && cubicValidation.isValid &&
                                                 meterValidation.isValid && kgValidation.isValid
    }

    // Main validation function
    function validateForm(isCreate) {
        let nameInput = isCreate ? uomNameTextBox : uomNameTextBoxEdit
        let typeCombo = isCreate ? unitTypeComboBox : unitTypeComboBoxEdit

        clearValidationErrors()

        let nameValidation = validator.validateNotEmpty(nameInput.text)
        if (!nameValidation.isValid) {
            unitOfMeasurementRoot.uomNameError = nameValidation.message
            return false
        }

        let typeValidation = validator.validateComboBoxSelection(typeCombo.currentIndex, "unit type")
        if (!typeValidation.isValid) {
            unitOfMeasurementRoot.unitTypeError = typeValidation.message
            return false
        }

        return true
    }

    function validateUOMName() {
        let text = newUnitOfMeasurementPopup.visible ? uomNameTextBox.text : uomNameTextBoxEdit.text
        let validation = validator.validateNotEmpty(text)

        if (!validation.isValid) {
            unitOfMeasurementRoot.uomNameError = validation.message
        } else {
            unitOfMeasurementRoot.uomNameError = ""
        }
    }

    function clearValidationErrors() {
        unitOfMeasurementRoot.uomNameError = ""
        unitOfMeasurementRoot.unitTypeError = ""
        unitOfMeasurementRoot.conversionError = ""
        unitOfMeasurementRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            uomNameTextBox.text = ""
            unitTypeComboBox.currentIndex = -1
            conToSqmTextBox.text = ""
            conToCubicMeterTextBox.text = ""
            conToMeterTextBox.text = ""
            conToKgTextBox.text = ""
            unitOfMeasurementRoot.isCreateFormValid = false
        } else {
            uomNameTextBoxEdit.text = ""
            unitTypeComboBoxEdit.currentIndex = -1
            conToSqmTextBoxEdit.text = ""
            conToCubicMeterTextBoxEdit.text = ""
            conToMeterTextBoxEdit.text = ""
            conToKgTextBoxEdit.text = ""
            unitOfMeasurementRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            unitOfMeasurementRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
        nameValidationTimer.stop()
    }

    // ========== DATA LOADING FUNCTIONS ==========

    // Load and display unit of measurements list
    function showList() {
        unitOfMeasurementRoot.unitOfMeasurementList = []
        if (!unitOfMeasurementRoot.visible) return

        var uoms = unitOfMeasurementController.getUOMList(isApproved)

        for (var i = 0; i < uoms.length; i++) {
            if (uoms[i].nextApprovingUser === userController.getCurrentId()) {
                uoms[i].displayStatus = "Pending"
            } else {
                uoms[i].displayStatus = uoms[i].approvalStatus
            }
        }

        unitOfMeasurementRoot.unitOfMeasurementList = uoms
    }

    // Fill edit popup with selected data
    function fillPopup() {
        if (!selectedUom) return

        uomNameTextBoxEdit.text = selectedUom.uomName || ""

        for (var k = 0; k < unitOfMeasurementRoot.unitTypeList.length; k++) {
            if (unitOfMeasurementRoot.unitTypeList[k] === selectedUom.unitType) {
                unitTypeComboBoxEdit.currentIndex = k
                break
            }
        }

        conToSqmTextBoxEdit.text = selectedUom.conversionToSqm !== null ? String(selectedUom.conversionToSqm) : ""
        conToCubicMeterTextBoxEdit.text = selectedUom.conversionToCubicMeter || ""
        conToMeterTextBoxEdit.text = selectedUom.conversionToMeter || ""
        conToKgTextBoxEdit.text = selectedUom.conversionToKilogram || ""

        checkEditFormValidity()
    }
}
