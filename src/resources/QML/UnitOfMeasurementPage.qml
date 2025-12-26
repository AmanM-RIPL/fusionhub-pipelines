import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers;

Column {
    // anchors.fill: parent
    id: unitOfMeasurementRoot
    width: parent.width
    padding: 10

    property var unitOfMeasurementList: []
    property var unitOfMeasurementText: []

    property var unitTypeText: [ "Weight", "Volume", "Count", "Length", "Area", "Time", "Temperature"]
    property var unitTypeList: [ "Weight", "Volume", "Count", "Length", "Area", "Time", "Temperature"]

    property bool isApproved:false

    property var selectedUom: null
    property string popupMode: "view"


    UnitOfMeasurementController {
        id: unitOfMeasurementController
    }


    /* ---------- View / Edit Popup ---------- */
    FHPopup {
        id: uomPopup
        popupWidth: 600
        popupHeight: 600
        title: popupMode === "view" ? "View Unit Of Measurement" : "Edit Unit Of Measurement"

        showAcceptButton: popupMode === "edit"
        buttonName: popupMode === "edit" ? "Update" : "Add"
        buttonSource: popupMode === "edit" ? "qrc:/resources/images/editWhite_icon.png"
                          : "qrc:/resources/images/addWhite_icon.png"

        onAcceptCallback: function () {

            if (popupMode === "edit" && selectedUom) {
                unitOfMeasurementController.update(
                            selectedUom.id,
                            nameField.text,
                            unitTypeList[typeCombo.currentIndex],
                            sqmField.text,
                            cubicField.text,
                            meterField.text,
                            kgField.text
                            )
                showList()
               // uomPopup.close()
            }
        }


        onCancelCallback: function () {
            conToSqmTextBox.text = "";
            conToCubicMeterTextBox.text = "";
            conToMeterTextBox.text = "";
            conToKgTextBox.text = "";
            uomNameBox.text = "";
            unitTypeComboBox.currentIndex = -1;
        }

        Column {
            width: parent.width
            height: 350//parent.height //30 for each top bottom

            Text{
                id: uomNameLabelE
                text: "UOM Name"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: nameField
                enabled: popupMode === "edit"
                placeholderText: "UOM Name"
                text:""
                color: "#323130"
            }

            Text{
                id: unitTypeLabelE
                text: "Unit Type"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomComboBox {
                id: typeCombo
                enabled: popupMode === "edit"
                model: unitOfMeasurementRoot.unitTypeText
            }

            Text{
                id: conToSqmLabelE
                text: "Conversion to sqm"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: sqmField
                enabled: popupMode === "edit"
                placeholderText: "Conversion to sqm"
                text:""
                color: "#323130"
            }


            Text{
                id: conToCubicMeterLabelE
                text: "Conversion to Cubic Meter"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: cubicField
                enabled: popupMode === "edit"
                placeholderText: "Conversion to Cubic Meter"
                text:""
                color: "#323130"
            }


            Text{
                id: conToMeterLabelE
                text: "Conversion to Meter"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: meterField
                enabled: popupMode === "edit"
                placeholderText: "Conversion to meter"
                text:""
                color: "#323130"
            }


            Text{
                id: conToKgLabelE
                text: "Conversion to kg"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: kgField
                enabled: popupMode === "edit"
                placeholderText: "Conversion to kg"
                text:""
                color: "#323130"
            }

        }

    }


    /* ----------Create Popup ---------- */
    FHPopup {
        id: newUnitOfMeasurementPopup
        popupWidth: 600
        popupHeight: 600
        title: "New Unit Of Measurement"

        onAcceptCallback: function () {
            unitOfMeasurementController.create( uomNameTextBox.text, unitOfMeasurementRoot.unitTypeList[unitTypeComboBox.currentIndex], conToSqmTextBox.text, conToCubicMeterTextBox.text,
                                               conToMeterTextBox.text,
                                               conToKgTextBox.text,
                                               //unitOfMeasurementRoot.unitTypeList[unitTypeComboBox.currentIndex].id
                                               );

            showList();

            // reset fields
            conToSqmTextBox.text = "";
            conToCubicMeterTextBox.text = "";
            conToMeterTextBox.text = "";
            conToKgTextBox.text = "";
            uomNameTextBox.text = "";
            unitTypeComboBox.currentIndex = -1;
        }


        onCancelCallback: function () {
            conToSqmTextBox.text = "";
            conToCubicMeterTextBox.text = "";
            conToMeterTextBox.text = "";
            conToKgTextBox.text = "";
            uomNameBox.text = "";
            unitTypeComboBox.currentIndex = -1;
        }

        Column {
            width: parent.width
            height: 350//parent.height //30 for each top bottom

            Text{
                id: uomNameLabel
                text: "UOM Name"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: uomNameTextBox
                placeholderText: "UOM Name"
                text:""
                color: "#323130"
            }

            Text{
                id: unitTypeLabel
                text: "Unit Type"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomComboBox {
                id: unitTypeComboBox
                model: unitOfMeasurementRoot.unitTypeText
            }

            Text{
                id: conToSqmLabel
                text: "Conversion to sqm"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: conToSqmTextBox
                placeholderText: "Conversion to sqm"
                text:""
                color: "#323130"
            }


            Text{
                id: conToCubicMeterLabel
                text: "Conversion to Cubic Meter"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: conToCubicMeterTextBox
                placeholderText: "Conversion to Cubic Meter"
                text:""
                color: "#323130"
            }


            Text{
                id: conToMeterLabel
                text: "Conversion to Meter"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: conToMeterTextBox
                placeholderText: "Conversion to meter"
                text:""
                color: "#323130"
            }


            Text{
                id: conToKgLabel
                text: "Conversion to kg"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: conToKgTextBox
                placeholderText: "Conversion to kg"
                text:""
                color: "#323130"
            }

        }

    }

    Row {
        spacing: 20
        Text {
            text: "Unit Of Measurement"
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
                    newUnitOfMeasurementPopup.open();
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



    MainTable {
        height: 300
        leftPadding: 20
        model: unitOfMeasurementRoot.unitOfMeasurementList
        columns: [
            { label: "Id", width: 100, key: "id" },
            { label: "Unit Name", width: 200, key: "uomName" },
            { label: "Unit Type", width: 200, key: "unitType" },
            { label: "Conversion to sqm", width: 200, key: "conversionToSqm" },
            { label: "Conversion to cubic meter", width: 200, key: "conversionToCubicMeter" },
            { label: "Conversion to meter", width: 200, key: "conversionToMeter" },
            { label: "Conversion to kilogram", width: 200, key: "conversionToKilogram" }
        ]

        onViewRequested: function(row) {
            popupMode = "view"
            selectedUom = row
            fillPopup()
            uomPopup.open()
        }

        onEditRequested: function(row) {
            popupMode = "edit"
            selectedUom = row
            fillPopup()
            uomPopup.open()
        }
    }

    Component.onCompleted: {
        showList();
    }

    onVisibleChanged: {
        showList();
    }

    function showList() {
        unitOfMeasurementRoot.unitOfMeasurementList = [];
        if (unitOfMeasurementRoot.visible) {
            unitOfMeasurementRoot.unitOfMeasurementList = unitOfMeasurementController.getUOMList(isApproved);
            // console.log("dd", JSON.stringify(unitOfMeasurementList));
        }
    }

    function fillPopup() {
        if (!selectedUom) return
        nameField.text = selectedUom.uomName
        typeCombo.currentIndex = unitTypeText.indexOf(selectedUom.unitType)
       // sqmField.text = selectedUom.conversionToSqm
        sqmField.text = selectedUom.conversionToSqm !== null
                             ? String(selectedUom.conversionToSqm)
                             : ""
        cubicField.text = selectedUom.conversionToCubicMeter
        meterField.text = selectedUom.conversionToMeter
        kgField.text = selectedUom.conversionToKilogram
    }
}




