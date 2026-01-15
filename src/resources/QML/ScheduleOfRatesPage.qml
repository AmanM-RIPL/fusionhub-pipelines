import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers
import QtQuick.Controls 2.15
import QtQuick.Dialogs

Column {
    id: scheduleOfRatesRoot
    width: parent.width
    padding: 10

    property var scheduleOfRatesList: []
    property bool isApproved: false

    property var scheduleSetupList: ["--","scheduleSetupList"]
    property var resourceList: ["--","resourceList"]
    property var costList: ["--","costList"]
    property var dataList: []

    property var dataMap: {"key": "", "value": ""}

    ScheduleSetupController {
        id: scheduleSetupController
    }

    ScheduleOfRatesController {
        id: scheduleOfRatesController
    }


    MessageDialog {
        id: confirmDialog
        title: "Confirm Action"
        text: "Do you want save as a draft?"
        buttons: MessageDialog.Yes | MessageDialog.No
        onAccepted: {
            newScheduleOfRatesPopup.close();
        }
        onRejected: {
            scheduleNameTextBox.text = ""
            dataList = []
            dataMap = ({})
        }
    }


    FHPopup {
        id: newScheduleOfRatesPopup
        popupWidth: 700
        popupHeight: 650
        title: "Create Schedule Of Rate"
        parent: Overlay.overlay       

        onAcceptCallback: function () {
            if (dataList.length > 0 /*&& resourceList.length > 0*/) {
                //scheduleOfRatesController.create(scheduleNameTextBox.text, dataList)
                scheduleOfRatesController.create(scheduleNameTextBox.text, dataMap)
                scheduleNameTextBox.text = ""
                dataList = []
                console.log("dataMap_Length:", Object.keys(dataMap).length)
                dataMap = ({})
            }
            showList()
        }


        onCancelCallback: function () {            
            //scheduleNameTextBox.text = ""
            //dataList = []
            let size = (dataMap) ? Object.keys(dataMap).length : 0;

            // Object.entries(dataMap).forEach(([key, value]) => {
            //     console.log("Key: " + key + " | Value: " + value);
            // });

            /*if(dataList.length > 0)
            {
                console.log("size:", size)
                confirmDialog.open();
            }*/
        }

        onOpened: {
            //dataList = []
            if (scheduleOfRatesRoot.visible) {
                scheduleSetupList = scheduleSetupController.getSetupList(true)
                var scheduleSetup = scheduleSetupList[scheduleNameComboBox.currentIndex];
                costList = scheduleSetup.vecCostParamDataDetails;
                resourceList = scheduleSetup.vecResourceParamDataDetails;

                dataMap.key = scheduleSetup.id;                
                if(dataMap[dataMap.key])
                {
                    dataList = dataMap[dataMap.key];
                }
                else
                {
                   dataList = [];
                }
            }
        }

        Column {
            width: parent.width
            height: 300
            //spacing: 5

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

            Rectangle {
                width: parent.width - 6
                height: 20
                color: "white"
            }

            Row {
                width: parent.width - 8
                height: 30
                leftPadding: 2

            Text {
                id: scheduleLabel
                text: "Select Schedule:"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                //topPadding: 10
            }
            /////
            CustomComboBox {
                id: scheduleNameComboBox
                width: 215
                height: 22

                model: scheduleSetupList
                textRole: "scheduleName"
                currentIndex: 0

                onCurrentIndexChanged: {
                    if (currentIndex >= 0) {
                        //dataList = [];
                        //var tempDataList = dataList;
                        //dataList = [];

                        var scheduleSetup = scheduleSetupList[currentIndex];
                        dataMap.key = scheduleSetup.id;
                        if(dataMap[dataMap.key])
                        {
                            dataList = dataMap[dataMap.key];
                        }
                        else
                        {
                           dataList = [];
                        }

                        costList = scheduleSetup.vecCostParamDataDetails;
                        resourceList = scheduleSetup.vecResourceParamDataDetails;
                    }
                }
            }
            ///
            }

            Rectangle {
                width: parent.width - 6
                height: parent.height-20//140
                color: "#EDF1F4"
                Column {

                    FHTable {
                        width: 644;//parent.width - 4
                        height: 130
                        leftPadding: 2
                       // removeRow: true
                        model: dataList
                        columns: [/*{
                                "label": "Schedule Name",
                                "width": 215,
                                "key": "scheduleSetup"
                            },*/
                            {
                                "label": "Cost",
                                //"width": 215,
                                "width": 322,
                                "key": "cost",

                            },
                            // {
                            //     "label": "Resource",
                            //     "width": 215,
                            //     "key": "resource"
                            // },
                            {
                                "label": "Value",
                                //"width": 215,
                                "width": 322,
                                "key": "value"
                            }

                        ]

                         onRemoveRowChanged: {
                        //     console.log("onRemoveRowChanged:", removedIndex.toString())
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
                        id: costComboBox
                        //width: 215
                        width: 322
                        height: 22
                        model: costList
                        textRole: "cost_param_name"
                        currentIndex: 0
                    }

                    // CustomComboBox {
                    //     id: resourceComboBox
                    //     width: 198
                    //     height: 22
                    //     model: resourceList
                    //     textRole: "resource_param_name"
                    //     currentIndex: 0
                    //     visible: false
                    // }

                    CustomTextBox {
                        id: costValueTextBox
                        placeholderText: "value"
                        text: ""
                        color: "#323130"
                       //width: 198
                        width: 305
                        height: 22

                        font.weight: 700
                        font.pixelSize: 10
                        font.family: "Segoe UI"
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
                                     "scheduleSetup": scheduleNameComboBox.currentText,
                                     "cost": costComboBox.currentText,
                                    // "resource": resourceComboBox.currentText
                                    "value": costValueTextBox.text,
                                    "uom": String(costList[costComboBox.currentIndex].type_of_bim_dimension)
                                }

                                dataList = dataList.concat(newElements)

                                dataMap[dataMap.key] = dataList;
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
                "label": "Global ID",
                "width": 200,
                "key": "globalId"
            },
            {
                "label": "SOR Name",
                "width": 300,
                "key": "scheduleOfRatesName"
            }//,
            // {
            //     "label": "Approval",
            //     "width": 100,
            //     "key": "approvalStatus"
            // }
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
