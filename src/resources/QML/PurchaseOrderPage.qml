import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: purchaseOrderRoot
    width: parent.width
    padding: 10

    // root properties (these are the lists used by UI)
    property var purchaseOrderList: []
    property var purchaseOrderData: []
    property bool isApproved: false
    property var vendorList: ["Krishna PVT. Ltd", "Tata", "Saif Iron", "Saif Traiders"]
    property var uomList: ["cubic mtr", "kg", "meter"]
    property var materialList: ["M-30 Steel", "M-20 Steel", "Gravel", "Bamboo", "Capstones", "Cement"]

    // Controllers (IDs must match uses below)
    VendorController {
        id: vendorController
    }

    MaterialController {
        id: materialController
    }

    PurchaseOrderController {
        id: purchaseOrderController
    }

    UnitOfMeasurementController {
        id: unitOfMeasurementController    // NOTE: spelled "Measurement" here
    }

    /*--------------------------------------
                    Pop UP
      --------------------------------------*/
    FHPopup {
        id: newPurchaseOrderPopup
        popupWidth: 900
        popupHeight: 450
        title: "Create PO"
        parent: Overlay.overlay

        // Accept: create only if we have rows
        onAcceptCallback: function () {
            if (purchaseOrderData.length > 0) {
                // pass vendor selected, and the line items
                purchaseOrderController.create(
                    vendor.currentText,
                    purchaseOrderData
                )

                // reset
                purchaseOrderData = []
            }
            showList()
        }

        onCancelCallback: function () {
            // reset fields
            purchaseOrderData = []
        }

        onOpened: {
            // Load lists from controllers and populate root lists without shadowing variables
            if (purchaseOrderRoot.visible) {
                var vendorsFromCtrl = vendorController.getVendorList(true)
                var materialsFromCtrl = materialController.getMaterialList(true)
                var uomFromCtrl = unitOfMeasurementController.getUOMList(true)

                // Clear current lists then populate
                purchaseOrderRoot.vendorList = []
                purchaseOrderRoot.materialList = []
                purchaseOrderRoot.uomList = []

                for (var i = 0; i < vendorsFromCtrl.length; i++) {
                    // assuming vendor object has vendorName
                    purchaseOrderRoot.vendorList = purchaseOrderRoot.vendorList.concat(vendorsFromCtrl[i].vendorName)
                }
                for (var j = 0; j < materialsFromCtrl.length; j++) {
                    // assuming material object has materialName
                    purchaseOrderRoot.materialList = purchaseOrderRoot.materialList.concat(materialsFromCtrl[j].materialName)
                }
                for (var k = 0; k < uomFromCtrl.length; k++) {
                    // assuming uom object has uomName or uomId; adjust as per your controller
                    // here I'll use uomName if available, otherwise uomId
                    var uomDisplay = uomFromCtrl[k].uomName !== undefined ? uomFromCtrl[k].uomName : uomFromCtrl[k].uomId
                    purchaseOrderRoot.uomList = purchaseOrderRoot.uomList.concat(uomDisplay)
                }
            }
        }

        Column {
            width: parent.width
            height: 300

            CustomComboBox {
                id: vendor
                width: 500
                height: 30
                model: purchaseOrderRoot.vendorList
                currentIndex: 0
            }

            Text {
                id: purchaseOrderLabel
                text: "Purchase Order Item:"
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
                        model: purchaseOrderData
                        // Make keys consistent with how you store data in rows (see add row)
                        columns: [
                            { "label": "Material", "width": 184, "key": "material_name" },
                            { "label": "UOM", "width": 120, "key": "unit_of_measurement" },
                            { "label": "Quantity", "width": 100, "key": "quantity" },
                            { "label": "Amount", "width": 100, "key": "amount" },
                            { "label": "Tax Amount", "width": 100, "key": "tax_amount" },
                            { "label": "Tax With Holding", "width": 230, "key": "tax_with_holding" }
                        ]

                        onRemoveRowChanged: {
                            // assumes `removedIndex` is available from the FHTable component
                            console.log("onRemoveRowChanged:", removedIndex.toString())
                            purchaseOrderData.splice(removedIndex, 1)
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
                        id: materialId
                        width: 184
                        height: 24
                        model: purchaseOrderRoot.materialList
                        currentIndex: 0
                    }

                    CustomComboBox {
                        id: unitOfMeasurementId
                        width: 100
                        height: 24
                        model: purchaseOrderRoot.uomList
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: quantityTextBox
                        placeholderText: "Quantity"
                        text: ""
                        color: "#323130"
                        width: 80
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: amountTextBox
                        placeholderText: "Amount"
                        text: ""
                        color: "#323130"
                        width: 80
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxAmountTextBox
                        placeholderText: "Tax Amount"
                        text: ""
                        color: "#323130"
                        width: 80
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxWithHoldingTextBox
                        placeholderText: "Tax With Holding"
                        text: ""
                        color: "#323130"
                        width: 200
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
                                // create new row object with consistent keys
                                var newElements = {
                                    "material_name": materialId.currentText,
                                    "unit_of_measurement": unitOfMeasurementId.currentText,
                                    "quantity": quantityTextBox.text,
                                    "amount": amountTextBox.text,
                                    "tax_amount": taxAmountTextBox.text,
                                    "tax_with_holding": taxWithHoldingTextBox.text
                                }

                                // append to purchaseOrderData
                                purchaseOrderData = purchaseOrderData.concat(newElements)

                                // clear inputs (optional)
                                quantityTextBox.text = ""
                                amountTextBox.text = ""
                                taxAmountTextBox.text = ""
                                taxWithHoldingTextBox.text = ""
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
            text: "Purchase Order"
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
                onClicked: newPurchaseOrderPopup.open()
            }
        }
    }

    /*--------------------------------------
            Approval Type
    --------------------------------------*/
    Row {
        spacing: 20
        anchors.left: parent.left
        anchors.leftMargin: 20

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
        model: purchaseOrderRoot.purchaseOrderList
        columns: [
            { "label": "Material", "width": 214, "key": "material_name" },
            { "label": "Quantity", "width": 180, "key": "quantity" },
            { "label": "UOM", "width": 170, "key": "unit_of_measurement" },
            { "label": "Amount", "width": 170, "key": "amount" },
            { "label": "Tax Amount", "width": 180, "key": "tax_amount" },
            { "label": "Tax With Holding", "width": 200, "key": "tax_with_holding" }
        ]
    }

    Component.onCompleted: showList()
    onVisibleChanged: showList()

    function showList() {
        purchaseOrderRoot.purchaseOrderList = []
        if (purchaseOrderRoot.visible) {
            var arr = purchaseOrderController.getPurchaseOrderList(isApproved)
            if (arr && arr.length) {
                purchaseOrderRoot.purchaseOrderList = purchaseOrderRoot.purchaseOrderList.concat(arr)
            }
        }
    }
}
