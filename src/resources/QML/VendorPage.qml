import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: vendorRoot
    width: parent.width
    padding: 10

    // ========== PROPERTIES - LISTS FOR UI ==========
    property var vendorList: []

    // ========== PROPERTIES - STATE MANAGEMENT ==========
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false
    property bool canApproveReject: false

    // ========== PROPERTIES - VALIDATION ERRORS ==========
    property string vendorNameError: ""
    property string vendorAddressError: ""
    property string vendorContactPersonError: ""
    property string vendorMobileError: ""
    property string vendorEmailError: ""
    property string cancellationReasonError: ""

    // ========== PROPERTIES - FORM VALIDITY ==========
    property bool isCreateFormValid: false
    property bool isEditFormValid: false

    // ========== COLOR VARIABLES ==========
    property string mandatoryColor: "#D13438"  // Red
    property string labelColor: "#323130"      // Dark Gray

    // ========== VALIDATION HELPER ==========
    ValidationHelper {
        id: validator
    }

    // ========== CONTROLLERS ==========
    VendorController {
        id: vendorController
    }

    UserController {
        id: userController
    }

    DraftEntityController {
        id: draftEntityController
    }

    // ========== VALIDATION TIMERS ==========
    Timer {
        id: vendorNameValidationTimer
        interval: 500
        repeat: false
        onTriggered: validateVendorName()
    }

    Timer {
        id: emailValidationTimer
        interval: 500
        repeat: false
        onTriggered: validateEmail()
    }

    Timer {
        id: phoneValidationTimer
        interval: 500
        repeat: false
        onTriggered: validatePhone()
    }

    /* ---------- CREATE VENDOR POPUP ---------- */
    FHPopup {
        id: newVendorPopup
        popupWidth: 600
        popupHeight: 550
        title: "Create Vendor"
        parent: Overlay.overlay
        buttonEnabled: vendorRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                vendorController.create(
                    vendorNameTextBox.text,
                    vendorAddressTextBox.text,
                    vendorContactPersonTextBox.text,
                    vendorMobileTextBox.text,
                    vendorEmailTextBox.text
                )
                resetForm(true)
                showVendorList()
                close()
            }
        }

        onCancelCallback: function () {
            resetForm(true)
            close()
        }

        onClosed: showVendorList()

        onOpened: {
            resetForm(true)
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Vendor Name <span style='color: " + vendorRoot.mandatoryColor + ";'>*</span>"
                color: vendorRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: vendorNameTextBox
                placeholderText: "Vendor Name"
                text: ""
                color: vendorRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    vendorNameValidationTimer.stop()
                    vendorNameValidationTimer.start()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: vendorRoot.vendorNameError
                color: vendorRoot.mandatoryColor
                font.pixelSize: 12
                visible: vendorRoot.vendorNameError !== ""
            }

            Text {
                text: "Vendor Address <span style='color: " + vendorRoot.mandatoryColor + ";'>*</span>"
                color: vendorRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: vendorAddressTextBox
                placeholderText: "Vendor Address"
                text: ""
                color: vendorRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    validateAddress()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: vendorRoot.vendorAddressError
                color: vendorRoot.mandatoryColor
                font.pixelSize: 12
                visible: vendorRoot.vendorAddressError !== ""
            }

            Text {
                text: "Vendor Contact Person <span style='color: " + vendorRoot.mandatoryColor + ";'>*</span>"
                color: vendorRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: vendorContactPersonTextBox
                placeholderText: "Vendor Contact Person"
                text: ""
                color: vendorRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    validateContactPerson()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: vendorRoot.vendorContactPersonError
                color: vendorRoot.mandatoryColor
                font.pixelSize: 12
                visible: vendorRoot.vendorContactPersonError !== ""
            }

            Text {
                text: "Vendor Mobile <span style='color: " + vendorRoot.mandatoryColor + ";'>*</span>"
                color: vendorRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: vendorMobileTextBox
                placeholderText: "Vendor Mobile"
                text: ""
                color: vendorRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    phoneValidationTimer.stop()
                    phoneValidationTimer.start()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: vendorRoot.vendorMobileError
                color: vendorRoot.mandatoryColor
                font.pixelSize: 12
                visible: vendorRoot.vendorMobileError !== ""
            }

            Text {
                text: "Vendor Email <span style='color: " + vendorRoot.mandatoryColor + ";'>*</span>"
                color: vendorRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: vendorEmailTextBox
                placeholderText: "Vendor Email"
                text: ""
                color: vendorRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    emailValidationTimer.stop()
                    emailValidationTimer.start()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: vendorRoot.vendorEmailError
                color: vendorRoot.mandatoryColor
                font.pixelSize: 12
                visible: vendorRoot.vendorEmailError !== ""
            }
        }
    }

    /* ---------- VIEW / EDIT VENDOR POPUP ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 600
        popupHeight: 600
        title: popupMode === "view" ? "View Vendor" : "Edit Vendor"
        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? vendorRoot.isEditFormValid : true

        property bool canApproveReject: selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData && validateForm(false)) {
                vendorController.update(
                    selectedData.id,
                    vendorNameTextBoxEdit.text,
                    vendorAddressTextBoxEdit.text,
                    vendorContactPersonTextBoxEdit.text,
                    vendorMobileTextBoxEdit.text,
                    vendorEmailTextBoxEdit.text
                )
                resetForm(false)
                showVendorList()
                close()
            } else if (popupMode === "view") {
                if (selectedData) {
                    draftEntityController.approve(selectedData.id)
                    resetForm(false)
                    showVendorList()
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
                vendorRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)
                fillPopup()
            }
        }

        onClosed: {
            vendorRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            spacing: 10
            bottomPadding: 25

            Text {
                text: "Vendor Name"
                color: vendorRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorNameTextBoxEdit
                placeholderText: "Vendor Name"
                text: ""
                color: vendorRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        vendorNameValidationTimer.stop()
                        vendorNameValidationTimer.start()
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: vendorRoot.vendorNameError
                color: vendorRoot.mandatoryColor
                font.pixelSize: 12
                visible: vendorRoot.vendorNameError !== ""
            }

            Text {
                text: "Vendor Address"
                color: vendorRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorAddressTextBoxEdit
                placeholderText: "Vendor Address"
                text: ""
                color: vendorRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        validateAddressEdit()
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: vendorRoot.vendorAddressError
                color: vendorRoot.mandatoryColor
                font.pixelSize: 12
                visible: vendorRoot.vendorAddressError !== ""
            }

            Text {
                text: "Vendor Contact Person"
                color: vendorRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorContactPersonTextBoxEdit
                placeholderText: "Vendor Contact Person"
                text: ""
                color: vendorRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        validateContactPersonEdit()
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: vendorRoot.vendorContactPersonError
                color: vendorRoot.mandatoryColor
                font.pixelSize: 12
                visible: vendorRoot.vendorContactPersonError !== ""
            }

            Text {
                text: "Vendor Mobile"
                color: vendorRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorMobileTextBoxEdit
                placeholderText: "Vendor Mobile"
                text: ""
                color: vendorRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        phoneValidationTimer.stop()
                        phoneValidationTimer.start()
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: vendorRoot.vendorMobileError
                color: vendorRoot.mandatoryColor
                font.pixelSize: 12
                visible: vendorRoot.vendorMobileError !== ""
            }

            Text {
                text: "Vendor Email"
                color: vendorRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorEmailTextBoxEdit
                placeholderText: "Vendor Email"
                text: ""
                color: vendorRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        emailValidationTimer.stop()
                        emailValidationTimer.start()
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: vendorRoot.vendorEmailError
                color: vendorRoot.mandatoryColor
                font.pixelSize: 12
                visible: vendorRoot.vendorEmailError !== ""
            }

            // Cancellation Reason
            Text {
                text: "Cancellation Reason"
                color: vendorRoot.labelColor
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
                color: vendorRoot.labelColor
                width: parent.width
                height: 60
                visible: popupMode === "view" && canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        vendorRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: vendorRoot.cancellationReasonError
                color: vendorRoot.mandatoryColor
                font.pixelSize: 12
                visible: vendorRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
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
                            draftEntityController.approve(selectedData.id)
                            resetForm(false)
                            showVendorList()
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

                        if (reason === "") {
                            vendorRoot.cancellationReasonError = "Please provide cancellation reason"
                            return
                        }

                        if (selectedData) {
                            console.log("Rejecting vendor ID: " + selectedData.id + " Reason: " + reason)
                            vendorController.cancel(selectedData.id, reason)
                            resetForm(false)
                            showVendorList()
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
            text: "Vendor"
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
                onClicked: newVendorPopup.open()
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
            color: vendorRoot.labelColor
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
                color: vendorRoot.labelColor
                font.pixelSize: 14
            }
            onCurrentTextChanged: {
                isApproved = (currentText === "Approved")
                showVendorList()
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
        model: vendorRoot.vendorList

        columns: [
            {"label": "Id", "width": 150, "key": "id"},
            {"label": "Vendor Name", "width": 250, "key": "vendorName"},
            {"label": "Address", "width": 250, "key": "vendorAddress"},
            {"label": "Contact Person", "width": 200, "key": "vendorContactPerson"},
            {"label": "Mobile", "width": 150, "key": "vendorMobile"},
            {"label": "Email", "width": 200, "key": "vendorEmail"},
            {"label": "Status", "width": 100, "key": "displayStatus"}
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
    Component.onCompleted: showVendorList()
    onVisibleChanged: showVendorList()

    // ========== VALIDATION FUNCTIONS ==========

    function checkCreateFormValidity() {
        let nameValid = validator.validateNotEmpty(vendorNameTextBox.text).isValid && validator.validateAlphanumeric(vendorNameTextBox.text).isValid
        let addressValid = validator.validateNotEmpty(vendorAddressTextBox.text).isValid
        let contactValid = validator.validateNotEmpty(vendorContactPersonTextBox.text).isValid && validator.validateAlphanumeric(vendorContactPersonTextBox.text).isValid
        let phoneValid = validator.validatePhone(vendorMobileTextBox.text).isValid
        let emailValid = validator.validateEmail(vendorEmailTextBox.text).isValid

        vendorRoot.isCreateFormValid = nameValid && addressValid && contactValid && phoneValid && emailValid
    }

    function checkEditFormValidity() {
        let nameValid = validator.validateNotEmpty(vendorNameTextBoxEdit.text).isValid && validator.validateAlphanumeric(vendorNameTextBoxEdit.text).isValid
        let addressValid = validator.validateNotEmpty(vendorAddressTextBoxEdit.text).isValid
        let contactValid = validator.validateNotEmpty(vendorContactPersonTextBoxEdit.text).isValid && validator.validateAlphanumeric(vendorContactPersonTextBoxEdit.text).isValid
        let phoneValid = validator.validatePhone(vendorMobileTextBoxEdit.text).isValid
        let emailValid = validator.validateEmail(vendorEmailTextBoxEdit.text).isValid

        vendorRoot.isEditFormValid = nameValid && addressValid && contactValid && phoneValid && emailValid
    }

    function validateForm(isCreate) {
        let nameInput = isCreate ? vendorNameTextBox : vendorNameTextBoxEdit
        let addressInput = isCreate ? vendorAddressTextBox : vendorAddressTextBoxEdit
        let contactInput = isCreate ? vendorContactPersonTextBox : vendorContactPersonTextBoxEdit
        let phoneInput = isCreate ? vendorMobileTextBox : vendorMobileTextBoxEdit
        let emailInput = isCreate ? vendorEmailTextBox : vendorEmailTextBoxEdit

        clearValidationErrors()

        let nameValidation = validator.validateNotEmpty(nameInput.text)
        if (!nameValidation.isValid) {
            vendorRoot.vendorNameError = nameValidation.message
            return false
        }
        nameValidation = validator.validateAlphanumeric(nameInput.text)
        if (!nameValidation.isValid) {
            vendorRoot.vendorNameError = nameValidation.message
            return false
        }

        let addressValidation = validator.validateNotEmpty(addressInput.text)
        if (!addressValidation.isValid) {
            vendorRoot.vendorAddressError = addressValidation.message
            return false
        }

        let contactValidation = validator.validateNotEmpty(contactInput.text)
        if (!contactValidation.isValid) {
            vendorRoot.vendorContactPersonError = contactValidation.message
            return false
        }
        contactValidation = validator.validateAlphanumeric(contactInput.text)
        if (!contactValidation.isValid) {
            vendorRoot.vendorContactPersonError = contactValidation.message
            return false
        }

        let phoneValidation = validator.validatePhone(phoneInput.text)
        if (!phoneValidation.isValid) {
            vendorRoot.vendorMobileError = phoneValidation.message
            return false
        }

        let emailValidation = validator.validateEmail(emailInput.text)
        if (!emailValidation.isValid) {
            vendorRoot.vendorEmailError = emailValidation.message
            return false
        }

        return true
    }

    function validateVendorName() {
        let text = newVendorPopup.visible ? vendorNameTextBox.text : vendorNameTextBoxEdit.text
        let result = validator.validateNotEmpty(text)

        if (!result.isValid) {
            vendorRoot.vendorNameError = result.message
            return
        }
        result = validator.validateAlphanumeric(text)
        if (!result.isValid) {
            vendorRoot.vendorNameError = result.message
        } else {
            vendorRoot.vendorNameError = ""
        }
    }

    function validateAddress() {
        let text = vendorAddressTextBox.text
        let result = validator.validateNotEmpty(text)

        if (!result.isValid) {
            vendorRoot.vendorAddressError = result.message
        } else {
            vendorRoot.vendorAddressError = ""
        }
    }

    function validateAddressEdit() {
        let text = vendorAddressTextBoxEdit.text
        let result = validator.validateNotEmpty(text)

        if (!result.isValid) {
            vendorRoot.vendorAddressError = result.message
        } else {
            vendorRoot.vendorAddressError = ""
        }
    }

    function validateContactPerson() {
        let text = vendorContactPersonTextBox.text
        let result = validator.validateNotEmpty(text)

        if (!result.isValid) {
            vendorRoot.vendorContactPersonError = result.message
            return
        }
        result = validator.validateAlphanumeric(text)
        if (!result.isValid) {
            vendorRoot.vendorContactPersonError = result.message
        } else {
            vendorRoot.vendorContactPersonError = ""
        }
    }

    function validateContactPersonEdit() {
        let text = vendorContactPersonTextBoxEdit.text
        let result = validator.validateNotEmpty(text)

        if (!result.isValid) {
            vendorRoot.vendorContactPersonError = result.message
            return
        }
        result = validator.validateAlphanumeric(text)
        if (!result.isValid) {
            vendorRoot.vendorContactPersonError = result.message
        } else {
            vendorRoot.vendorContactPersonError = ""
        }
    }

    function validateEmail() {
        let text = newVendorPopup.visible ? vendorEmailTextBox.text : vendorEmailTextBoxEdit.text
        let result = validator.validateEmail(text)

        if (!result.isValid) {
            vendorRoot.vendorEmailError = result.message
        } else {
            vendorRoot.vendorEmailError = ""
        }
    }

    function validatePhone() {
        let text = newVendorPopup.visible ? vendorMobileTextBox.text : vendorMobileTextBoxEdit.text
        let result = validator.validatePhone(text)

        if (!result.isValid) {
            vendorRoot.vendorMobileError = result.message
        } else {
            vendorRoot.vendorMobileError = ""
        }
    }

    function clearValidationErrors() {
        vendorRoot.vendorNameError = ""
        vendorRoot.vendorAddressError = ""
        vendorRoot.vendorContactPersonError = ""
        vendorRoot.vendorMobileError = ""
        vendorRoot.vendorEmailError = ""
        vendorRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            vendorNameTextBox.text = ""
            vendorAddressTextBox.text = ""
            vendorContactPersonTextBox.text = ""
            vendorMobileTextBox.text = ""
            vendorEmailTextBox.text = ""
            vendorRoot.isCreateFormValid = false
        } else {
            vendorNameTextBoxEdit.text = ""
            vendorAddressTextBoxEdit.text = ""
            vendorContactPersonTextBoxEdit.text = ""
            vendorMobileTextBoxEdit.text = ""
            vendorEmailTextBoxEdit.text = ""
            vendorRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            vendorRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
        vendorNameValidationTimer.stop()
        emailValidationTimer.stop()
        phoneValidationTimer.stop()
    }

    // ========== DATA LOADING FUNCTIONS ==========

    function showVendorList() {
        vendorRoot.vendorList = []
        if (!vendorRoot.visible) return

        var vendors = vendorController.getVendorList(isApproved)

        for (var i = 0; i < vendors.length; i++) {
            if (vendors[i].nextApprovingUser === userController.getCurrentId()) {
                vendors[i].displayStatus = "Pending"
            } else {
                vendors[i].displayStatus = vendors[i].approvalStatus
            }
        }

        vendorRoot.vendorList = vendors
    }

    function fillPopup() {
        if (!selectedData) return

        vendorNameTextBoxEdit.text = selectedData.vendorName || ""
        vendorAddressTextBoxEdit.text = selectedData.vendorAddress || ""
        vendorContactPersonTextBoxEdit.text = selectedData.vendorContactPerson || ""
        vendorMobileTextBoxEdit.text = selectedData.vendorMobile || ""
        vendorEmailTextBoxEdit.text = selectedData.vendorEmail || ""

        checkEditFormValidity()
    }
}
