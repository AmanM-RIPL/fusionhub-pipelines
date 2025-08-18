import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0

Column {
    // anchors.fill: parent
    id: budgetHeadRoot
    width: parent.width
    padding: 10

    property var budgetHeadList: []
    // property alias visible: budgetHeadRoot.visible

    FHPopup {
        id: newBudgetHeadPopup
        popupWidth: 500
        popupHeight: 200
        title: "New Budget Head"

        onAcceptCallback: function () {
            // Create BudgetHead object dynamically
            let budgetHead = Qt.createQmlObject('import com.fh.models 1.0; BudgetHead {}', parent);
            budgetHead.description = descriptionTextBox.text;
            budgetHead.globalId = "";
            budgetHead.approvalStatus = true;

            budgetHeadRepository.saveQML(budgetHead);
            descriptionTextBox.text = "";

            budgetHeadRoot.budgetHeadList = budgetHeadRepository.findAllQML();
        }

        onCancelCallback: function () {
            descriptionTextBox.text = "";
        }

        Column {
            width: parent.width
            height: parent.height //30 for each top bottom

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

    FHTable {
        height: 200
        leftPadding: 20
        model: budgetHeadRoot.budgetHeadList
        columns: [
            { label: "Id", width: 100, key: "id" },
            { label: "Description", width: 200, key: "description" },
        ]
    }

    Component.onCompleted: {
        budgetHeadList = budgetHeadRepository.findAllQML();
    }
}
