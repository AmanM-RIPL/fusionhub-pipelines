import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts



Column {
    width: totalColumnWidth
    spacing: 5//10
    id: tableRoot

    property var model
    property var columns // array of { label, key, width }
    property bool removeRow: false
    property int removedIndex: -1
    property int headerHeight: 30
    property int headerFontPixelSize: 12
    //property string rowColor: "transparent"
    property bool isTextVisible: true
    property bool isRectVisible: false    

    readonly property int totalColumnWidth: {
        var total = 0;
        for (var i = 0; i < columns.length; i++) {
            total += columns[i].width;
        }

        total = total + (headerRow.spacing * (tableRoot.columns.length - 1)); // last spacing needs to be ignored

        return total;
    }


    Row {
        id: headerRow
        //spacing: 20
        spacing: 2
        width: parent.width

        Repeater {
            model: tableRoot.columns
            Rectangle {
                width: modelData.width
                height: headerHeight//30
                color: "#E0E0E0"

                Text {
                    anchors.centerIn: parent
                    text: modelData.label
                    font.weight: Font.Bold

                    font.pixelSize: headerFontPixelSize
                }
            }
        }
    }


    Rectangle {
        width: headerRow.width
        height: parent.height - 30
        // radius: 4
        border.width: 1
        border.color: "#7676801F"//"#8A888629"
        color: "white"


        ListView {
            id: listView
            anchors.fill: parent
            model: tableRoot.model
            clip: true
            focus: true

            /*
            ScrollBar.vertical: ScrollBar {
                        policy: ScrollBar.AlwaysOn
            }
            */

            delegate: Column {
                //padding: 10
                property var rowData: modelData

                Row {
                    //spacing: 20
                    spacing: 2

                    /************Row Removed Button***************/
                    Rectangle {
                        id:removeRowId
                        width: 10
                        height: 30
                        color: "transparent"
                        visible: removeRow
                        focus: true
                        anchors.verticalCenter: parent.verticalCenter
                    Text {
                          text: "x"
                          font.pixelSize: 15
                          color: "blue"
                        }



                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked:{

                                if (listView.currentIndex !== -1) {
                                    removedIndex = index
                                    console.log("removedIndex:" , removedIndex, "removd:", index)
                                    tableRoot.model.splice(index, 1)
                                    listView.model.splice(index, 1)
                                }
                            }
                        }
                    }
                    /*********Row Removed Button End************/


                    Repeater {
                        model: tableRoot.columns

                        Rectangle {                            
                            width: modelData.width
                            height: 30
                            Text {
                                text: rowData[modelData.key] !== undefined ? rowData[modelData.key] : ""
                                font.pixelSize: 15
                                anchors.centerIn: parent
                            }
                        }
                    }
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#EDF1F4"
                }
            }           
        }        
    }
}



