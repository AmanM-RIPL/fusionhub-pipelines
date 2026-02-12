import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: materialIndentRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var materialsFromCtrl: []
    property var tasksFromCtrl: []

    // Properties - Lists for UI
    property var materialIndentList: []
    property var materialList: []
    property var taskList: []

    // Properties - State management
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false
    property bool canApproveReject: false

    // Properties - Validation errors
    property string materialError: ""
    property string taskError: ""
    property string quantityError: ""
    property string cancellationReasonError: ""

    // Properties - Form validity
    property bool isCreateFormValid: false
    property bool isEditFormValid: false

    // Color Variables
    property string mandatoryColor: "#D13438"  // Red
    property string labelColor: "#323130"      // Dark Gray

    // Controllers
    MaterialIndentController {
        id: materialIndentController
    }

    MaterialController {
        id: materialController
    }

    TaskController {
        id: taskController
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
        id: quantityValidationTimer
        interval: 500
        repeat: false
        onTriggered: validateQuantity()
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newMaterialIndentPopup
        popupWidth: 600
        popupHeight: 550
        title: "Create Material Indent"
        parent: Overlay.overlay
        buttonEnabled: materialIndentRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {
                materialIndentController.create(
                            quantityTextBox.text,
                            materialsFromCtrl[materialComboBox.currentIndex].id,
                            tasksFromCtrl[taskComboBox.currentIndex].id
                            )
                resetForm(true)
                showMaterialIndentList()
                close()
            }
        }

        onCancelCallback: function () {
            resetForm(true)
            close()
        }

        onClosed: showMaterialIndentList()

        onOpened: {
            resetForm(true)
            if (materialIndentRoot.visible) {
                materialsFromCtrl = []
                tasksFromCtrl = []

                materialsFromCtrl = materialController.getMaterialList(true)
                tasksFromCtrl = taskController.getTaskList(true)

                // Clear current lists then populate
                materialIndentRoot.materialList = []
                materialIndentRoot.taskList = []

                for (var i = 0; i < materialsFromCtrl.length; i++) {
                    materialIndentRoot.materialList = materialIndentRoot.materialList.concat(
                                materialsFromCtrl[i].materialName)
                }
                for (var j = 0; j < tasksFromCtrl.length; j++) {
                    materialIndentRoot.taskList = materialIndentRoot.taskList.concat(
                                tasksFromCtrl[j].taskName)
                }
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Material <span style='color: " + materialIndentRoot.mandatoryColor + ";'>*</span>"
                color: materialIndentRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomComboBox {
                id: materialComboBox
                width: parent.width
                model: materialIndentRoot.materialList
                currentIndex: -1
                onCurrentIndexChanged: {
                    if (currentIndex !== -1) {
                        materialIndentRoot.materialError = ""
                    } else {
                        materialIndentRoot.materialError = "Please select material"
                    }
                    checkCreateFormValidity()
                }
            }

            Text {
                text: materialIndentRoot.materialError
                color: materialIndentRoot.mandatoryColor
                font.pixelSize: 12
                visible: materialIndentRoot.materialError !== ""
            }

            Text {
                text: "Task <span style='color: " + materialIndentRoot.mandatoryColor + ";'>*</span>"
                color: materialIndentRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomComboBox {
                id: taskComboBox
                width: parent.width
                model: materialIndentRoot.taskList
                currentIndex: -1
                onCurrentIndexChanged: {
                    if (currentIndex !== -1) {
                        materialIndentRoot.taskError = ""
                    } else {
                        materialIndentRoot.taskError = "Please select task"
                    }
                    checkCreateFormValidity()
                }
            }

            Text {
                text: materialIndentRoot.taskError
                color: materialIndentRoot.mandatoryColor
                font.pixelSize: 12
                visible: materialIndentRoot.taskError !== ""
            }

            Text {
                text: "Quantity <span style='color: " + materialIndentRoot.mandatoryColor + ";'>*</span>"
                color: materialIndentRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            CustomTextBox {
                id: quantityTextBox
                placeholderText: "Quantity"
                text: ""
                color: materialIndentRoot.labelColor
                width: parent.width
                height: 30
                onTextChanged: {
                    quantityValidationTimer.stop()
                    quantityValidationTimer.start()
                    checkCreateFormValidity()
                }
            }

            Text {
                text: materialIndentRoot.quantityError
                color: materialIndentRoot.mandatoryColor
                font.pixelSize: 12
                visible: materialIndentRoot.quantityError !== ""
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 600
        popupHeight: 550
        title: popupMode === "view" ? "View Material Indent" : "Edit Material Indent"
        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? materialIndentRoot.isEditFormValid : true

        // Property to check if user can approve/reject
        property bool canApproveReject: selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData && validateForm(false)) {
                materialIndentController.update(
                            selectedData.id,
                            quantityTextBoxEdit.text,
                            materialsFromCtrl[materialComboBoxEdit.currentIndex].id,
                            tasksFromCtrl[taskComboBoxEdit.currentIndex].id
                            )
                resetForm(false)
                showMaterialIndentList()
                close()
            } else if (popupMode === "view") {
                // Approve when in view mode
                if (selectedData) {
                    console.log("Approving Material Indent ID: " + selectedData.id)
                    draftEntityController.approve(selectedData.id)
                    resetForm(false)
                    showMaterialIndentList()
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
                materialIndentRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)

                // Load lists from controllers
                materialsFromCtrl = []
                tasksFromCtrl = []

                materialsFromCtrl = materialController.getMaterialList(true)
                tasksFromCtrl = taskController.getTaskList(true)

                // Clear and populate material and task lists
                var tempMaterialList = []
                var tempTaskList = []

                for (var i = 0; i < materialsFromCtrl.length; i++) {
                    tempMaterialList = tempMaterialList.concat(materialsFromCtrl[i].materialName)
                }

                for (var k = 0; k < tasksFromCtrl.length; k++) {
                    tempTaskList = tempTaskList.concat(tasksFromCtrl[k].taskName)
                }

                materialIndentRoot.materialList = tempMaterialList
                materialIndentRoot.taskList = tempTaskList

                fillPopup()
            }
        }

        onClosed: {
            materialIndentRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Material"
                color: materialIndentRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: materialComboBoxEdit
                width: parent.width
                model: materialIndentRoot.materialList
                currentIndex: -1
                enabled: popupMode === "edit"
                onCurrentIndexChanged: {
                    if (popupMode === "edit") {
                        materialIndentRoot.materialError = (currentIndex !== -1) ? "" : "Please select material"
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: materialIndentRoot.materialError
                color: materialIndentRoot.mandatoryColor
                font.pixelSize: 12
                visible: materialIndentRoot.materialError !== "" && popupMode === "edit"
            }

            Text {
                text: "Task"
                color: materialIndentRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: taskComboBoxEdit
                width: parent.width
                model: materialIndentRoot.taskList
                currentIndex: -1
                enabled: popupMode === "edit"
                onCurrentIndexChanged: {
                    if (popupMode === "edit") {
                        materialIndentRoot.taskError = (currentIndex !== -1) ? "" : "Please select task"
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: materialIndentRoot.taskError
                color: materialIndentRoot.mandatoryColor
                font.pixelSize: 12
                visible: materialIndentRoot.taskError !== "" && popupMode === "edit"
            }

            Text {
                text: "Quantity"
                color: materialIndentRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: quantityTextBoxEdit
                placeholderText: "Quantity"
                text: ""
                color: materialIndentRoot.labelColor
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        quantityValidationTimer.stop()
                        quantityValidationTimer.start()
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: materialIndentRoot.quantityError
                color: materialIndentRoot.mandatoryColor
                font.pixelSize: 12
                visible: materialIndentRoot.quantityError !== "" && popupMode === "edit"
            }

            // Cancellation Reason
            Text {
                text: "Cancellation Reason"
                color: materialIndentRoot.labelColor
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
                color: materialIndentRoot.labelColor
                width: parent.width
                height: 60
                visible: popupMode === "view" && canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        materialIndentRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: materialIndentRoot.cancellationReasonError
                color: materialIndentRoot.mandatoryColor
                font.pixelSize: 12
                visible: materialIndentRoot.cancellationReasonError !== "" && popupMode === "view" && canApproveReject
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
                        // Approve button
                        if (selectedData) {
                            console.log("Approving Material Indent ID: " + selectedData.id)
                            draftEntityController.approve(selectedData.id)
                            resetForm(false)
                            showMaterialIndentList()
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
                            materialIndentRoot.cancellationReasonError = reasonValidation.message
                            return
                        }

                        if (selectedData) {
                            console.log("Cancelling Material Indent ID: " + selectedData.id + " Reason: " + reason)
                            draftEntityController.cancel(selectedData.id, reason)
                            resetForm(false)
                            showMaterialIndentList()
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
            text: "Material Indent"
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
                onClicked: newMaterialIndentPopup.open()
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
            color: materialIndentRoot.labelColor
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
                color: materialIndentRoot.labelColor
                font.pixelSize: 14
            }

            onCurrentTextChanged: {
                isApproved = (currentText === "Approved")
                showMaterialIndentList()
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
        model: materialIndentRoot.materialIndentList

        columns: [
            {
                "label": "Id",
                "width": 200,
                "key": "id"
            },
            {
                "label": "Material",
                "width": 400,
                "key": "materialName"
            },
            {
                "label": "Task",
                "width": 400,
                "key": "taskName"
            },
            {
                "label": "Quantity",
                "width": 200,
                "key": "quantity"
            },
            {
                "label": "Status",
                "width": 100,
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
    Component.onCompleted: showMaterialIndentList()
    onVisibleChanged: showMaterialIndentList()

    // ========== VALIDATION FUNCTIONS ==========

    // Check if create form is valid
    function checkCreateFormValidity() {
        let materialValidation = validator.validateComboBoxSelection(materialComboBox.currentIndex, "material")
        let taskValidation = validator.validateComboBoxSelection(taskComboBox.currentIndex, "task")
        let quantityValidation = validator.validateInteger(quantityTextBox.text)

        if (!materialValidation.isValid) {
            materialIndentRoot.materialError = materialValidation.message
        } else {
            materialIndentRoot.materialError = ""
        }

        if (!taskValidation.isValid) {
            materialIndentRoot.taskError = taskValidation.message
        } else {
            materialIndentRoot.taskError = ""
        }

        if (!quantityValidation.isValid) {
            materialIndentRoot.quantityError = quantityValidation.message
        } else {
            materialIndentRoot.quantityError = ""
        }

        materialIndentRoot.isCreateFormValid = materialValidation.isValid && taskValidation.isValid && quantityValidation.isValid
    }

    // Check if edit form is valid
    function checkEditFormValidity() {
        let materialValidation = validator.validateComboBoxSelection(materialComboBoxEdit.currentIndex, "material")
        let taskValidation = validator.validateComboBoxSelection(taskComboBoxEdit.currentIndex, "task")
        let quantityValidation = validator.validateInteger(quantityTextBoxEdit.text)

        if (!materialValidation.isValid) {
            materialIndentRoot.materialError = materialValidation.message
        } else {
            materialIndentRoot.materialError = ""
        }

        if (!taskValidation.isValid) {
            materialIndentRoot.taskError = taskValidation.message
        } else {
            materialIndentRoot.taskError = ""
        }

        if (!quantityValidation.isValid) {
            materialIndentRoot.quantityError = quantityValidation.message
        } else {
            materialIndentRoot.quantityError = ""
        }

        materialIndentRoot.isEditFormValid = materialValidation.isValid && taskValidation.isValid && quantityValidation.isValid
    }

    // Main validation function
    function validateForm(isCreate) {
        let materialCombo = isCreate ? materialComboBox : materialComboBoxEdit
        let taskCombo = isCreate ? taskComboBox : taskComboBoxEdit
        let quantityInput = isCreate ? quantityTextBox : quantityTextBoxEdit

        clearValidationErrors()

        let materialValidation = validator.validateComboBoxSelection(materialCombo.currentIndex, "material")
        if (!materialValidation.isValid) {
            materialIndentRoot.materialError = materialValidation.message
            return false
        }

        let taskValidation = validator.validateComboBoxSelection(taskCombo.currentIndex, "task")
        if (!taskValidation.isValid) {
            materialIndentRoot.taskError = taskValidation.message
            return false
        }

        let quantityValidation = validator.validateNotEmpty(quantityInput.text)
        if (!quantityValidation.isValid) {
            materialIndentRoot.quantityError = quantityValidation.message
            return false
        }

        return true
    }

    function validateQuantity() {
        let text = newMaterialIndentPopup.visible ? quantityTextBox.text : quantityTextBoxEdit.text
        let validation = validator.validateNotEmpty(text)

        if (!validation.isValid) {
            materialIndentRoot.quantityError = validation.message
        } else {
            materialIndentRoot.quantityError = ""
        }
    }

    function clearValidationErrors() {
        materialIndentRoot.materialError = ""
        materialIndentRoot.taskError = ""
        materialIndentRoot.quantityError = ""
        materialIndentRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            quantityTextBox.text = ""
            materialComboBox.currentIndex = -1
            taskComboBox.currentIndex = -1
            materialIndentRoot.isCreateFormValid = false
        } else {
            quantityTextBoxEdit.text = ""
            materialComboBoxEdit.currentIndex = -1
            taskComboBoxEdit.currentIndex = -1
            materialIndentRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            materialIndentRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
        quantityValidationTimer.stop()
    }

    // ========== DATA LOADING FUNCTIONS ==========

    // Load and display material indent list
    function showMaterialIndentList() {
        materialIndentRoot.materialIndentList = []

        if (!materialIndentRoot.visible)
            return

        var materialIndents = materialIndentController.getMaterialIndentList(isApproved)

        for (var i = 0; i < materialIndents.length; i++) {
            if (materialIndents[i].nextApprovingUser === userController.getCurrentId()) {
                materialIndents[i].displayStatus = "Pending"
            } else {
                materialIndents[i].displayStatus = materialIndents[i].approvalStatus
            }
        }

        materialIndentRoot.materialIndentList = materialIndents
    }

    // Fill edit popup with selected data
    function fillPopup() {
        if (!selectedData)
            return

        // Find and set material
        for (var i = 0; i < materialsFromCtrl.length; i++) {
            if (materialsFromCtrl[i].id === selectedData.materialId) {
                materialComboBoxEdit.currentIndex = i
                break
            }
        }

        // Find and set task
        for (var j = 0; j < tasksFromCtrl.length; j++) {
            if (tasksFromCtrl[j].id === selectedData.taskId) {
                taskComboBoxEdit.currentIndex = j
                break
            }
        }

        quantityTextBoxEdit.text = selectedData.quantity || ""

        checkEditFormValidity()
    }
}
