import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import "utils"

Column {
    id: workDashboardRoot
    width: parent.width
    padding: 10

    // ========== PROPERTIES - DATA ==========
    property var vendorList: []
    property var workOrderTableList: []
    // property var selectedData: null
    property bool isApproved: true
    property int tableBodyHeight: 200


    // ========== PROPERTIES - DASHBOARD ==========

    property string selectedVendor: ""

    // ========== COLOR VARIABLES ==========
    property string labelColor:   "#323130"
    property string successColor: "#28A745"
    property string dangerColor:  "#DC3545"

    // ========== CHART DATA ==========
    property var chartData: []
    property int chartMaxValue: 1

    // ========== CONTROLLER ==========

    VendorController {
        id: vendorController
    }

    // ========== HEADER ==========
    Row {
        spacing: 20
        Text {
            text: "Works Dashboard"
            color: "#000000"
            font.family: "Segoe UI"
            font.weight: 700
            font.pixelSize: 44
            leftPadding: 20
        }
    }

    Item { width: 1; height: 12 }

    // =====================================================================
    // SECTION 1 : Bar Chart
    // =====================================================================
    Item {
        id: middleSection
        width: parent.width - 20
        height: 180
        x: 10

        //======= LEFT BLOCK =============================
        Item {
            id: leftBlock
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width * 0.58

            Rectangle {
                id: chartCard
                anchors.fill: parent
                radius: 8
                color: "#FFFFFF"
                border.color: "#E5E7EB"
                border.width: 1
                clip: true

                // Y-Axis Labels Section
                Column {
                    id: yAxisColumn
                    anchors {
                        left: parent.left; leftMargin: 8;
                        top: parent.top; topMargin: 20;
                        bottom: parent.bottom; bottomMargin: 40
                    }
                    width: 30
                    spacing: 0

                    Repeater {
                        model: [
                            workDashboardRoot.chartMaxValue,
                            workDashboardRoot.chartMaxValue * 0.75,
                            workDashboardRoot.chartMaxValue * 0.5,
                            workDashboardRoot.chartMaxValue * 0.25,
                            "0"
                        ]
                        delegate: Item {
                            width: parent.width
                            height: chartPlotArea.height / 4
                            Text {
                                anchors.right: parent.right
                                text: Math.round(modelData)
                                font.pixelSize: 10
                                font.family: "Segoe UI"
                                color: "#9CA3AF"
                            }
                        }
                    }
                }

                // Main Chart Plotting Area
                Item {
                    id: chartPlotArea
                    anchors {
                        left: yAxisColumn.right; leftMargin: 10;
                        right: parent.right; rightMargin: 20;
                        top: parent.top; topMargin: 20;
                        bottom: parent.bottom; bottomMargin: 40
                    }

                    // Background Grid Lines
                    Repeater {
                        model: 5
                        delegate: Rectangle {
                            width: chartPlotArea.width; height: 1; color: "#F3F4F6"
                            y: index * (chartPlotArea.height / 4)
                        }
                    }

                    // Bars Row
                    Row {
                        anchors.fill: parent
                        spacing: 0

                        Repeater {
                            model: workDashboardRoot.chartData
                            delegate: Item {
                                width: chartPlotArea.width / (workDashboardRoot.chartData.length || 1)
                                height: chartPlotArea.height

                                Rectangle {
                                    id: barRect
                                    width: Math.min(35, parent.width * 0.5)
                                    height: Math.max(2, (modelData.value / (workDashboardRoot.chartMaxValue || 1)) * chartPlotArea.height)
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    anchors.bottom: parent.bottom
                                    radius: 4
                                    color: modelData.barColor

                                    Behavior on height {
                                        NumberAnimation { duration: 600; easing.type: Easing.OutQuart }
                                    }
                                }

                                // Value on top of bar
                                Text {
                                    anchors.bottom: barRect.top
                                    anchors.bottomMargin: 5
                                    anchors.horizontalCenter: barRect.horizontalCenter
                                    text: modelData.value
                                    font.pixelSize: 11
                                    font.weight: Font.Bold
                                    color: "#374151"
                                }

                                // Bottom label
                                Text {
                                    anchors.top: parent.bottom
                                    anchors.topMargin: 10
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: modelData.label
                                    font.pixelSize: 11
                                    font.family: "Segoe UI"
                                    font.weight: Font.Medium
                                    color: "#4B5563"
                                }
                            }

                        }
                    }
                }
            }
        }
        //======  RIGHT BLOCK ===========================
        Rectangle {
            anchors.left: leftBlock.right; anchors.leftMargin: 14
            anchors.rightMargin: 14
            anchors.right: parent.right;   anchors.top: parent.top
            height: 140
            radius: 8; color: "#FFFFFF"
            border.color: "#E5E7EB"; border.width: 1


            Column {
                anchors.fill: parent; anchors.margins: 20; spacing: 20

                // Vendor dropdown
                Row {
                    width: parent.width
                    spacing: 10

                    Text {
                        text: "Vendor:"
                        font.pixelSize: 13
                        font.family: "Segoe UI"
                        color: workDashboardRoot.labelColor
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
                                text: workDashboardRoot.selectedVendor === "" ? "Select" : workDashboardRoot.selectedVendor
                                font.pixelSize: 12
                                font.family: "Segoe UI"
                                color: workDashboardRoot.selectedVendor === "" ? "#9CA3AF" : workDashboardRoot.labelColor
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
                            onClicked: {
                                // showVendorList()
                                vendorDropMenu.open()
                            }
                        }

                        Menu {
                            id: vendorDropMenu
                            width: parent.width
                            Repeater {
                                model: workDashboardRoot.vendorList

                                delegate: MenuItem {
                                    text: modelData.vendorName ? modelData.vendorName : "No Name"

                                    onTriggered: {
                                        workDashboardRoot.selectedVendor = modelData.vendorName
                                        fillCharts(modelData.id)
                                        showWorkOrderList(modelData.id)
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
        id: workOrderTableList
        width: parent.width - 20
        x: 10
        tableBodyHeight: 200
        model: workDashboardRoot.workOrderTableList

        columns: [
            { label: "Id", width: 300, key: "serialNo" },
            { label: "Work Order Description", width: 400, key: "vendorName"},
            { label: "Total Amount", width: 300, key: "totalAmount"},
            { label: "Total Billing", width: 300, key: "totalBilling"}
        ]
    }

    // ========== INITIALIZATION ==========
    Component.onCompleted: {
    }

    onVisibleChanged: {
        if (visible) {
            showVendorList()
        }
    }

    function showVendorList() {
        workDashboardRoot.vendorList = []
        if (!workDashboardRoot.visible) return
        var vendors = vendorController.getVendorList(isApproved)
        workDashboardRoot.vendorList = vendors
    }

    function fillCharts(vendorId) {

        if (!workDashboardRoot.visible)
            return

        var result = vendorController.getDashboardData(vendorId, isApproved)

        var workOrder = result[0].totalWorkOrderAmount
        var billing = result[0].totalBillingAmount
        var remaining = result[0].totalRemaining

        chartData = [
                    { label: "Work Order", value: workOrder, barColor: "#4CAF50" },
                    { label: "Billing", value: billing, barColor: "red" },
                    { label: "Remaining", value: remaining, barColor: "orange" }
                ]

        // Max value 20% set
        chartMaxValue = Math.max(workOrder, billing, remaining) * 1.2
    }

    function showWorkOrderList(vendorId) {

        workDashboardRoot.workOrderTableList = []

        if (!workDashboardRoot.visible) return

        var vendors = vendorController.getWorkOrderTableList(vendorId, isApproved)
        for (var i = 0; i < vendors.length; i++) {
            vendors[i].serialNo = i + 1
        }

        workDashboardRoot.workOrderTableList = vendors
    }
}
