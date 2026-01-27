import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: vendorRoot
    width: parent.width
    padding: 10

    property var vendorList: []
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false

    VendorController {
        id: vendorController
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newVendorPopup
        popupWidth: 600
        popupHeight: 550
        title: "New Vendor"

        onAcceptCallback: function () {
            if (vendorNameTextBox.text === "" || vendorAddressTextBox.text === "" ||
                    vendorContactPersonTextBox.text === "" || vendorMobileTextBox.text === "" ||
                    vendorEmailTextBox.text === "") {
                return
            }

            vendorController.create(
                        vendorNameTextBox.text,
                        vendorAddressTextBox.text,
                        vendorContactPersonTextBox.text,
                        vendorMobileTextBox.text,
                        vendorEmailTextBox.text
                        )

            vendorNameTextBox.text = ""
            vendorAddressTextBox.text = ""
            vendorContactPersonTextBox.text = ""
            vendorMobileTextBox.text = ""
            vendorEmailTextBox.text = ""

            showList()
            close()
        }

        onCancelCallback: function () {
            vendorNameTextBox.text = ""
            vendorAddressTextBox.text = ""
            vendorContactPersonTextBox.text = ""
            vendorMobileTextBox.text = ""
            vendorEmailTextBox.text = ""
            close()
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                id: vendorNameLabel
                text: "Vendor Name"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorNameTextBox
                placeholderText: "Vendor Name"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
            }

            Text {
                id: vendorAddressLabel
                text: "Vendor Address"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorAddressTextBox
                placeholderText: "Vendor Address"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
            }

            Text {
                id: vendorContactPersonLabel
                text: "Vendor Contact Person"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorContactPersonTextBox
                placeholderText: "Vendor Contact Person"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
            }

            Text {
                id: vendorMobileLabel
                text: "Vendor Mobile"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorMobileTextBox
                placeholderText: "Vendor Mobile"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
            }

            Text {
                id: vendorEmailLabel
                text: "Vendor Email"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorEmailTextBox
                placeholderText: "Vendor Email"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 600
        popupHeight: 550
        title: popupMode === "view" ? "View Vendor" : "Edit Vendor"

        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData) {
                if (vendorNameTextBoxEdit.text === "" || vendorAddressTextBoxEdit.text === "" ||
                        vendorContactPersonTextBoxEdit.text === "" || vendorMobileTextBoxEdit.text === "" ||
                        vendorEmailTextBoxEdit.text === "") {
                    return
                }

                vendorController.update(
                            selectedData.id,
                            vendorNameTextBoxEdit.text,
                            vendorAddressTextBoxEdit.text,
                            vendorContactPersonTextBoxEdit.text,
                            vendorMobileTextBoxEdit.text,
                            vendorEmailTextBoxEdit.text
                            )

                vendorNameTextBoxEdit.text = ""
                vendorAddressTextBoxEdit.text = ""
                vendorContactPersonTextBoxEdit.text = ""
                vendorMobileTextBoxEdit.text = ""
                vendorEmailTextBoxEdit.text = ""

                showList()
                close()
            }
        }

        onCancelCallback: function () {
            vendorNameTextBoxEdit.text = ""
            vendorAddressTextBoxEdit.text = ""
            vendorContactPersonTextBoxEdit.text = ""
            vendorMobileTextBoxEdit.text = ""
            vendorEmailTextBoxEdit.text = ""
            close()
        }

        onOpened: {
            vendorNameTextBoxEdit.text = ""
            vendorAddressTextBoxEdit.text = ""
            vendorContactPersonTextBoxEdit.text = ""
            vendorMobileTextBoxEdit.text = ""
            vendorEmailTextBoxEdit.text = ""

            if (selectedData) {
                fillPopup()
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                id: vendorNameLabelEdit
                text: "Vendor Name"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorNameTextBoxEdit
                placeholderText: "Vendor Name"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
            }

            Text {
                id: vendorAddressLabelEdit
                text: "Vendor Address"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorAddressTextBoxEdit
                placeholderText: "Vendor Address"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
            }

            Text {
                id: vendorContactPersonLabelEdit
                text: "Vendor Contact Person"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorContactPersonTextBoxEdit
                placeholderText: "Vendor Contact Person"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
            }

            Text {
                id: vendorMobileLabelEdit
                text: "Vendor Mobile"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorMobileTextBoxEdit
                placeholderText: "Vendor Mobile"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
            }

            Text {
                id: vendorEmailLabelEdit
                text: "Vendor Email"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: vendorEmailTextBoxEdit
                placeholderText: "Vendor Email"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
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
    Column {
        spacing: 20
        Row {
            spacing: 20
            anchors.left: parent.left
            Text {
                id: approvalTypeLabel
                text: "Choose Approval Type"
                color: "#323130"
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
                    color: "#323130"
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
        height: 5
        color: "#EDF1F4"
    }

    // -------- Main Table --------
    FHTable {
        height: 200
        leftPadding: 20
        model: vendorRoot.vendorList

        columns: [
            { label: "Id", width: 200, key: "id" },
            { label: "Name", width: 200, key: "vendorName" },
            { label: "Address", width: 300, key: "vendorAddress" },
            { label: "Contact Person", width: 200, key: "vendorContactPerson" },
            { label: "Mobile", width: 200, key: "vendorMobile" },
            { label: "Email", width: 200, key: "vendorEmail" }
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

    // -------- Functions --------
    function showList() {
        vendorRoot.vendorList = []

        if (!vendorRoot.visible)
            return

        vendorRoot.vendorList = vendorController.getVendorList(isApproved)
    }

    function fillPopup() {
        if (!selectedData)
            return

        vendorNameTextBoxEdit.text = selectedData.vendorName || ""
        vendorAddressTextBoxEdit.text = selectedData.vendorAddress || ""
        vendorContactPersonTextBoxEdit.text = selectedData.vendorContactPerson || ""
        vendorMobileTextBoxEdit.text = selectedData.vendorMobile || ""
        vendorEmailTextBoxEdit.text = selectedData.vendorEmail || ""
    }
}
