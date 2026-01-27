import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: purchaseOrderRoot
    width: parent.width
    padding: 10

    // Properties - Data from controllers
    property var vendorsFromCtrl: []
    property var materialsFromCtrl: []
    property var unitsOfMeasurementFromCtrl: []

    // Properties - Lists for UI
    property var purchaseOrderList: []
    property var vendorList: []
    property var materialList: []
    property var unitOfMeasurementList: []

    // Properties - Purchase order data
    property var purchaseOrderLineData: []
    property var purchaseOrderLineDataEdit: []

    // Properties - State management
    property var selectedData: null
    property string popupMode: "view"
    property bool isApproved: false

    // Controllers
    VendorController {
        id: vendorController
    }

    MaterialController {
        id: materialController
    }

    UnitOfMeasurementController {
        id: unitOfMeasurementController
    }

    PurchaseOrderController {
        id: purchaseOrderController
    }

    PurchaseOrderLineController {
        id: purchaseOrderLineController
    }

    /* ---------- Create Popup ---------- */
    FHPopup {
        id: newPurchaseOrderPopup
        popupWidth: 1000
        popupHeight: 600
        title: "Create Purchase Order"
        parent: Overlay.overlay

        onAcceptCallback: function () {
            if (purchaseOrderLineData.length > 0) {
                purchaseOrderLineController.create(
                            vendorsFromCtrl[vendorCombo.currentIndex].id,
                            purchaseOrderLineData
                            )
                // Reset
                purchaseOrderLineData = []
            }
            close()
        }

        onCancelCallback: function () {
            purchaseOrderLineData = []
            close()
        }

        onClosed: {
            showPurchaseOrderList()
        }

        onOpened: {
            // Load lists
            purchaseOrderLineData = []

            if (purchaseOrderRoot.visible) {
                vendorsFromCtrl = []
                materialsFromCtrl = []
                unitsOfMeasurementFromCtrl = []

                vendorsFromCtrl = vendorController.getVendorList(true)
                materialsFromCtrl = materialController.getMaterialList(true)
                unitsOfMeasurementFromCtrl = unitOfMeasurementController.getUOMList(true)

                // Clear current lists then populate
                purchaseOrderRoot.vendorList = []
                purchaseOrderRoot.materialList = []
                purchaseOrderRoot.unitOfMeasurementList = []

                for (var i = 0; i < vendorsFromCtrl.length; i++) {
                    purchaseOrderRoot.vendorList = purchaseOrderRoot.vendorList.concat(
                                vendorsFromCtrl[i].vendorName)
                }
                for (var j = 0; j < materialsFromCtrl.length; j++) {
                    purchaseOrderRoot.materialList = purchaseOrderRoot.materialList.concat(
                                materialsFromCtrl[j].materialName)
                }
                for (var k = 0; k < unitsOfMeasurementFromCtrl.length; k++) {
                    purchaseOrderRoot.unitOfMeasurementList = purchaseOrderRoot.unitOfMeasurementList.concat(
                                unitsOfMeasurementFromCtrl[k].uomName)
                }
            }
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Vendor Selection --------
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
                        id: vendorCombo
                        width: 600
                        model: vendorList
                        currentIndex: 0
                    }
                }
            }

            // -------- Purchase Order Line Items --------
            Text {
                id: purchaseOrderLineLabel
                text: "Purchase Order Items:"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            Rectangle {
                width: parent.width - 6
                height: 420
                color: "#EDF1F4"

                Column {
                    width: parent.width

                    FHTable{
                        id: popupTable
                        width: parent.width - 4
                        height: 300
                        leftPadding: 2
                        removeRow: true
                        model: purchaseOrderLineData
                        columns: [
                            {
                                "label": "Material",
                                "width": 180,
                                "key": "material_name"
                            },
                            {
                                "label": "Quantity",
                                "width": 180,
                                "key": "quantity"
                            },
                            {
                                "label": "Unit",
                                "width": 140,
                                "key": "unit_name"
                            },
                            {
                                "label": "Dollar Value",
                                "width": 140,
                                "key": "dollar_value"
                            },
                            {
                                "label": "Tax Amount",
                                "width": 140,
                                "key": "tax_amount"
                            },
                            {
                                "label": "Tax Withholding",
                                "width": 140,
                                "key": "tax_withholding"
                            }
                        ]
                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < purchaseOrderLineData.length) {
                                var temp = []
                                for (var i = 0; i < purchaseOrderLineData.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(purchaseOrderLineData[i])
                                    }
                                }
                                purchaseOrderLineData = temp
                            }
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
                        id: materialCombo
                        width: 150
                        height: 24
                        model: purchaseOrderRoot.materialList
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: quantityTextBox
                        placeholderText: "Quantity"
                        text: ""
                        color: "#323130"
                        width: 150
                        height: 24
                        topPadding: 1
                    }

                    CustomComboBox {
                        id: unitCombo
                        width: 150
                        height: 24
                        model: purchaseOrderRoot.unitOfMeasurementList
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: dollarValueTextBox
                        placeholderText: "Dollar Value"
                        text: ""
                        color: "#323130"
                        width: 150
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxAmountTextBox
                        placeholderText: "Tax Amount"
                        text: ""
                        color: "#323130"
                        width: 150
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxWithholdingTextBox
                        placeholderText: "Tax Withholding"
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

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true

                            onEntered: parent.color = "#f2f2f2"
                            onExited: parent.color = "transparent"

                            onClicked: {
                                var newElements = {
                                    "material_id": String(materialsFromCtrl[materialCombo.currentIndex].id),
                                    "material_name": materialCombo.currentValue,
                                    "quantity": quantityTextBox.text,
                                    "unit_of_measurement_id": String(unitsOfMeasurementFromCtrl[unitCombo.currentIndex].id),
                                    "unit_name": unitCombo.currentValue,
                                    "dollar_value": dollarValueTextBox.text,
                                    "tax_amount": taxAmountTextBox.text,
                                    "tax_withholding": taxWithholdingTextBox.text
                                }

                                // Append to purchaseOrderLineData
                                purchaseOrderLineData = purchaseOrderLineData.concat(newElements)

                                // Clear the inputs
                                quantityTextBox.text = ""
                                dollarValueTextBox.text = ""
                                taxAmountTextBox.text = ""
                                taxWithholdingTextBox.text = ""
                            }
                        }
                    }
                }
            }

            Item {
                width: 1
                height: 20
            }
        }
    }

    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: viewEditPopup
        popupWidth: 1000
        popupHeight: 600
        title: popupMode === "view" ? "View Purchase Order" : "Edit Purchase Order"

        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : ""
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png" : ""

        onAcceptCallback: function () {
            if (popupMode === "edit" && selectedData) {
                if (purchaseOrderLineDataEdit.length > 0) {
                    purchaseOrderLineController.update(
                                selectedData.id,
                                vendorsFromCtrl[vendorComboEdit.currentIndex].id,
                                purchaseOrderLineDataEdit
                                )
                    // Reset
                    purchaseOrderLineDataEdit = []
                    vendorComboEdit.currentIndex = -1
                }
                showPurchaseOrderList()
            }
        }

        onCancelCallback: function () {
            purchaseOrderLineDataEdit = []
        }

        onOpened: {
            // Load lists from controllers
            purchaseOrderLineDataEdit = []

            vendorsFromCtrl = []
            materialsFromCtrl = []
            unitsOfMeasurementFromCtrl = []

            vendorsFromCtrl = vendorController.getVendorList(true)
            materialsFromCtrl = materialController.getMaterialList(true)
            unitsOfMeasurementFromCtrl = unitOfMeasurementController.getUOMList(true)

            // Clear and populate lists
            var tempVendorList = []
            var tempMaterialList = []
            var tempUnitList = []

            for (var i = 0; i < vendorsFromCtrl.length; i++) {
                tempVendorList = tempVendorList.concat(vendorsFromCtrl[i].vendorName)
            }

            for (var j = 0; j < materialsFromCtrl.length; j++) {
                tempMaterialList = tempMaterialList.concat(materialsFromCtrl[j].materialName)
            }

            for (var k = 0; k < unitsOfMeasurementFromCtrl.length; k++) {
                tempUnitList = tempUnitList.concat(unitsOfMeasurementFromCtrl[k].uomName)
            }

            purchaseOrderRoot.vendorList = tempVendorList
            purchaseOrderRoot.materialList = tempMaterialList
            purchaseOrderRoot.unitOfMeasurementList = tempUnitList

            // Fill popup with selected data
            if (selectedData) {
                fillPopup()
            }
        }

        Column {
            width: parent.width
            spacing: 10

            // -------- Vendor Selection --------
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
                        id: vendorComboEdit
                        width: 500
                        model: vendorList
                        currentIndex: 0
                        enabled: popupMode === "edit"
                    }
                }
            }

            // -------- Purchase Order Line Items --------
            Text {
                id: purchaseOrderLineLabelEdit
                text: "Purchase Order Items:"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            Rectangle {
                width: parent.width - 6
                height: 420
                color: "#EDF1F4"

                Column {
                    width: parent.width

                    FHTable {
                        id: popupTableEdit
                        width: parent.width - 4
                        height: 300
                        leftPadding: 2
                        removeRow: popupMode === "edit"
                        model: purchaseOrderLineDataEdit
                        columns: [
                            {
                                "label": "Material",
                                "width": 200,
                                "key": "material_name"
                            },
                            {
                                "label": "Quantity",
                                "width": 130,
                                "key": "quantity"
                            },
                            {
                                "label": "Unit",
                                "width": 200,
                                "key": "unit_name"
                            },
                            {
                                "label": "Amount",
                                "width": 120,
                                "key": "dollar_value"
                            },
                            {
                                "label": "Tax Amount",
                                "width": 130,
                                "key": "tax_amount"
                            },
                            {
                                "label": "Tax Withholding",
                                "width": 150,
                                "key": "tax_withholding"
                            }
                        ]
                        onRemovedIndexChanged: {
                            if (removedIndex >= 0 && removedIndex < purchaseOrderLineDataEdit.length) {
                                var temp = []
                                for (var i = 0; i < purchaseOrderLineDataEdit.length; i++) {
                                    if (i !== removedIndex) {
                                        temp.push(purchaseOrderLineDataEdit[i])
                                    }
                                }
                                purchaseOrderLineDataEdit = temp
                            }
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width - 8
                height: 28
                color: "white"
                visible: popupMode === "edit"

                Row {
                    width: parent.width - 8
                    height: 28
                    leftPadding: 2
                    spacing: 6

                    CustomComboBox {
                        id: materialComboEdit
                        width: 150
                        height: 24
                        model: purchaseOrderRoot.materialList
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: quantityTextBoxEdit
                        placeholderText: "Quantity"
                        text: ""
                        color: "#323130"
                        width: 150
                        height: 24
                        topPadding: 1
                    }

                    CustomComboBox {
                        id: unitComboEdit
                        width: 150
                        height: 24
                        model: purchaseOrderRoot.unitOfMeasurementList
                        currentIndex: 0
                    }

                    CustomTextBox {
                        id: dollarValueTextBoxEdit
                        placeholderText: "Amount"
                        text: ""
                        color: "#323130"
                        width: 150
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxAmountTextBoxEdit
                        placeholderText: "Tax Amount"
                        text: ""
                        color: "#323130"
                        width: 150
                        height: 24
                        topPadding: 1
                    }

                    CustomTextBox {
                        id: taxWithholdingTextBoxEdit
                        placeholderText: "Tax Withholding"
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

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true

                            onEntered: parent.color = "#f2f2f2"
                            onExited: parent.color = "transparent"

                            onClicked: {
                                var newElements = {
                                    "material_id": String(materialsFromCtrl[materialComboEdit.currentIndex].id),
                                    "material_name": materialComboEdit.currentValue,
                                    "quantity": quantityTextBoxEdit.text,
                                    "unit_of_measurement_id": String(unitsOfMeasurementFromCtrl[unitComboEdit.currentIndex].id),
                                    "unit_name": unitComboEdit.currentValue,
                                    "dollar_value": dollarValueTextBoxEdit.text,
                                    "tax_amount": taxAmountTextBoxEdit.text,
                                    "tax_withholding": taxWithholdingTextBoxEdit.text
                                }

                                // Append to purchaseOrderLineDataEdit
                                purchaseOrderLineDataEdit = purchaseOrderLineDataEdit.concat(newElements)

                                // Clear inputs
                                quantityTextBoxEdit.text = ""
                                dollarValueTextBoxEdit.text = ""
                                taxAmountTextBoxEdit.text = ""
                                taxWithholdingTextBoxEdit.text = ""
                            }
                        }
                    }
                }
            }

            Item {
                width: 1
                height: 20
            }
        }
    }

    // -------- Header Row --------
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
                cursorShape: Qt.PointingHandCursor
                onClicked: newPurchaseOrderPopup.open()
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
                    showPurchaseOrderList()
                }
            }
        }
    }

    Rectangle {
        width: 100
        height: 40
        color: "#EDF1F4"
    }

    // -------- Main Table --------
    FHTable {
        height: 200
        leftPadding: 20
        model: purchaseOrderRoot.purchaseOrderList

        columns: [
            {
                "label": "Id",
                "width": 600,
                "key": "id"
            },
            {
                "label": "Vendor",
                "width": 700,
                "key": "vendorName"
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
    Component.onCompleted: showPurchaseOrderList()
    onVisibleChanged: showPurchaseOrderList()

    // -------- Functions --------
    function showPurchaseOrderList() {
        purchaseOrderRoot.purchaseOrderList = []

        if (!purchaseOrderRoot.visible)
            return

        purchaseOrderRoot.purchaseOrderList = purchaseOrderController.getPurchaseOrderList(isApproved)
    }

    function fillPopup() {
        if (!selectedData)
            return

        // Find and set vendor
        for (var i = 0; i < vendorsFromCtrl.length; i++) {
            if (vendorsFromCtrl[i].id === selectedData.vendorId) {
                vendorComboEdit.currentIndex = i
                break
            }
        }

        // Load line items
        var tempLines = []
        var allLines = purchaseOrderLineController.getPurchaseOrderLineList(isApproved)

        for (var j = 0; j < allLines.length; j++) {
            if (allLines[j].id === selectedData.id) {
                var lineItem = {
                    "material_id": String(allLines[j].materialId || ""),
                    "material_name": allLines[j].materialName || "",
                    "quantity": String(allLines[j].quantity || "0"),
                    "unit_of_measurement_id": String(allLines[j].unitOfMeasurementId || ""),
                    "unit_name": allLines[j].unitOfMeasurementName || "",
                    "dollar_value": String(allLines[j].amount || "0"),
                    "tax_amount": String(allLines[j].taxAmount || "0"),
                    "tax_withholding": String(allLines[j].taxWithHolding || "0")
                }
                tempLines.push(lineItem)
            }
        }

        purchaseOrderLineDataEdit = tempLines
    }
}
