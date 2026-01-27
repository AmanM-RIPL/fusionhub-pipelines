import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: materialRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var unitOfMeasurementsFromCtrl: []

    // Properties - Lists for UI
    property var materialList: []
    property var unitOfMeasurementList: []

    // Properties - State management
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false
    property bool canApproveReject: false

    // Properties - Validation errors
    property string materialNameError: ""
    property string categoryError: ""
    property string unitError: ""
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

    // Controllers
    MaterialController {
        id: materialController
    }

    UnitOfMeasurementController {
        id: unitOfMeasurementController
    }

    Timer {
        id: nameValidationTimer
        interval: 500
        repeat: false
        onTriggered: validateMaterialName()
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newMaterialPopup
        popupWidth: 600
        popupHeight: 550
        title: "Create Material"
        parent: Overlay.overlay
        buttonEnabled: materialRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                materialController.create(
                            materialNameTextBox.text,
                            categoryComboBox.currentText,
                            unitOfMeasurementsFromCtrl[unitOfMeasurementComboBox.currentIndex].id
                            )
                resetForm(true)
                showMaterialList()
                close()
            }
        }

        onCancelCallback: function () {
            resetForm(true)
            close()
        }

        onClosed: showMaterialList()

        onOpened: {
            resetForm(true)
            if (materialRoot.visible) {
                loadUnitOfMeasurements()
                unitOfMeasurementComboBox.model = materialRoot.unitOfMeasurementList
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Material Name <span style='color: " + materialRoot.mandatoryColor + ";'>*</span>"
                color: materialRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: materialNameTextBox
                placeholderText: "Material Name"
                text: ""
                color: materialRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    nameValidationTimer.stop()
                    nameValidationTimer.start()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: materialRoot.materialNameError
                color: materialRoot.mandatoryColor
                font.pixelSize: 12
                visible: materialRoot.materialNameError !== ""
            }

            Text {
                text: "Category"
                color: materialRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: categoryComboBox
                width: parent.width
                model: ["Raw Material", "Non-Construction", "Finished Good"]
                currentIndex: -1
                onCurrentIndexChanged: {
                    materialRoot.categoryError = (currentIndex !== -1) ? "" : "Please select category"
                    checkCreateFormValidity()
                }
            }

            Text {
                text: materialRoot.categoryError
                color: materialRoot.mandatoryColor
                font.pixelSize: 12
                visible: materialRoot.categoryError !== ""
            }

            Text {
                text: "Unit of Measurement"
                color: materialRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: unitOfMeasurementComboBox
                width: parent.width
                model: materialRoot.unitOfMeasurementList
                currentIndex: -1
                onCurrentIndexChanged: {
                    materialRoot.unitError = (currentIndex !== -1) ? "" : "Please select unit of measurement"
                    checkCreateFormValidity()
                }
            }

            Text {
                text: materialRoot.unitError
                color: materialRoot.mandatoryColor
                font.pixelSize: 12
                visible: materialRoot.unitError !== ""
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
        FHPopup {
            id: viewEditPopup
            popupWidth: 600
            popupHeight: 600
            title: popupMode === "view" ? "View Material" : "Edit Material"
            showAcceptButton: popupMode === "edit"
            buttonName: popupMode === "edit" ? "Update" : ""
            buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
            buttonEnabled: popupMode === "edit" ? materialRoot.isEditFormValid : true

            // Property to check if user can approve/reject
            property bool canApproveReject: selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)

            onAcceptCallback: function () {
                if (popupMode === "edit" && selectedData && validateForm(false)) {
                    materialController.update(
                                selectedData.id,
                                materialNameTextBoxEdit.text,
                                categoryComboBoxEdit.currentText,
                                unitOfMeasurementsFromCtrl[unitOfMeasurementComboBoxEdit.currentIndex].id
                                )
                    resetForm(false)
                    showMaterialList()
                    close()
                } else if (popupMode === "view") {
                    // Approve when in view mode
                    if (selectedData) {
                        console.log("Approving material ID: " + selectedData.id)
                        materialController.approve(selectedData.id)
                        resetForm(false)
                        showMaterialList()
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
                loadUnitOfMeasurements()
                unitOfMeasurementComboBoxEdit.model = materialRoot.unitOfMeasurementList
                if (selectedData) {
                       materialRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)
                       fillPopup()
                   }
            }

            onClosed: {
                materialRoot.cancellationReasonError = ""
                cancellationReasonTextBox.text = ""
            }

            Column {
                width: parent.width
                spacing: 10

                Text {
                    text: "Material Name"
                    color: materialRoot.labelColor
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                    topPadding: 10
                }

                CustomTextBox {
                    id: materialNameTextBoxEdit
                    placeholderText: "Material Name"
                    text: ""
                    color: materialRoot.labelColor
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
                    text: materialRoot.materialNameError
                    color: materialRoot.mandatoryColor
                    font.pixelSize: 12
                    visible: materialRoot.materialNameError !== ""
                }

                Text {
                    text: "Category"
                    color: materialRoot.labelColor
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                    topPadding: 10
                }

                CustomComboBox {
                    id: categoryComboBoxEdit
                    width: parent.width
                    model: ["Raw Material", "Non-Construction", "Finished Good"]
                    currentIndex: -1
                    enabled: popupMode === "edit"
                    onCurrentIndexChanged: {
                        materialRoot.categoryError = (currentIndex !== -1) ? "" : "Please select category"
                        checkEditFormValidity()
                    }
                }

                Text {
                    text: materialRoot.categoryError
                    color: materialRoot.mandatoryColor
                    font.pixelSize: 12
                    visible: materialRoot.categoryError !== ""
                }

                Text {
                    text: "Unit of Measurement"
                    color: materialRoot.labelColor
                    font.weight: 700
                    font.pixelSize: 14
                    font.family: "Segoe UI"
                    topPadding: 10
                }

                CustomComboBox {
                    id: unitOfMeasurementComboBoxEdit
                    width: parent.width
                    model: materialRoot.unitOfMeasurementList
                    currentIndex: -1
                    enabled: popupMode === "edit"
                    onCurrentIndexChanged: {
                        materialRoot.unitError = (currentIndex !== -1) ? "" : "Please select unit of measurement"
                        checkEditFormValidity()
                    }
                }

                Text {
                    text: materialRoot.unitError
                    color: materialRoot.mandatoryColor
                    font.pixelSize: 12
                    visible: materialRoot.unitError !== ""
                }

                // Cancellation Reason
                Text {
                    text: "Cancellation Reason"
                    color: materialRoot.labelColor
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
                    color: materialRoot.labelColor
                    width: parent.width
                    height: 60
                    visible: popupMode === "view" && canApproveReject
                    wrapMode: TextEdit.Wrap
                    onTextChanged: {
                        if (text.trim() !== "") {
                            materialRoot.cancellationReasonError = ""
                        }
                    }
                }

                Text {
                    text: materialRoot.cancellationReasonError
                    color: materialRoot.mandatoryColor
                    font.pixelSize: 12
                    visible: materialRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
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
                                console.log("Approving material ID: " + selectedData.id)
                                materialController.approve(selectedData.id)
                                resetForm(false)
                                showMaterialList()
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
                                materialRoot.cancellationReasonError = "Please provide cancellation reason"
                                return
                            }

                            if (selectedData) {
                              //  console.log("Cancelling material ID: " + selectedData.id + " Reason: " + reason)
                                materialController.cancel(selectedData.id, reason)
                                resetForm(false)
                                showMaterialList()
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
        } // -------- Header Row --------
    Row {
        spacing: 20
        Text {
            text: "Material"
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
                onClicked: newMaterialPopup.open()
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
            color: materialRoot.labelColor
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
                color: materialRoot.labelColor
                font.pixelSize: 14
            }
            onCurrentTextChanged: {
                isApproved = (currentText === "Approved")
                showMaterialList()
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
        model: materialRoot.materialList

        columns: [
            {"label": "Id", "width": 300, "key": "id"},
            {"label": "Material Name", "width": 300, "key": "materialName"},
            {"label": "Category", "width": 300, "key": "category"},
            {"label": "Unit of Measurement", "width": 300, "key": "unitName"},
            {"label": "Status", "width": 90, "key": "displayStatus"}
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
    Component.onCompleted: showMaterialList()
    onVisibleChanged: showMaterialList()

    // ========== VALIDATION FUNCTIONS ==========

    // Check if create form is valid
    function checkCreateFormValidity() {
        let nameValid = validateField(materialNameTextBox.text)
        let categoryValid = categoryComboBox.currentIndex !== -1
        let unitValid = unitOfMeasurementComboBox.currentIndex !== -1

        materialRoot.isCreateFormValid = nameValid && categoryValid && unitValid
    }

    // Check if edit form is valid
    function checkEditFormValidity() {
        let nameValid = validateField(materialNameTextBoxEdit.text)
        let categoryValid = categoryComboBoxEdit.currentIndex !== -1
        let unitValid = unitOfMeasurementComboBoxEdit.currentIndex !== -1

        materialRoot.isEditFormValid = nameValid && categoryValid && unitValid
    }

    // Main validation function
    function validateForm(isCreate) {
        let nameInput = isCreate ? materialNameTextBox : materialNameTextBoxEdit
        let categoryCombo = isCreate ? categoryComboBox : categoryComboBoxEdit
        let unitCombo = isCreate ? unitOfMeasurementComboBox : unitOfMeasurementComboBoxEdit

        clearValidationErrors()

        if (!validateField(nameInput.text)) {
            return false
        }

        if (categoryCombo.currentIndex === -1) {
            materialRoot.categoryError = "Please select category"
            return false
        }

        if (unitCombo.currentIndex === -1) {
            materialRoot.unitError = "Please select unit of measurement"
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
            materialRoot.materialNameError = result.message
            return false
        } else {
            materialRoot.materialNameError = ""
            return true
        }
    }

    function validateMaterialName() {
        let text = newMaterialPopup.visible ? materialNameTextBox.text : materialNameTextBoxEdit.text
        validateField(text)
    }

    function clearValidationErrors() {
        materialRoot.materialNameError = ""
        materialRoot.categoryError = ""
        materialRoot.unitError = ""
        materialRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            materialNameTextBox.text = ""
            categoryComboBox.currentIndex = -1
            unitOfMeasurementComboBox.currentIndex = -1
            materialRoot.isCreateFormValid = false
        } else {
            materialNameTextBoxEdit.text = ""
            categoryComboBoxEdit.currentIndex = -1
            unitOfMeasurementComboBoxEdit.currentIndex = -1
            materialRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            materialRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
        nameValidationTimer.stop()
    }

    // ========== DATA LOADING FUNCTIONS ==========

    // Load unit of measurements list
    function loadUnitOfMeasurements() {
        unitOfMeasurementsFromCtrl = unitOfMeasurementController.getUOMList(true)
        var tempUnitList = []

        for (var i = 0; i < unitOfMeasurementsFromCtrl.length; i++) {
            tempUnitList.push(unitOfMeasurementsFromCtrl[i].uomName)
        }

        materialRoot.unitOfMeasurementList = tempUnitList
    }

    // Load and display materials list
    function showMaterialList() {
        materialRoot.materialList = []
        if (!materialRoot.visible) return

        var materials = materialController.getMaterialList(isApproved)
        var unitOfMeasurementList = unitOfMeasurementController.getUOMList(true)

        for (var i = 0; i < materials.length; i++) {
            var unitName = ""
            for (var j = 0; j < unitOfMeasurementList.length; j++) {
                if (unitOfMeasurementList[j].id === materials[i].unitOfMeasurementId) {
                    unitName = unitOfMeasurementList[j].uomName
                    break
                }
            }
            materials[i].unitName = unitName

            if (materials[i].nextApprovingUser === 2 ) {
                        materials[i].displayStatus = "Pending"
                    } else {
                        materials[i].displayStatus = materials[i].approvalStatus
                    }
        }

        materialRoot.materialList = materials
    }

    // Fill edit popup with selected data
    function fillPopup() {
        if (!selectedData) return

        materialNameTextBoxEdit.text = selectedData.materialName || ""

        var categories = ["Raw Material", "Non-Construction", "Finished Good"]
        for (var k = 0; k < categories.length; k++) {
            if (categories[k] === selectedData.category) {
                categoryComboBoxEdit.currentIndex = k
                break
            }
        }

        for (var j = 0; j < unitOfMeasurementsFromCtrl.length; j++) {
            if (unitOfMeasurementsFromCtrl[j].id === selectedData.unitOfMeasurementId) {
                unitOfMeasurementComboBoxEdit.currentIndex = j
                break
            }
        }
        checkEditFormValidity()
    }
}
