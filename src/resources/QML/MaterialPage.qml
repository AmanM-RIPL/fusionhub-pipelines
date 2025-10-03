import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers;

Column {
    // anchors.fill: parent
    id: materialRoot
    width: parent.width
    padding: 10

    property var materialList: []
    property var materialListForTable: [] // here we add UOM name rather than id
    property var unitOfMeasurementText: [] // text for dropdown
    property var unitOfMeasurementList: [] // raw data of uom

    property bool isApproved:false


    MaterialController {
        id: materialController
    }

    UnitOfMeasurementController
    {
        id:unitOfMeasurementController
    }

    FHPopup {
        id: newMaterialPopup
        popupWidth: 500
        popupHeight: 350
        title: "New Material"

        onAcceptCallback: function () {

            materialController.create(materialNameTextBox.text, categoryComboBox.currentText,
                                      materialRoot.unitOfMeasurementList[unitOfMeasurementComboBox.currentIndex].id);

            materialNameTextBox.text = "";
            categoryComboBox.currentIndex = -1;
            unitOfMeasurementComboBox.currentIndex = -1;
            showList();
        }

        onCancelCallback: function () {
            materialNameTextBox.text = "";
            categoryComboBox.currentIndex = -1;
            unitOfMeasurementComboBox.currentIndex = -1;
        }

        Column {
            width: parent.width
            height: 200//parent.height //30 for each top bottom

            Text{
                id: materialNameLabel
                text: "Material Name"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomTextBox{
                id: materialNameTextBox
                placeholderText: "Material Name"
                text:""
                color: "#323130"
            }


            Text{
                id: categoryLabel
                text: "Category"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: categoryComboBox
                model: ["Raw Material", "Non-Construction", "Finished Good"]
            }


            Text{
                id: unitOfMeasurementLabel
                text: "Unit of Measurement"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: unitOfMeasurementComboBox
                model: materialRoot.unitOfMeasurementText
            }
        }

    }

    Row {
        spacing: 20
        Text {
            text: "Material"
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
                    newMaterialPopup.open();
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
        model: materialRoot.materialListForTable
        columns: [
            { label: "Id", width: 100, key: "id" },
            { label: "Name", width: 200, key: "materialName" },
            { label: "Category", width: 200, key: "category" },
            { label: "Unit Of Measurement", width: 200, key: "unitOfMeasurement" }
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
        //material data
        materialList = materialController.getMaterialList(isApproved);

        //uom data
        unitOfMeasurementList = materialController.getUOMList();

        const uomNames = unitOfMeasurementList.map(uom => uom.uomName);
        unitOfMeasurementText = uomNames;

        //material data for table
        materialListForTable = materialList.map((material, materialIndex) => {
            const uom = unitOfMeasurementList.filter(x => x.id === material.unitOfMeasurementId);
            let uomText = "Unknown";
            if (uom.length > 0)
            {
                uomText = uom[0].uomName;
            }

            return {
                id: material.id,
                materialName: material.materialName,
                category: material.category,
                unitOfMeasurement: uomText
            };
        });
    }
}
