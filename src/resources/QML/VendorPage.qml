import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers;

Column {
    // anchors.fill: parent
    id: vendorRoot
    width: parent.width
    padding: 10

    property var vendorList: []
    property bool isApproved:false    

    VendorController {
        id: vendorController       
    }

    FHPopup {
        id: newVendorPopup
        popupWidth: 500
        popupHeight: 500
        title: "New Vendor"

        onAcceptCallback: function () {                 

            vendorController.create(vendorNameTextBox.text, vendorAddressTextBox.text, vendorContactPersonTextBox.text,
                                    vendorMobileTextBox.text, vendorEmailTextBox.text);

            vendorNameTextBox.text = "";
            vendorAddressTextBox.text = "";
            vendorContactPersonTextBox.text = "";
            vendorMobileTextBox.text = "";
            vendorEmailTextBox.text = "";

            showList();
        }

        onCancelCallback: function () {
            vendorNameTextBox.text = "";
            vendorAddressTextBox.text = "";
            vendorContactPersonTextBox.text = "";
            vendorMobileTextBox.text = "";
            vendorEmailTextBox.text = "";            
        }

        Column {
            width: parent.width
            height: 300//parent.height //30 for each top bottom

            Text{
                id: vendorNameLabel
                text: "Vendor Name"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }


            CustomTextBox{
                id: vendorNameTextBox
                placeholderText: "Vendor Name"
                text:""
                color: "#323130"
            }

            Text{
                id: vendorAddressLabel
                text: "Vendor Address"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }


            CustomTextBox{
                id: vendorAddressTextBox
                placeholderText: "Vendor Address"
                text:""
                color: "#323130"
            }

            Text{
                id: vendorContactPersonLabel
                text: "Vendor Contact Person"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }


            CustomTextBox{
                id: vendorContactPersonTextBox
                placeholderText: "Vendor Contact Person"
                text:""
                color: "#323130"
            }

            Text{
                id: vendorMobileLabel
                text: "Vendor Mobile"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }


            CustomTextBox{
                id: vendorMobileTextBox
                placeholderText: "Vendor Mobile"
                text:""
                color: "#323130"
            }

            Text{
                id: vendorEmailLabel
                text: "Vendor Email"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }


            CustomTextBox{
                id: vendorEmailTextBox
                placeholderText: "Vendor Email"
                text:""
                color: "#323130"
            }
        }
    }

    Row {
        spacing: 20
        Text {
            text: "Vendor"
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
                    newVendorPopup.open();
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
                    isApproved = true;
                    console.log("Vendor::Selected text1:", approvalTypeComboBox.currentText)
                }
                else{
                    isApproved = false;
                    console.log("Vendor::Selected text2:", approvalTypeComboBox.currentText)
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
        model: vendorRoot.vendorList
        columns: [
            { label: "Id", width: 100, key: "id" },
            { label: "Name", width: 200, key: "vendorName" },
            { label: "Address", width: 200, key: "vendorAddress" },
            { label: "Contact Person", width: 200, key: "vendorContactPerson" },
            { label: "Mobile", width: 200, key: "vendorMobile" },
            { label: "Email", width: 200, key: "vendorEmail" },
        ]
    }

    Component.onCompleted: {        
        showList();
    }

    onVisibleChanged: {
        showList();
    }

    function showList(){
        vendorRoot.vendorList = vendorController.getVendorList(isApproved);
    }
}
