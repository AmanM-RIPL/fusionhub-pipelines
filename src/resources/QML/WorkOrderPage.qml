import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers;

Column {
    id: workOrderRoot
    width: parent.width
    padding: 10

    property var workOrderList: []
    property var workOrderListForTable: [] // here we add UOM name rather than id
    property var vendorText: [] // text for dropdown
    property var vendorList: [] // raw data of vendor

    property bool isApproved:false


    WorkOrderController {
        id: workOrderController
    }

    VendorController
    {
        id:vendorController
    }

    FHPopup {
        id: newWorkOrderPopup
        popupWidth: 500
        popupHeight: 350
        title: "New WorkOrder"

        onAcceptCallback: function () {

            workOrderController.create(workOrderNameTextBox.text,
                                      workOrderRoot.vendorList[vendorComboBox.currentIndex].id);

            workOrderNameTextBox.text = "";
            vendorComboBox.currentIndex = -1;
            showList();
        }

        onCancelCallback: function () {
            workOrderNameTextBox.text = "";
            vendorComboBox.currentIndex = -1;
        }

        Column {
            width: parent.width
            height: 200//parent.height //30 for each top bottom

            Text{
                id: workOrderNameLabel
                text: "WorkOrder Name"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomTextBox{
                id: workOrderNameTextBox
                placeholderText: "WorkOrder Name"
                text:""
                color: "#323130"
            }


            Text{
                id: vendorLabel
                text: "Vendor"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: vendorComboBox
                model: workOrderRoot.vendorText
            }
        }

    }

    Row {
        spacing: 20
        Text {
            text: "Work Order"
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
                    newWorkOrderPopup.open();
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

            onCurrentTextChanged: {
                if(approvalTypeComboBox.currentText === "Approved"){
                    isApproved = true
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
        model: workOrderRoot.workOrderListForTable
        columns: [
            { label: "Id", width: 100, key: "id" },
            { label: "Work Order", width: 400, key: "workOrderName" },
            { label: "Vendor", width: 400, key: "vendor" }
        ]
    }

    Component.onCompleted: {
        showList();
    }

    onVisibleChanged: {
         showList();
    }

    function showList()
    {
        if(workOrderRoot.visible){
            //workOrder data
            workOrderList = workOrderController.getWorkOrderList(isApproved);

            //vendor data
            vendorList = workOrderController.getVendorList();

            const vendorNames = vendorList.map(vendor => vendor.vendorName);
            vendorText = vendorNames;

            //workOrder data for table
            workOrderListForTable = workOrderList.map((workOrder, workOrderIndex) => {
                const vendor = vendorList.filter(x => x.id === workOrder.vendorId);
                let vendorText = "Unknown";
                if (vendor.length > 0)
                {
                    vendorText = vendor[0].vendorName;
                }

                return {
                    id: workOrder.id,
                    workOrderName: workOrder.workOrderName,
                    vendor: vendorText
                };
            });
        }
    }
}
