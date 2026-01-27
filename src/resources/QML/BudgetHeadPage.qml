import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: budgetHeadRoot
    width: parent.width
    padding: 10

    property var budgetHeadList: []
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false

    BudgetHeadController {
        id: budgetheadController
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newBudgetHeadPopup
        popupWidth: 500
        popupHeight: 230
        title: "New Budget Head"

        onAcceptCallback: function () {
            if (descriptionTextBox.text === "") {
                return
            }

            budgetheadController.create(descriptionTextBox.text)
            descriptionTextBox.text = ""
            showList()
            close()
        }

        onCancelCallback: function () {
            descriptionTextBox.text = ""
            close()
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                id: desciptionLabel
                text: "Description"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: descriptionTextBox
                placeholderText: "Description"
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
        popupWidth: 500
        popupHeight: 230
        title: popupMode === "view" ? "View Budget Head" : "Edit Budget Head"

        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData) {
                if (descriptionTextBoxEdit.text === "") {
                    return
                }

                budgetheadController.update(
                    selectedData.id,
                    descriptionTextBoxEdit.text
                )

                descriptionTextBoxEdit.text = ""
                showList()
                close()
            }
        }

        onCancelCallback: function () {
            descriptionTextBoxEdit.text = ""
            close()
        }

        onOpened: {
            descriptionTextBoxEdit.text = ""

            if (selectedData) {
                fillPopup()
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                id: desciptionLabelEdit
                text: "Description"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: descriptionTextBoxEdit
                placeholderText: "Description"
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
            text: "Budget Head"
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
                onClicked: newBudgetHeadPopup.open()
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

    Rectangle {
        width: 100
        height: 5
        color: "#EDF1F4"
    }

    // -------- Main Table --------
    FHTable {
        id: ftabl
        height: 200
        leftPadding: 20
        model: budgetHeadRoot.budgetHeadList

        columns: [
            { label: "Id", width: 650, key: "id" },
            { label: "Description", width: 650, key: "description" }
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
        budgetHeadRoot.budgetHeadList = []

        if (!budgetHeadRoot.visible)
            return

        budgetHeadRoot.budgetHeadList = budgetheadController.getBudgetHeadList(isApproved)
    }

    function fillPopup() {
        if (!selectedData)
            return

        descriptionTextBoxEdit.text = selectedData.description || ""
    }
}
