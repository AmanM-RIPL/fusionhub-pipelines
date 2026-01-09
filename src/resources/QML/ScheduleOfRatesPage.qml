import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers

Column {
    id: scheduleOfRatesRoot
    width: parent.width
    padding: 10

    property var scheduleOfRatesList: []
    property bool isApproved: false

    property var scheduleSetupList: ["--","scheduleSetupList"]
    property var resourceList: ["--","resourceList"]
    property var costeList: ["--","costeList"]
    property var dataList: []

    ScheduleSetupController {
        id: scheduleSetupController
    }

    ScheduleOfRatesController {
        id: scheduleOfRatesController
    }

    FHPopup {
        id: newScheduleOfRatesPopup
        popupWidth: 700
        popupHeight: 650
        title: "Create Schedule Of Rate"
        parent: Overlay.overlay

        onAcceptCallback: function () {
            if (dataList.length > 0 && resourceList.length > 0) {
                scheduleOfRatesController.create(scheduleNameTextBox.text,
                                                 descriptionTextBox.text,
                                                 dataList, resourceList)

                scheduleNameTextBox.text = ""
                descriptionTextBox.text = ""
                dataList = []
                resourceList = []
            }
            showList()
        }

        onCancelCallback: function () {
            scheduleNameTextBox.text = ""
            dataList = []
            resourceList = []
        }

        onOpened: {
            dataList = []
            if (scheduleOfRatesRoot.visible) {
                //var scheduleSetupData = scheduleSetupController.getScheduleSetupList(true)
                var scheduleSetupData = scheduleSetupController.getSetupList(true)
                for (var i = 0; i < scheduleSetupData.length; i++) {
                    dataList.push(scheduleSetupData[i].scheduleSetupName)
                }
            }
        }

        Column {
            width: parent.width
            height: 300

            Text {
                id: scheduleNameLabel
                text: "Schedule Of Rate"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomTextBox {
                id: scheduleNameTextBox
                placeholderText: "SOR Name"
                text: ""
                color: "#323130"
                width: parent.width - 8
            }

            Text {
                id: costParemeterLabel
                text: "Select Schedule:"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            Rectangle {
                width: parent.width - 6
                height: parent.height//140
                color: "#EDF1F4"
                Column {

                    FHTable {
                        width: 644;//parent.width - 4
                        height: 130
                        leftPadding: 2
                        removeRow: true
                        model: dataList
                        columns: [{
                                "label": "Schedule Name",
                                "width": 215,
                                "key": "schedule_name"
                            }, {
                                "label": "Cost",
                                "width": 215,
                                "key": "cost"
                            }, {
                                "label": "Resource",
                                "width": 215,
                                "key": "resource"
                            }]

                        onRemoveRowChanged: {
                            //console.log("onRemoveRowChanged:", removedIndex.toString())
                            dataList.splice(removedIndex, 1)
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width - 8
                height: 20
                color: "white"

                Row {
                    width: parent.width - 8
                    height: 20
                    leftPadding: 2


                    CustomComboBox {
                        id: scheduleNameComboBox
                        width: 215
                        height: 22
                        model: scheduleSetupList
                        currentIndex: 0
                    }

                    CustomComboBox {
                        id: scheduleSetupComboBox
                        width: 215
                        height: 22
                        model: costeList
                        currentIndex: 0
                    }

                    CustomComboBox {
                        id: resourceForCostParam
                        width: 198
                        height: 22
                        model: resourceList
                        currentIndex: 0
                    }

                    CustomButton {
                        color: "transparent"
                        width: 20
                        height: 22
                        border.color: "#8080808C"
                        btnSource: "qrc:/resources/images/add.svg"
                        btnName: ""
                        btnNameColor: "blue"
                        anchors.verticalCenter: parent.verticalCenter

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true

                            onEntered: {
                                parent.color = "#f2f2f2"
                            }

                            onExited: {
                                parent.color = "transparent"
                            }

                            onClicked: {
                                var newElements = {
                                    "schedule_name": scheduleParamComboBox.currentText,
                                    "cost": scheduleSetupComboBox.currentText,
                                    "resource": resourceForCostParam.currentText
                                }

                                dataList = dataList.concat(newElements)
                            }
                        }
                    }
                }
            }
        }
    }

    Row {
        spacing: 20
        Text {
            text: "Schedule of Rate"
            color: "#000000"
            font.family: "Segoe UI"
            font.weight: 700
            font.pixelSize: 44
            leftPadding: 20
        }

        CustomButton {
            color: "#007AFF"
            width: 85
            height: 38
            radius: 4
            btnSource: "qrc:/resources/images/addWhite_icon.png"
            btnName: "New"
            btnNameColor: "white"
            anchors.verticalCenter: parent.verticalCenter

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    newScheduleOfRatesPopup.open()
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
        Text {
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
            width: 200
            currentIndex: 0

            onCurrentTextChanged: {
                if (approvalTypeComboBox.currentText === "Approved") {
                    isApproved = true
                } else {
                    isApproved = false
                }
                showList()
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
        model: scheduleOfRatesRoot.scheduleOfRatesList

        columns: [
            {
                "label": "SOR Name",
                "width": 300,
                "key": "scheduleOfRatesName"
            },
            {
                "label": "Global ID",
                "width": 200,
                "key": "globalId"
            },
            {
                "label": "Approval",
                "width": 100,
                "key": "approvalStatus"
            }
        ]
    }

    Component.onCompleted: {
        showList()
    }

    onVisibleChanged: {
        showList()
    }

    function showList() {
        if (!scheduleOfRatesRoot.visible) return;

        // Reset list every time
        scheduleOfRatesRoot.scheduleOfRatesList = [];

        // Fetch array from C++ controller
        var arr = scheduleOfRatesController.getScheduleOfRatesList(isApproved);

        // Add items
        scheduleOfRatesRoot.scheduleOfRatesList = arr;
    }
}
