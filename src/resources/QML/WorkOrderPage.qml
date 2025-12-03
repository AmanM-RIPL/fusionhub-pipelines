import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: workOrderLineRoot
    width: parent.width
    padding: 10

    property var workOrderLineList: []
    property var workOrderLineData: []
    property bool isApproved: false

    property var vendorList: []
    property var taskList: []
    property var workOrderList: []
    property var vendorsFromCtrl:[]

    // Controllers
    VendorController {
        id: vendorController
    }

    WorkOrderController {
        id: workOrderController
    }

    WorkOrderLineController {
        id: workOrderLineController
    }

    TaskController {
        id: taskController
    }


    /*--------------------------------------
                    Pop UP
      --------------------------------------*/
    FHPopup {
        id: newWorkOrderLinePopup
        popupWidth: 900
        popupHeight: 450
        title: "Create Work Order"
        parent: Overlay.overlay

        onAcceptCallback: function () {
            if (workOrderLineData.length > 0) {
                workOrderLineController.create(vendorsFromCtrl[vendor.currentIndex].id, descriptionTextBox.text, workOrderLineData)
                // reset
                workOrderLineData = []
                descriptionTextBox.text = ""
            }
            showList()
        }

        onCancelCallback: function () {
            workOrderLineData = []
            descriptionTextBox.text = ""
        }

        onOpened: {
            // Load lists from controllers and populate root lists
            if (workOrderLineRoot.visible) {
                vendorsFromCtrl = vendorController.getVendorList(true)
                var workOrdersFromCtrl = workOrderController.getWorkOrderList(true)
                var taskFromCtrl = taskController.getTaskList(true)

                // Clear current lists then populate
                workOrderLineRoot.vendorList = []
                workOrderLineRoot.workOrderList = []
                workOrderLineRoot.taskList = []


                for (var i = 0; i < vendorsFromCtrl.length; i++) {
                    workOrderLineRoot.vendorList = workOrderLineRoot.vendorList.concat(
                                vendorsFromCtrl[i].vendorName)
                }
                for (var j = 0; j < workOrdersFromCtrl.length; j++) {
                    workOrderLineRoot.workOrderList = workOrderLineRoot.workOrderList.concat(
                                workOrdersFromCtrl[j].description)
                }
                for (var k = 0; k < taskFromCtrl.length; k++) {
                    workOrderLineRoot.taskList = workOrderLineRoot.taskList.concat(
                                taskFromCtrl[k].description)
                }


            }
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Vendor + Description Row --------
            Row {
                width: parent.width
                spacing: 40
                Column {
                    spacing: 4
                    Text {
                        text: "Select Vendor"
                        color: "#323130"
                        font.weight: 700
                        font.pixelSize: 14
                    }
                    CustomComboBox {
                        id: vendor
                        width: 300
                        model: vendorList
                        currentIndex: 0
                    }
                }

                Column {
                    spacing: 4
                    Text {
                        text: "Description"
                        color: "#323130"
                        font.weight: 700
                        font.pixelSize: 14
                    }
                    CustomTextBox {
                        id: descriptionTextBox
                        placeholderText: "Work Order"
                        width: 500
                        height: 30
                    }
                }
            }

            // -------- WorkOrderLine Row --------
            Text {
                id: workOrderLineLabel
                text: "Work Order Item:"
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
                        model: workOrderLineData
                        columns: [{
                                "label": "QTY",
                                "width": 120,
                                "key": "amount"
                            }, {
                                "label": "Description",
                                "width": 120,
                                "key": "description"
                            }, {
                                "label": "Tax Amount",
                                "width": 120,
                                "key": "tax_amount"
                            }, {
                                "label": "Tax With Holding",
                                "width": 120,
                                "key": "tax_with_holding"
                            },{
                                "label": "Task",
                                "width": 184,
                                "key": "task_id"
                            },{
                                "label": "Retention Amount",
                                "width": 175,
                                "key": "retention_amount"
                            }]

                        onRemoveRowChanged: {
                            workOrderLineData.splice(removedIndex, 1)
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
                        id: lineDescriptionTextBox
                        placeholderText: "Description"
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

                    CustomComboBox {
                        id: taskId
                        width: 184
                        height: 24
                        model: workOrderLineRoot.taskList

                        currentIndex: 0
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
                                var newElements = {
                                    "description": lineDescriptionTextBox.text,
                                    "task_id": taskId.currentValue,
                                    "amount": amountTextBox.text,
                                    "tax_amount": taxAmountTextBox.text,
                                    "tax_with_holding": taxWithHoldingTextBox.text,
                                    "retention_amount": retentionAmountTextBox.text,
                                }

                                // append to workOrderLineData
                                workOrderLineData = workOrderLineData.concat(newElements)

                                // Clear the correct inputs
                                lineDescriptionTextBox.text = ""
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

    // Header row
    Row {
        spacing: 20
        Text {
            text: "Work Order"
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
                onClicked: newWorkOrderLinePopup.open()
            }
        }
    }


    /*--------------------------------------
            Approval Type
    --------------------------------------*/
    Column {
        spacing: 10
        Row {
            spacing: 20
            anchors.left: parent.left
            anchors.leftMargin: 20

            // ------- APPROVAL TYPE --------
            Text {
                id: approvalTypeLabel
                text: "Choose Approval Type"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
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


        /*--------------------------------------
                Main Table
    --------------------------------------*/
        FHTable {
            height: 200
            leftPadding: 20
            model: workOrderLineRoot.workOrderLineList
            columns: [{
                    "label": "Vendor",
                    "width": 214,
                    "key": "vendorName"
                }, {
                    "label": "Work Order",
                    "width": 214,
                    "key": "description"
                }, {
                    "label": "task Id",
                    "width": 170,
                    "key": "taskId"
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
                },]
        }
    }

    Component.onCompleted: showList()
    onVisibleChanged: showList()

    function showList() {

        workOrderLineRoot.workOrderLineList = [];
        if (workOrderLineRoot.visible) {
            var arr = workOrderLineController.getWorkOrderLineList(isApproved)

            var flatList = []

            for (var i = 0; i < arr.length; i++) {
                var po = arr[i]
                try {
                    po.parsedWorkOrderLine = JSON.parse(po.workOrderLineData)
                } catch (e) {
                    console.error("Failed to parse workOrderLine JSON:", e,
                                  po.workOrderLineData)
                    po.parsedWorkOrderLine = {
                        "data": [],
                        "rows": 0
                    }
                }

                // For each in purchase order
                for (var j = 0; j < po.parsedWorkOrderLine.data.length; j++) {
                    var item = po.parsedWorkOrderLine.data[j];
                    flatList.push({
                        "vendorName": po.vendorId,
                        "description": item.description,
                        "taskId": item.task_id ?? item.taskId,
                        "amount": item.amount,
                        "taxAmount": item.tax_amount ?? item.taxAmount,
                        "taxWithHolding": item.tax_with_holding ?? item.taxWithHolding,
                        "retentionAmount": item.retention_amount ?? item.retentionAmount,
                    })

                }
            }

            workOrderLineRoot.workOrderLineList = flatList
          //    console.log("Table Data:", JSON.stringify(flatList))
        }
    }
}
