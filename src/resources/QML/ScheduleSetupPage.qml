import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0
import com.fh.controllers;

Column {
    // anchors.fill: parent
    id: scheduleSetupRoot
    width: parent.width
    padding: 10

    property var scheduleSetupList: []
    property bool isApproved:false

    property var costParameterList: []
    property var resourceParameterList: []
    property var typeOfBimDimensionList:["Nos", "cubic mtr", "kg", "meter"]
    property var purchaseMaterialList:["--", "M-30 Steel", "M-20 Steel", "Gravel", "Bamboo", "Capstones", "Cement"]

    MaterialController {
        id: materialController
    }

    ScheduleSetupController {
        id: scheduleSetupController
    }
    ProjectController{
        id:projController
    }

    FHPopup {
        id: newScheduleSetupPopup
        popupWidth: 700
        popupHeight: 650
        title: "Create Setup"
        parent: Overlay.overlay

        onAcceptCallback: function () {
            if(costParameterList.length > 0 && resourceParameterList.length > 0)
            {
                scheduleSetupController.create(scheduleNameTextBox.text, descriptionTextBox.text,
                                               costParameterList, resourceParameterList);

                scheduleNameTextBox.text = "";
                descriptionTextBox.text = "";
                costParameterList = [];
                resourceParameterList = [];
            }
            showList();
        }

        onCancelCallback: function () {
            scheduleNameTextBox.text = "";
            descriptionTextBox.text = "";
            costParameterList = [];
            resourceParameterList = [];
        }

        onOpened: {
             purchaseMaterialList = [];
            if(scheduleSetupRoot.visible){
                var materialList = materialController.getMaterialList(true);
                for (var i = 0; i < materialList.length; i++)
                {
                    purchaseMaterialList = purchaseMaterialList.concat(materialList[i].materialName)
                }
            }
        }


        Column {
            width: parent.width
            height: 300//parent.height //30 for each top bottom

            Text{
                id: scheduleNameLabel
                text: "Schedule Name"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomTextBox{
                id: scheduleNameTextBox
                placeholderText: "Schedule Name"
                text:""
                color: "#323130"
                width: parent.width-8
            }


            Text{
                id: descriptionLabel
                text: "Description"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }
            CustomTextBox{
                id: descriptionTextBox
                placeholderText: "Description"
                text:""
                color: "#323130"
                width: parent.width-8
            }

            Text{
                id: costParemeterLabel
                text: "Cost Parameter:"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            Rectangle {
                width: parent.width - 6
                height: 140
                color: "#EDF1F4"
                Column{

                    FHTable {
                        width: parent.width - 4
                        height: 300
                        leftPadding: 2
                        removeRow: true
                        model: costParameterList
                        columns: [
                            { label: "Cost Parameter Name", width: 214, key: "cost_param_name" },
                            { label: "Purchase Material", width: 214, key: "purchase_material" },
                            { label: "Type of BIM Dimension", width: 214, key: "type_of_bim_dimension" },
                        ]


                        onRemoveRowChanged: {
                          //  console.log("onRemoveRowChanged:", removedIndex.toString())
                            costParameterList.splice(removedIndex, 1)
                        }
                    }
                }
            }


            Rectangle {
                width: parent.width-8
                height: 20
                color:"white"

                Row{
                    width: parent.width-8
                    height: 20
                    leftPadding: 2

                    CustomTextBox{
                        id: costParamNameTextBox
                        placeholderText: "Cost Parameter Name"
                        text:""
                        color: "#323130"
                        width: 214
                        height: 22
                        topPadding:1
                    }


                    CustomComboBox{
                        id: purchaseMaterial
                        width: 214
                        height: 22
                        model: purchaseMaterialList
                        currentIndex: 0
                    }


                    CustomComboBox{
                        id: typeOfBimDimensionForCostParam
                        width: 198
                        height: 22
                        model: typeOfBimDimensionList
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


                        MouseArea{
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true

                            onEntered: {
                                parent.color = "#f2f2f2";
                            }

                            onExited: {
                                parent.color = "transparent";
                            }


                            onClicked: {
                                var newElements = {
                                    cost_param_name: costParamNameTextBox.text,
                                    purchase_material: purchaseMaterial.currentText,
                                    type_of_bim_dimension: typeOfBimDimensionForCostParam.currentText
                                };

                                costParameterList = costParameterList.concat(newElements);
                                costParamNameTextBox.text = "";
                            }
                        }
                    }
                }
            }


            Text{
                id: resourceParemeterLabel
                text: "Resource Parameter:"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
                topPadding: 10
            }

            Rectangle {
                width: parent.width-6
                height: 140
                color: "#EDF1F4"
                Column{
                    FHTable {
                        width: parent.width - 4
                        height: 300
                        leftPadding: 2
                        removeRow: true
                        model: resourceParameterList
                        columns: [
                            { label: "Resource Parameter Name", width: 322, key: "resource_param_name" },
                            { label: "Type of BIM Dimension", width: 322, key: "type_of_bim_dimension" },
                        ]


                        onRemoveRowChanged: {
                            console.log("onRemoveRowChanged2:", removedIndex.toString())
                            resourceParameterList.splice(removedIndex, 1)
                        }
                    }
                }
            }

            Rectangle {
                width: parent.width-8
                height: 20
                //color: "#EDF1F4"
                color:"white"
                Row{
                    width: parent.width-8
                    height: 20
                    leftPadding: 2

                    CustomTextBox{
                        id: resourceParamNameTextBox
                        placeholderText: "Resource Parameter Name"
                        text:""
                        color: "#323130"
                        width: 322
                        height: 22
                        topPadding:1
                    }

                    CustomComboBox{
                        id:typeOfBimDimensionForResourceParam
                        model: typeOfBimDimensionList
                        width: 304
                        height: 22
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

                        MouseArea{
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            hoverEnabled: true

                            onEntered: {
                                parent.color = "#f2f2f2";
                            }

                            onExited: {
                                parent.color = "transparent";
                            }

                            onClicked: {
                                var newElements = {
                                    resource_param_name: resourceParamNameTextBox.text,
                                    type_of_bim_dimension: typeOfBimDimensionForResourceParam.currentText
                                };

                                resourceParameterList = resourceParameterList.concat(newElements);
                                resourceParamNameTextBox.text = "";
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
            text: "Schedule Setup"
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

                    newScheduleSetupPopup.open();
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
            currentIndex: 0

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
        height: 200
        leftPadding: 20
        model: scheduleSetupRoot.scheduleSetupList
        columns: [
            { label: "Schedule Description", width: 250, key: "description" },
            { label: "# of Cost Parameters", width: 250, key: "costParameter" },
            { label: "# of Resource Parameters", width: 250, key: "resourceParameter" },
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
        // var lastSyncdOn =   projController.getLastSyncedOn();
        // console.log("lastSyncedOn:", String(lastSyncdOn));

        // var msSinceEpoch = new Date().getTime()
        // projController.updateLastSyncedOn(msSinceEpoch);
        // var updatedSyncedOn =   projController.getLastSyncedOn();
        // console.log("updatedSyncedOn:", String(updatedSyncedOn));

        scheduleSetupRoot.scheduleSetupList = [];
        if(scheduleSetupRoot.visible){
            var arr = scheduleSetupController.getSetupList(isApproved);
            scheduleSetupRoot.scheduleSetupList = scheduleSetupRoot.scheduleSetupList.concat(arr);
        }
    }


    /*
    function createJson(arr)
    {
        var len = arr.length;
        var jsonString = "{" + "\n" + "\"rows\":" + len + "," + "\n" + "\"data\":" + "[" + "\n";
        for(var i = 0; i < len; i++)
        {
            jsonString += "{" + "\n";
            var elem = arr[i];
            const keys = Object.keys(elem); // Get all keys as an array
            const lastKey = keys[keys.length - 1];
            for (const key in elem) {
                if(key === lastKey){
                    jsonString += "\"" + key + "\"" + ":" + "\"" + elem[key] + "\"" + "\n";
                }
                else{
                    jsonString += "\"" + key + "\"" + ":" + "\"" + elem[key] + "\"" + "," + "\n";
                }
            }
            if(i == len-1){
                jsonString += "}" + "\n";
            }
            else{
                jsonString += "}" + "," + "\n";
            }
        }
        jsonString += "]" + "\n" + "}" ;

        console.log("jsonString:", jsonString);

        return jsonString;
    }
   */
}
