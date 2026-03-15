import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: purchaseDashboardRoot
    width: parent.width
    padding: 10

    // ========== PROPERTIES - DATA ==========
    property var materialList: []
    property var purchaseMaterialList: []
    property var vendorTableList: []
    property bool isApproved: true
    property int tableBodyHeight: 200

    // ========== PROPERTIES - DASHBOARD ==========
    property string totalStoreAmount: "0"
    property string totalPurchaseExpense: "0"
    property string selectedMaterial: ""
    property string selectedTillDate: ""

    // ========== COLOR VARIABLES ==========
    property string labelColor:   "#323130"
    property string successColor: "#28A745"
    property string dangerColor:  "#DC3545"

    // ========== CHART DATA ==========
    property var chartData: [
        { label: "PO",     value: 0.9, barColor: "#4CAF50" },
        { label: "Indent", value: 0.5, barColor: "#F44336" },
        { label: "GRN",    value: 0.2, barColor: "#F44336" }
    ]
    property int chartMaxValue: 1

    // ========== CONTROLLER ==========

    PurchaseOrderLineController {
        id: purchaseOrderLineController
    }

    MaterialController {
        id: materialController
    }

    UnitOfMeasurementController {
        id: unitOfMeasurementController
    }

    // ========== HEADER ==========
    Row {
        spacing: 20
        Text {
            text: "Procurement Dashboard"
            color: "#000000"
            font.family: "Segoe UI"
            font.weight: 700
            font.pixelSize: 44
            leftPadding: 20
        }
    }

    Item { width: 1; height: 12 }

    // =====================================================================
    // SECTION 1 :  KPI Cards + Bar Chart (left)
    // =====================================================================
    Item {
        id: middleSection
        width: parent.width - 20
        height: 225
        x: 10

        //======= LEFT BLOCK =============================
        Item {
            id: leftBlock
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width * 0.58

            // KPI Cards
            Row {
                id: kpiRow
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                height: 90
                spacing: 12

                // Card 1 – Total Store Amount
                Rectangle {
                    width: (kpiRow.width - 12) / 2
                    height: parent.height
                    radius: 8; color: "#FFFFFF"
                    border.color: "#E5E7EB"; border.width: 1

                    Text {
                        anchors.top: parent.top; anchors.right: parent.right
                        anchors.topMargin: 8;    anchors.rightMargin: 10
                        text: "⋮"; font.pixelSize: 16; color: "#9CA3AF"
                    }
                    Column {
                        anchors { left: parent.left; top: parent.top; leftMargin: 14; topMargin: 10 }
                        spacing: 4
                        Text {
                            text: "TOTAL STORE AMOUNT"
                            font.pixelSize: 9; font.family: "Segoe UI"
                            font.weight: Font.DemiBold; color: "#6B7280"
                        }
                        Text {
                            text: "₹ " + purchaseDashboardRoot.totalStoreAmount
                            font.pixelSize: 28; font.family: "Segoe UI"
                            font.weight: Font.Bold; color: "#1A1A2E"
                        }
                        Row {
                            spacing: 3
                            Text { text: "▲"; font.pixelSize: 10; color: purchaseDashboardRoot.successColor; anchors.verticalCenter: parent.verticalCenter }
                            Text { text: "50%"; font.pixelSize: 11; font.family: "Segoe UI"; color: purchaseDashboardRoot.successColor; anchors.verticalCenter: parent.verticalCenter }
                        }
                    }
                }

                // Card 2 – Total Purchase Expense
                Rectangle {
                    width: (kpiRow.width - 12) / 2
                    height: parent.height
                    radius: 8; color: "#FFFFFF"
                    border.color: "#E5E7EB"; border.width: 1

                    Text {
                        anchors.top: parent.top; anchors.right: parent.right
                        anchors.topMargin: 8;    anchors.rightMargin: 10
                        text: "⋮"; font.pixelSize: 16; color: "#9CA3AF"
                    }
                    Column {
                        anchors { left: parent.left; top: parent.top; leftMargin: 14; topMargin: 10 }
                        spacing: 4
                        Text {
                            text: "TOTAL PURCHASE EXPENSE"
                            font.pixelSize: 9; font.family: "Segoe UI"
                            font.weight: Font.DemiBold; color: "#6B7280"
                        }
                        Text {
                            text: "₹ " +purchaseDashboardRoot.totalPurchaseExpense
                            font.pixelSize: 28; font.family: "Segoe UI"
                            font.weight: Font.Bold; color: "#1A1A2E"
                        }
                        Row {
                            spacing: 3
                            Text { text: "▼"; font.pixelSize: 10; color: purchaseDashboardRoot.dangerColor; anchors.verticalCenter: parent.verticalCenter }
                            Text { text: "50%"; font.pixelSize: 11; font.family: "Segoe UI"; color: purchaseDashboardRoot.dangerColor; anchors.verticalCenter: parent.verticalCenter }
                        }
                    }
                }
            }

            // Bar Chart Card
            Rectangle {
                anchors.top: kpiRow.bottom; anchors.topMargin: 12
                anchors.left: parent.left;  anchors.right: parent.right
                anchors.bottom: parent.bottom
                radius: 8; color: "#FFFFFF"
                border.color: "#E5E7EB"; border.width: 1
                clip: true

                Column {
                    anchors { left: parent.left; leftMargin: 8; top: parent.top; topMargin: 12 }
                    anchors { bottom: parent.bottom; bottomMargin: 28 }
                    spacing: 0
                    Repeater {
                        model: [purchaseDashboardRoot.chartMaxValue,
                            purchaseDashboardRoot.chartMaxValue * 0.75,
                            purchaseDashboardRoot.chartMaxValue * 0.5,
                            purchaseDashboardRoot.chartMaxValue * 0.25,
                            "0"]
                        delegate: Item {
                            width: 22
                            height: chartPlotArea.height / 4
                            Text { anchors.right: parent.right; anchors.top: parent.top; text: modelData; font.pixelSize: 9; color: "#9CA3AF" }
                        }
                    }
                }

                Item {
                    id: chartPlotArea
                    anchors { left: parent.left; leftMargin: 36; right: parent.right; rightMargin: 10 }
                    anchors { top: parent.top; topMargin: 12; bottom: parent.bottom; bottomMargin: 28 }

                    Repeater {
                        model: 5
                        delegate: Rectangle {
                            width: chartPlotArea.width; height: 1; color: "#E5E7EB"
                            y: index * (chartPlotArea.height / 4)
                        }
                    }

                    Row {
                        anchors.fill: parent; spacing: 0
                        Repeater {
                            model: purchaseDashboardRoot.chartData
                            delegate: Item {
                                width: chartPlotArea.width / purchaseDashboardRoot.chartData.length
                                height: chartPlotArea.height
                                Rectangle {
                                    width: 20
                                    height: Math.max(
                                                4,
                                                (modelData.value / purchaseDashboardRoot.chartMaxValue)
                                                * chartPlotArea.height
                                                )
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.bottom: parent.bottom
                                    radius: 3; color: modelData.barColor
                                }
                                Text {
                                    anchors.top: parent.bottom; anchors.topMargin: 5
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: modelData.label
                                    font.pixelSize: 10; font.family: "Segoe UI"; color: "#6B7280"
                                }
                            }
                        }
                    }
                }
            }
        }

        //RIGHT BLOCK
        Rectangle {
            anchors.left: leftBlock.right; anchors.leftMargin: 14
            anchors.rightMargin: 14
            anchors.right: parent.right;   anchors.top: parent.top
            height: 140
            radius: 8; color: "#FFFFFF"
            border.color: "#E5E7EB"; border.width: 1


            Column {
                anchors.fill: parent; anchors.margins: 20; spacing: 20

                // Material dropdown
                Row {
                    width: parent.width
                    spacing: 10

                    Text {
                        text: "Material:"
                        font.pixelSize: 13
                        font.family: "Segoe UI"
                        color: purchaseDashboardRoot.labelColor
                        width: 70
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Rectangle {
                        width: parent.width - 80
                        height: 30
                        radius: 4
                        color: "#FAFAFA"
                        border.color: "#D1D5DB"
                        border.width: 1

                        Row {
                            anchors.fill: parent
                            anchors.leftMargin: 10
                            anchors.rightMargin: 8

                            Text {
                                text: purchaseDashboardRoot.selectedMaterial === "" ? "Select" : purchaseDashboardRoot.selectedMaterial
                                font.pixelSize: 12
                                font.family: "Segoe UI"
                                color: purchaseDashboardRoot.selectedMaterial === "" ? "#9CA3AF" : purchaseDashboardRoot.labelColor
                                anchors.verticalCenter: parent.verticalCenter
                                width: parent.width - 20
                                elide: Text.ElideRight
                            }
                            Text {
                                text: "▾"
                                font.pixelSize: 20
                                color: "#6B7280"
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: materialDropMenu.open()
                        }

                        Menu {
                            id: materialDropMenu
                            width: parent.width
                            Repeater {
                                model: purchaseDashboardRoot.materialList

                                delegate: MenuItem {
                                    text: modelData.materialName ? modelData.materialName : "No Name"

                                    onTriggered: {
                                        if (modelData && modelData.id > 0) {
                                            purchaseDashboardRoot.selectedMaterial = modelData.materialName
                                            fillCharts(modelData.id)
                                            showTableList(modelData.id)
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }

    }

    Item { width: 1; height: 14 }

    // =====================================================================
    // SECTION 2 : Material List Table
    // =====================================================================
    FHTable {
        id: vendorTableList
        width: parent.width - 20
        x: 10
        tableBodyHeight: 200
        model: purchaseDashboardRoot.vendorTableList
        columns: [
            { label: "Id",   width: 300, key: "serialNo" },
            { label: "Vendor Name",        width: 400, key: "vendorName"     },
            { label: "Total Volume Purchased",    width: 300, key: "totalVolume"  },
            { label: "Avg Price",       width: 300, key: "avgPrice"     }
        ]
    }

    // ========== INITIALIZATION ==========
    Component.onCompleted: {
    }

    onVisibleChanged: {
        showMaterialList()
        // showTableVendorList()
    }

    function showMaterialList() {
        purchaseDashboardRoot.materialList = []
        if (!purchaseDashboardRoot.visible) return

        var materials = materialController.getMaterialList(isApproved)
        var unitOfMeasurementList = unitOfMeasurementController.getUOMList(true)

        for (var i = 0; i < materials.length; i++) {
            var unitName = ""
            for (var j = 0; j < unitOfMeasurementList.length; j++) {
                if (unitOfMeasurementList[j].id === materials[i].unitOfMeasurementId) {
                    unitName = unitOfMeasurementList[j].uomName
                    break
                }
            }
            materials[i].unitName = unitName
        }
        purchaseDashboardRoot.materialList = materials
    }


    function fillCharts(materialId) {
        if (!purchaseDashboardRoot.visible)
            return

        var result = purchaseOrderLineController.getDashboardData(materialId, isApproved)

        if (result.length === 0) return

        var data = result[0]

        chartData = [
                    { label: "Purchase Order", value: data.totalPO, barColor: "#4CAF50" },
                    { label: "Indent", value: data.totalIndent, barColor: "red" },
                    { label: "GRN", value: data.totalGRN, barColor: "orange" }
                ]

        chartMaxValue = Math.max(data.totalPO, data.totalIndent, data.totalGRN) * 1.2

        purchaseDashboardRoot.totalStoreAmount = data.totalStoreAmount
        purchaseDashboardRoot.totalPurchaseExpense = data.totalPurchaseExpense
    }

    function showTableList(materialId) {

        purchaseDashboardRoot.vendorTableList = []

        if (!purchaseDashboardRoot.visible) return
        var vendors = purchaseOrderLineController.getDashboardTableData(materialId, isApproved)
        for (var i = 0; i < vendors.length; i++) {
            vendors[i].serialNo = i + 1
        }
        purchaseDashboardRoot.vendorTableList = vendors
    }
}
