import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: scheduleSetupRoot
    width: parent.width
    padding: 10

    // ========== PROPERTIES - DATA ==========
    property var scheduleSetupList: []
    property var costParameterList: []
    property var resourceParameterList: []
    property var typeOfBimDimensionList: ["Nos", "cubic mtr", "kg", "meter"]
    property var purchaseMaterialList: ["--", "M-30 Steel", "M-20 Steel", "Gravel", "Bamboo", "Capstones", "Cement"]

    // ========== PROPERTIES - STATE MANAGEMENT ==========
    property bool isApproved: false
    property var selectedData: null
    property string popupMode: "view"
    property bool canApproveReject: false

    // ========== PROPERTIES - VALIDATION ERRORS ==========
    property string scheduleNameError: ""
    property string descriptionError: ""
    property string costParameterError: ""
    property string resourceParameterError: ""
    property string cancellationReasonError: ""

    // ========== PROPERTIES - FORM VALIDITY ==========
    property bool isCreateFormValid: false
    property bool isEditFormValid: false

    // ========== COLOR VARIABLES ==========
    property string mandatoryColor: "#D13438" // Red
    property string labelColor: "#323130" // Dark Gray
    property string successColor: "#28A745" // Green
    property string dangerColor: "#DC3545" // Red

    // ========== CONTROLLERS ==========
    MaterialController {
        id: materialController
    }

    ScheduleSetupController {
        id: scheduleSetupController
    }

    ProjectController {
        id: projController
    }

    UserController {
        id: userController
    }

    DraftEntityController {
        id: draftEntityController
    }

    ValidationHelper {
        id: validator
    }

    // ========== CREATE POPUP ==========
    FHPopup {
        id: newScheduleSetupPopup
        popupWidth: 700
        popupHeight: 650
        title: "Create Schedule Setup"
        parent: Overlay.overlay
        buttonEnabled: scheduleSetupRoot.isCreateFormValid

        onAcceptCallback: function () {
            if (validateForm(true)) {

                scheduleSetupController.create(
                            scheduleNameTextBox.text,
                            descriptionTextBox.text,
                            costParameterList,
                            resourceParameterList
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

        onOpened: {
            resetForm(true)
            if (scheduleSetupRoot.visible) {
                purchaseMaterialList = []
                var materialList = materialController.getMaterialList(true)
                for (var i = 0; i < materialList.length; i++) {
                    purchaseMaterialList = purchaseMaterialList.concat(materialList[i].materialName)
                }
            }
        }

        Column {
            width: parent.width
            spacing: 10

            // Schedule Name
            Text {
                text: "Schedule Name *"
                color: scheduleSetupRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
            }

            CustomTextBox {
                id: scheduleNameTextBox
                placeholderText: "Schedule Name"
                text: ""
                color: scheduleSetupRoot.labelColor
                width: parent.width - 8
                onTextChanged: {
                    checkCreateFormValidity()
                }
            }

            Text {
                text: scheduleSetupRoot.scheduleNameError
                color: scheduleSetupRoot.mandatoryColor
                font.pixelSize: 12
                visible: scheduleSetupRoot.scheduleNameError !== ""
            }

            // Description
            Text {
                text: "Description *"
                color: scheduleSetupRoot.labelColor
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
                color: scheduleSetupRoot.labelColor
                width: parent.width - 8
                onTextChanged: {
                    checkCreateFormValidity()
                }
            }

            Text {
                text: scheduleSetupRoot.descriptionError
                color: scheduleSetupRoot.mandatoryColor
                font.pixelSize: 12
                visible: scheduleSetupRoot.descriptionError !== ""
            }

            // Cost Parameter Label
            Text {
                text: "Cost Parameter *"
                color: scheduleSetupRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            // Cost Parameter Table
            Rectangle {
                width: parent.width - 6
                height: 140
                color: "#EDF1F4"
                border.color: "#D0D0D0"
                border.width: 1

                Column {
                    FHTable {
                        width: parent.width - 4
                        height: 120
                        leftPadding: 2
                        removeRow: true
                        model: costParameterList
                        columns: [
                            { label: "Cost Parameter Name", width: 210, key: "cost_param_name" },
                            { label: "Purchase Material", width: 210, key: "purchase_material" },
                            { label: "Type of BIM Dimension", width: 210, key: "type_of_bim_dimension" }
                        ]

                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < costParameterList.length) {
                                var temp = []
                                for (var i = 0; i < costParameterList.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(costParameterList[i])
                                    }
                                }
                                costParameterList = temp
                                checkCreateFormValidity()
                            }
                        }
                    }
                }
            }

            Text {
                text: scheduleSetupRoot.costParameterError
                color: scheduleSetupRoot.mandatoryColor
                font.pixelSize: 12
                visible: scheduleSetupRoot.costParameterError !== ""
                leftPadding: 2
            }

            // Add Cost Parameter Row
            Rectangle {
                width: parent.width - 8
                height: 25
                color: "white"
                border.color: "#D0D0D0"
                border.width: 1

                Row {
                    width: parent.width - 4
                    height: 24
                    leftPadding: 2
                    spacing: 6
                    anchors.verticalCenter: parent.verticalCenter

                    CustomTextBox {
                        id: costParamNameTextBox
                        placeholderText: "Cost Parameter Name"
                        text: ""
                        color: scheduleSetupRoot.labelColor
                        width: 210
                        height: 24
                        topPadding: 1
                    }

                    CustomComboBox {
                        id: purchaseMaterial
                        width: 210
                        height: 24
                        model: purchaseMaterialList
                        currentIndex: 0
                    }

                    CustomComboBox {
                        id: typeOfBimDimensionForCostParam
                        width: 180
                        height: 24
                        model: typeOfBimDimensionList
                        currentIndex: 0
                    }

                    CustomButton {
                        color: "transparent"
                        width: 24
                        height: 24
                        border.color: "#8080808C"
                        btnSource: "qrc:/resources/images/add.svg"
                        btnName: ""
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
                                if (costParamNameTextBox.text === "") {
                                    scheduleSetupRoot.costParameterError = "Please enter Cost Parameter Name"
                                    return
                                }

                                scheduleSetupRoot.costParameterError = ""
                                var newElements = {
                                    cost_param_name: costParamNameTextBox.text,
                                    purchase_material: purchaseMaterial.currentText,
                                    type_of_bim_dimension: typeOfBimDimensionForCostParam.currentText
                                }

                                costParameterList = costParameterList.concat(newElements)
                                costParamNameTextBox.text = ""
                                checkCreateFormValidity()
                            }
                        }
                    }
                }
            }

            // Resource Parameter Label
            Text {
                text: "Resource Parameter *"
                color: scheduleSetupRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            // Resource Parameter Table
            Rectangle {
                width: parent.width - 6
                height: 140
                color: "#EDF1F4"
                border.color: "#D0D0D0"
                border.width: 1


                Column {
                    FHTable {
                        width: parent.width - 4
                        height: 120
                        leftPadding: 2
                        removeRow: true
                        model: resourceParameterList
                        columns: [
                            { label: "Resource Parameter Name", width: 325, key: "resource_param_name" },
                            { label: "Type of BIM Dimension", width: 300, key: "type_of_bim_dimension" }
                        ]

                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < resourceParameterList.length) {
                                var temp = []
                                for (var i = 0; i < resourceParameterList.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(resourceParameterList[i])
                                    }
                                }
                                resourceParameterList = temp
                                checkCreateFormValidity()
                            }
                        }
                    }
                }
            }

            Text {
                text: scheduleSetupRoot.resourceParameterError
                color: scheduleSetupRoot.mandatoryColor
                font.pixelSize: 12
                visible: scheduleSetupRoot.resourceParameterError !== ""
                leftPadding: 2

            }

            // Add Resource Parameter Row
            Rectangle {
                width: parent.width - 8
                height: 28
                color: "white"
                border.color: "#D0D0D0"
                border.width: 1

                Row {
                    width: parent.width - 4
                    height: 24
                    leftPadding: 2
                    spacing: 20
                    anchors.verticalCenter: parent.verticalCenter

                    CustomTextBox {
                        id: resourceParamNameTextBox
                        placeholderText: "Resource Parameter Name"
                        text: ""
                        color: scheduleSetupRoot.labelColor
                        width: 300
                        height: 24
                        topPadding: 1
                    }

                    CustomComboBox {
                        id: typeOfBimDimensionForResourceParam
                        model: typeOfBimDimensionList
                        width: 300
                        height: 24
                        currentIndex: 0
                    }

                    CustomButton {
                        color: "transparent"
                        width: 24
                        height: 24
                        border.color: "#8080808C"
                        btnSource: "qrc:/resources/images/add.svg"
                        btnName: ""
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
                                if (resourceParamNameTextBox.text === "") {
                                    scheduleSetupRoot.resourceParameterError = "Please enter Resource Parameter Name"
                                    return
                                }

                                scheduleSetupRoot.resourceParameterError = ""
                                var newElements = {
                                    resource_param_name: resourceParamNameTextBox.text,
                                    type_of_bim_dimension: typeOfBimDimensionForResourceParam.currentText
                                }

                                resourceParameterList = resourceParameterList.concat(newElements)
                                resourceParamNameTextBox.text = ""
                                checkCreateFormValidity()
                            }
                        }
                    }
                }
            }

        }
    }

    // ========== VIEW / EDIT POPUP ==========
    FHPopup {
        id: viewEditPopup
        popupWidth: 700
        popupHeight: 650
        title: popupMode === "view" ? "View Schedule Setup" : "Edit Schedule Setup"
        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""
        buttonEnabled: popupMode === "edit" ? scheduleSetupRoot.isEditFormValid : true

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData && validateForm(false)) {
                var draftData = {
                    "scheduleSetupName": scheduleNameTextBoxEdit.text,
                    "description": descriptionTextBoxEdit.text,
                    "vecCostParamDataDetails": costParameterList,
                    "vecResourceParamDataDetails": resourceParameterList
                }

                scheduleSetupController.update(selectedData.id, draftData)
                resetForm(false)
                showList()
                close()
            }
        }

        onCancelCallback: function () {
            resetForm(false)
            close()
        }

        onOpened: {
            resetForm(false)
            if (selectedData) {
                scheduleSetupRoot.canApproveReject = selectedData && (selectedData.nextApprovingUser === selectedData.createdByUser)
                fillPopup()
            }
        }

        onClosed: {
            scheduleSetupRoot.cancellationReasonError = ""
            cancellationReasonTextBox.text = ""
        }

        Column {
            width: parent.width
            spacing: 10

            // Schedule Name
            Text {
                text: "Schedule Name"
                color: scheduleSetupRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: scheduleNameTextBoxEdit
                placeholderText: "Schedule Name"
                text: ""
                color: scheduleSetupRoot.labelColor
                width: parent.width - 8
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: scheduleSetupRoot.scheduleNameError
                color: scheduleSetupRoot.mandatoryColor
                font.pixelSize: 12
                visible: scheduleSetupRoot.scheduleNameError !== "" && popupMode === "edit"
            }

            // Description
            Text {
                text: "Description"
                color: scheduleSetupRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: descriptionTextBoxEdit
                placeholderText: "Description"
                text: ""
                color: scheduleSetupRoot.labelColor
                width: parent.width - 8
                enabled: popupMode === "edit"
                onTextChanged: {
                    if (popupMode === "edit") {
                        checkEditFormValidity()
                    }
                }
            }

            Text {
                text: scheduleSetupRoot.descriptionError
                color: scheduleSetupRoot.mandatoryColor
                font.pixelSize: 12
                visible: scheduleSetupRoot.descriptionError !== "" && popupMode === "edit"
            }

            // Cost Parameters Label
            Text {
                text: "Cost Parameters"
                color: scheduleSetupRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            // Cost Parameters Table
            Rectangle {
                width: parent.width - 6
                height: 140
                color: "#EDF1F4"
                border.color: "#D0D0D0"
                border.width: 1

                Column {
                    FHTable {
                        width: parent.width - 4
                        height: 120
                        leftPadding: 2
                        removeRow: popupMode === "edit"
                        model: costParameterList
                        columns: [
                            { label: "Cost Parameter Name", width: 210, key: "cost_param_name" },
                            { label: "Purchase Material", width: 210, key: "purchase_material" },
                            { label: "Type of BIM Dimension", width: 210, key: "type_of_bim_dimension" }
                        ]

                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < costParameterList.length) {
                                var temp = []
                                for (var i = 0; i < costParameterList.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(costParameterList[i])
                                    }
                                }
                                costParameterList = temp
                                checkEditFormValidity()
                            }
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width - 8
                height: 25
                color: "white"
                border.color: "#D0D0D0"
                border.width: 1
                visible: popupMode === "edit"

                Row {
                    width: parent.width - 4
                    height: 24
                    leftPadding: 2
                    spacing: 6
                    anchors.verticalCenter: parent.verticalCenter

                    CustomTextBox {
                        id: costParamNameTextBoxEdit
                        placeholderText: "Cost Parameter Name"
                        text: ""
                        color: scheduleSetupRoot.labelColor
                        width: 210
                        height: 24
                        topPadding: 1
                    }

                    CustomComboBox {
                        id: purchaseMaterialEdit
                        width: 210
                        height: 24
                        model: purchaseMaterialList
                        currentIndex: 0
                    }

                    CustomComboBox {
                        id: typeOfBimDimensionForCostParamEdit
                        width: 180
                        height: 24
                        model: typeOfBimDimensionList
                        currentIndex: 0
                    }

                    CustomButton {
                        color: "transparent"
                        width: 24
                        height: 24
                        border.color: "#8080808C"
                        btnSource: "qrc:/resources/images/add.svg"
                        btnName: ""
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
                                if (costParamNameTextBoxEdit.text === "") {
                                    scheduleSetupRoot.costParameterError = "Please enter Cost Parameter Name"
                                    return
                                }

                                scheduleSetupRoot.costParameterError = ""
                                var newElements = {
                                    cost_param_name: costParamNameTextBoxEdit.text,
                                    purchase_material: purchaseMaterialEdit.currentText,
                                    type_of_bim_dimension: typeOfBimDimensionForCostParamEdit.currentText
                                }

                                costParameterList = costParameterList.concat(newElements)
                                costParamNameTextBoxEdit.text = ""
                                checkEditFormValidity()
                            }
                        }
                    }
                }
            }
            // Resource Parameters Label
            Text {
                text: "Resource Parameters"
                color: scheduleSetupRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            // Resource Parameters Table
            Rectangle {
                width: parent.width - 6
                height: 140
                color: "#EDF1F4"
                border.color: "#D0D0D0"
                border.width: 1

                Column {
                    FHTable {
                        width: parent.width - 4
                        height: 120
                        leftPadding: 2
                        removeRow: popupMode === "edit"
                        model: resourceParameterList
                        columns: [
                            { label: "Resource Parameter Name", width: 325, key: "resource_param_name" },
                            { label: "Type of BIM Dimension", width: 325, key: "type_of_bim_dimension" }
                        ]

                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < resourceParameterList.length) {
                                var temp = []
                                for (var i = 0; i < resourceParameterList.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(resourceParameterList[i])
                                    }
                                }
                                resourceParameterList = temp
                                checkEditFormValidity()
                            }
                        }
                    }
                }
            }
            // Add Resource Parameter Row (Edit Mode)
            Rectangle {
                width: parent.width - 8
                height: 28
                color: "white"
                border.color: "#D0D0D0"
                border.width: 1
                visible: popupMode === "edit"

                Row {
                    width: parent.width - 4
                    height: 24
                    leftPadding: 2
                    spacing: 20
                    anchors.verticalCenter: parent.verticalCenter

                    CustomTextBox {
                        id: resourceParamNameTextBoxEdit
                        placeholderText: "Resource Parameter Name"
                        text: ""
                        color: scheduleSetupRoot.labelColor
                        width: 280
                        height: 24
                        topPadding: 1
                    }

                    CustomComboBox {
                        id: typeOfBimDimensionForResourceParamEdit
                        model: typeOfBimDimensionList
                        width: 280
                        height: 24
                        currentIndex: 0
                    }

                    CustomButton {
                        color: "transparent"
                        width: 24
                        height: 24
                        border.color: "#8080808C"
                        btnSource: "qrc:/resources/images/add.svg"
                        btnName: ""
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
                                if (resourceParamNameTextBoxEdit.text === "") {
                                    scheduleSetupRoot.resourceParameterError = "Please enter Resource Parameter Name"
                                    return
                                }

                                scheduleSetupRoot.resourceParameterError = ""
                                var newElements = {
                                    resource_param_name: resourceParamNameTextBoxEdit.text,
                                    type_of_bim_dimension: typeOfBimDimensionForResourceParamEdit.currentText
                                }

                                resourceParameterList = resourceParameterList.concat(newElements)
                                resourceParamNameTextBoxEdit.text = ""
                                checkEditFormValidity()
                            }
                        }
                    }
                }
            }
            // Cancellation Reason (for view mode with approval rights)
            Text {
                text: "Cancellation Reason"
                color: scheduleSetupRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 25
                visible: popupMode === "view" && scheduleSetupRoot.canApproveReject
            }

            CustomTextBox {
                id: cancellationReasonTextBox
                placeholderText: "Enter cancellation reason"
                text: ""
                color: scheduleSetupRoot.labelColor
                width: parent.width - 8
                height: 60
                visible: popupMode === "view" && scheduleSetupRoot.canApproveReject
                wrapMode: TextEdit.Wrap
                onTextChanged: {
                    if (text.trim() !== "") {
                        scheduleSetupRoot.cancellationReasonError = ""
                    }
                }
            }

            Text {
                text: scheduleSetupRoot.cancellationReasonError
                color: scheduleSetupRoot.mandatoryColor
                font.pixelSize: 12
                visible: scheduleSetupRoot.cancellationReasonError !== "" && popupMode === "view" && scheduleSetupRoot.canApproveReject
            }

            // Approve / Reject Buttons
            Row {
                width: parent.width
                spacing: 10
                topPadding: 20
                bottomPadding: 20
                visible: popupMode === "view" && scheduleSetupRoot.canApproveReject
                layoutDirection: Qt.RightToLeft

                MouseArea {
                    width: 100
                    height: 34
                    onClicked: {
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
                        color: scheduleSetupRoot.successColor
                    }
                }

                MouseArea {
                    width: 100
                    height: 34
                    onClicked: {
                        let reason = cancellationReasonTextBox.text.trim()
                        let reasonValidation = validator.validateNotEmpty(reason)
                        if (!reasonValidation.isValid) {
                            scheduleSetupRoot.cancellationReasonError = reasonValidation.message
                            return
                        }
                        if (selectedData) {
                            console.log("Cancelling Schedule Setup ID: " + selectedData.id + " Reason: " + reason)
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
                        color: scheduleSetupRoot.dangerColor
                    }
                }
            }
        }
    }

    // ========== HEADER ROW ==========
    Row {
        spacing: 20
        Text {
            text: "Schedule Setup"
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
                onClicked: {
                    newScheduleSetupPopup.open()
                }
            }
        }
    }

    Rectangle {
        width: 100
        height: 40
        color: "#EDF1F4"
    }

    // ========== APPROVAL TYPE SELECTION ==========
    Row {
        spacing: 20
        Text {
            id: approvalTypeLabel
            text: "Choose Approval Type"
            color: scheduleSetupRoot.labelColor
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
                color: scheduleSetupRoot.labelColor
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

    // ========== MAIN TABLE ==========
    FHTable {
        height: 200
        leftPadding: 20
        model: scheduleSetupRoot.scheduleSetupList
        columns: [
            { label: "Id", width: 300, key: "id" },
            { label: "Schedule Setup Name", width: 400, key: "scheduleSetupName" },
            { label: "Description", width: 400, key: "description" },
            { label: "Status", width: 200, key: "displayStatus" }
        ]

        onViewRequested: function (row) {
            popupMode = "view"
            selectedData = row
            fillPopup()
            viewEditPopup.open()
        }

        onEditRequested: function (row) {
            popupMode = "edit"
            selectedData = row
            fillPopup()
            viewEditPopup.open()
        }
    }

    // ========== COMPONENT INITIALIZATION ==========
    Component.onCompleted: {
        showList()
    }

    onVisibleChanged: {
        showList()
    }

    // ========== VALIDATION FUNCTIONS ==========
    function checkCreateFormValidity() {
        let scheduleNameValidation = validator.validateNotEmpty(scheduleNameTextBox.text)
        let descriptionValidation = validator.validateNotEmpty(descriptionTextBox.text)
        let costParamsValidation = costParameterList.length > 0
        let resourceParamsValidation = resourceParameterList.length > 0

        if (!scheduleNameValidation.isValid) {
            scheduleSetupRoot.scheduleNameError = scheduleNameValidation.message
        } else {
            scheduleSetupRoot.scheduleNameError = ""
        }

        if (!descriptionValidation.isValid) {
            scheduleSetupRoot.descriptionError = descriptionValidation.message
        } else {
            scheduleSetupRoot.descriptionError = ""
        }

        if (!costParamsValidation) {
            scheduleSetupRoot.costParameterError = "At least one cost parameter is required"
        } else {
            scheduleSetupRoot.costParameterError = ""
        }

        if (!resourceParamsValidation) {
            scheduleSetupRoot.resourceParameterError = "At least one resource parameter is required"
        } else {
            scheduleSetupRoot.resourceParameterError = ""
        }

        scheduleSetupRoot.isCreateFormValid = scheduleNameValidation.isValid &&
                descriptionValidation.isValid &&
                costParamsValidation &&
                resourceParamsValidation
    }

    function checkEditFormValidity() {
        let scheduleNameValidation = validator.validateNotEmpty(scheduleNameTextBoxEdit.text)
        let descriptionValidation = validator.validateNotEmpty(descriptionTextBoxEdit.text)
        let costParamsValidation = costParameterList.length > 0
        let resourceParamsValidation = resourceParameterList.length > 0

        if (!scheduleNameValidation.isValid) {
            scheduleSetupRoot.scheduleNameError = scheduleNameValidation.message
        } else {
            scheduleSetupRoot.scheduleNameError = ""
        }

        if (!descriptionValidation.isValid) {
            scheduleSetupRoot.descriptionError = descriptionValidation.message
        } else {
            scheduleSetupRoot.descriptionError = ""
        }

        if (!costParamsValidation) {
            scheduleSetupRoot.costParameterError = "At least one cost parameter is required"
        } else {
            scheduleSetupRoot.costParameterError = ""
        }

        if (!resourceParamsValidation) {
            scheduleSetupRoot.resourceParameterError = "At least one resource parameter is required"
        } else {
            scheduleSetupRoot.resourceParameterError = ""
        }

        scheduleSetupRoot.isEditFormValid = scheduleNameValidation.isValid &&
                descriptionValidation.isValid &&
                costParamsValidation &&
                resourceParamsValidation
    }

    function validateForm(isCreate) {
        let scheduleName_ref = isCreate ? scheduleNameTextBox : scheduleNameTextBoxEdit
        let description_ref = isCreate ? descriptionTextBox : descriptionTextBoxEdit

        clearValidationErrors()

        let scheduleNameValidation = validator.validateNotEmpty(scheduleName_ref.text)
        if (!scheduleNameValidation.isValid) {
            scheduleSetupRoot.scheduleNameError = scheduleNameValidation.message
            return false
        }

        let descriptionValidation = validator.validateNotEmpty(description_ref.text)
        if (!descriptionValidation.isValid) {
            scheduleSetupRoot.descriptionError = descriptionValidation.message
            return false
        }

        if (costParameterList.length === 0) {
            scheduleSetupRoot.costParameterError = "At least one cost parameter is required"
            return false
        }

        if (resourceParameterList.length === 0) {
            scheduleSetupRoot.resourceParameterError = "At least one resource parameter is required"
            return false
        }

        return true
    }

    function clearValidationErrors() {
        scheduleSetupRoot.scheduleNameError = ""
        scheduleSetupRoot.descriptionError = ""
        scheduleSetupRoot.costParameterError = ""
        scheduleSetupRoot.resourceParameterError = ""
        scheduleSetupRoot.cancellationReasonError = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            scheduleNameTextBox.text = ""
            descriptionTextBox.text = ""
            costParameterList = []
            resourceParameterList = []
            costParamNameTextBox.text = ""
            resourceParamNameTextBox.text = ""
            scheduleSetupRoot.isCreateFormValid = false
        } else {
            scheduleNameTextBoxEdit.text = ""
            descriptionTextBoxEdit.text = ""
            costParameterList = []
            resourceParameterList = []
            scheduleSetupRoot.isEditFormValid = false
            cancellationReasonTextBox.text = ""
            scheduleSetupRoot.cancellationReasonError = ""
        }
        clearValidationErrors()
    }

    // ========== DATA LOADING FUNCTIONS ==========
    function showList() {
        scheduleSetupRoot.scheduleSetupList = []
        if (scheduleSetupRoot.visible) {
            var arr = scheduleSetupController.getSetupList(isApproved)

            for (var i = 0; i < arr.length; i++) {
                if (arr[i].nextApprovingUser === userController.getCurrentId()) {
                    arr[i].displayStatus = "Pending"
                } else {
                    arr[i].displayStatus = arr[i].approvalStatus
                }
            }
            scheduleSetupRoot.scheduleSetupList = arr
        }
    }

    function fillPopup() {
        if (!selectedData) {
            console.log("selectedData is NULL")
            return
        }

        scheduleNameTextBoxEdit.text = selectedData.scheduleSetupName || ""
        descriptionTextBoxEdit.text = selectedData.description || ""

        var costParams = []
        var resourceParams = []

        if (selectedData.vecCostParamDataDetails) {
            costParams = selectedData.vecCostParamDataDetails
        }

        if (selectedData.vecResourceParamDataDetails) {
            resourceParams = selectedData.vecResourceParamDataDetails
        }

        costParameterList = costParams
        resourceParameterList = resourceParams

        checkEditFormValidity()
    }
}
