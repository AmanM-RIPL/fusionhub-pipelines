import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers;

Column {
    // anchors.fill: parent
    id: billOfQuantityRoot
    width: parent.width
    padding: 10

    property var billOfQuantityList: []
    property var billOfQuantityListForTable: []
    property var scheduleOfRatesList: []
    property var scheduleText: [] // text for dropdown

    property bool isApproved:false


    BillOfQuantityController {
        id: billOfQuantityController
    }

    ScheduleOfRatesController {
        id: scheduleOfRatesController
    }

    FHPopup {
        id: newBillOfQuantityPopup
        popupWidth: 500
        popupHeight: 350
        title: "New Bill Of Quantity"

        onAcceptCallback: function () {

            billOfQuantityController.create(billOfQuantityNameTextBox.text,
                                      billOfQuantityRoot.scheduleOfRatesList[scheduleOfRatesComboBox.currentIndex].id);

            billOfQuantityNameTextBox.text = "";
            scheduleOfRatesComboBox.currentIndex = -1;
            showList();
        }

        onCancelCallback: function () {
            billOfQuantityNameTextBox.text = "";
            scheduleOfRatesComboBox.currentIndex = -1;
        }

        Column {
            width: parent.width
            height: 200//parent.height //30 for each top bottom

            Text{
                id: billOfQuantityNameLabel
                text: "Bill Of Quantity"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomTextBox{
                id: billOfQuantityNameTextBox
                placeholderText: "Bill Of Quantity"
                text:""
                color: "#323130"
            }


            Text{
                id: scheduleLabel
                text: "Schedule Rate"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: scheduleOfRatesComboBox
                model: billOfQuantityRoot.scheduleText
            }
        }

    }

    Row {
        spacing: 20
        Text {
            text: "Bill Of Quantity"
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
                    newBillOfQuantityPopup.open();
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
        model: billOfQuantityRoot.billOfQuantityListForTable
        columns: [
            { label: "Id", width: 100, key: "id" },
            { label: "Name", width: 300, key: "description" },
            { label: "Schedule Rate", width: 500, key: "schedule_of_rates_id" }
        ]
    }


    Component.onCompleted: {
        showList();
    }

    onVisibleChanged: {
         showList();
    }

    function showList() {
        if (!billOfQuantityRoot.visible)
            return;

        // Fetch bill of quantity list
        billOfQuantityRoot.billOfQuantityList = billOfQuantityController.getBillOfQuantityList(isApproved);

        // Fetch schedule of rates list
        billOfQuantityRoot.scheduleOfRatesList = billOfQuantityController.getScheduleOfRatesList();


        // Prepare text list for combo box
        const scheduleNames = billOfQuantityRoot.scheduleOfRatesList.map(
            schedule => schedule.scheduleOfRatesName
        );
        billOfQuantityRoot.scheduleText = scheduleNames;

        // Prepare table data
        billOfQuantityRoot.billOfQuantityListForTable = billOfQuantityRoot.billOfQuantityList.map(
            bill => {
                const matchedSchedule = billOfQuantityRoot.scheduleOfRatesList.find(
                    s => Number(s.id) === Number(bill.scheduleOfRatesId)
                );
                return {
                    id: bill.id,
                    description: bill.description,
                    schedule_of_rates_id: matchedSchedule ? matchedSchedule.scheduleOfRatesName : "Unknown"
                };
            }
        );       
    }

}
