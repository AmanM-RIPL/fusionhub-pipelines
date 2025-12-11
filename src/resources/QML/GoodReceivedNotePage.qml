import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    // anchors.fill: parent
    id: goodReceivedNoteRoot
    width: parent.width
    padding: 10

    property var goodReceivedNoteList: []
    property var goodReceivedNoteListForTable: []
    property var purchaseOrderLineText: [] // text for dropdown
    property var purchaseOrderLineList: ["--"] // raw data of po
    property var purchaseOrderText: [] // text for dropdown
    property var purchaseOrderList: ["--"] // raw data of po

    property bool isApproved: false

    GoodReceivedNoteController {
        id: goodReceivedNoteController
    }

    PurchaseOrderLineController {
        id: purchaseOrderLineController
    }

    PurchaseOrderController {
        id: purchaseOrderController
    }

    FHPopup {
        id: newGoodReceivedNotePopup
        popupWidth: 500
        popupHeight: 350
        title: "New Good Received Note"

        onAcceptCallback: function () {

            goodReceivedNoteController.create(
                        goodReceivedNoteQuantityTextBox.text,
                        goodReceivedNoteRoot.purchaseOrderLineList[purchaseOrderLineComboBox.currentIndex].id)

            purchaseOrderLineComboBox.currentIndex = -1
            goodReceivedNoteQuantityTextBox.text = ""

            showList()
        }

        onCancelCallback: function () {
            purchaseOrderLineComboBox.currentIndex = -1
            goodReceivedNoteQuantityTextBox.text = ""
        }

        Column {
            width: parent.width
            height: 200 //parent.height //30 for each top bottom

            Text {
                id: purchaseOrderLineLabel
                text: "Purchase Order line"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: purchaseOrderLineComboBox
                model: goodReceivedNoteRoot.purchaseOrderLineText
            }

            Text {
                id: goodReceivedNoteQuantityLabel
                text: "Quantity"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomTextBox {
                id: goodReceivedNoteQuantityTextBox
                placeholderText: "quantity"
                text: ""
                color: "#323130"
            }
        }
    }

    Row {
        spacing: 20
        Text {
            text: "Goods Received Note"
            color: "#000000"
            font.family: "Segoe UI"
            font.weight: 700
            font.pixelSize: 44
            leftPadding: 20
            // topPadding: 20
        }

        CustomButton {
            color: "#007AFF"
            width: 85
            height: 38
            radius: 4
            // border.color: "#007AFF"
            btnSource: "qrc:/resources/images/addWhite_icon.png"
            btnName: "New"
            btnNameColor: "white"
            anchors.verticalCenter: parent.verticalCenter

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    newGoodReceivedNotePopup.open()
                }
            }
        }
    }

    Rectangle {
        width: 100
        height: 40
        color: "#EDF1F4"
    }

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

            onCurrentTextChanged: {
                if (approvalTypeComboBox.currentText === "Approved") {
                    isApproved = true
                } else {
                    isApproved = false
                }
                showList()
            }
        }
    }

    Rectangle {
        width: 100
        height: 40
        color: "#EDF1F4"
    }

    FHTable {
        height: 200
        leftPadding: 20
        model: goodReceivedNoteRoot.goodReceivedNoteList
        columns: [{
                "label": "Id",
                "width": 100,
                "key": "id"
            }, {
                "label": "Purchase Order",
                "width": 300,
                "key": "purchaseOrderLineId"
            }, {
                "label": "Quantity",
                "width": 300,
                "key": "quantity"
            }]
    }

    Component.onCompleted: {
        showList()
    }

    onVisibleChanged: {
        showList()
    }

    function showList() {
        if (goodReceivedNoteRoot.visible) {
            goodReceivedNoteRoot.goodReceivedNoteList =
                    goodReceivedNoteController.getGoodReceivedNoteList(isApproved)

            //console.log("List:", JSON.stringify(goodReceivedNoteRoot.goodReceivedNoteList))
        }
    }
}
