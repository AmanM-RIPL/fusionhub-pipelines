import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: analyticsRoot
    width: parent.width
    padding: 10

    // ========== PROPERTIES - DATA ==========
    property var vendorTableList: []
    property var purchaseMaterialList: []
    // ========== PROPERTIES - DASHBOARD ==========
    property string totalStoreAmount: "$0"
    property string totalPurchaseExpense: "$0"
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
    // SECTION 1 :  KPI Cards + Bar Chart (left)  |  Filter Panel (right)
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
                            text: analyticsRoot.totalStoreAmount
                            font.pixelSize: 28; font.family: "Segoe UI"
                            font.weight: Font.Bold; color: "#1A1A2E"
                        }
                        Row {
                            spacing: 3
                            Text { text: "▲"; font.pixelSize: 10; color: analyticsRoot.successColor; anchors.verticalCenter: parent.verticalCenter }
                            Text { text: "50%"; font.pixelSize: 11; font.family: "Segoe UI"; color: analyticsRoot.successColor; anchors.verticalCenter: parent.verticalCenter }
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
                            text: analyticsRoot.totalPurchaseExpense
                            font.pixelSize: 28; font.family: "Segoe UI"
                            font.weight: Font.Bold; color: "#1A1A2E"
                        }
                        Row {
                            spacing: 3
                            Text { text: "▼"; font.pixelSize: 10; color: analyticsRoot.dangerColor; anchors.verticalCenter: parent.verticalCenter }
                            Text { text: "50%"; font.pixelSize: 11; font.family: "Segoe UI"; color: analyticsRoot.dangerColor; anchors.verticalCenter: parent.verticalCenter }
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
                        model: [analyticsRoot.chartMaxValue,
                            analyticsRoot.chartMaxValue * 0.75,
                            analyticsRoot.chartMaxValue * 0.5,
                            analyticsRoot.chartMaxValue * 0.25,
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
                            model: analyticsRoot.chartData
                            delegate: Item {
                                width: chartPlotArea.width / analyticsRoot.chartData.length
                                height: chartPlotArea.height
                                Rectangle {
                                    width: 20
                                    height: Math.max(
                                        4,
                                        (modelData.value / analyticsRoot.chartMaxValue)
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
            anchors.right: parent.right;   anchors.top: parent.top
            height: 160
            radius: 8; color: "#FFFFFF"
            border.color: "#E5E7EB"; border.width: 1

            Column {
                anchors.fill: parent; anchors.margins: 20; spacing: 20

                // Material dropdown
                Row {
                    width: parent.width; spacing: 10
                    Text {
                        text: "Material:"; font.pixelSize: 13; font.family: "Segoe UI"
                        color: analyticsRoot.labelColor; width: 70
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Rectangle {
                        width: parent.width - 80; height: 30
                        radius: 4; color: "#FAFAFA"
                        border.color: "#D1D5DB"; border.width: 1
                        Row {
                            anchors.fill: parent; anchors.leftMargin: 10; anchors.rightMargin: 8
                            Text {
                                text: analyticsRoot.selectedMaterial === "" ? "Select" : analyticsRoot.selectedMaterial
                                font.pixelSize: 12; font.family: "Segoe UI"
                                color: analyticsRoot.selectedMaterial === "" ? "#9CA3AF" : analyticsRoot.labelColor
                                anchors.verticalCenter: parent.verticalCenter
                                width: parent.width - 20; elide: Text.ElideRight
                            }
                            Text { text: "▾"; font.pixelSize: 20; color: "#6B7280"; anchors.verticalCenter: parent.verticalCenter }
                        }
                        MouseArea {
                            anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                            onClicked: materialDropMenu.open()
                        }
                        Menu {
                            id: materialDropMenu
                            Repeater {
                               model: analyticsRoot.purchaseMaterialList
                               Repeater {
                                   // Agar model string hai toh JSON.parse karein, warna direct list use karein
                                   model: {
                                       try {
                                           return analyticsRoot.purchaseMaterialList
                                       } catch(e) {
                                           return []
                                       }
                                   }

                                   delegate: MenuItem {
                                       // modelData ko safe tarike se access karein
                                       text: (modelData && modelData.materialName) ? modelData.materialName : "No Name"

                                       onTriggered: {
                                           if (modelData && modelData.materialName) {
                                               analyticsRoot.selectedMaterial = modelData.materialName
                                               loadDashboard()
                                           }
                                       }
                                   }
                               }
                            }
                        }
                    }
                }

                // Till Date dropdown
                Row {
                    width: parent.width; spacing: 10
                    Text {
                        text: "Till Date:"; font.pixelSize: 13; font.family: "Segoe UI"
                        color: analyticsRoot.labelColor; width: 70
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Rectangle {
                        width: parent.width - 80; height: 30
                        radius: 4; color: "#FAFAFA"
                        border.color: "#D1D5DB"; border.width: 1
                        Row {
                            anchors.fill: parent; anchors.leftMargin: 10; anchors.rightMargin: 8
                            Text {
                                text: analyticsRoot.selectedTillDate === "" ? "Select" : analyticsRoot.selectedTillDate
                                font.pixelSize: 12; font.family: "Segoe UI"
                                color: analyticsRoot.selectedTillDate === "" ? "#9CA3AF" : analyticsRoot.labelColor
                                anchors.verticalCenter: parent.verticalCenter
                                width: parent.width - 20; elide: Text.ElideRight
                            }
                            Text { text: "▾"; font.pixelSize: 20; color: "#6B7280"; anchors.verticalCenter: parent.verticalCenter }
                        }
                        MouseArea {
                            anchors.fill: parent; cursorShape: Qt.PointingHandCursor
                            onClicked: dateDropMenu.open()
                        }
                        Menu {
                            id: dateDropMenu
                            Repeater {
                                model: ["Jan 2024", "Feb 2024", "Mar 2024", "Apr 2024", "May 2024"]
                                delegate: MenuItem {
                                    text: modelData
                                    onTriggered: {
                                        analyticsRoot.selectedTillDate = modelData
                                        loadDashboard()
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
    // SECTION 2 : Vendor List Table
    // =====================================================================
    FHTable {
        id: vendorTable
        width: parent.width - 20
        x: 10
        height: 250
        model: analyticsRoot.vendorTableList
        columns: [
            { label: "Id",   width: 300, key: "id" },
            { label: "Vendor Name",        width: 400, key: "vendorName"     },
            { label: "Total Volume Purchased",    width: 300, key: "totalVolume"  },
            { label: "Avg Price",       width: 300, key: "avgPrice"     }
        ]
    }

    // ========== INITIALIZATION ==========
    Component.onCompleted: {

        // console.log("Analytics Page Loaded")

        // var total = purchaseOrderLineController.getTotalPurchaseExpense()
        // console.log("total:", total)

        // var list = purchaseOrderLineController.getMaterialExpenseList()
        // console.log("list:", JSON.stringify(list))
    }

    onVisibleChanged: {
        var total = purchaseOrderLineController.getTotalPurchaseExpense()
        console.log("total:", total)
        var list = purchaseOrderLineController.getMaterialExpenseList()
       // console.log("list:", JSON.stringify(list))
        loadMaterials()
        loadVendorTable()
       // if (visible) loadDashboard()
    }
    function loadMaterials() {
        var materials = purchaseOrderLineController.getPurchaseOrderLineMaterialList(true)
        console.log("materials",JSON.stringify(materials))
        var temporaryList = []

        for (var i = 0; i < materials.length; i++) {
            // Unique check
            var exists = temporaryList.some(item => item.materialId === materials[i].materialId)

            if (!exists && materials[i].materialName) {
                temporaryList.push({
                    "materialId": materials[i].id,
                    "materialName": materials[i].materialName
                })
            }
        }

        // Poori list assignment
        purchaseMaterialList = temporaryList
    }

    function loadVendorTable() {
        var rawData = purchaseOrderLineController.getPurchaseOrderLineVendorList(true);
        var summary = {};

        for (var i = 0; i < rawData.length; i++) {
            var d = rawData[i];
           // console.log("ddd",JSON.stringify(d))
            var vId = d.vendorId;

            if (!summary[vId]) {
                summary[vId] = {
                    "id": vId,
                    "vendorName": d.vendorName || "Unknown",
                    "totalVol": 0,
                    "totalAmt": 0
                };
            }
            summary[vId].totalVol += d.quantity;
            summary[vId].totalAmt += d.amount;
        }

        var finalModel = [];
        for (var key in summary) {
            var item = summary[key];
            finalModel.push({
                "id": item.id,
                "vendorName": item.vendorName,
                "totalVolume": item.totalVol.toFixed(2),
                "avgPrice": (item.totalVol > 0 ? (item.totalAmt / item.totalVol) : 0).toFixed(2)
            });
        }

        analyticsRoot.vendorTableList = finalModel;
    }
    // ========== DATA LOADING ==========
    // function loadDashboard() {

    //     var dashboard = analyticsController.getDashboardData(
    //         analyticsRoot.selectedMaterial,
    //         analyticsRoot.selectedTillDate
    //     )

    //     if (dashboard) {

    //         analyticsRoot.totalStoreAmount =
    //                 dashboard.totalStoreAmount || "$0"

    //         analyticsRoot.totalPurchaseExpense =
    //                 dashboard.totalPurchaseExpense || "$0"

    //         analyticsRoot.chartData =
    //                 dashboard.chartData || analyticsRoot.chartData

    //         analyticsRoot.chartMaxValue = 1

    //         for (var i = 0; i < analyticsRoot.chartData.length; i++) {
    //             if (analyticsRoot.chartData[i].value > analyticsRoot.chartMaxValue)
    //                 analyticsRoot.chartMaxValue = analyticsRoot.chartData[i].value
    //         }
    //     }

    //     analyticsRoot.vendorTableList =
    //             purchaseOrderLineController.getMaterialList(
    //                 analyticsRoot.selectedMaterial,
    //                 analyticsRoot.selectedTillDate
    //             ) || []
    // }

}
