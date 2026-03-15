import QtQuick 2.15
import QtQuick.Controls
import com.fh.controllers;
import QtQuick.Window 2.15
import QtQuick.Dialogs
import "utils"


Rectangle {
    id: root
    anchors.fill: parent
    color: "#EDF1F4"

    signal logOutClicked()
    signal newAddUserClicked()

    // ========== VALIDATION PROPERTIES ==========
    property string firstNameError: ""
    property string lastNameError: ""
    property string userNameError: ""
    property string passwordError: ""
    property string emailError: ""
    property string mobileError: ""

    property bool isCreateFormValid: false
    property bool isEditFormValid: false

    property string mandatoryColor: "#D13438"
    property string labelColor: "#323130"

    property var selectedUserData: null
    property string popupMode: "view"

    property var userlist:[]

    // ========== VALIDATION HELPER ==========
    ValidationHelper {
        id: validator
    }


    // ========== MAIN UI ==========
    Column {
        anchors.fill: parent
        padding: 2

        // -------- Tool Bar --------
        Rectangle {
            id: toolbar
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

                CustomButton {
                    color: "transparent"
                    width: 129
                    height: 38
                    radius: 4
                    border.color: "#8080808C"
                    btnSource: "qrc:/resources/images/add.svg"
                    btnName: "Add User"
                    btnNameColor: "blue"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        hoverEnabled: true
                        onEntered: parent.color = "#f2f2f2"
                        onExited:  parent.color = "transparent"
                        onClicked: newUserPopup.open()
                    }
                }

                Rectangle { width: 1; height: 60; color: "transparent" }
            }

            CustomButton {
                color: "#007AFF"
                width: 89
                height: 38
                radius: 4
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

        Column {
            id: users
            height: parent.height
            width: parent.width
            padding: 10

            Row {
                spacing: 7
                topPadding: 50
                leftPadding: 50
                rightPadding: 50
                bottomPadding: 25

                Item {
                    width: 30
                    height: 38

                    Image {
                        id: backbuttonUsers
                        source: "qrc:/resources/images/backArrow.svg"
                        anchors.fill: parent
                        anchors.verticalCenter: parent.verticalCenter
                        fillMode: Image.PreserveAspectFit

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true
                            onClicked: {
                                addUserPage.visible = false
                                organizationSettingsPage.visible = true
                            }
                        }
                    }

                    Rectangle {
                        id: blankUsers
                        width: 10
                        height: 38
                        color: "transparent"
                        anchors.left: backbuttonUsers.right
                    }

                    Text {
                        text: "Users"
                        color: "Black"
                        font.weight: 700
                        font.pixelSize: 40
                        font.family: "Segoe UI"
                        anchors.left: blankUsers.right
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
            }


            Column {
                id: userRoot
                width: parent.width
                padding: 10

                UserController { id: userController }

                MessageDialog {
                    id: alertMessageDialog
                    title: "Alert!"
                    text: "Input text should not be blank."
                    buttons: MessageDialog.Ok
                    modality: Qt.ApplicationModal
                    onAccepted: {}
                }

                // ========== ADD USER POPUP ==========
                FHPopup {
                    id: newUserPopup
                    popupWidth: 500
                    popupHeight: 500
                    title: "New User"
                    parent: Overlay.overlay
                    buttonEnabled: root.isCreateFormValid

                    onAcceptCallback: function () {
                        if (validateForm(true)) {
                            userController.createUser(
                                userNameTextBox.text,
                                userPasswordTextBox.text,
                                userEmailTextBox.text,
                                userFirstNameTextBox.text,
                                userLastNameTextBox.text,
                                userMobileTextBox.text
                            )
                        }
                     showUserList()
                    }

                    onCancelCallback: function () { resetForm(true) }
                    onOpened: { resetForm(true) }

                    content: Column {
                        width: parent.width
                        spacing: 10
                        padding: 10

                        Text {
                            text: "First Name <span style='color: " + root.mandatoryColor + ";'>*</span>"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                            textFormat: Text.RichText
                        }
                        CustomTextBox {
                            id: userFirstNameTextBox
                            placeholderText: "First Name"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            onTextChanged: checkCreateFormValidity()
                        }
                        Text { text: root.firstNameError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.firstNameError !== "" }

                        Text {
                            text: "Last Name <span style='color: " + root.mandatoryColor + ";'>*</span>"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                            textFormat: Text.RichText; topPadding: 10
                        }
                        CustomTextBox {
                            id: userLastNameTextBox
                            placeholderText: "Last Name"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            onTextChanged: checkCreateFormValidity()
                        }
                        Text { text: root.lastNameError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.lastNameError !== "" }

                        Text {
                            text: "User Name <span style='color: " + root.mandatoryColor + ";'>*</span>"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                            textFormat: Text.RichText; topPadding: 10
                        }
                        CustomTextBox {
                            id: userNameTextBox
                            placeholderText: "User Name"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            onTextChanged: checkCreateFormValidity()
                        }
                        Text { text: root.userNameError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.userNameError !== "" }

                        Text {
                            text: "Password <span style='color: " + root.mandatoryColor + ";'>*</span>"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                            textFormat: Text.RichText; topPadding: 10
                        }
                        CustomTextBox {
                            id: userPasswordTextBox
                            placeholderText: "Password"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            echoMode: TextInput.Password
                            onTextChanged: checkCreateFormValidity()
                        }
                        Text { text: root.passwordError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.passwordError !== "" }

                        Text {
                            text: "Email <span style='color: " + root.mandatoryColor + ";'>*</span>"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                            textFormat: Text.RichText; topPadding: 10
                        }
                        CustomTextBox {
                            id: userEmailTextBox
                            placeholderText: "Email"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            onTextChanged: checkCreateFormValidity()
                        }
                        Text { text: root.emailError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.emailError !== "" }

                        Text {
                            text: "Mobile <span style='color: " + root.mandatoryColor + ";'>*</span>"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                            textFormat: Text.RichText; topPadding: 10
                        }
                        CustomTextBox {
                            id: userMobileTextBox
                            placeholderText: "Mobile Number"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            onTextChanged: checkCreateFormValidity()
                        }
                        Text { text: root.mobileError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.mobileError !== "" }
                    }
                }

                // ========== VIEW / EDIT USER POPUP ==========
                FHPopup {
                    id: viewEditPopup
                    popupWidth: 500
                    popupHeight: 500
                    title: root.popupMode === "view" ? "View User" : "Edit User"
                    parent: Overlay.overlay
                    showAcceptButton: root.popupMode === "edit"
                    buttonName: root.popupMode === "edit" ? "Update" : ""
                    buttonEnabled: root.popupMode === "edit" ? root.isEditFormValid : true

                    onAcceptCallback: function () {
                        if (root.popupMode === "edit" && root.selectedUserData && validateForm(false)) {
                            userController.updateUser(
                                root.selectedUserData.id,
                                userNameTextBoxEdit.text,
                                userFirstNameTextBoxEdit.text,
                                userLastNameTextBoxEdit.text,
                                userEmailTextBoxEdit.text,
                                userMobileTextBoxEdit.text,
                                userPasswordTextBoxEdit.text
                            )
                        }
                    }

                    onCancelCallback: function () { resetForm(false) }

                    onOpened: {
                        console.log("Popup opened with mode:", root.popupMode)
                        if (root.popupMode === "view" && root.selectedUserData) {
                            fillEditPopup()
                        }
                    }

                    onClosed: { clearValidationErrors() }

                    content: Column {
                        width: parent.width
                        spacing: 10
                        padding: 10

                        Text {
                            text: "First Name"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                        }
                        CustomTextBox {
                            id: userFirstNameTextBoxEdit
                            placeholderText: "First Name"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            enabled: root.popupMode === "edit"
                            onTextChanged: { if (root.popupMode === "edit") checkEditFormValidity() }
                        }
                        Text { text: root.firstNameError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.firstNameError !== "" && root.popupMode === "edit" }

                        Text {
                            text: "Last Name"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox {
                            id: userLastNameTextBoxEdit
                            placeholderText: "Last Name"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            enabled: root.popupMode === "edit"
                            onTextChanged: { if (root.popupMode === "edit") checkEditFormValidity() }
                        }
                        Text { text: root.lastNameError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.lastNameError !== "" && root.popupMode === "edit" }

                        Text {
                            text: "User Name"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox {
                            id: userNameTextBoxEdit
                            placeholderText: "User Name"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            enabled: root.popupMode === "edit"
                            onTextChanged: { if (root.popupMode === "edit") checkEditFormValidity() }
                        }
                        Text { text: root.userNameError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.userNameError !== "" && root.popupMode === "edit" }

                        Text {
                            text: "Password"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox {
                            id: userPasswordTextBoxEdit
                            placeholderText: "Password"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            echoMode: TextInput.Password
                            enabled: root.popupMode === "edit"
                            onTextChanged: { if (root.popupMode === "edit") checkEditFormValidity() }
                        }
                        Text { text: root.passwordError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.passwordError !== "" && root.popupMode === "edit" }

                        Text {
                            text: "Email"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox {
                            id: userEmailTextBoxEdit
                            placeholderText: "Email"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            enabled: root.popupMode === "edit"
                            onTextChanged: { if (root.popupMode === "edit") checkEditFormValidity() }
                        }
                        Text { text: root.emailError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.emailError !== "" && root.popupMode === "edit" }

                        Text {
                            text: "Mobile"
                            color: root.labelColor; font.weight: 700; font.pixelSize: 14; font.family: "Segoe UI"
                            topPadding: 10
                        }
                        CustomTextBox {
                            id: userMobileTextBoxEdit
                            placeholderText: "Mobile Number"; text: ""; color: root.labelColor
                            width: parent.width - 20
                            enabled: root.popupMode === "edit"
                            onTextChanged: { if (root.popupMode === "edit") checkEditFormValidity() }
                        }
                        Text { text: root.mobileError; color: root.mandatoryColor; font.pixelSize: 12; visible: root.mobileError !== "" && root.popupMode === "edit" }
                    }
                }

                // -------- User Table --------
                // ScrollView {
                //     height: Screen.height - backbuttonUsers.y - backbuttonUsers.height
                //             - toolbar.y - toolbar.height - blankUsers.y - blankUsers.height - 170
                //     width: Screen.width - 60
                //     ScrollBar.horizontal.policy: ScrollBar.AlwaysOn
                //     ScrollBar.vertical.policy: ScrollBar.AlwaysOff

                    FHTable {
                        id: userTable
                        height: 400
                        leftPadding: 20
                        anchors {
                            top: toolbar.bottom
                           // bottom: parent.bottom
                            left: parent.left
                            right: parent.right
                            margins: 20
                        }
                        model: userlist

                        columns: [
                            { label: "Id",         width: 250, key: "id"        },
                            { label: "First Name", width: 200, key: "firstName" },
                            { label: "Last Name",  width: 200, key: "lastName"  },
                            { label: "User Name",  width: 200, key: "username"  },
                            { label: "Email",      width: 250, key: "email"     },
                            { label: "Mobile",     width: 150, key: "mobile"    }
                        ]

                        onViewRequested: function(row) {
                            root.popupMode = "view"
                            root.selectedUserData = row
                            fillEditPopup()
                            viewEditPopup.open()
                        }

                        onEditRequested: function(row) {
                            root.popupMode = "edit"
                            root.selectedUserData = row
                            fillEditPopup()
                            viewEditPopup.open()
                            //userController.getUserById(row.id)

                        }

                    }
                    Component.onCompleted: showUserList()
                    onVisibleChanged: showUserList()
               // }
            }
        }
    }

    // ========== VALIDATION FUNCTIONS ==========

    function checkCreateFormValidity() {
        let firstNameValidation = validator.validateNotEmpty(userFirstNameTextBox.text)
        let lastNameValidation  = validator.validateNotEmpty(userLastNameTextBox.text)
        let userNameValidation  = validator.validateNotEmpty(userNameTextBox.text)
        let passwordValidation  = validator.validateNotEmpty(userPasswordTextBox.text)
        let emailValidation     = validator.validateEmail(userEmailTextBox.text)
        let mobileValidation    = validator.validateNotEmpty(userMobileTextBox.text)

        root.firstNameError = firstNameValidation.isValid ? "" : firstNameValidation.message
        root.lastNameError  = lastNameValidation.isValid  ? "" : lastNameValidation.message
        root.userNameError  = userNameValidation.isValid  ? "" : userNameValidation.message
        root.passwordError  = passwordValidation.isValid  ? "" : passwordValidation.message
        root.emailError     = emailValidation.isValid     ? "" : emailValidation.message
        root.mobileError    = mobileValidation.isValid    ? "" : mobileValidation.message

        root.isCreateFormValid = firstNameValidation.isValid && lastNameValidation.isValid &&
                                 userNameValidation.isValid  && passwordValidation.isValid &&
                                 emailValidation.isValid     && mobileValidation.isValid
    }

    function checkEditFormValidity() {
        let firstNameValidation = validator.validateNotEmpty(userFirstNameTextBoxEdit.text)
        let lastNameValidation  = validator.validateNotEmpty(userLastNameTextBoxEdit.text)
        let userNameValidation  = validator.validateNotEmpty(userNameTextBoxEdit.text)
        let passwordValidation  = validator.validateNotEmpty(userPasswordTextBoxEdit.text)
        let emailValidation     = validator.validateEmail(userEmailTextBoxEdit.text)
        let mobileValidation    = validator.validateNotEmpty(userMobileTextBoxEdit.text)

        root.firstNameError = firstNameValidation.isValid ? "" : firstNameValidation.message
        root.lastNameError  = lastNameValidation.isValid  ? "" : lastNameValidation.message
        root.userNameError  = userNameValidation.isValid  ? "" : userNameValidation.message
        root.passwordError  = passwordValidation.isValid  ? "" : passwordValidation.message
        root.emailError     = emailValidation.isValid     ? "" : emailValidation.message
        root.mobileError    = mobileValidation.isValid    ? "" : mobileValidation.message

        root.isEditFormValid = firstNameValidation.isValid && lastNameValidation.isValid &&
                               userNameValidation.isValid  && passwordValidation.isValid &&
                               emailValidation.isValid     && mobileValidation.isValid
    }

    function validateForm(isCreate) {
        clearValidationErrors()

        let firstName = isCreate ? userFirstNameTextBox.text : userFirstNameTextBoxEdit.text
        let lastName  = isCreate ? userLastNameTextBox.text  : userLastNameTextBoxEdit.text
        let userName  = isCreate ? userNameTextBox.text      : userNameTextBoxEdit.text
        let password  = isCreate ? userPasswordTextBox.text  : userPasswordTextBoxEdit.text
        let email     = isCreate ? userEmailTextBox.text     : userEmailTextBoxEdit.text
        let mobile    = isCreate ? userMobileTextBox.text    : userMobileTextBoxEdit.text

        let firstNameValidation = validator.validateNotEmpty(firstName)
        if (!firstNameValidation.isValid) { root.firstNameError = firstNameValidation.message; return false }

        let lastNameValidation = validator.validateNotEmpty(lastName)
        if (!lastNameValidation.isValid) { root.lastNameError = lastNameValidation.message; return false }

        let userNameValidation = validator.validateNotEmpty(userName)
        if (!userNameValidation.isValid) { root.userNameError = userNameValidation.message; return false }

        let passwordValidation = validator.validateNotEmpty(password)
        if (!passwordValidation.isValid) { root.passwordError = passwordValidation.message; return false }

        let emailValidation = validator.validateEmail(email)
        if (!emailValidation.isValid) { root.emailError = emailValidation.message; return false }

        let mobileValidation = validator.validateNotEmpty(mobile)
        if (!mobileValidation.isValid) { root.mobileError = mobileValidation.message; return false }

        return true
    }

    function clearValidationErrors() {
        root.firstNameError = ""
        root.lastNameError  = ""
        root.userNameError  = ""
        root.passwordError  = ""
        root.emailError     = ""
        root.mobileError    = ""
    }

    function resetForm(isCreate) {
        if (isCreate) {
            userFirstNameTextBox.text = ""
            userLastNameTextBox.text  = ""
            userNameTextBox.text      = ""
            userPasswordTextBox.text  = ""
            userEmailTextBox.text     = ""
            userMobileTextBox.text    = ""
            root.isCreateFormValid    = false
        } else {
            userFirstNameTextBoxEdit.text = ""
            userLastNameTextBoxEdit.text  = ""
            userNameTextBoxEdit.text      = ""
            userPasswordTextBoxEdit.text  = ""
            userEmailTextBoxEdit.text     = ""
            userMobileTextBoxEdit.text    = ""
            root.isEditFormValid          = false
        }
        clearValidationErrors()
    }

    function fillEditPopup() {
        if (!root.selectedUserData) {
            console.log("No selectedUserData")
            return
        }

        userFirstNameTextBoxEdit.text = root.selectedUserData.firstName || ""
        userLastNameTextBoxEdit.text  = root.selectedUserData.lastName  || ""
        userNameTextBoxEdit.text      = root.selectedUserData.username  || ""
        userPasswordTextBoxEdit.text  = root.selectedUserData.password  || ""
        userEmailTextBoxEdit.text     = root.selectedUserData.email     || ""
        userMobileTextBoxEdit.text    = root.selectedUserData.mobile    || ""

        checkEditFormValidity()
    }

    Timer {
        id: retryTimer
        interval: 250
        repeat: false
        onTriggered: {
            userController.getUserList()
        }
    }

    Connections {
        target: userController

        function onUserListReceived(data) {
            root.userlist = data
        }

        function onUserListFailed(error) {
            if (error.includes("login") && !retryTimer.running) {
              //  console.log("QML: Token might not be ready, starting retry timer...")
                retryTimer.start()
            }
        }
        function onUserDetailsReceived(details) {
            root.selectedUserData = details
                    fillEditPopup()
                    viewEditPopup.open()
            }

            function onUserDetailsFailed(error) {
                console.log("Error fetching user:", error)
            }
    }

    function showUserList() {
        console.log("QML: Requesting data...")
        userController.getUserList()
    }
}
