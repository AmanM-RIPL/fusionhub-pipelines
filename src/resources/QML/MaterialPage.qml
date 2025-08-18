import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0

Column {
    // anchors.fill: parent
    id: materialRoot
    width: parent.width
    padding: 10

    property var materialList: []
    property var materialListForTable: [] // here we add UOM name rather than id
    property var unitOfMeasurementText: [] // text for dropdown
    property var unitOfMeasurementList: [] // raw data of uom

    FHPopup {
        id: newMaterialPopup
        popupWidth: 500
        popupHeight: 350
        title: "New Material"

        onAcceptCallback: function () {
            // Create Material object dynamically
            let material = Qt.createQmlObject('import com.fh.models 1.0; Material {}', parent);
            material.materialName = materialNameTextBox.text;
            material.globalId = "";
            material.approvalStatus = true;
            material.category = categoryComboBox.currentText;
            material.unitOfMeasurementId = materialRoot.unitOfMeasurementList[unitOfMeasurementComboBox.currentIndex].id;

            materialRepository.saveQML(material);

            materialNameTextBox.text = "";
            categoryComboBox.currentIndex = -1;
            unitOfMeasurementComboBox.currentIndex = -1;

            materialRoot.materialList = materialRepository.findAllQML();

            // material data for table
            materialRoot.materialListForTable = materialRoot.materialList.map((material, materialIndex) => {
                const uom = materialRoot.unitOfMeasurementList.filter(x => x.id === material.unitOfMeasurementId);
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

        onCancelCallback: function () {
            materialNameTextBox.text = "";
            categoryComboBox.currentIndex = -1;
            unitOfMeasurementComboBox.currentIndex = -1;
        }

        Column {
            width: parent.width
            height: parent.height //30 for each top bottom

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

        // material data
        materialList = materialRepository.findAllQML();


        // uom data
        unitOfMeasurementList = unitOfMeasurementRepository.findAllQML();

        const uomNames = unitOfMeasurementList.map(uom => uom.uomName);
        unitOfMeasurementText = uomNames;

        // material data for table
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
