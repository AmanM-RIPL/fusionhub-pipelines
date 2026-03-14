import QtQuick 2.15
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick
import com.fh.models 1.0
import com.fh.controllers;



Row {
    // anchors.fill: parent
    id: plannedBIMRoot
    width: parent.width
    height: 500
    padding: 10
    spacing: 10

    property var ifcDetailList: [];
    property string pageType: "PlannedBIM";
    property int treeviewWidth: parent.width/2 - 20
    property int glsceneWidth: parent.width/2
    property bool glsceneVisible: false

    property int glsceneAbsoluteX: 0
    property int glsceneAbsoluteY: 0
    property int helperPointIndexToFocus: -1

    //property int expandedIndex: -1
    property int wallExpandedIndex: -1
    property int beamExpandedIndex: -1
    property int columnExpandedIndex: -1
    property int slabExpandedIndex: -1
    property int doorExpandedIndex: -1
    property int windowExpandedIndex: -1
    property int stairsExpandedIndex: -1
    property int morphExpandedIndex: -1

    property var scheduleSetupList: []


    IFCWallController {
        id: wallController
    }

    BIMElementController{
        id: bimElementController
    }

    ScheduleSetupController{
        id:scheduleSetupController
    }


    Rectangle {
        //width: parent.width/2 - 20
        width: treeviewWidth
        height: parent.height
        color: "white"
        border.color: "#000000"


        ScrollView {
            id: horizontalScrollView
            width: parent.width
            height: 500
            padding: 20
            clip: true

            FHTable {
                visible: plannedBIMRoot.pageType === "Collision"
                height: 500
                leftPadding: 20
                model: plannedBIMRoot.ifcDetailList
                columns: [
                    { label: "Type", width: 300, key: "type" },
                    { label: "Clash Details", width: 300, key: "globalId" },
                    { label: "Name", width: 500, key: "name" },
                ]
            }

            Row {
                id: headerRow
                spacing: 20
                visible: plannedBIMRoot.pageType === "Gantt"
                width: parent.width
                anchors.left: parent.left
                anchors.top: parent.top

                Rectangle {
                    width: 200
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "Milestone"
                        font.weight: Font.Bold
                    }
                }

                Rectangle {
                    width: 50
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "JAN"
                        font.weight: Font.Bold
                    }
                }

                Rectangle {
                    width: 50
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "FEB"
                        font.weight: Font.Bold
                    }
                }

                Rectangle {
                    width: 50
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "MAR"
                        font.weight: Font.Bold
                    }
                }

                Rectangle {
                    width: 50
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "APR"
                        font.weight: Font.Bold
                    }
                }

                Rectangle {
                    width: 50
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "MAY"
                        font.weight: Font.Bold
                    }
                }

                Rectangle {
                    width: 50
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "JUN"
                        font.weight: Font.Bold
                    }
                }

                Rectangle {
                    width: 50
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "JUL"
                        font.weight: Font.Bold
                    }
                }

                Rectangle {
                    width: 50
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "AUG"
                        font.weight: Font.Bold
                    }
                }

                Rectangle {
                    width: 50
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "SEP"
                        font.weight: Font.Bold
                    }
                }

                Rectangle {
                    width: 50
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "OCT"
                        font.weight: Font.Bold
                    }
                }

                Rectangle {
                    width: 50
                    height: 30
                    color: "#E0E0E0"
                    Text {
                        anchors.centerIn: parent
                        text: "NOV"
                        font.weight: Font.Bold
                    }
                }
            }

            TreeView {
                visible: plannedBIMRoot.pageType === "Gantt"
                anchors.fill: parent
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 10
                anchors.topMargin: 50
                delegate: TreeViewDelegate {
                    onClicked: {
                        // Object.keys(model).forEach((x) => {
                        //     console.log(x);
                        // });

                        // console.log("Row" + model.row);
                        // console.log("Column" + model.column);
                        // console.log(model.display);

                        // console.log(depth);
                        // console.log(indentation);
                    }

                    indicator: Item {
                        x: 0

                        Text {
                            anchors.left: parent.left
                            text: "+"
                            font.pixelSize: 12
                        }
                    }

                    contentItem: Item {
                        anchors.fill: parent

                        // Column 2 is the GlobalId column
                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"

                            function getWidth(level) {
                                if (level === 0) {
                                    return 600
                                } else if (level === 1) {
                                    return 400
                                } else if (level === 2) {
                                    return 350
                                } else if (level === 3) {
                                    return 300
                                } else if (level === 4) {
                                    return 150
                                } else {
                                    return 50
                                }
                            }

                            // Show a colored bar for the Gantt chart cell
                            Rectangle {
                                visible: model.column === 2
                                x: 200 + (depth * 20) + (model.row*2) // leftMargin + (depth * indentation) + 10 //x: 5
                                y: parent.height / 4
                                height: parent.height / 2
                                width: parent.getWidth(depth) //100   // you can bind this to some duration property
                                color: "skyblue"
                                radius: 1
                            }

                            // Fallback: normal text for other columns
                            Text {
                                visible: model.column === 0
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                leftPadding: 50 //leftMargin + (depth * indentation) + 10
                                text: model.display
                                font.pixelSize: 12
                            }
                        }
                    }
                }
               // model: treeModel
            }

            /*TreeView {
                visible: plannedBIMRoot.pageType === "PlannedBIM"
                anchors.fill: parent
                anchors.margins: 10
                delegate: TreeViewDelegate {}
                model: treeModel

                IFCDetailController {
                    id: ifcDetailController
                }
            }*/

            //Start
            ListModel {
                id: mainModel
                Component.onCompleted: {
                    let categories = [
                        {
                            categories:"Architecture",
                            name: "General",
                            items: [
                                {itemName: "Wall"},
                                {itemName: "Door"},
                                {itemName: "Window"},
                                {itemName: "Beam"},
                                {itemName: "Column"},
                                {itemName: "Slab"},
                                {itemName: "Stairs"},
                                {itemName: "Morph"}
                            ]
                        },
                        {
                            categories:"Architecture",
                            name: "Interior",
                            items: []
                        },
                        {
                            categories:"Architecture",
                            name: "Finishing",
                            items: []
                        },
                        {
                            categories:"Structure",
                            name: "General",
                            items: []
                        },
                        {
                            categories:"MEPF",
                            name: "Mechanical",
                            items: []
                        },
                        {
                            categories:"MEPF",
                            name: "Electrical",
                            items: []
                        },
                        {
                            categories:"MEPF",
                            name: "Plumbing",
                            items: []
                        }
                    ]
                    append(categories)
                }
            }

            //Start of outerlist
            ListView {
                visible: plannedBIMRoot.pageType === "PlannedBIM"
                id: mainList
                anchors.fill: parent
                model: mainModel
                clip: true

                delegate: Column {
                    id: mainDelegate
                    spacing: 5
                    property bool collapsed: true

                    Text {
                        id: categoryText
                        text: categories
                        font.pointSize: 10
                        color: "gray"
                    }

                    MouseArea {
                        width: parent.width
                        height: nameText.implicitHeight
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            collapsed = !collapsed;
                            console.log("Clicked on outer item:", name);
                            itemList.visible = !itemList.visible
                        }

                        Text {
                            id: nameText
                            text: name
                            font.pointSize: 14
                            color: "black"
                            font.underline: true
                        }
                    }//End of Outerlist

                    //Start of innerlist
                    ListView {
                        id: itemList
                        width: parent.width
                        height: contentHeight+20
                        model: items
                        clip: true

                        delegate: MouseArea {
                            width: parent.width
                            height: itemText.implicitHeight + 5 // Include padding for better touch
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                console.log("Clicked on inner item:", itemName);


                                if(itemName === "Wall")
                                {
                                    wallSettingsPopup.open();
                                }
                                else if(itemName === "Beam")
                                {
                                    beamSettingsPopup.open();
                                }
                                else if(itemName === "Column")
                                {
                                    columnSettingsPopup.open();
                                }
                                else if(itemName === "Slab")
                                {
                                    slabSettingsPopup.open();
                                }
                                else if(itemName === "Door")
                                {
                                    doorSettingsPopup.open();
                                }
                                else if(itemName === "Window")
                                {
                                    windowSettingsPopup.open();
                                }
                                else if(itemName === "Stairs")
                                {
                                    stairsSettingsPopup.open();
                                }
                                else if(itemName === "Morph")
                                {
                                    morphSettingsPopup.open();
                                }
                            }

                            Text {
                                id: itemText
                                text: "- " + itemName
                                anchors.leftMargin: 20
                                font.pointSize: 11
                                color: "black"
                            }
                        }
                    }
                }
            }
            //End of innerlist
        }

        //List Model start
        ListModel {
            id: popupModel
            Component.onCompleted: {
                let categories = [
                    {
                        rowIndexText:"0",
                        name: "Geometry and Positioning"
                    },
                    {
                        rowIndexText:"1",
                        name: "Model"
                    },
                    {
                        rowIndexText:"2",
                        name: "Classification and Properties"
                    }
                ]
                append(categories)
            }
        }
        //End of List Model

        //Start of WallSettings
        FHPopup {
            id: wallSettingsPopup
            popupWidth: 500
            popupHeight: 600
            title: "Wall Settings"
            //parent: Overlay
            anchors.centerIn: Overlay.overlay

            property string wallTotalHeightText: ""
            property string wallWidthText: ""
            property string wallSlantAngleText: ""
            property string wallTaperAngleText: ""
            property string wallReferenceLinePositionText: ""
            property string wallSheduleSetupText: ""


            onAcceptCallback: function () {
                //wallController.create("projectname", wallTotalHeightTextBox.text, wallWidthTextBox.text);

                let bimElementPtr = bimElementController.create("Wall", "Front Wall", 0, 0);
                bimElementController.addParameter(bimElementPtr, "Height", wallTotalHeightText);
                bimElementController.addParameter(bimElementPtr, "Width", wallWidthText);
                bimElementController.addParameter(bimElementPtr, "ReferenceLine", "[]");
                bimElementController.addParameter(bimElementPtr, "Layers", "[]");
                bimElementController.addParameter(bimElementPtr, "SlantAngle", wallSlantAngleText);
                bimElementController.addParameter(bimElementPtr, "TaperAngle", wallTaperAngleText);
                bimElementController.addParameter(bimElementPtr, "ReferenceLinePosition", wallReferenceLinePositionText);

                bimElementController.addParameter(bimElementPtr, "ScheduleSetup", wallSheduleSetupText);

                glscene.updateEditableBimElement(bimElementPtr);

                wallTotalHeightText = "";
                wallWidthText = "";
                wallSlantAngleText = "";
                wallTaperAngleText = "";
                wallReferenceLinePositionText = "";

                glscene.setCurrentItem("Wall");
                glscene.update();
            }

            onCancelCallback: function () {
                wallTotalHeightText = "";
                wallWidthText = "";
                wallSlantAngleText = "";
                wallTaperAngleText = "";
                wallReferenceLinePositionText = "";
                //wallSheduleSetupText = "";
                glscene.update();
            }

            onOpened:
            {
                var list = scheduleSetupController.getSetupList(true)

                for(var x = 0; x < list.length; x++)
                {
                   //scheduleSetupList.push(String(list[x].description))
                    scheduleSetupList.push(list[x])
                }
            }


            ColumnLayout {
                id:columnLayout
                width: wallSettingsPopup.popupWidth-65
                height: wallSettingsPopup.popupHeight-65
                ListView{
                    id:mainListView
                    model: popupModel
                    clip: true
                    orientation: Qt.Vertical
                    spacing: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    delegate: Rectangle {
                        //id: firstColumn
                        id: rectId
                        width:columnLayout.width
                        height: wallExpandedIndex === index ? 360 : 60
                        radius: 5
                        color:height === 60 ? "lightgray": "white"

                        // Animate the height change
                        Behavior on height {
                            NumberAnimation { duration: 200 }
                        }

                        Text {
                            id: nameId
                            text: name
                            objectName:rowIndexText
                            topPadding: 10
                            bottomPadding: 10
                            font.pointSize: 14
                            color: "black"
                            font.weight: 700
                            font.family: "Segoe UI"
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                        }

                        //Start of row
                        RowLayout{
                            id: wallRowLayout
                            width: parent.width
                            height: 300
                            spacing: 5
                            anchors.top: nameId.bottom
                            anchors.left: nameId.left
                            visible: wallExpandedIndex === index
                            Loader {
                                        Layout.fillWidth: true
                                        sourceComponent: {
                                            if (nameId.objectName === "0")
                                            {
                                                return gpDelegateComponentForWall;
                                            }
                                            else if (nameId.objectName === "1")
                                            {
                                                return modelDelegateComponentForWall;
                                            }
                                            else
                                            {
                                                return cpDelegateComponentForWall;
                                            }
                                        }
                                    }
                        }//End of row
                        MouseArea {
                            anchors.fill: nameId
                            onClicked: {
                               //If this item is already expanded, collapse it. Otherwise, expand it.
                                if (wallExpandedIndex === index) {
                                    wallExpandedIndex = -1
                                }
                                else {
                                    wallExpandedIndex = index
                                    rectId.border.color = "lightgray"
                                }
                            }
                        }
                    }
                }
            }
        }
        //End of WallSetting

        //Start of BeamSettings
        FHPopup {
            id: beamSettingsPopup
            popupWidth: 500
            popupHeight: 600
            title: "Beam Settings"
            //parent: Overlay
            anchors.centerIn: Overlay.overlay

            property string beamTotalHeightText: ""
            property string beamWidthText: ""


            onAcceptCallback: function () {
                //beamController.create("projectname", beamTotalHeightTextBox.text, beamWidthTextBox.text);
                let bimElementPtr = bimElementController.create("Beam", "Front Beam", 0, 0);
                bimElementController.addParameter(bimElementPtr, "Height", beamTotalHeightText);
                bimElementController.addParameter(bimElementPtr, "Width", beamWidthText);
                bimElementController.addParameter(bimElementPtr, "ReferenceLine", "[]");

                glscene.updateEditableBimElement(bimElementPtr);

                beamTotalHeightText = "";
                beamWidthText = "";

                glscene.setCurrentItem("Beam");
                glscene.update();
            }

            onCancelCallback: function () {
                beamTotalHeightText = "";
                beamWidthText= "";

                glscene.update();
            }


            ColumnLayout {
                id:beamColumnLayout
                width: beamSettingsPopup.popupWidth-65
                height: beamSettingsPopup.popupHeight-65
                ListView{
                    id:beamMainListView
                    model: popupModel
                    clip: true
                    orientation: Qt.Vertical
                    spacing: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    delegate: Rectangle {
                        //id: firstColumn
                        id: beamRectId
                        width:columnLayout.width
                        height: beamExpandedIndex === index ? 360 : 60
                        radius: 5
                        color:height === 60 ? "lightgray": "white"

                        // Animate the height change
                        Behavior on height {
                            NumberAnimation { duration: 200 }
                        }

                        Text {
                            id: beamNameId
                            text: name
                            objectName:rowIndexText
                            topPadding: 10
                            bottomPadding: 10
                            font.pointSize: 14
                            color: "black"
                            font.weight: 700
                            font.family: "Segoe UI"
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                        }

                        //Start of row
                        RowLayout{
                            id: beamRowLayout
                            width: parent.width
                            height: 300
                            spacing: 5
                            anchors.top: beamNameId.bottom
                            anchors.left: beamNameId.left
                            visible: beamExpandedIndex === index
                            Loader {
                                        Layout.fillWidth: true
                                        sourceComponent: {
                                            if (beamNameId.objectName === "0")
                                            {
                                                return gpDelegateComponentForBeam;
                                            }
                                            else if (beamNameId.objectName === "1")
                                            {
                                                return modelDelegateComponentForBeam;
                                            }
                                            else
                                            {
                                                return cpDelegateComponentForBeam;
                                            }
                                        }
                                    }
                        }//End of row
                        MouseArea {
                            anchors.fill: beamNameId
                            onClicked: {
                               //If this item is already expanded, collapse it. Otherwise, expand it.
                                if (beamExpandedIndex === index) {
                                    beamExpandedIndex = -1
                                }
                                else {
                                    beamExpandedIndex = index
                                    beamRectId.border.color = "lightgray"
                                }
                            }
                        }
                    }
                }
            }
        }
        //End of BeamSetting


        //Start of ColumnSettings
        FHPopup {
            id: columnSettingsPopup
            popupWidth: 500
            popupHeight: 600
            title: "Column Settings"
            //parent: Overlay
            anchors.centerIn: Overlay.overlay

            property string columnTotalHeightText: ""
            property string columnWidthText: ""


            onAcceptCallback: function () {
                //columnController.create("projectname", columnTotalHeightTextBox.text, columnWidthTextBox.text);
                let bimElementPtr = bimElementController.create("Column", "Front Column", 0, 0);
                bimElementController.addParameter(bimElementPtr, "Height", columnTotalHeightText);
                bimElementController.addParameter(bimElementPtr, "Width", columnWidthText);
                bimElementController.addParameter(bimElementPtr, "ReferenceLine", "[]");

                glscene.updateEditableBimElement(bimElementPtr);

                columnTotalHeightText = "";
                columnWidthText = "";

                glscene.setCurrentItem("Column");
                glscene.update();
            }

            onCancelCallback: function () {
                columnTotalHeightText = "";
                columnWidthText= "";

                 glscene.update();
            }


            ColumnLayout {
                id:columnColumnLayout
                width: columnSettingsPopup.popupWidth-65
                height: columnSettingsPopup.popupHeight-65
                ListView{
                    id:columnMainListView
                    model: popupModel
                    clip: true
                    orientation: Qt.Vertical
                    spacing: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    delegate: Rectangle {
                        //id: firstColumn
                        id: columnRectId
                        width:columnLayout.width
                        height: columnExpandedIndex === index ? 360 : 60
                        radius: 5
                        color:height === 60 ? "lightgray": "white"

                        // Animate the height change
                        Behavior on height {
                            NumberAnimation { duration: 200 }
                        }

                        Text {
                            id: columnNameId
                            text: name
                            objectName:rowIndexText
                            topPadding: 10
                            bottomPadding: 10
                            font.pointSize: 14
                            color: "black"
                            font.weight: 700
                            font.family: "Segoe UI"
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                        }

                        //Start of row
                        RowLayout{
                            id: columnRowLayout
                            width: parent.width
                            height: 300
                            spacing: 5
                            anchors.top: columnNameId.bottom
                            anchors.left: columnNameId.left
                            visible: columnExpandedIndex === index
                            Loader {
                                        Layout.fillWidth: true
                                        sourceComponent: {
                                            if (columnNameId.objectName === "0")
                                            {
                                                return gpDelegateComponentForColumn;
                                            }
                                            else if (columnNameId.objectName === "1")
                                            {
                                                return modelDelegateComponentForColumn;
                                            }
                                            else
                                            {
                                                return cpDelegateComponentForColumn;
                                            }
                                        }
                                    }
                        }//End of row
                        MouseArea {
                            anchors.fill: columnNameId
                            onClicked: {
                               //If this item is already expanded, collapse it. Otherwise, expand it.
                                if (columnExpandedIndex === index) {
                                    columnExpandedIndex = -1
                                }
                                else {
                                    columnExpandedIndex = index
                                    columnRectId.border.color = "lightgray"
                                }
                            }
                        }
                    }
                }
            }
        }
        //End of ColumnSettings

        //Start of SlabSettings
        FHPopup {
            id: slabSettingsPopup
            popupWidth: 500
            popupHeight: 600
            title: "Slab Settings"
            //parent: Overlay
            anchors.centerIn: Overlay.overlay

            property string slabTotalHeightText: ""
            property string slabDistanceText: ""


            onAcceptCallback: function () {
                //slabController.create("projectname", slabTotalHeightTextBox.text, slabWidthTextBox.text);
                let bimElementPtr = bimElementController.create("Slab", "Front Slab", 0, 0);
                bimElementController.addParameter(bimElementPtr, "Height", slabTotalHeightText);
                bimElementController.addParameter(bimElementPtr, "Distance", slabDistanceText);
                bimElementController.addParameter(bimElementPtr, "ReferenceLine", "[]");

                glscene.updateEditableBimElement(bimElementPtr);

                slabTotalHeightText = "";
                slabDistanceText = "";

                glscene.setCurrentItem("Slab");
                glscene.update();
            }

            onCancelCallback: function () {
                slabTotalHeightText = "";
                slabDistanceText= "";

                 glscene.update();
            }


            ColumnLayout {
                id:slabColumnLayout
                width: slabSettingsPopup.popupWidth-65
                height: slabSettingsPopup.popupHeight-65
                ListView{
                    id:slabMainListView
                    model: popupModel
                    clip: true
                    orientation: Qt.Vertical
                    spacing: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    delegate: Rectangle {
                        //id: firstColumn
                        id: slabRectId
                        width:columnLayout.width
                        height: slabExpandedIndex === index ? 360 : 60
                        radius: 5
                        color:height === 60 ? "lightgray": "white"

                        // Animate the height change
                        Behavior on height {
                            NumberAnimation { duration: 200 }
                        }

                        Text {
                            id: slabNameId
                            text: name
                            objectName:rowIndexText
                            topPadding: 10
                            bottomPadding: 10
                            font.pointSize: 14
                            color: "black"
                            font.weight: 700
                            font.family: "Segoe UI"
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                        }

                        //Start of row
                        RowLayout{
                            id: slabRowLayout
                            width: parent.width
                            height: 300
                            spacing: 5
                            anchors.top: slabNameId.bottom
                            anchors.left: slabNameId.left
                            visible: slabExpandedIndex === index
                            Loader {
                                        Layout.fillWidth: true
                                        sourceComponent: {
                                            if (slabNameId.objectName === "0")
                                            {
                                                return gpDelegateComponentForSlab;
                                            }
                                            else if (slabNameId.objectName === "1")
                                            {
                                                return modelDelegateComponentForSlab;
                                            }
                                            else
                                            {
                                                return cpDelegateComponentForSlab;
                                            }
                                        }
                                    }
                        }//End of row
                        MouseArea {
                            anchors.fill: slabNameId
                            onClicked: {
                               //If this item is already expanded, collapse it. Otherwise, expand it.
                                if (slabExpandedIndex === index) {
                                    slabExpandedIndex = -1
                                }
                                else {
                                    slabExpandedIndex = index
                                    slabRectId.border.color = "lightgray"
                                }
                            }
                        }
                    }
                }
            }
        }
        //End of SlabSettings


        //Start of DoorSettings
        FHPopup {
            id: doorSettingsPopup
            popupWidth: 500
            popupHeight: 600
            title: "Door Settings"
            //parent: Overlay
            anchors.centerIn: Overlay.overlay

            property string doorTotalHeightText: ""
            property string doorDistanceText: ""
            property string doorWidthText: ""


            onAcceptCallback: function () {
                //doorController.create("projectname", doorTotalHeightTextBox.text, doorWidthTextBox.text);
                let bimElementPtr = bimElementController.create("Door", "Front Door", 0, 0);
                bimElementController.addParameter(bimElementPtr, "Height", doorTotalHeightText);
                bimElementController.addParameter(bimElementPtr, "Width", doorWidthText);
                bimElementController.addParameter(bimElementPtr, "Distance", doorDistanceText);
                bimElementController.addParameter(bimElementPtr, "ReferenceLine", "[]");

                glscene.updateEditableBimElement(bimElementPtr);

                doorTotalHeightText = "";
                doorDistanceText = "";
                doorWidthText = "";

                glscene.setCurrentItem("Door");
                glscene.update();
            }

            onCancelCallback: function () {
                doorTotalHeightText = "";
                doorDistanceText= "";
                doorWidthText= "";

                 glscene.update();
            }


            ColumnLayout {
                id:doorColumnLayout
                width: doorSettingsPopup.popupWidth-65
                height: doorSettingsPopup.popupHeight-65
                ListView{
                    id:doorMainListView
                    model: popupModel
                    clip: true
                    orientation: Qt.Vertical
                    spacing: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    delegate: Rectangle {
                        //id: firstColumn
                        id: doorRectId
                        width:columnLayout.width
                        height: doorExpandedIndex === index ? 460 : 60
                        radius: 5
                        color:height === 60 ? "lightgray": "white"

                        // Animate the height change
                        Behavior on height {
                            NumberAnimation { duration: 200 }
                        }

                        Text {
                            id: doorNameId
                            text: name
                            objectName:rowIndexText
                            topPadding: 10
                            bottomPadding: 10
                            font.pointSize: 14
                            color: "black"
                            font.weight: 700
                            font.family: "Segoe UI"
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                        }

                        //Start of row
                        RowLayout{
                            id: doorRowLayout
                            width: parent.width
                            height: 300
                            spacing: 5
                            anchors.top: doorNameId.bottom
                            anchors.left: doorNameId.left
                            visible: doorExpandedIndex === index
                            Loader {
                                        Layout.fillWidth: true
                                        sourceComponent: {
                                            if (doorNameId.objectName === "0")
                                            {
                                                return gpDelegateComponentForDoor;
                                            }
                                            else if (doorNameId.objectName === "1")
                                            {
                                                return modelDelegateComponentForDoor;
                                            }
                                            else
                                            {
                                                return cpDelegateComponentForDoor;
                                            }
                                        }
                                    }
                        }//End of row
                        MouseArea {
                            anchors.fill: doorNameId
                            onClicked: {
                               //If this item is already expanded, collapse it. Otherwise, expand it.
                                if (doorExpandedIndex === index) {
                                    doorExpandedIndex = -1
                                }
                                else {
                                    doorExpandedIndex = index
                                    doorRectId.border.color = "lightgray"
                                }
                            }
                        }
                    }
                }
            }
        }
        //End of DoorSettings


        //Start of WindowSettings
        FHPopup {
            id: windowSettingsPopup
            popupWidth: 500
            popupHeight: 600
            title: "Window Settings"
            //parent: Overlay
            anchors.centerIn: Overlay.overlay

            property string windowTotalHeightText: ""
            property string windowDistanceText: ""
            property string windowWidthText: ""


            onAcceptCallback: function () {
                //windowController.create("projectname", windowTotalHeightTextBox.text, windowWidthTextBox.text);
                let bimElementPtr = bimElementController.create("Window", "Front Window", 0, 0);
                bimElementController.addParameter(bimElementPtr, "Height", windowTotalHeightText);
                bimElementController.addParameter(bimElementPtr, "Width", windowWidthText);
                bimElementController.addParameter(bimElementPtr, "Distance", windowDistanceText);
                bimElementController.addParameter(bimElementPtr, "ReferenceLine", "[]");

                glscene.updateEditableBimElement(bimElementPtr);

                windowTotalHeightText = "";
                windowDistanceText = "";
                windowWidthText = "";

                glscene.setCurrentItem("Window");
                glscene.update();
            }

            onCancelCallback: function () {
                windowTotalHeightText = "";
                windowDistanceText = "";
                windowWidthText = "";

                glscene.update();
            }


            ColumnLayout {
                id:windowColumnLayout
                width: windowSettingsPopup.popupWidth-65
                height: windowSettingsPopup.popupHeight-65
                ListView{
                    id:windowMainListView
                    model: popupModel
                    clip: true
                    orientation: Qt.Vertical
                    spacing: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    delegate: Rectangle {
                        //id: firstColumn
                        id: windowRectId
                        width:columnLayout.width
                        //height: windowExpandedIndex === index ? 360 : 60
                        height: windowExpandedIndex === index ? 460 : 60
                        radius: 5
                        color:height === 60 ? "lightgray": "white"

                        // Animate the height change
                        Behavior on height {
                            NumberAnimation { duration: 200 }
                        }

                        Text {
                            id: windowNameId
                            text: name
                            objectName:rowIndexText
                            topPadding: 10
                            bottomPadding: 10
                            font.pointSize: 14
                            color: "black"
                            font.weight: 700
                            font.family: "Segoe UI"
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                        }

                        //Start of row
                        RowLayout{
                            id: windowRowLayout
                            width: parent.width
                            height: 300
                            spacing: 5
                            anchors.top: windowNameId.bottom
                            anchors.left: windowNameId.left
                            visible: windowExpandedIndex === index
                            Loader {
                                        Layout.fillWidth: true
                                        sourceComponent: {
                                            if (windowNameId.objectName === "0")
                                            {
                                                return gpDelegateComponentForWindow;
                                            }
                                            else if (windowNameId.objectName === "1")
                                            {
                                                return modelDelegateComponentForWindow;
                                            }
                                            else
                                            {
                                                return cpDelegateComponentForWindow;
                                            }
                                        }
                                    }
                        }//End of row
                        MouseArea {
                            anchors.fill: windowNameId
                            onClicked: {
                               //If this item is already expanded, collapse it. Otherwise, expand it.
                                if (windowExpandedIndex === index) {
                                    windowExpandedIndex = -1
                                }
                                else {
                                    windowExpandedIndex = index
                                    windowRectId.border.color = "lightgray"
                                }
                            }
                        }
                    }
                }
            }
        }
        //End of WindowSettings


        //Start of StairsSettings
        FHPopup {
            id: stairsSettingsPopup
            popupWidth: 500
            popupHeight: 600
            title: "Stairs Settings"
            //parent: Overlay
            anchors.centerIn: Overlay.overlay

            property string stairsTotalHeightText: ""
            property string stairsDistanceText: ""


            onAcceptCallback: function () {
                //stairsController.create("projectname", stairsTotalHeightTextBox.text, stairsWidthTextBox.text);
                let bimElementPtr = bimElementController.create("Stairs", "Front Stairs", 0);
                bimElementController.addParameter(bimElementPtr, "Height", stairsTotalHeightText);
                bimElementController.addParameter(bimElementPtr, "Distance", stairsDistanceText);
                bimElementController.addParameter(bimElementPtr, "ReferenceLine", "[]");

                stairsTotalHeightText = "";
                stairsDistanceText = "";

                glscene.setCurrentItem("Stairs");
                glscene.update();
            }

            onCancelCallback: function () {
                stairsTotalHeightText = "";
                stairsDistanceText= "";

                 glscene.update();
            }


            ColumnLayout {
                id:stairsColumnLayout
                width: stairsSettingsPopup.popupWidth-65
                height: stairsSettingsPopup.popupHeight-65
                ListView{
                    id:stairsMainListView
                    model: popupModel
                    clip: true
                    orientation: Qt.Vertical
                    spacing: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    delegate: Rectangle {
                        //id: firstColumn
                        id: stairsRectId
                        width:columnLayout.width
                        height: stairsExpandedIndex === index ? 360 : 60
                        radius: 5
                        color:height === 60 ? "lightgray": "white"

                        // Animate the height change
                        Behavior on height {
                            NumberAnimation { duration: 200 }
                        }

                        Text {
                            id: stairsNameId
                            text: name
                            objectName:rowIndexText
                            topPadding: 10
                            bottomPadding: 10
                            font.pointSize: 14
                            color: "black"
                            font.weight: 700
                            font.family: "Segoe UI"
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                        }

                        //Start of row
                        RowLayout{
                            id: stairsRowLayout
                            width: parent.width
                            height: 300
                            spacing: 5
                            anchors.top: stairsNameId.bottom
                            anchors.left: stairsNameId.left
                            visible: stairsExpandedIndex === index
                            Loader {
                                        Layout.fillWidth: true
                                        sourceComponent: {
                                            if (stairsNameId.objectName === "0")
                                            {
                                                return gpDelegateComponentForStairs;
                                            }
                                            else if (stairsNameId.objectName === "1")
                                            {
                                                return modelDelegateComponentForStairs;
                                            }
                                            else
                                            {
                                                return cpDelegateComponentForStairs;
                                            }
                                        }
                                    }
                        }//End of row
                        MouseArea {
                            anchors.fill: stairsNameId
                            onClicked: {
                               //If this item is already expanded, collapse it. Otherwise, expand it.
                                if (stairsExpandedIndex === index) {
                                    stairsExpandedIndex = -1
                                }
                                else {
                                    stairsExpandedIndex = index
                                    stairsRectId.border.color = "lightgray"
                                }
                            }
                        }
                    }
                }
            }
        }
        //End of StairsSettings

        //Start of MorphSettings
        FHPopup {
            id: morphSettingsPopup
            popupWidth: 500
            popupHeight: 600
            title: "Morph Settings"
            //parent: Overlay
            anchors.centerIn: Overlay.overlay

            onAcceptCallback: function () {
                let bimElementPtr = bimElementController.create("Morph", "New Morph Object", 0, 0);
                bimElementController.addParameter(bimElementPtr, "SATFileName", "");

                glscene.updateEditableBimElement(bimElementPtr);

                glscene.setCurrentItem("Morph");
                glscene.update();
            }

            onCancelCallback: function () {
                glscene.update();
            }


            ColumnLayout {
                id: morphColumnLayout
                width: morphSettingsPopup.popupWidth-65
                height: morphSettingsPopup.popupHeight-65
                ListView{
                    id: morphMainListView
                    model: popupModel
                    clip: true
                    orientation: Qt.Vertical
                    spacing: 10
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    delegate: Rectangle {
                        //id: firstColumn
                        id: morphRectId
                        width:columnLayout.width
                        height: morphExpandedIndex === index ? 360 : 60
                        radius: 5
                        color:height === 60 ? "lightgray": "white"

                        // Animate the height change
                        Behavior on height {
                            NumberAnimation { duration: 200 }
                        }

                        Text {
                            id: morphNameId
                            text: name
                            objectName:rowIndexText
                            topPadding: 10
                            bottomPadding: 10
                            font.pointSize: 14
                            color: "black"
                            font.weight: 700
                            font.family: "Segoe UI"
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                        }

                        //Start of row
                        RowLayout{
                            id: morphRowLayout
                            width: parent.width
                            height: 300
                            spacing: 5
                            anchors.top: morphNameId.bottom
                            anchors.left: morphNameId.left
                            visible: morphExpandedIndex === index
                            Loader {
                                        Layout.fillWidth: true
                                        sourceComponent: {
                                            if (morphNameId.objectName === "0")
                                            {
                                                return gpDelegateComponentForMorph;
                                            }
                                            else if (morphNameId.objectName === "1")
                                            {
                                                return modelDelegateComponentForMorph;
                                            }
                                            else
                                            {
                                                return cpDelegateComponentForMorph;
                                            }
                                        }
                                    }
                        }//End of row
                        MouseArea {
                            anchors.fill: morphNameId
                            onClicked: {
                               //If this item is already expanded, collapse it. Otherwise, expand it.
                                if (morphExpandedIndex === index) {
                                    morphExpandedIndex = -1
                                }
                                else {
                                    morphExpandedIndex = index
                                    morphRectId.border.color = "lightgray"
                                }
                            }
                        }
                    }
                }
            }
        }
        //End of MorphSetting
    }


    Rectangle {
        //width: parent.width/2
        width: glsceneWidth
        height: 500
        color: "gray"
        visible: glsceneVisible

        GLScene {
            id: glscene
            anchors.fill: parent

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onWheel: function(wheel) { // Declare 'wheel' as a formal parameter
                    if (wheel.modifiers & Qt.ControlModifier) {
                        if (wheel.angleDelta.y > 0) {
                            glscene.zoomIn();
                        } else {
                            glscene.zoomOut();
                        }
                        wheel.accepted = true;
                    } else {
                        wheel.accepted = false;
                    }
                }
                onPressed: function(mouse) {
                    // glscene.mousePressed(mouse.x, mouse.y);
                    // mouse.accepted = true;
                    glscene.focus = true;
                    glscene.requestPick(mouse.x, mouse.y);
                }
                // onReleased: function(mouse) {
                //     glscene.mouseReleased();
                //     mouse.accepted = true;
                // }
                onPositionChanged: function(mouse) {
                    // glscene.mousePositionChanged(mouse.x, mouse.y);
                    // mouse.accepted = true;
                    var screenPos = glscene.mapToGlobal(0, 0);

                    glscene.requestHover(mouse.x, mouse.y, screenPos.x, screenPos.y);
                }
            }


            Keys.onPressed: function (event) {
                if (event.key === Qt.Key_Up) {
                    glscene.cameraMoveUp();
                } else if (event.key === Qt.Key_Down) {
                    glscene.cameraMoveDown();
                } else if (event.key === Qt.Key_Left) {
                    glscene.cameraMoveLeft();
                } else if (event.key === Qt.Key_Right) {
                    glscene.cameraMoveRight();
                } else if (event.key === Qt.Key_W) {
                    glscene.cameraPanUp();
                } else if (event.key === Qt.Key_S) {
                    glscene.cameraPanDown();
                } else if (event.key === Qt.Key_A) {
                    glscene.cameraPanLeft();
                } else if (event.key === Qt.Key_D) {
                    glscene.cameraPanRight();
                } else if (event.key === Qt.Key_Return) {
                    glscene.saveEditableBimElement();
                } else if (event.key === Qt.Key_Shift) {
                    console.log("Shift in GLScene");
                    helperPointIndexToFocus = 0;
                    const item = helperPointRepeater.itemAt(0);
                    if (item && item.internalTextField) {
                        item.internalTextField.focus = true;
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        //if(plannedBIMRoot.visible)
        {
            //ifcDetailList = ifcDetailRepository.getIFCDetails();
             // ifcDetailList = ifcDetailController.loadIFC(ifcDetailController.getIfcFilePath());
             //treeModel = ifcDetailController.getTreeModel();
            // glscene.viewIfc();
            // glscene.update();
        }
    }

    onVisibleChanged: {
        //if(plannedBIMRoot.visible && plannedBIMRoot.pageType === "PlannedBIM")
        {
            //ifcDetailList = ifcDetailRepository.getIFCDetails();
            //ifcDetailList = ifcDetailController.loadIFC(ifcDetailController.getIfcFilePath());
            //treeModel = ifcDetailController.getTreeModel();
            // glscene.viewIfc();
            // glscene.update();

        }

    }

    // Rectangle for length of wall
    Connections {
        target: glscene
        onMiddlePointPositionChanged: function() {

            const absolutePos = glscene.mapToItem(null, 0, 0);

            glsceneAbsoluteX = absolutePos.x;
            glsceneAbsoluteY = absolutePos.y;
            const helperPointList = glscene.getMiddlePointValue();

            helperPointModel.clear();

            for (const helperPoint of helperPointList) {
                helperPointModel.append({
                    xPos: helperPoint.x,
                    yPos: helperPoint.y,
                    helperText: helperPoint.text,
                    helperValue: helperPoint.value,
                    helperVisible: helperPoint.visible
                });
            }

            const item = helperPointRepeater.itemAt(helperPointIndexToFocus);
            if (item && item.internalTextField) {
                item.internalTextField.focus = true;
            }
        }
    }

    ListModel {
        id: helperPointModel
    }

    Item {
        id: helperPointItem

        Repeater {
            id: helperPointRepeater
            model: helperPointModel

            delegate: Rectangle {
                property alias internalTextField: helperPointTextField
                x: glsceneAbsoluteX + xPos
                y: yPos
                visible: helperVisible
                width: 150
                height: 29
                border.width: 2
                border.color: "black"

                Row {
                    anchors.fill: parent
                    anchors.margins: 2

                    Text {
                        text: qsTr(helperText)
                        verticalAlignment: Text.AlignVCenter
                        width: parent.width * 0.50
                        height: parent.height
                    }

                    TextField {
                        id: helperPointTextField
                        width: parent.width * 0.50
                        height: 25
                        text: helperValue.toFixed(2)

                        background: Rectangle {
                            border.width: 0
                        }

                        Keys.onPressed: function (event) {
                            if (event.key === Qt.Key_Shift)
                            {
                                console.log("Shift in Rectangle");
                                glscene.updateMiddlePointValue(parseFloat(text), index);

                                helperPointIndexToFocus = helperPointIndexToFocus < helperPointModel.count - 1 ? helperPointIndexToFocus + 1 : 0;
                            }
                        }
                    }
                }
            }
        }
    }

    //Components for Geometry and Positioning For WallSettings
    Component {
        id: gpDelegateComponentForWall
        RowLayout {
            id:wallRowlLayoutGP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (wallRowlLayoutGP.width - wallRowlLayoutGP.spacing) / 2-10
                height: 300

                ColumnLayout {
                    width: firstColumn.width
                    height: firstColumn.height

                    Text{
                        id: wallTopLinkLabel
                        text: "Wall top link"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: wallTopLinkTextBox
                        placeholderText: "Top Link"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: wallHeightFromTopLabel
                        text: "Height From Top"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: wallHeightFromTopTextBox
                        placeholderText: "Height From Top"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: wallTotalHeightLabel
                        text: "Total Height"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: wallTotalHeightTextBox
                        placeholderText: "Total Height"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            wallSettingsPopup.wallTotalHeightText = wallTotalHeightTextBox.text;
                        }
                    }


                    Text{
                        id: wallSlantAngleLabel
                        text: "Slant Angle (in Degrees)"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: wallSlantAngleTextBox
                        placeholderText: "0"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            wallSettingsPopup.wallSlantAngleText = wallSlantAngleTextBox.text;
                        }
                    }
                }
            }

            Rectangle {
                id:secondColumn
                width: (wallRowlLayoutGP.width - wallRowlLayoutGP.spacing) / 2-10
                height: 300

                ColumnLayout {
                    width: secondColumn.width
                    height: secondColumn.height

                    Text{
                        id: wallHomeFloorLabel
                        text: "Home Floor"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: wallHomeFloorTextBox
                        placeholderText: "Home Floor"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: wallHeightFromBottomLabel
                        text: "Height From Bottom"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: wallHeightFromBottomTextBox
                        placeholderText: "Height From Bottom"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: wallWidthLabel
                        text: "Width"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: wallWidthTextBox
                        placeholderText: "Width"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            wallSettingsPopup.wallWidthText = wallWidthTextBox.text;
                        }
                    }


                    Text{
                        id: wallTaperAngleLabel
                        text: "Taper Angle (in Degrees)"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                     }
                    CustomTextBox{
                        id: wallTaperAngleTextBox
                        placeholderText: "0"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            wallSettingsPopup.wallTaperAngleText = wallTaperAngleTextBox.text;
                        }
                    }
                }
            }
        }
    }

   //Components for Model
    Component {
        id: modelDelegateComponentForWall
        RowLayout {
            id:wallRowLayoutModel
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (wallRowLayoutModel.width - wallRowLayoutModel.spacing) / 2-20
                height: 300

                ColumnLayout {
                    width: firstColumn.width
                    height:40// firstColumn.height

                    Text{
                        id: wallReferenceLinePositionLabel
                        text: "Orientation Type"
                        color: "#323130"
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomComboBox{
                        id: wallReferenceLinePositionComboBox
                        model: ["Inner", "Outer"]
                        width: parent.width
                        currentIndex: 0
                        onCurrentTextChanged: {
                            const referenceLinePosition = wallReferenceLinePositionComboBox.currentIndex === 0 ? "inner" : "outer";
                            wallSettingsPopup.wallReferenceLinePositionText = referenceLinePosition;
                        }
                    }
                }

            }
        }
    }

    //Components for Classification and Properties
    Component {
        id: cpDelegateComponentForWall
        RowLayout {
            id: wallRowLayoutCP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                //width: (wallRowLayoutCP.width - wallRowLayoutCP.spacing) / 2-20
                width: wallRowLayoutCP.width - wallRowLayoutCP.spacing
                height: 300

                ColumnLayout {
                    width: firstColumn.width
                    height:40// firstColumn.height

                    Text{
                        id: wallHomeScheduleSetup
                        text: "Schedule Setup"
                        color: "#323130"
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomComboBox{
                        id: wallHomeScheduleSetupComboBox
                        model: scheduleSetupList
                        width: parent.width
                        currentIndex: 0
                        textRole: "description"
                        onCurrentTextChanged: {
                            if(scheduleSetupList.length > 0)
                            {
                              wallSettingsPopup.wallSheduleSetupText = String(scheduleSetupList[currentIndex].id);
                            }
                        }
                    }
                }

            }
        }
    }
    //End of WallSettingd Component

    //Components for Geometry and Positioning For BeamSettings
    Component {
        id: gpDelegateComponentForBeam
        RowLayout {
            id:beamRowLayoutGP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (beamRowLayoutGP.width - beamRowLayoutGP.spacing) / 2-10
                height: 300

                ColumnLayout {
                    width: firstColumn.width
                    height: firstColumn.height

                    Text{
                        id: beamTopLinkLabel
                        text: "Wall top link"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: beamTopLinkTextBox
                        placeholderText: "Top Link"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: beamHeightFromTopLabel
                        text: "Height From Top"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: beamHeightFromTopTextBox
                        placeholderText: "Height From Top"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: beamTotalHeightLabel
                        text: "Total Height"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: beamTotalHeightTextBox
                        placeholderText: "Total Height"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            beamSettingsPopup.beamTotalHeightText = beamTotalHeightTextBox.text;
                        }
                    }


                    Text{
                        id: beamTypeLabel
                        text: "Beam Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: beamTypeTextBox
                        placeholderText: "Beam Type"
                        text:""
                        color: "#323130"
                    }
                }
            }

            Rectangle {
                id:secondColumn
                width: (beamRowLayoutGP.width - beamRowLayoutGP.spacing) / 2-10
                height: 300

                ColumnLayout {
                    width: secondColumn.width
                    height: secondColumn.height

                    Text{
                        id: beamHomeFloorLabel
                        text: "Home Floor"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: beamHomeFloorTextBox
                        placeholderText: "Home Floor"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: beamHeightFromBottomLabel
                        text: "Height From Bottom"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: beamHeightFromBottomTextBox
                        placeholderText: "Height From Bottom"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: beamWidthLabel
                        text: "Width"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: beamWidthTextBox
                        placeholderText: "Width"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            beamSettingsPopup.beamWidthText = beamWidthTextBox.text;
                        }
                    }


                    Text{
                        id: beamGeometryTypeLabel
                        text: "Geometry Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                     }
                    CustomTextBox{
                        id: beamGeometryTypeTextBox
                        placeholderText: "Geometry Type"
                        text:""
                        color: "#323130"
                    }
                }
            }
        }
    }

   //Components for Model
    Component {
        id: modelDelegateComponentForBeam
        RowLayout {
            id:beamRowLayoutModel
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (beamRowLayoutModel.width - beamRowLayoutModel.spacing) / 2-20
                height: 300
            }
        }
    }

    //Components for Classification and Properties
    Component {
        id: cpDelegateComponentForBeam
        RowLayout {
            id: beamRowLayoutCP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (beamRowLayoutCP.width - beamRowLayoutCP.spacing) / 2-20
                height: 300
            }
        }
    }
    //End of BeamSettings Component

    //Components for Geometry and Positioning For ColumnSettings
    Component {
        id: gpDelegateComponentForColumn
        RowLayout {
            id:columnRowLayoutGP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (columnRowLayoutGP.width - columnRowLayoutGP.spacing) / 2-10
                height: 300

                ColumnLayout {
                    width: firstColumn.width
                    height: firstColumn.height

                    Text{
                        id: columnTopLinkLabel
                        text: "Wall top link"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: columnTopLinkTextBox
                        placeholderText: "Top Link"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: columnHeightFromTopLabel
                        text: "Height From Top"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: columnHeightFromTopTextBox
                        placeholderText: "Height From Top"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: columnTotalHeightLabel
                        text: "Total Height"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: columnTotalHeightTextBox
                        placeholderText: "Total Height"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            columnSettingsPopup.columnTotalHeightText = columnTotalHeightTextBox.text;
                        }
                    }


                    Text{
                        id: columnTypeLabel
                        text: "Column Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: columnTypeTextBox
                        placeholderText: "Column Type"
                        text:""
                        color: "#323130"
                    }
                }
            }

            Rectangle {
                id:secondColumn
                width: (columnRowLayoutGP.width - columnRowLayoutGP.spacing) / 2-10
                height: 300

                ColumnLayout {
                    width: secondColumn.width
                    height: secondColumn.height

                    Text{
                        id: columnHomeFloorLabel
                        text: "Home Floor"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: columnHomeFloorTextBox
                        placeholderText: "Home Floor"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: columnHeightFromBottomLabel
                        text: "Height From Bottom"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: columnHeightFromBottomTextBox
                        placeholderText: "Height From Bottom"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: columnWidthLabel
                        text: "Width"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: columnWidthTextBox
                        placeholderText: "Width"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            columnSettingsPopup.columnWidthText = columnWidthTextBox.text;
                        }
                    }


                    Text{
                        id: columnGeometryTypeLabel
                        text: "Geometry Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                     }
                    CustomTextBox{
                        id: columnGeometryTypeTextBox
                        placeholderText: "Geometry Type"
                        text:""
                        color: "#323130"
                    }
                }
            }
        }
    }

   //Components for Model
    Component {
        id: modelDelegateComponentForColumn
        RowLayout {
            id:columnRowLayoutModel
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (columnRowLayoutModel.width - columnRowLayoutModel.spacing) / 2-20
                height: 300
            }
        }
    }

    //Components for Classification and Properties
    Component {
        id: cpDelegateComponentForColumn
        RowLayout {
            id: columnRowLayoutCP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (columnRowLayoutCP.width - columnRowLayoutCP.spacing) / 2-20
                height: 300
            }
        }
    }
    //End of ColumnSettings Component


    //Components for Geometry and Positioning For SlabSettings
    Component {
        id: gpDelegateComponentForSlab
        RowLayout {
            id:slabRowLayoutGP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (slabRowLayoutGP.width - slabRowLayoutGP.spacing) / 2-10
                height: 300

                ColumnLayout {
                    width: firstColumn.width
                    height: firstColumn.height

                    Text{
                        id: slabTopLinkLabel
                        text: "Wall top link"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: slabTopLinkTextBox
                        placeholderText: "Top Link"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: slabHeightFromTopLabel
                        text: "Height From Top"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: slabHeightFromTopTextBox
                        placeholderText: "Height From Top"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: slabTotalHeightLabel
                        text: "Total Height"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: slabTotalHeightTextBox
                        placeholderText: "Total Height"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            slabSettingsPopup.slabTotalHeightText = slabTotalHeightTextBox.text;
                        }
                    }


                    Text{
                        id: slabTypeLabel
                        text: "Slab Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: slabTypeTextBox
                        placeholderText: "Slab Type"
                        text:""
                        color: "#323130"
                    }
                }
            }

            Rectangle {
                id:secondColumn
                width: (slabRowLayoutGP.width - slabRowLayoutGP.spacing) / 2-10
                height: 300

                ColumnLayout {
                    width: secondColumn.width
                    height: secondColumn.height

                    Text{
                        id: slabHomeFloorLabel
                        text: "Home Floor"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: slabHomeFloorTextBox
                        placeholderText: "Home Floor"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: slabHeightFromBottomLabel
                        text: "Height From Bottom"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: slabHeightFromBottomTextBox
                        placeholderText: "Height From Bottom"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: slabDistanceLabel
                        text: "Distance From Level"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: slabDistanceTextBox
                        placeholderText: "Distance"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            slabSettingsPopup.slabDistanceText = slabDistanceTextBox.text;
                        }
                    }


                    Text{
                        id: slabGeometryTypeLabel
                        text: "Slab Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                     }
                    CustomTextBox{
                        id: slabGeometryTypeTextBox
                        placeholderText: "Slab Type"
                        text:""
                        color: "#323130"
                    }
                }
            }
        }
    }

   //Components for Slab Model
    Component {
        id: modelDelegateComponentForSlab
        RowLayout {
            id:slabRowLayoutModel
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (slabRowLayoutModel.width - slabRowLayoutModel.spacing) / 2-20
                height: 300
            }
        }
    }

    //Components for Slab Classification and Properties
    Component {
        id: cpDelegateComponentForSlab
        RowLayout {
            id: slabRowLayoutCP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (slabRowLayoutCP.width - slabRowLayoutCP.spacing) / 2-20
                height: 300
            }
        }
    }
    //End of SlabSettings Component


    //Components for Geometry and Positioning For DoorSettings
    Component {
        id: gpDelegateComponentForDoor
        RowLayout {
            id:doorRowLayoutGP
            width: parent.width
            height: 400
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (doorRowLayoutGP.width - doorRowLayoutGP.spacing) / 2-10
                height: 400

                ColumnLayout {
                    width: firstColumn.width
                    height: firstColumn.height

                    Text{
                        id: doorTopLinkLabel
                        text: "Door top link"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: doorTopLinkTextBox
                        placeholderText: "Top Link"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: doorHeightFromTopLabel
                        text: "Height From Top"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: doorHeightFromTopTextBox
                        placeholderText: "Height From Top"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: doorTotalHeightLabel
                        text: "Total Height"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: doorTotalHeightTextBox
                        placeholderText: "Total Height"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            doorSettingsPopup.doorTotalHeightText = doorTotalHeightTextBox.text;
                        }
                    }


                    Text{
                        id: doorWidthLabel
                        text: "Width"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: doorWidthTextBox
                        placeholderText: "Width"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            doorSettingsPopup.doorWidthText = doorWidthTextBox.text;
                        }
                    }


                    Text{
                        id: doorTypeLabel
                        text: "Door Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: doorTypeTextBox
                        placeholderText: "Door Type"
                        text:""
                        color: "#323130"
                    }
                }
            }

            Rectangle {
                id:secondColumn
                width: (doorRowLayoutGP.width - doorRowLayoutGP.spacing) / 2-10
                height: 400


                ColumnLayout {
                    width: secondColumn.width
                    height: secondColumn.height

                    Text{
                        id: doorHomeFloorLabel
                        text: "Home Floor"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: doorHomeFloorTextBox
                        placeholderText: "Home Floor"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: doorHeightFromBottomLabel
                        text: "Height From Bottom"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: doorHeightFromBottomTextBox
                        placeholderText: "Height From Bottom"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: doorDistanceLabel
                        text: "Distance From Level"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: doorDistanceTextBox
                        placeholderText: "Distance"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            doorSettingsPopup.doorDistanceText = doorDistanceTextBox.text;
                        }
                    }


                    Text{
                        id: doorGeometryTypeLabel
                        text: "Geometry Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                     }
                    CustomTextBox{
                        id: doorGeometryTypeTextBox
                        placeholderText: "Geometry Type"
                        text:""
                        color: "#323130"
                    }



                    Text{
                        id: doorGeometryTypeLabelDummy
                        text:""// "Geometry TypeDummy"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                     }
                    Text{//CustomTextBox{
                        id: doorGeometryTypeTextBoxDummy
                        //placeholderText: "Geometry TypeDummy"
                        text:""
                        color: "#323130"
                        topPadding: 25
                    }
                }
            }
        }
    }

   //Components for Door Model
    Component {
        id: modelDelegateComponentForDoor
        RowLayout {
            id:doorRowLayoutModel
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (doorRowLayoutModel.width - doorRowLayoutModel.spacing) / 2-20
                height: 300
            }
        }
    }

    //Components for Door Classification and Properties
    Component {
        id: cpDelegateComponentForDoor
        RowLayout {
            id: doorRowLayoutCP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (doorRowLayoutCP.width - doorRowLayoutCP.spacing) / 2-20
                height: 300
            }
        }
    }
    //End of DoorSettings Component


    //Components for Geometry and Positioning For WindowSettings
    Component {
        id: gpDelegateComponentForWindow
        RowLayout {
            id:windowRowLayoutGP
            width: parent.width
            height: 400
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (windowRowLayoutGP.width - windowRowLayoutGP.spacing) / 2-10
                height: 400

                ColumnLayout {
                    width: firstColumn.width
                    height: firstColumn.height

                    Text{
                        id: windowTopLinkLabel
                        text: "Window top link"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: windowTopLinkTextBox
                        placeholderText: "Top Link"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: windowHeightFromTopLabel
                        text: "Height From Top"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: windowHeightFromTopTextBox
                        placeholderText: "Height From Top"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: windowTotalHeightLabel
                        text: "Total Height"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: windowTotalHeightTextBox
                        placeholderText: "Total Height"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            windowSettingsPopup.windowTotalHeightText = windowTotalHeightTextBox.text;
                        }
                    }


                    Text{
                        id: windowWidthLabel
                        text: "Width"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: windowWidthTextBox
                        placeholderText: "Width"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            windowSettingsPopup.windowWidthText = windowWidthTextBox.text;
                        }
                    }


                    Text{
                        id: windowTypeLabel
                        text: "Window Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: windowTypeTextBox
                        placeholderText: "Window Type"
                        text:""
                        color: "#323130"
                    }
                }
            }

            Rectangle {
                id:secondColumn
                width: (windowRowLayoutGP.width - windowRowLayoutGP.spacing) / 2-10
                height: 400

                ColumnLayout {
                    width: secondColumn.width
                    height: secondColumn.height

                    Text{
                        id: windowHomeFloorLabel
                        text: "Home Floor"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: windowHomeFloorTextBox
                        placeholderText: "Home Floor"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: windowHeightFromBottomLabel
                        text: "Height From Bottom"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: windowHeightFromBottomTextBox
                        placeholderText: "Height From Bottom"
                        text:""
                        color: "#323130"
                    }

                    Text{
                        id: windowDistanceLabel
                        text: "Distance From Level"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: windowDistanceTextBox
                        placeholderText: "Distance"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            windowSettingsPopup.windowDistanceText = windowDistanceTextBox.text;
                        }
                    }


                    Text{
                        id: windowGeometryTypeLabel
                        text: "Geometry Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                     }
                    CustomTextBox{
                        id: windowGeometryTypeTextBox
                        placeholderText: "Geometry Type"
                        text:""
                        color: "#323130"
                    }



                    Text{
                        id: windowGeometryTypeLabelDummy
                        text:""// "Geometry TypeDummy"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10

                     }
                    Text{//CustomTextBox{
                        id: windowGeometryTypeTextBoxDummy
                        //placeholderText: "Geometry TypeDummy"
                        text:""
                        color: "#323130"
                        topPadding: 25
                    }
                }
            }
        }
    }

   //Components for Window Model
    Component {
        id: modelDelegateComponentForWindow
        RowLayout {
            id:windowRowLayoutModel
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (windowRowLayoutModel.width - windowRowLayoutModel.spacing) / 2-20
                height: 300
            }
        }
    }

    //Components for Window Classification and Properties
    Component {
        id: cpDelegateComponentForWindow
        RowLayout {
            id: windowRowLayoutCP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (windowRowLayoutCP.width - windowRowLayoutCP.spacing) / 2-20
                height: 300
            }
        }
    }
    //End of WindowSettings Component


    //Components for Geometry and Positioning For StairsSettings
    Component {
        id: gpDelegateComponentForStairs
        RowLayout {
            id:stairsRowLayoutGP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (stairsRowLayoutGP.width - stairsRowLayoutGP.spacing) / 2-10
                height: 300

                ColumnLayout {
                    width: firstColumn.width
                    height: firstColumn.height

                    Text{
                        id: stairsTopLinkLabel
                        text: "Stairs top link"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: stairsTopLinkTextBox
                        placeholderText: "Top Link"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: stairsHeightFromTopLabel
                        text: "Height From Top"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: stairsHeightFromTopTextBox
                        placeholderText: "Height From Top"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: stairsTotalHeightLabel
                        text: "Total Height"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: stairsTotalHeightTextBox
                        placeholderText: "Total Height"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            stairsSettingsPopup.stairsTotalHeightText = stairsTotalHeightTextBox.text;
                        }
                    }


                    Text{
                        id: stairsTypeLabel
                        text: "Stairs Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: stairsTypeTextBox
                        placeholderText: "Stairs Type"
                        text:""
                        color: "#323130"
                    }
                }
            }

            Rectangle {
                id:secondColumn
                width: (stairsRowLayoutGP.width - stairsRowLayoutGP.spacing) / 2-10
                height: 300

                ColumnLayout {
                    width: secondColumn.width
                    height: secondColumn.height

                    Text{
                        id: stairsHomeFloorLabel
                        text: "Home Floor"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                    }
                    CustomTextBox{
                        id: stairsHomeFloorTextBox
                        placeholderText: "Home Floor"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: stairsHeightFromBottomLabel
                        text: "Height From Bottom"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: stairsHeightFromBottomTextBox
                        placeholderText: "Height From Bottom"
                        text:""
                        color: "#323130"
                    }


                    Text{
                        id: stairsDistanceLabel
                        text: "Distance From Level"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: stairsDistanceTextBox
                        placeholderText: "Distance"
                        text:""
                        color: "#323130"

                        onTextChanged: {
                            stairsSettingsPopup.stairsDistanceText = stairsDistanceTextBox.text;
                        }
                    }


                    Text{
                        id: stairsGeometryTypeLabel
                        text: "Geometry Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                     }
                    CustomTextBox{
                        id: stairsGeometryTypeTextBox
                        placeholderText: "Geometry Type"
                        text:""
                        color: "#323130"
                    }
                }
            }
        }
    }

   //Components for Stairs Model
    Component {
        id: modelDelegateComponentForStairs
        RowLayout {
            id:stairsRowLayoutModel
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (stairsRowLayoutModel.width - stairsRowLayoutModel.spacing) / 2-20
                height: 300
            }
        }
    }

    //Components for Stairs Classification and Properties
    Component {
        id: cpDelegateComponentForStairs
        RowLayout {
            id: stairsRowLayoutCP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (stairsRowLayoutCP.width - stairsRowLayoutCP.spacing) / 2-20
                height: 300
            }
        }
    }
    //End of StairsSettings Component


    //Components for Geometry and Positioning For MorphSettings
    Component {
        id: gpDelegateComponentForMorph
        RowLayout {
            id:morphRowLayoutGP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (morphRowLayoutGP.width - morphRowLayoutGP.spacing) / 2-10
                height: 300
            }

            Rectangle {
                id:secondColumn
                width: (morphRowLayoutGP.width - morphRowLayoutGP.spacing) / 2-10
                height: 300
            }
        }
    }

   //Components for morph Model
    Component {
        id: modelDelegateComponentForMorph
        RowLayout {
            id:morphRowLayoutModel
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (morphRowLayoutModel.width - morphRowLayoutModel.spacing) / 2-20
                height: 300
            }
        }
    }

    //Components for morph Classification and Properties
    Component {
        id: cpDelegateComponentForMorph
        RowLayout {
            id: morphRowLayoutCP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (morphRowLayoutCP.width - morphRowLayoutCP.spacing) / 2-20
                height: 300
            }
        }
    }
    //End of MorphSettings Component
}
