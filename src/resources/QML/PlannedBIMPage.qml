import QtQuick 2.15
import QtQuick.Controls
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
    property string pageAction: "ModelView";
    property int treeviewWidth: parent.width/2 - 20
    property int glsceneWidth: parent.width/2
    property bool glsceneVisible: false

    property int expandedIndex: -1

    onPageActionChanged: {
        if (pageAction === "ModelView" || pageAction === "PlanView")
        {
            glscene.updateView(plannedBIMRoot.pageAction);
        }
    }


    IFCWallController {
        id: wallController
    }

    BIMElementController{
        id: bimElementController
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
                model: treeModel
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
                                {itemName: "Opening"},
                                {itemName: "Roof"},
                                {itemName: "Stairs"}
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
                                    wallsettingsPopup.open();
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

        FHPopup {
            id: wallsettingsPopup
            popupWidth: 500
            popupHeight: 600
            title: "Wall Settings"
            //parent: Overlay
            anchors.centerIn: Overlay.overlay

            property string wallTotalHeightText: ""
            property string wallWidthText: ""


            onAcceptCallback: function () {                
                //wallController.create("projectname", wallTotalHeightTextBox.text, wallWidthTextBox.text);
                let bimElementPtr = bimElementController.create("Wall", "Front Wall", 0);
                bimElementController.addParameter(bimElementPtr, "Height", wallTotalHeightText);
                bimElementController.addParameter(bimElementPtr, "Width", wallWidthText);
                bimElementController.addParameter(bimElementPtr, "ReferenceLine", "[]");

                wallTotalHeightText = "";
                wallWidthText = "";
            }

            onCancelCallback: function () {
                wallTotalHeightText = "";
                wallWidthText= "";
            }


            ColumnLayout {
                id:columnLayout
                width: wallsettingsPopup.popupWidth-65
                height: wallsettingsPopup.popupHeight-65
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
                        height: expandedIndex === index ? 360 : 60
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
                            visible: expandedIndex === index                            
                            Loader {                                        
                                        Layout.fillWidth: true
                                        sourceComponent: {
                                            if (nameId.objectName === "0")
                                            {
                                                return gpDelegateComponent;
                                            }
                                            else if (nameId.objectName === "1")
                                            {
                                                return modelDelegateComponent;
                                            }
                                            else
                                            {
                                                return cpDelegateComponent;
                                            }
                                        }
                                    }
                        }//End of row
                        MouseArea {
                            anchors.fill: nameId
                            onClicked: {
                               //If this item is already expanded, collapse it. Otherwise, expand it.
                                if (expandedIndex === index) {
                                    expandedIndex = -1
                                }
                                else {
                                    expandedIndex = index                                    
                                    rectId.border.color = "lightgray"
                                }                                
                            }
                        }
                    }
                }
            }                        
        }
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
                // onPositionChanged: function(mouse) {
                //     glscene.mousePositionChanged(mouse.x, mouse.y);
                //     mouse.accepted = true;
                // }
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
                }
            }
        }
    }

    Component.onCompleted: {
        //if(plannedBIMRoot.visible){
            //ifcDetailList = ifcDetailRepository.getIFCDetails();
             // ifcDetailList = ifcDetailController.loadIFC(ifcDetailController.getIfcFilePath());
             //treeModel = ifcDetailController.getTreeModel();
        //}

    }

    onVisibleChanged: {
        //if(plannedBIMRoot.visible && plannedBIMRoot.pageType === "PlannedBIM"){
            //ifcDetailList = ifcDetailRepository.getIFCDetails();
            // ifcDetailList = ifcDetailController.loadIFC(ifcDetailController.getIfcFilePath());
             //treeModel = ifcDetailController.getTreeModel();
        //}
    }



    //Components for Geometry and Positioning
    Component {
        id: gpDelegateComponent
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
                            wallsettingsPopup.wallTotalHeightText = wallTotalHeightTextBox.text;
                        }
                    }


                    Text{
                        id: wallTypeLabel
                        text: "Wall Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                    }
                    CustomTextBox{
                        id: wallTypeTextBox
                        placeholderText: "Wall Type"
                        text:""
                        color: "#323130"
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
                            wallsettingsPopup.wallWidthText = wallWidthTextBox.text;
                        }
                    }


                    Text{
                        id: wallGeometryTypeLabel
                        text: "Geometry Type"
                        color: "#323130"
                        //font.weight: 700
                        font.pixelSize: 14
                        font.family: "Segoe UI"
                        topPadding: 10
                     }
                    CustomTextBox{
                        id: wallGeometryTypeTextBox
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
        id: modelDelegateComponent
        RowLayout {
            id:wallRowLayoutModel
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (wallRowLayoutModel.width - wallRowLayoutModel.spacing) / 2-20
                height: 300
            }
        }
    }    

    //Components for Classification and Properties
    Component {
        id: cpDelegateComponent
        RowLayout {
            id:wallRowLayoutCP
            width: parent.width
            height: 300
            spacing: 5
            Rectangle {
                id: firstColumn
                width: (wallRowLayoutCP.width - wallRowLayoutCP.spacing) / 2-20
                height: 300
            }
        }
    }
}
