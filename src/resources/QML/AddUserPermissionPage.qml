import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import QtQuick.Layouts 1.15
import com.fh.controllers
import QtQuick.Dialogs
import "utils"


Rectangle {
    id: permissionRoot
    anchors.fill: parent
    color: "#EDF1F4"

    signal logOutClicked()
    signal organizationSettingsClicked()
    signal userSettingsClicked()
    property var permissionList: []
    property var currentPermissionData: null
    property int currentUserId: 0
    property int currentProjectId: 0
    property int currentSelectedId: 0
    property int currentSelectedPermissionId: 0
    property variant allProjects: []

    // Properties - Form validity
    property bool isCreateFormValid: false
    property bool isEditFormValid: false
    property string popupMode: "view"

    ValidationHelper {
        id: validator
    }

    UserController {
        id: userController
        onUserListReceived: function(list) {
            permissionRoot.allUsers = list;
        }
        onUserListFailed: function(error) {
            console.error("Failed to fetch users: " + error);
        }
    }

    ProjectController {
        id: projectController
        onProjectListReceived: (list) => {
                                   permissionRoot.allProjects = list
                               }
        onProjectListFailed: (error) =>{
                                 console.error("Project Error: " + error);
                             }
    }

    UserController {
        id: approverUserController
    }

    PermissionController {
        id: permissionController
    }

    // --- Data Properties ---
    property var allUsers: []
    property var approvalUserData: []
    property string lineItemsError: ""

    // Validation error strings - Create
    property string createUserError: ""
    property string createEntityError: ""
    property string createProjectError: ""
    property string createApproverError: ""
    property string createAccessError: ""

    // Validation error strings - Edit
    property string editUserError: ""
    property string editEntityError: ""
    property string editProjectError: ""
    property string editApproverError: ""
    property string editAccessError: ""

    // Color Variables
    property string mandatoryColor: "#D13438"
    property string labelColor: "#323130"
    property string errorColor: "#D13438"

    MessageDialog {
        id: deleteConfirmationDialog
        title: "Confirm Delete"
        text: "Are you sure you want to delete this permission?"
        buttons: MessageDialog.Yes | MessageDialog.No
       // icon: MessageDialog.Warning

        property int idToDelete: 0

        onButtonClicked: function (button, role) {
            if (role === MessageDialog.YesRole) {
                console.log("Deleting ID:", idToDelete)
                permissionController.deletePermission(idToDelete)
            }
        }
    }

    onVisibleChanged: {
        if (visible) {
            userController.getUserList();
            projectController.getAllProjectList(false);
            showPermissionList(currentUserId, currentProjectId);
        }
    }

    onCurrentUserIdChanged: {
        if (visible) {
            showPermissionList(currentUserId, currentProjectId)
        }
    }

    /* ===========================================
       ADD PERMISSION POPUP
    ==================================================== */
    FHPopup {
        id: addPermissionPopup
        popupWidth: 500
        popupHeight: 660
        title: "New Permission"
        parent: Overlay.overlay
        buttonEnabled: true
        onAcceptCallback: function () {
            if (!validateCreateForm()) return
            var appIds = []
            for (var i = 0; i < approvalUserData.length; i++) {
                appIds.push(approvalUserData[i].id)
            }
            var selectedEntity = entityCombo.currentText
            var selectedProjectId = projectCombo.model[projectCombo.currentIndex].id
            permissionController.createPermission(
                        currentUserId,
                        selectedEntity,
                        selectedProjectId,
                        chkCreate.checked,
                        "self",
                        chkUpdate.checked,
                        appIds
                        )

            resetCreateForm()
            close()
        }
        onCancelCallback: function () {
            resetCreateForm()
            close()
        }

        Column {
            width: parent.width
            spacing: 4

            /* ---- Entity ---- */
            Text {
                text: "Entity <span style='color: #FF3B30;'>*</span>"
                font.weight: 700; font.pixelSize: 14; textFormat: Text.RichText
                topPadding: 6
            }
            CustomComboBox {
                id: entityCombo
                width: parent.width
                model: ["Vendor", "Project", "Asset", "Inventory","Task","BudgetHead","ScheduleSetup","WorkOrder"]
                displayText: currentIndex === -1 ? "Choose Entity" : currentText
                onCurrentIndexChanged: permissionRoot.createEntityError = ""
            }
            Text {
                text: permissionRoot.createEntityError
                color: permissionRoot.errorColor
                font.pixelSize: 11
                visible: permissionRoot.createEntityError !== ""
            }

            /* ---- Project ---- */
            Text {
                text: "Project <span style='color: #FF3B30;'>*</span>"
                font.weight: 700; font.pixelSize: 14; textFormat: Text.RichText
                topPadding: 6
            }
            CustomComboBox {
                id: projectCombo
                width: parent.width
                model: permissionRoot.allProjects
                textRole: "projectName"
                currentIndex: model.length > 0 ? 0 : -1

                onCurrentIndexChanged: {
                    if (currentIndex !== -1) {
                        var selectedId = model[currentIndex].id;
                        console.log("Project ID:", selectedId);
                    }
                }
            }
            Text {
                text: permissionRoot.createProjectError
                color: permissionRoot.errorColor
                font.pixelSize: 11
                visible: permissionRoot.createProjectError !== ""
            }

            /* ---- Approvers ---- */
            Text {
                text: "Approvers List <span style='color: #FF3B30;'>*</span>"
                color: permissionRoot.labelColor
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                textFormat: Text.RichText
                topPadding: 10
            }

            Rectangle {
                width: parent.width
                height: 200
                color: "#EDF1F4"
                border.color: permissionRoot.createApproverError !== "" ? permissionRoot.errorColor : "#D0D0D0"
                border.width: 1

                Column {
                    width: parent.width
                    height: parent.height

                    FHTable {
                        id: popupTable
                        width: parent.width
                        height: 130
                        leftPadding: 2
                        removeRow: true
                        model: approvalUserData
                        columns: [
                            { "label": "Approval User", "width": 450, "key": "user_name" }
                        ]
                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < approvalUserData.length) {
                                var temp = []
                                for (var i = 0; i < approvalUserData.length; i++) {
                                    if (i !== removedIndex) temp.push(approvalUserData[i])
                                }
                                approvalUserData = temp
                                if (approvalUserData.length > 0)
                                    permissionRoot.createApproverError = ""
                            }
                        }
                    }

                    Rectangle {
                        width: parent.width
                        height: 70
                        color: "white"
                        border.color: "#D0D0D0"
                        border.width: 1

                        Column {
                            width: parent.width
                            height: parent.height
                            spacing: 4
                            topPadding: 4

                            Row {
                                width: parent.width - 4
                                height: 28
                                leftPadding: 2
                                spacing: 6

                                CustomComboBox {
                                    id: approvalUserCombo
                                    width: 400
                                    height: 24
                                    model: permissionRoot.allUsers
                                    textRole: "username"
                                    currentIndex: model && model.length > 0 ? 0 : -1
                                    onCurrentIndexChanged: {
                                        if (currentIndex !== -1 && model[currentIndex]) {
                                            console.log("Selected User ID:", model[currentIndex].id)
                                        }
                                    }
                                }

                                CustomButton {
                                    color: "transparent"
                                    width: 24
                                    height: 24
                                    border.color: "#8080808C"
                                    btnSource: "qrc:/resources/images/add.svg"
                                    btnName: ""

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        hoverEnabled: true
                                        onEntered: parent.color = "#f2f2f2"
                                        onExited:  parent.color = "transparent"

                                        onClicked: {
                                            if (approvalUserCombo.currentIndex < 0) {
                                                permissionRoot.createApproverError = "Please select an approver to add"
                                                return
                                            }

                                            // Duplicate check
                                            for (var i = 0; i < approvalUserData.length; i++) {
                                                if (approvalUserData[i].user_name === approvalUserCombo.currentText) {
                                                    permissionRoot.createApproverError = "Approver already added"
                                                    return
                                                }
                                            }

                                            var newUser = {
                                                "id": approvalUserCombo.model[approvalUserCombo.currentIndex].id,
                                                "user_name": approvalUserCombo.currentText
                                            }
                                            approvalUserData = approvalUserData.concat(newUser)
                                            permissionRoot.createApproverError = ""
                                        }
                                    }
                                }
                            }

                            Text {
                                text: permissionRoot.createApproverError
                                color: permissionRoot.errorColor
                                font.pixelSize: 11
                                visible: permissionRoot.createApproverError !== ""
                                leftPadding: 2
                            }
                        }
                    }
                }
            }

            /* ---- Access Level ---- */
            Text {
                text: "Access Level <span style='color: #FF3B30;'>*</span>"
                font.weight: 700; font.pixelSize: 14; textFormat: Text.RichText
                topPadding: 10
            }

            Rectangle {
                width: parent.width
                height: 120
                color: "#F8F9FA"
                border.color: permissionRoot.createAccessError !== "" ? permissionRoot.errorColor : "#E0E0E0"
                radius: 4

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 10

                    CheckBox {
                        id: chkCreate
                        text: "Create (Add New Records)"
                        font.pixelSize: 13
                        onCheckedChanged: permissionRoot.createAccessError = ""
                    }
                    CheckBox {
                        id: chkRead
                        text: "Read (View Details)"
                        font.pixelSize: 13
                        onCheckedChanged: permissionRoot.createAccessError = ""
                    }
                    CheckBox {
                        id: chkUpdate
                        text: "Update (Edit Existing Records)"
                        font.pixelSize: 13
                        onCheckedChanged: permissionRoot.createAccessError = ""
                    }
                }
            }
            Text {
                text: permissionRoot.createAccessError
                color: permissionRoot.errorColor
                font.pixelSize: 11
                visible: permissionRoot.createAccessError !== ""
            }
        }
    }


    /* ====================================================
        VIEW / EDIT PERMISSION POPUP
    ==================================================== */
    FHPopup {
        id: viewEditPermissionPopup
        popupWidth: 500
        popupHeight: 660
        title: popupMode === "view" ? "View Permission" : "Edit Permission"
        parent: Overlay.overlay

        showAcceptButton: popupMode === "edit"
        buttonName: "Update"
        buttonSource: "qrc:/resources/images/editWhite_icon.png"
        buttonEnabled: true

        onAcceptCallback: function () {
            if (popupMode !== "edit") return
            if (!validateEditForm()) return

            var appIds = []
            for (var i = 0; i < approvalUserData.length; i++) {
                appIds.push(approvalUserData[i].id)
            }

            permissionController.updateUserPermission(
                        currentSelectedPermissionId,
                           currentUserId,
                           editProjectCombo.model[editProjectCombo.currentIndex].id,
                           editEntityCombo.currentText,
                           appIds,
                           chkEditCreate.checked,
                           "self",
                           chkEditUpdate.checked
                        )
            close()
        }

        onCancelCallback: function () {
            resetEditErrors()
            close()
        }

        Column {
            width: parent.width
            spacing: 4

            /* ---- Entity (Read-only in View) ---- */
            Text {
                text: "Entity <span style='color: #FF3B30;'>*</span>"
                font.weight: 700; font.pixelSize: 14; textFormat: Text.RichText
                topPadding: 6
            }
            CustomComboBox {
                id: editEntityCombo
                width: parent.width
                model: ["Vendor", "Project", "Asset", "Inventory", "Task", "BudgetHead", "ScheduleSetup", "WorkOrder"]
                enabled: popupMode === "edit"
                opacity: enabled ? 1.0 : 0.8
            }

            /* ---- Project (Read-only in View) ---- */
            Text {
                text: "Project <span style='color: #FF3B30;'>*</span>"
                font.weight: 700; font.pixelSize: 14; textFormat: Text.RichText
                topPadding: 6
            }
            CustomComboBox {
                id: editProjectCombo
                width: parent.width
                model: permissionRoot.allProjects
                textRole: "projectName"
                enabled: popupMode === "edit"
                opacity: enabled ? 1.0 : 0.8
            }

            /* ---- Approvers List ---- */
            Text {
                text: "Approvers List <span style='color: #FF3B30;'>*</span>"
                font.weight: 700; font.pixelSize: 14; textFormat: Text.RichText
                topPadding: 10
            }

            Rectangle {
                width: parent.width
                height: popupMode === "edit" ? 220 : 200
                color: "#EDF1F4"
                border.color: "#D0D0D0"
                radius: 4
               // clip: true

                Column {
                    width: parent.width
                    height: parent.height

                    FHTable {
                        id: editPopupTable
                        width: parent.width
                        height: 200
                        removeRow: popupMode === "edit"
                        model: approvalUserData
                        columns: [
                            {
                                "label": "Approval User",
                                "width": 500,
                                "key": "name"
                            }

                        ]
                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && popupMode === "edit") {
                                var temp = []
                                for (var i = 0; i < approvalUserData.length; i++) {
                                    if (i !== removedIndex) temp.push(approvalUserData[i])
                                }
                                approvalUserData = temp
                            }
                        }
                    }

                    // Add Row
                    Rectangle {
                        width: parent.width
                        visible: popupMode === "edit"
                        height: visible ? 70 : 0
                        color: "white"
                        border.color: "#D0D0D0"
                        border.width: 1

                        Column {
                            width: parent.width
                            height: parent.height
                            spacing: 4
                            topPadding: 4

                            Row {
                                width: parent.width - 4
                                height: 28
                                leftPadding: 2
                                spacing: 6

                                CustomComboBox {
                                    id: editApprovalUserCombo
                                    width: 400
                                    height: 24
                                    model: permissionRoot.allUsers
                                    textRole: "username"
                                    currentIndex: model && model.length > 0 ? 0 : -1
                                    onCurrentIndexChanged: {
                                        if (currentIndex !== -1 && model[currentIndex]) {
                                            console.log("Selected User ID:", model[currentIndex].id)
                                        }
                                    }
                                }

                                CustomButton {
                                    id: addBtn
                                    color: "transparent"
                                    width: 24
                                    height: 24
                                    border.color: "#8080808C"
                                    btnSource: "qrc:/resources/images/add.svg"
                                    btnName: ""

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        hoverEnabled: true
                                        onEntered: parent.color = "#f2f2f2"
                                        onExited:  parent.color = "transparent"

                                        onClicked: {
                                            if (editApprovalUserCombo.currentIndex < 0) return

                                            for (var i = 0; i < approvalUserData.length; i++) {
                                                if (approvalUserData[i].name === editApprovalUserCombo.currentText) {
                                                    permissionRoot.editApproverError = "Approver already added"
                                                    return
                                                }
                                            }

                                            var newUser = {
                                                "id":   editApprovalUserCombo.model[editApprovalUserCombo.currentIndex].id,
                                                "name": editApprovalUserCombo.currentText
                                            }
                                            approvalUserData = approvalUserData.concat(newUser)
                                            permissionRoot.editApproverError = ""
                                        }
                                    }
                                }
                            }

                            Text {
                                text: permissionRoot.createApproverError
                                color: permissionRoot.errorColor
                                font.pixelSize: 11
                                visible: permissionRoot.createApproverError !== ""
                                leftPadding: 2
                            }
                        }
                    }
                    }
            }

            /* ---- Access Levels ---- */
            Text {
                text: "Access Level <span style='color: #FF3B30;'>*</span>"
                font.weight: 700; font.pixelSize: 14; textFormat: Text.RichText
                topPadding: 20
            }

            Rectangle {
                width: parent.width
                height: 120
                color: "#F8F9FA"
                radius: 4
                border.color: "#E0E0E0"

                ColumnLayout {
                    anchors.fill: parent; anchors.margins: 10
                    spacing: 8
                    CheckBox {
                        id: chkEditCreate
                        text: "Create (Add New Records)"
                        enabled: popupMode === "edit"
                    }
                    CheckBox {
                        id: chkEditRead
                        text: "Read (View Details)"
                        enabled: popupMode === "edit"
                    }
                    CheckBox {
                        id: chkEditUpdate
                        text: "Update (Edit Existing Records)"
                        enabled: popupMode === "edit"
                    }
                }
            }
        }
    }

    /* ==========================================================
       MAIN LAYOUT
    ==================================================== */
    Column {
        anchors.fill: parent
        padding: 10

        /* ── Tool Bar ── */
        Rectangle {
            width: parent.width - 20
            height: 75
            radius: 8
            border.width: 1
            border.color: "#7676801F"
            color: "white"
            anchors.horizontalCenter: parent.horizontalCenter

            Row {
                spacing: 7
                padding: 10

                Rectangle { width: 1; height: 60; color: "transparent" }

                CustomButton {
                    color: "transparent"
                    width: 186; height: 38; radius: 4
                    border.color: "#007AFF"
                    btnSource: "qrc:/resources/images/add.svg"
                    btnName: "Add New Permission"
                    btnNameColor: "black"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onEntered: parent.color = "#f0f0f5"
                        onExited:  parent.color = "transparent"
                        onClicked: {
                            resetCreateForm()
                            addPermissionPopup.open()
                        }
                    }
                }

                CustomButton {
                    color: "transparent"
                    width: 186; height: 38; radius: 4
                    border.color: "#007AFF"
                    btnSource: "qrc:/resources/images/settings.svg"
                    btnName: "Organization Settings"
                    btnNameColor: "black"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onEntered: parent.color = "#f2f2f2"
                        onExited:  parent.color = "transparent"
                        onClicked: { /* organizationSettingsClicked() */ }
                    }
                }

                CustomButton {
                    color: "transparent"
                    width: 186; height: 38; radius: 4
                    border.color: "#007AFF"
                    btnSource: "qrc:/resources/images/settings.svg"
                    btnName: "User Permission List"
                    btnNameColor: "#007AFF"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onEntered: parent.color = "#f2f2f2"
                        onExited:  parent.color = "transparent"
                        onClicked: {}
                    }
                }
            }

            CustomButton {
                color: "#007AFF"
                width: 89; height: 38; radius: 4
                btnSource: "qrc:/resources/images/logout.svg"
                btnName: "Logout"
                btnNameColor: "white"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 13

                MouseArea {
                    anchors.fill: parent
                    onClicked: logOutClicked()
                }
            }
        }

        /* ── Content ── */
        Column {
            id: users
            height: parent.height
            width: parent.width
            padding: 10

            Row {
                spacing: 7
                padding: 50

                Item {
                    width: 30; height: 38

                    Image {
                        id: backbuttonAdmin
                        source: "qrc:/resources/images/backArrow.svg"
                        anchors.fill: parent
                        anchors.verticalCenter: parent.verticalCenter
                        fillMode: Image.PreserveAspectFit
                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true
                            onClicked: userPermissionPage.visible = true
                        }
                    }

                    Rectangle { id: blankAdmin; width: 10; height: 38; color: "transparent"; anchors.left: backbuttonAdmin.right }

                    Text {
                        text: "Permission"
                        color: "Black"
                        font.weight: 700
                        font.pixelSize: 40
                        font.family: "Segoe UI"
                        anchors.left: blankAdmin.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }

            Rectangle { width: 100; height: 20; color: "#EDF1F4" }

            Text {
                id: emptyMessage
                text: "Permission Not Found"
                font.pixelSize: 20
                color: "#888888"
                anchors.horizontalCenter: parent.horizontalCenter
                visible: permissionRoot.permissionList.length === 0
            }

            GridLayout {
                id: userGrid
                columns: 3
                rowSpacing: 20
                columnSpacing: 20
                width: parent.width - 40
                anchors.horizontalCenter: parent.horizontalCenter
                visible: permissionRoot.permissionList.length > 0

                Repeater {
                    model: permissionRoot.permissionList

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 164
                        radius: 8
                        color: "white"
                        border.width: 1
                        border.color: "#E0E0E0"

                        Column {
                            anchors.fill: parent
                            anchors.margins: 15
                            spacing: 12

                            RowLayout {
                                width: parent.width
                                spacing: 8

                                Image {
                                    source: "qrc:/resources/images/profile.svg"
                                    Layout.preferredWidth: 24
                                    Layout.preferredHeight: 24
                                    fillMode: Image.PreserveAspectFit
                                }
                                Text {
                                    text: modelData.entity || "Unknown"
                                    font.pixelSize: 18
                                    font.weight: Font.DemiBold
                                    color: "#323130"
                                    Layout.alignment: Qt.AlignVCenter
                                }
                                Item { Layout.fillWidth: true }

                                /* ── View button ── */
                                Rectangle {
                                    Layout.preferredWidth: 60
                                    Layout.preferredHeight: 26
                                    radius: 4
                                    color: "#F0F7FF"
                                    border.color: "#007AFF"
                                    border.width: 1
                                    Text {
                                        anchors.centerIn: parent
                                        text: "View"
                                        color: "#007AFF"
                                        font.bold: true
                                        font.pixelSize: 12
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            permissionRoot.currentSelectedId = modelData.id || 0
                                            permissionRoot.popupMode = "view"
                                            resetEditErrors()
                                            populateEditFields(modelData)
                                            viewEditPermissionPopup.open()
                                        }
                                    }
                                }

                                /* ── Edit button ── */
                                Rectangle {
                                    Layout.preferredWidth: 60
                                    Layout.preferredHeight: 26
                                    radius: 4
                                    color: "#F0F7FF"
                                    border.color: "#007AFF"
                                    border.width: 1
                                    Text {
                                        anchors.centerIn: parent
                                        text: "Edit"
                                        color: "#007AFF"
                                        font.bold: true
                                        font.pixelSize: 12
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            permissionRoot.currentSelectedId = modelData.id || 0
                                            permissionRoot.popupMode = "edit"
                                            resetEditErrors()
                                            populateEditFields(modelData)
                                            viewEditPermissionPopup.open()
                                        }
                                    }
                                }

                                /* ── Delete button ── */
                                Rectangle {
                                    Layout.preferredWidth: 60
                                    Layout.preferredHeight: 26
                                    radius: 4
                                    color: "#FFF5F5"
                                    border.color: "#FF3B30"
                                    border.width: 1

                                    Text {
                                        anchors.centerIn: parent
                                        text: "Delete"
                                        color: "#FF3B30"
                                        font.bold: true
                                        font.pixelSize: 12
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor

                                        onEntered: parent.color = "#FFE5E5"
                                        onExited: parent.color = "#FFF5F5"

                                        onClicked: {
                                            var deleteId = modelData.id || 0
                                            deleteConfirmationDialog.idToDelete = modelData.id || 0
                                            deleteConfirmationDialog.open()
                                        }
                                    }
                                }
                            }

                            /* ── Access badges ── */
                            Row {
                                spacing: 6
                                Rectangle {
                                    visible: modelData.canCreate
                                    width: 60; height: 22; radius: 4; color: "#E6F4EA"
                                    Text { anchors.centerIn: parent; text: "Create"; font.pixelSize: 11; color: "#2E7D32" }
                                }
                                Rectangle {
                                    visible: modelData.canUpdate
                                    width: 60; height: 22; radius: 4; color: "#FFF3E0"
                                    Text { anchors.centerIn: parent; text: "Update"; font.pixelSize: 11; color: "#E65100" }
                                }
                                Rectangle {
                                    visible: modelData.readType !== ""
                                    width: 60; height: 22; radius: 4; color: "#E3F2FD"
                                    Text { anchors.centerIn: parent; text: "Read"; font.pixelSize: 11; color: "#1565C0" }
                                }
                            }

                            Text {
                                width: parent.width
                                text: "User ID: " + (modelData.user || "-") + "  |  Project: " + (modelData.project || "-")
                                wrapMode: Text.WordWrap; font.pixelSize: 13; color: "#666666"
                            }
                            Text {
                                width: parent.width
                                text: "Created: " + (modelData.createdOn ? modelData.createdOn.substring(0, 10) : "-")
                                font.pixelSize: 12; color: "#999999"
                            }
                        }
                    }
                }
            }

            Rectangle { width: 100; height: 20; color: "#EDF1F4" }
        }
    }


    /* ====================================================
       CONNECTIONS & FUNCTIONS
    ==================================================== */
    Connections {
        target: permissionController
        function onPermissionListReceived(data) {
            permissionRoot.permissionList = data
        }
        function onPermissionOperationSuccess(message) {
               console.log(message)
               viewEditPermissionPopup.close()
               showPermissionList(currentUserId, currentProjectId)
        }
    }

    function showPermissionList(targetUser, targetProject) {
        if (!targetUser) {
            console.log("UserId missing")
            return
        }
        permissionRoot.permissionList = []
        var pId = targetProject ? targetProject : 0
        permissionController.getUserPermissionList(targetUser, pId)
    }

    function populateEditFields(data) {
        currentSelectedPermissionId = data.id || 0


        approvalUserData = []
        var rawApprovers = data.approvalData || []

       // console.log("Full rowData:", JSON.stringify(data))

        var formattedApprovers = []
        for (var j = 0; j < rawApprovers.length; j++) {
          //  console.log("rawApprovers",JSON.stringify(rawApprovers[j]))
            formattedApprovers.push({
                "id":        rawApprovers[j].id,
                "name": rawApprovers[j].username || rawApprovers[j].name || ""
            })
        }
        approvalUserData = formattedApprovers


        var entityList = ["Vendor", "Project", "Asset", "Inventory", "Task",
                          "BudgetHead", "ScheduleSetup", "WorkOrder"]
        var eIdx = entityList.indexOf(data.entity)
        editEntityCombo.currentIndex = (eIdx !== -1) ? eIdx : -1


        editProjectCombo.currentIndex = -1
        var projId = data.project || data.projectId || -1
        for (var i = 0; i < permissionRoot.allProjects.length; i++) {
            if (permissionRoot.allProjects[i].id === projId) {
                editProjectCombo.currentIndex = i
                break
            }
        }

        chkEditCreate.checked = data.canCreate  || false
        chkEditRead.checked   = (data.readType !== "" && data.readType !== undefined && data.readType !== null)
        chkEditUpdate.checked = data.canUpdate  || false

    }

    function populateFields(data) {
        populateEditFields(data)
    }


    // ====================================================
    //  Helper: validate create form, return bool
    // ====================================================
    function validateCreateForm() {
        var valid = true

        if (entityCombo.currentIndex === -1) {
            permissionRoot.createEntityError = "Please select an entity"
            valid = false
        } else {
            permissionRoot.createEntityError = ""
        }

        if (projectCombo.currentIndex === -1) {
            permissionRoot.createProjectError = "Please select a project"
            valid = false
        } else {
            permissionRoot.createProjectError = ""
        }

        if (approvalUserData.length === 0) {
            permissionRoot.createApproverError = "Please add at least one approver"
            valid = false
        } else {
            permissionRoot.createApproverError = ""
        }

        if (!chkCreate.checked && !chkRead.checked && !chkUpdate.checked) {
            permissionRoot.createAccessError = "Please select at least one access level"
            valid = false
        } else {
            permissionRoot.createAccessError = ""
        }

        return valid
    }

    // ====================================================
    //  Helper: validate edit form, return bool
    // ====================================================
    function validateEditForm() {
        var valid = true

        if (editEntityCombo.currentIndex === -1) {
            permissionRoot.editEntityError = "Please select an entity"
            valid = false
        } else {
            permissionRoot.editEntityError = ""
        }

        if (editProjectCombo.currentIndex === -1) {
            permissionRoot.editProjectError = "Please select a project"
            valid = false
        } else {
            permissionRoot.editProjectError = ""
        }

        if (approvalUserData.length === 0) {
            permissionRoot.editApproverError = "Please add at least one approver"
            valid = false
        } else {
            permissionRoot.editApproverError = ""
        }

        if (!chkEditCreate.checked && !chkEditRead.checked && !chkEditUpdate.checked) {
            permissionRoot.editAccessError = "Please select at least one access level"
            valid = false
        } else {
            permissionRoot.editAccessError = ""
        }

        return valid
    }

    // ====================================================
    //  Helper: clear create form errors & fields
    // ====================================================
    function resetCreateForm() {
        permissionRoot.createEntityError  = ""
        permissionRoot.createProjectError = ""
        permissionRoot.createApproverError= ""
        permissionRoot.createAccessError  = ""
        permissionRoot.lineItemsError     = ""
        entityCombo.currentIndex  = -1
        projectCombo.currentIndex = -1
        chkCreate.checked = false
        chkRead.checked   = false
        chkUpdate.checked = false
        approvalUserData  = []
    }

    // ====================================================
    //  Helper: clear edit form errors
    // ====================================================
    function resetEditErrors() {
        permissionRoot.editEntityError  = ""
        permissionRoot.editProjectError = ""
        permissionRoot.editApproverError= ""
        permissionRoot.editAccessError  = ""
    }


}
