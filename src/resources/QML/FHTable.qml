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

    property int monthScale: 5
    property var rowDataForGantt: []


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
                            // Text {
                            //     text: rowData[modelData.key] !== undefined ? rowData[modelData.key] : ""
                            //     font.pixelSize: 15
                            //     anchors.centerIn: parent
                            // }

                            RowLayout{
                                width: modelData.width
                                height: 30
                                Rectangle {
                                    id:idRect
                                    width: rowData[modelData.days] !== undefined ? rowData[modelData.days]*monthScale : modelData.width //monthScale = 5
                                    Layout.leftMargin: rowData[modelData.startx] !== undefined ? rowData[modelData.startx]*monthScale : 0
                                    height: 15
                                    color: rowColorFunc(modelData.key, rowData[modelData.key].trim(), rowData[modelData.id], rowData[modelData.pid],  idRect.x, idRect.y, idRect.width, idRect.height)

                                    Text {
                                        text: rowData[modelData.key] !== undefined ? rowData[modelData.key] : ""
                                        font.pixelSize: 15
                                        anchors.centerIn: parent
                                    }

                                    /*Component.onCompleted: {
                                         console.log("rowkey:", modelData.key, "value:", rowData[modelData.key], "id:", rowData[modelData.id], "pid:", rowData[modelData.pid], "x:",idRect.x, "y:",idRect.y ,"width:", idRect.width, "height:", idRect.height );
                                    }*/
                                }
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
            //onContentYChanged: linesOverlay.requestPaint()
        }
        /*Canvas
        {
            id: linesOverlay
            anchors.fill: parent

            // This ensures the canvas is transparent and above the ListView visually
            z: 1
            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height); // Clear previous frame
                ctx.lineWidth = 2;
                ctx.strokeStyle = "red";
                ctx.beginPath();

                //ctx.moveTo(startX, startY);
                //ctx.lineTo(endX, endY);

                var x = 100;// rowData[modelData.startx] !== undefined ? rowData[modelData.startx]*monthScale : 0

                if(x > 1)
                {
                    ctx.moveTo(0, 0);
                    ctx.lineTo(x, 200);
                }
                ctx.stroke();
            }
        }*/
    }


    function rowColorFunc(rowkey, value, id, pid, x, y, width, height) {
        if((rowkey === "jan" || rowkey === "feb" || rowkey === "mar" || rowkey === "apr"
          || rowkey === "may" || rowkey === "jun" || rowkey === "jul" || rowkey === "aug"
          || rowkey === "sep" || rowkey === "oct" || rowkey === "nov" || rowkey === "dec")
          //&& value !== "0" || value !== " ")
          && value.length > 0 && value !== "0" )
        {
            return "red"
        }
        return "transparent"
    }
}



