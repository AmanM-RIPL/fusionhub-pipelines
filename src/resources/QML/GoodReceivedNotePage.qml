import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: goodReceivedNoteRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var purchaseOrderLinesFromCtrl: []
    property var purchaseOrdersFromCtrl: []

    // Properties - Lists for UI
    property var goodReceivedNoteList: []
    property var purchaseOrderLineList: []

    // Properties - State management
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false

    // Controllers
    GoodReceivedNoteController {
        id: goodReceivedNoteController
    }

    PurchaseOrderLineController {
        id: purchaseOrderLineController
    }

    PurchaseOrderController {
        id: purchaseOrderController
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newGoodReceivedNotePopup
        popupWidth: 500
        popupHeight: 350
        title: "Create Good Received Note"
        parent: Overlay.overlay

        onAcceptCallback: function () {
            if (purchaseOrderLineCombo.currentIndex >= 0 && amountReceivedTextBox.text !== "") {
                goodReceivedNoteController.create(
                            purchaseOrderLinesFromCtrl[purchaseOrderLineCombo.currentIndex].id,
                            amountReceivedTextBox.text
                            )
                // Reset
                amountReceivedTextBox.text = ""
                purchaseOrderLineCombo.currentIndex = 0
            }
            close()
        }

        onCancelCallback: function () {
            amountReceivedTextBox.text = ""
            purchaseOrderLineCombo.currentIndex = 0
            close()
        }

        onClosed: {
            showGoodReceivedNoteList()
        }

        onOpened: {
            // Load lists
            amountReceivedTextBox.text = ""
            purchaseOrderLineCombo.currentIndex = 0

            if (goodReceivedNoteRoot.visible) {
                purchaseOrderLinesFromCtrl = []
                purchaseOrdersFromCtrl = []

                purchaseOrderLinesFromCtrl = purchaseOrderLineController.getPurchaseOrderLineList(true)
                purchaseOrdersFromCtrl = purchaseOrderController.getPurchaseOrderList(true)

                // Clear current list then populate
                goodReceivedNoteRoot.purchaseOrderLineList = []

                for (var i = 0; i < purchaseOrderLinesFromCtrl.length; i++) {
                    goodReceivedNoteRoot.purchaseOrderLineList = goodReceivedNoteRoot.purchaseOrderLineList.concat(
                                purchaseOrderLinesFromCtrl[i].id)
                }
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                id: purchaseOrderLineLabel
                text: "Purchase Order Line"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: purchaseOrderLineCombo
                width: parent.width
                model: purchaseOrderLineList
                currentIndex: 0
            }

            Text {
                id: amountReceivedLabel
                text: "Amount of Material Received"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: amountReceivedTextBox
                placeholderText: "Amount of Material Received"
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
        popupHeight: 350
        title: popupMode === "view" ? "View Good Received Note" : "Edit Good Received Note"

        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData) {
                goodReceivedNoteController.update(
                            selectedData.id,
                            purchaseOrderLinesFromCtrl[poLineComboEdit.currentIndex].id,
                            amountReceivedFieldEdit.text
                            )
                // Reset
                amountReceivedFieldEdit.text = ""
                poLineComboEdit.currentIndex = 0
                showGoodReceivedNoteList()
            }
        }

        onCancelCallback: function () {
            amountReceivedFieldEdit.text = ""
            poLineComboEdit.currentIndex = 0
        }

        onOpened: {
            // Load lists from controllers
            amountReceivedFieldEdit.text = ""

            purchaseOrderLinesFromCtrl = []
            purchaseOrdersFromCtrl = []

            purchaseOrderLinesFromCtrl = purchaseOrderLineController.getPurchaseOrderLineList(true)
            purchaseOrdersFromCtrl = purchaseOrderController.getPurchaseOrderList(true)

            // Clear and populate purchase order line list
            var tempPurchaseOrderLineList = []

            for (var i = 0; i < purchaseOrderLinesFromCtrl.length; i++) {
                tempPurchaseOrderLineList = tempPurchaseOrderLineList.concat(
                            purchaseOrderLinesFromCtrl[i].id)
            }

            goodReceivedNoteRoot.purchaseOrderLineList = tempPurchaseOrderLineList

            // Fill popup with selected data
            if (selectedData) {
                fillPopup()
            }
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                id: purchaseOrderLineLabelEdit
                text: "Purchase Order Line"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: poLineComboEdit
                width: parent.width
                model: purchaseOrderLineList
                currentIndex: 0
                enabled: popupMode === "edit"
            }

            Text {
                id: amountReceivedLabelEdit
                text: "Amount of Material Received"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomTextBox {
                id: amountReceivedFieldEdit
                placeholderText: "Amount of Material Received"
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
            text: "Goods Received Note"
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
                onClicked: newGoodReceivedNotePopup.open()
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
                    showGoodReceivedNoteList()
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
        height: 300
        leftPadding: 20
        model: goodReceivedNoteRoot.goodReceivedNoteList

        columns: [
            {
                "label": "Id",
                "width": 650,
                "key": "id"
            },
            {
                "label": "Amount Received",
                "width": 650,
                "key": "quantity"
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
    Component.onCompleted: showGoodReceivedNoteList()
    onVisibleChanged: showGoodReceivedNoteList()

    // -------- Functions --------
    function showGoodReceivedNoteList() {
        goodReceivedNoteRoot.goodReceivedNoteList = []

        if (!goodReceivedNoteRoot.visible)
            return

        goodReceivedNoteRoot.goodReceivedNoteList = goodReceivedNoteController.getGoodReceivedNoteList(isApproved)
    }

    function fillPopup() {
        if (!selectedData)
            return

        // Set amount of material received
        amountReceivedFieldEdit.text = selectedData.quantity !== null
                ? String(selectedData.quantity)
                : ""

        // Find and set purchase order line
        for (var i = 0; i < purchaseOrderLinesFromCtrl.length; i++) {
            if (purchaseOrderLinesFromCtrl[i].id === selectedData.purchaseOrderLineId) {
                poLineComboEdit.currentIndex = i
                break
            }
        }
    }
}
