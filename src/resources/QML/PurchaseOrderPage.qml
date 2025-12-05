import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: purchaseOrderRoot
    width: parent.width
    padding: 10

    property var purchaseOrderList: []
    property var purchaseOrderData: []
    property bool isApproved: false

    property var vendorList: []
    property var uomList: []
    property var materialList: []

    property var vendorsFromCtrl: []
    property var materialsFromCtrl: []
    property var uomFromCtrl: []


    // Controllers
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
        id: unitOfMeasurementController
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

        onAcceptCallback: function () {
            if (purchaseOrderData.length > 0) {
                purchaseOrderController.create(vendor.currentText, purchaseOrderData)

                // reset
                purchaseOrderData = []
            }
            showList()
        }

        onCancelCallback: function () {
            purchaseOrderData = []
        }

        onOpened: {
            // Load lists from controllers and populate root lists
            if (purchaseOrderRoot.visible) {
                // var vendorsFromCtrl = vendorController.getVendorList(true)
                // var materialsFromCtrl = materialController.getMaterialList(true)
                // var uomFromCtrl = unitOfMeasurementController.getUOMList(true)

                vendorsFromCtrl = []
                materialsFromCtrl = []
                uomFromCtrl = []
                vendorsFromCtrl = vendorController.getVendorList(true)
                materialsFromCtrl = materialController.getMaterialList(true)
                uomFromCtrl = unitOfMeasurementController.getUOMList(true)

                //Clear current lists then populate
                purchaseOrderRoot.vendorList = []
                purchaseOrderRoot.materialList = []
                purchaseOrderRoot.uomList = []

                for (var i = 0; i < vendorsFromCtrl.length; i++) {
                    purchaseOrderRoot.vendorList = purchaseOrderRoot.vendorList.concat(
                                vendorsFromCtrl[i].vendorName)
                }
                for (var j = 0; j < materialsFromCtrl.length; j++) {
                    purchaseOrderRoot.materialList = purchaseOrderRoot.materialList.concat(
                                materialsFromCtrl[j].materialName)
                }
                for (var k = 0; k < uomFromCtrl.length; k++) {
                    purchaseOrderRoot.uomList = purchaseOrderRoot.uomList.concat(
                                uomFromCtrl[k].uomName)
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
                        columns: [{
                                "label": "Material",
                                "width": 184,
                                "key": "material_name"
                            }, {
                                "label": "UOM",
                                "width": 120,
                                "key": "unit_of_measurement_name"
                            }, {
                                "label": "Quantity",
                                "width": 100,
                                "key": "quantity"
                            }, {
                                "label": "Amount",
                                "width": 100,
                                "key": "amount"
                            }, {
                                "label": "Tax Amount",
                                "width": 100,
                                "key": "tax_amount"
                            }, {
                                "label": "Tax With Holding",
                                "width": 230,
                                "key": "tax_with_holding"
                            }]

                        onRemoveRowChanged: {
                            // console.log("onRemoveRowChanged:", removedIndex.toString())
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
                    spacing: 1

                    CustomComboBox {
                        id: materialId
                        width: 184
                        height: 24
                        model: purchaseOrderRoot.materialList
                        currentIndex: 0
                    }

                    CustomComboBox {
                        id: unitOfMeasurementId
                        width: 120
                        height: 24
                        model: purchaseOrderRoot.uomList
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: quantityTextBox
                        placeholderText: "Quantity"
                        text: ""
                        color: "#323130"
                        width: 100
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: amountTextBox
                        placeholderText: "Amount"
                        text: ""
                        color: "#323130"
                        width: 100
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxAmountTextBox
                        placeholderText: "Tax Amount"
                        text: ""
                        color: "#323130"
                        width: 100
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxWithHoldingTextBox
                        placeholderText: "Tax With Holding"
                        text: ""
                        color: "#323130"
                        width: 210
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
                                    "vendor_id":String(vendorsFromCtrl[vendor.currentIndex].id),
                                    "material_id":String(materialsFromCtrl[materialId.currentIndex].id),
                                    "material_name": materialId.currentText,
                                    "unit_of_measurement_id": String(uomFromCtrl[unitOfMeasurementId.currentIndex].id),
                                    "unit_of_measurement_name": unitOfMeasurementId.currentText,
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

            // ------- VENDOR DROPDOWN --------
            // Text {
            //     id: vendorLabel
            //     text: "Select Vendor"
            //     color: "#323130"
            //     font.weight: 700
            //     font.pixelSize: 14
            //     font.family: "Segoe UI"
            //     topPadding: 10
            // }

            // CustomComboBox {
            //     id: vendorComboBox
            //     width: 200
            //     model: vendorList
            //     currentIndex: 0

            //     contentItem: Text {
            //         text: vendorComboBox.displayText
            //         leftPadding: 10
            //         verticalAlignment: Text.AlignVCenter
            //         color: "#323130"
            //         font.pixelSize: 14
            //     }

            //     onCurrentIndexChanged: {
            //         console.log("Vendor:", vendorComboBox.currentText)
            //     }
            // }
        }


        /*--------------------------------------
                Main Table
    --------------------------------------*/
        FHTable {
            height: 200
            leftPadding: 20
            model: purchaseOrderRoot.purchaseOrderList
            columns: [{
                    "label": "Vendor",
                    "width": 214,
                    "key": "vendorName"
                }, {
                    "label": "Material",
                    "width": 214,
                    "key": "materialName"
                }, {
                    "label": "Quantity",
                    "width": 180,
                    "key": "quantity"
                }, {
                    "label": "UOM",
                    "width": 170,
                    "key": "unitOfMeasurementName"
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
                }]
        }
    }

    Component.onCompleted: showList()
    onVisibleChanged: showList()

    function showList() {
        purchaseOrderRoot.purchaseOrderList = [];

        if (purchaseOrderRoot.visible) {
            purchaseOrderRoot.purchaseOrderList = purchaseOrderController.getPurchaseOrderList(isApproved)
        }
    }
}
