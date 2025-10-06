import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers;



Column {
    // anchors.fill: parent
    id: budgetHeadRoot
    width: parent.width
    padding: 10

    property var budgetHeadList: []  
    property bool isApproved:false

    BudgetHeadController {
        id: budgetheadController
    }   


    FHPopup {
        id: newBudgetHeadPopup
        popupWidth: 500
        popupHeight: 230
        title: "New Budget Head"

        onAcceptCallback: function () {

            budgetheadController.create(descriptionTextBox.text);
            descriptionTextBox.text = "";           
            showList();
        }

        onCancelCallback: function () {
            descriptionTextBox.text = "";
        }

        Column {
            width: parent.width
            height: 75//parent.height //30 for each top bottom

            Text{
                id: desciptionLabel
                text: "Description"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }


            CustomTextBox{
                id: descriptionTextBox
                placeholderText: "Description"
                text:""
                color: "#323130"
            }
        }
    }

    Row {
        spacing: 20
        Text {
            text: "Budget Head"
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
                    newBudgetHeadPopup.open();
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
        id: ftabl
        height: 200
        leftPadding: 20        
        model: budgetHeadRoot.budgetHeadList
        columns: [
            { label: "Id", width: 100, key: "id" },
            { label: "Description", width: 500, key: "description" },
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
        if(budgetHeadRoot.visible){
            budgetHeadRoot.budgetHeadList = budgetheadController.getBudgetHeadList(isApproved);
        }
    }
}


