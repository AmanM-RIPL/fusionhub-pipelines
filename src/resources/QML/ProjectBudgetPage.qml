import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers;

Column {
    id: projectBudgetRoot
    width: parent.width
    padding: 10

    property var projectBudgetList: []
    property var projectBudgetListForTable: []
    property var budgetHeadText: []
    property var budgetHeadList: []

    property bool isApproved:false


    ProjectBudgetController {
        id: projectBudgetController
    }

    BudgetHeadController
    {
        id:budgetHeadController
    }

    FHPopup {
        id: newProjectBudgetPopup
        popupWidth: 500
        popupHeight: 350
        title: "New Project Budget"

        onAcceptCallback: function () {

            projectBudgetController.create(projectBudgetNameTextBox.text,
                                      projectBudgetRoot.budgetHeadList[budgetHeadComboBox.currentIndex].id);

            projectBudgetNameTextBox.text = "";
            budgetHeadComboBox.currentIndex = -1;
            showList();
        }

        onCancelCallback: function () {
            projectBudgetNameTextBox.text = "";
            budgetHeadComboBox.currentIndex = -1;
        }

        Column {
            width: parent.width
            height: 200//parent.height //30 for each top bottom

            Text{
                id: projectBudgetNameLabel
                text: "Project Budget"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomTextBox{
                id: projectBudgetNameTextBox
                placeholderText: "Project Budget"
                text:""
                color: "#323130"
            }


            Text{
                id: budgetHeadLabel
                text: "Budget Head"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            CustomComboBox {
                id: budgetHeadComboBox
                model: projectBudgetRoot.budgetHeadText
            }
        }

    }

    Row {
        spacing: 20
        Text {
            text: "Project Budget"
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
                    newProjectBudgetPopup.open();
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
        model: projectBudgetRoot.projectBudgetListForTable
        columns: [
            { label: "Id", width: 100, key: "id" },
            { label: "Project Budget", width: 200, key: "projectBudgetName" },
            { label: "Budget Head", width: 200, key: "budgetHead" }
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
        if(projectBudgetRoot.visible){
            //projectBudget data
            projectBudgetList = projectBudgetController.getProjectBudgetList(isApproved);

            //budgetHead data
            budgetHeadList = projectBudgetController.getBudgetHeadList();

            const budgetHeadNames = budgetHeadList.map(budgetHead => budgetHead.description);

            budgetHeadText = budgetHeadNames;

            //projectBudget data for table
            projectBudgetListForTable = projectBudgetList.map((projectBudget, projectBudgetIndex) => {
                const budgetHead = budgetHeadList.filter(x => x.id === projectBudget.budgetHeadId);

                let budgetHeadText = "Unknown";
                if (budgetHead.length > 0)
                {
                    budgetHeadText = budgetHead[0].description;
                }

                return {
                    id: projectBudget.id,
                    projectBudgetName: projectBudget.dollarValue,
                    budgetHead: budgetHeadText
                };
            });
        }
    }
}
