import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers;

Column {
    id: workBillingLineRoot
    width: parent.width
    padding: 10

    property var workOrderLineList: []
    property var workBillingLineList: []
    property bool isApproved:false

    property var workOrderList:[]
    property var listData:[]
    property var workOrdersFromCtrl:[]
    property var workOrderLineFromCtrl:[]

    WorkOrderController {
        id: workOrderController
    }

    WorkOrderLineController {
        id: workOrderLineController
    }

    WorkBillingLineController {
        id: workBillingLineController
    }



    FHPopup {
        id: newWorkBillingLinePopup
        popupWidth: 900
        popupHeight: 450
        title: "Create Work Billing"
        parent: Overlay.overlay

        onAcceptCallback: function () {
            if (listData.length > 0) {
                workBillingLineController.create(workOrdersFromCtrl[workOrderId.currentIndex].id, listData)
                // reset
                listData = []
            }
            showList()
        }

        onCancelCallback: function () {
            listData = []
        }

        onOpened: {
            // Load lists from controllers and populate root lists
            if (workBillingLineRoot.visible) {
                //workOrdersFromCtrl = workOrderController.getWorkOrderList(true)
               // workOrderLineFromCtrl = workOrderLineController.getWorkOrderLineList(true)

                workOrdersFromCtrl = workOrderController.getWorkOrderList(false)
                workOrderLineFromCtrl = workOrderLineController.getWorkOrderLineList(false)

                // Clear current lists then populate
                workOrderList = []
                workOrderLineList = []

                //console.log("workOrdersFromCtrl.length:", workOrdersFromCtrl.length)


                for (var j = 0; j < workOrdersFromCtrl.length; j++) {
                    workOrderList = workOrderList.concat(
                                 workOrdersFromCtrl[j].workOrderName)

                }

                // Build display list for combo box with description
                for (var k = 0; k < workOrderLineFromCtrl.length; k++) {
                    var displayText = workOrderLineFromCtrl[k].description ||
                            "Line " + workOrderLineFromCtrl[k].id;
                    workOrderLineList = workOrderLineList.concat({
                                                                     "id": workOrderLineFromCtrl[k].id,
                                                                     "display": displayText
                                                                 });
                }
            }
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Work Order--------
            Row {
                width: parent.width
                spacing: 40
                Column {
                    spacing: 4
                    Text {
                        text: "Select Work Order"
                        color: "#323130"
                        font.weight: 700
                        font.pixelSize: 14
                    }
                    CustomComboBox {
                        id: workOrderId
                        width: 300
                        model: workOrderList
                        currentIndex: 0
                    }
                }
            }

            // -------- WorkBillingLine Row --------
            Text {
                id: workOrderLineLabel
                text: "Work Billing Item:"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            Rectangle {
                width: parent.width - 6
                height: 140
                color: "#EDF1F4"

                Column {
                    FHTable {
                        id: popupTable
                        height: 130
                        leftPadding: 2
                        removeRow: true
                        model: listData
                        columns: [{
                                "label": "Work Order Line",
                                "width": 120,
                                "key": "work_order_line_display"
                            },{
                                "label": "QTY",
                                "width": 120,
                                "key": "amount"
                            }, {
                                "label": "Tax Amount",
                                "width": 120,
                                "key": "tax_amount"
                            }, {
                                "label": "Tax With Holding",
                                "width": 120,
                                "key": "tax_with_holding"
                            },{
                                "label": "Retention Amount",
                                "width": 175,
                                "key": "retention_amount"
                            }]

                        onRemoveRowChanged: {
                            listData.splice(removedIndex, 1)
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width - 8
                height: 28
                color: "white"

                Row {
                    width: parent.width - 8
                    height: 28
                    leftPadding: 2
                    spacing: 6

                    CustomComboBox {
                        id: workOrderLineId
                        width: 184
                        height: 24
                        model: workOrderLineList
                        textRole: "display"
                        valueRole: "id"
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: amountTextBox
                        placeholderText: "QTY"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxAmountTextBox
                        placeholderText: "Tax Amount"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxWithHoldingTextBox
                        placeholderText: "Tax With Holding"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: retentionAmountTextBox
                        placeholderText: "Retention Amount"
                        text: ""
                        color: "#323130"
                        width: 120
                        height: 24
                        topPadding: 1
                    }


                    CustomButton {
                        color: "transparent"
                        width: 24
                        height: 24
                        border.color: "#8080808C"
                        btnSource: "qrc:/resources/images/add.svg"
                        btnName: ""
                        btnNameColor: "blue"
                        anchors.verticalCenter: parent.verticalCenter

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true

                            onEntered: parent.color = "#f2f2f2"
                            onExited: parent.color = "transparent"

                            onClicked: {
                                var selectedLine = workOrderLineList[workOrderLineId.currentIndex];
                                var newElements = {
                                    "work_order_line_id": selectedLine.id,
                                    "work_order_line_display": selectedLine.display,
                                    "amount": amountTextBox.text,
                                    "tax_amount": taxAmountTextBox.text,
                                    "tax_with_holding": taxWithHoldingTextBox.text,
                                    "retention_amount": retentionAmountTextBox.text,
                                }

                                // append to listData
                                listData = listData.concat(newElements)

                                // Clear the correct inputs
                                amountTextBox.text = ""
                                taxAmountTextBox.text = ""
                                taxWithHoldingTextBox.text = ""
                                retentionAmountTextBox.text = ""
                            }
                        }
                    }
                }
            }
        }
    }


    Row {
        spacing: 20
        Text {
            text: "Work Billing "
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

            MouseArea{
                anchors.fill: parent

                onClicked: {

                    newWorkBillingLinePopup.open();
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
        Text{
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
            width:200
            currentIndex: 0

            onCurrentTextChanged: {
                if(approvalTypeComboBox.currentText === "Approved"){
                    isApproved = true;
                }
                else{
                    isApproved = false;
                }
                showList();
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
        model: workBillingLineList
        columns: [{
                "label": "Work Order",
                "width": 214,
                "key": "workOrderName"
            }, {
                "label": "Work Order Line Name",
                "width": 170,
                "key": "workOrderLineName"
            }, {
                "label": "Amount",
                "width": 130,
                "key": "amount"
            }, {
                "label": "Tax Amount",
                "width": 130,
                "key": "taxAmount"
            }, {
                "label": "Tax With Holding",
                "width": 200,
                "key": "taxWithHolding"
            }, {
                "label": "Retention Amount",
                "width": 130,
                "key": "retentionAmount"
            },
        ]
    }


    Component.onCompleted: {
        showList();
    }

    onVisibleChanged: {
        showList();
    }
    function showList() {
        workBillingLineRoot.workBillingLineList = [];
        if (workBillingLineRoot.visible) {
            workBillingLineRoot.workBillingLineList = workBillingLineController.getWorkBillingLineList(isApproved);
            //console.log("dd_WorkBiling_data", JSON.stringify(workBillingLineList));
        }
    }

}
