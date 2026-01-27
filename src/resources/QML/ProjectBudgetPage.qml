import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: projectBudgetRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var budgetHeadsFromCtrl: []

    // Properties - Lists for UI
    property var budgetHeadText: []
    property var budgetHeadList: []
    property var projectBudgetList: []

    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false

    ProjectBudgetController {
        id: projectBudgetController
    }

    BudgetHeadController {
        id: budgetHeadController
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newProjectBudgetPopup
        popupWidth: 500
        popupHeight: 350
        title: "New Project Budget"

        onAcceptCallback: function () {
            if (projectBudgetNameTextBox.text === "" || budgetHeadComboBox.currentIndex === -1) {
                return
            }

            projectBudgetController.create(
                        projectBudgetNameTextBox.text,
                        projectBudgetRoot.budgetHeadList[budgetHeadComboBox.currentIndex].id
                        )

            projectBudgetNameTextBox.text = ""
            budgetHeadComboBox.currentIndex = -1
            showList()
            close()
        }

        onCancelCallback: function () {
            projectBudgetNameTextBox.text = ""
            budgetHeadComboBox.currentIndex = -1
            close()
        }

        onOpened: {
            projectBudgetNameTextBox.text = ""
            budgetHeadComboBox.currentIndex = -1

            if (projectBudgetRoot.visible) {
                // Load budget heads from controller
                budgetHeadsFromCtrl = budgetHeadController.getBudgetHeadList(true)
                projectBudgetRoot.budgetHeadList = budgetHeadsFromCtrl

                var tempBudgetHeadText = []
                for (var j = 0; j < budgetHeadsFromCtrl.length; j++) {
                    tempBudgetHeadText.push(budgetHeadsFromCtrl[j].description)
                }
                projectBudgetRoot.budgetHeadText = tempBudgetHeadText
                budgetHeadComboBox.model = projectBudgetRoot.budgetHeadText
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                id: projectBudgetNameLabel
                text: "Project Budget"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: projectBudgetNameTextBox
                placeholderText: "Project Budget"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
            }

            Text {
                id: budgetHeadLabel
                text: "Budget Head"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: budgetHeadComboBox
                model: projectBudgetRoot.budgetHeadText
                width: parent.width
                currentIndex: -1
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 500
        popupHeight: 350
        title: popupMode === "view" ? "View Project Budget" : "Edit Project Budget"

        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData) {
                if (projectBudgetNameTextBoxEdit.text === "" || budgetHeadComboBoxEdit.currentIndex === -1) {
                    return
                }

                projectBudgetController.update(
                            selectedData.id,
                            projectBudgetNameTextBoxEdit.text,
                            projectBudgetRoot.budgetHeadList[budgetHeadComboBoxEdit.currentIndex].id
                            )

                projectBudgetNameTextBoxEdit.text = ""
                budgetHeadComboBoxEdit.currentIndex = -1
                showList()
                close()
            }
        }

        onCancelCallback: function () {
            projectBudgetNameTextBoxEdit.text = ""
            budgetHeadComboBoxEdit.currentIndex = -1
            close()
        }

        onOpened: {
            projectBudgetNameTextBoxEdit.text = ""
            budgetHeadComboBoxEdit.currentIndex = -1

            // Load budget heads from controller
            budgetHeadsFromCtrl = budgetHeadController.getBudgetHeadList(true)
            projectBudgetRoot.budgetHeadList = budgetHeadsFromCtrl

            var tempBudgetHeadText = []
            for (var k = 0; k < budgetHeadsFromCtrl.length; k++) {
                tempBudgetHeadText.push(budgetHeadsFromCtrl[k].description)
            }
            projectBudgetRoot.budgetHeadText = tempBudgetHeadText
            budgetHeadComboBoxEdit.model = projectBudgetRoot.budgetHeadText

            if (selectedData) {
                fillPopup()
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                id: projectBudgetNameLabelEdit
                text: "Project Budget"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: projectBudgetNameTextBoxEdit
                placeholderText: "Project Budget"
                text: ""
                color: "#323130"
                width: parent.width
                height: 30
                enabled: popupMode === "edit"
            }

            Text {
                id: budgetHeadLabelEdit
                text: "Budget Head"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: budgetHeadComboBoxEdit
                model: projectBudgetRoot.budgetHeadText
                width: parent.width
                currentIndex: -1
                enabled: popupMode === "edit"
            }
        }
    }

    // -------- Header Row --------
    Row {
        spacing: 20
        Text {
            text: "Project Budget"
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
                onClicked: newProjectBudgetPopup.open()
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
        height: 200
        leftPadding: 20
        model: projectBudgetRoot.projectBudgetList

        columns: [
            { label: "Id", width: 300, key: "id" },
            { label: "Project Budget", width: 500, key: "dollarValue" },
            { label: "Budget Head", width: 500, key: "budgetHeadName" }
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
        projectBudgetRoot.projectBudgetList = []

        if (!projectBudgetRoot.visible)
            return

        var projectBudgets = projectBudgetController.getProjectBudgetList(isApproved)
        var budgetHeads = budgetHeadController.getBudgetHeadList(true)

        // Store budget heads for later use in fillPopup
        projectBudgetRoot.budgetHeadList = budgetHeads

        for (var i = 0; i < projectBudgets.length; i++) {
            var budgetHeadName = ""
            for (var j = 0; j < budgetHeads.length; j++) {
                if (budgetHeads[j].id === projectBudgets[i].budgetHeadId) {
                    budgetHeadName = budgetHeads[j].description
                    break
                }
            }
            projectBudgets[i].budgetHeadName = budgetHeadName
        }

        projectBudgetRoot.projectBudgetList = projectBudgets
    }

    function fillPopup() {
        if (!selectedData)
            return

        projectBudgetNameTextBoxEdit.text = selectedData.dollarValue || ""

        for (var i = 0; i < projectBudgetRoot.budgetHeadList.length; i++) {
            if (projectBudgetRoot.budgetHeadList[i].id === selectedData.budgetHeadId) {
                budgetHeadComboBoxEdit.currentIndex = i
                break
            }
        }
    }
}
