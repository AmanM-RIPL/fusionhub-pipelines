import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick
import com.fh.models 1.0

Row {
    // anchors.fill: parent
    id: plannedBIMRoot
    width: parent.width
    height: 500
    padding: 10
    spacing: 10

    property var ifcDetailList: [];
    property string pageType: "PlannedBIM";

    // Rectangle {
    //     width: parent.width/2 - 20
    //     height: parent.height
    //     color: "white"
    //     border.color: "#000000"

    //     ScrollView {
    //         id: horizontalScrollView
    //         width: parent.width
    //         height: 500
    //         padding: 20
    //         clip: true

    //         FHTable {
    //             visible: plannedBIMRoot.pageType === "Collision"
    //             height: 500
    //             leftPadding: 20
    //             model: plannedBIMRoot.ifcDetailList
    //             columns: [
    //                 { label: "Type", width: 300, key: "type" },
    //                 { label: "Clash Details", width: 300, key: "globalId" },
    //                 { label: "Name", width: 500, key: "name" },
    //             ]
    //         }

    //         Row {
    //             id: headerRow
    //             spacing: 20
    //             visible: plannedBIMRoot.pageType === "Gantt"
    //             width: parent.width
    //             anchors.left: parent.left
    //             anchors.top: parent.top

    //             Rectangle {
    //                 width: 200
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "Milestone"
    //                     font.weight: Font.Bold
    //                 }
    //             }

    //             Rectangle {
    //                 width: 50
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "JAN"
    //                     font.weight: Font.Bold
    //                 }
    //             }

    //             Rectangle {
    //                 width: 50
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "FEB"
    //                     font.weight: Font.Bold
    //                 }
    //             }

    //             Rectangle {
    //                 width: 50
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "MAR"
    //                     font.weight: Font.Bold
    //                 }
    //             }

    //             Rectangle {
    //                 width: 50
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "APR"
    //                     font.weight: Font.Bold
    //                 }
    //             }

    //             Rectangle {
    //                 width: 50
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "MAY"
    //                     font.weight: Font.Bold
    //                 }
    //             }

    //             Rectangle {
    //                 width: 50
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "JUN"
    //                     font.weight: Font.Bold
    //                 }
    //             }

    //             Rectangle {
    //                 width: 50
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "JUL"
    //                     font.weight: Font.Bold
    //                 }
    //             }

    //             Rectangle {
    //                 width: 50
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "AUG"
    //                     font.weight: Font.Bold
    //                 }
    //             }

    //             Rectangle {
    //                 width: 50
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "SEP"
    //                     font.weight: Font.Bold
    //                 }
    //             }

    //             Rectangle {
    //                 width: 50
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "OCT"
    //                     font.weight: Font.Bold
    //                 }
    //             }

    //             Rectangle {
    //                 width: 50
    //                 height: 30
    //                 color: "#E0E0E0"
    //                 Text {
    //                     anchors.centerIn: parent
    //                     text: "NOV"
    //                     font.weight: Font.Bold
    //                 }
    //             }
    //         }

    //         TreeView {
    //             visible: plannedBIMRoot.pageType === "Gantt"
    //             anchors.fill: parent
    //             anchors.left: parent.left
    //             anchors.top: parent.top
    //             anchors.leftMargin: 10
    //             anchors.topMargin: 50
    //             delegate: TreeViewDelegate {
    //                 onClicked: {
    //                     // Object.keys(model).forEach((x) => {
    //                     //     console.log(x);
    //                     // });

    //                     // console.log("Row" + model.row);
    //                     // console.log("Column" + model.column);
    //                     // console.log(model.display);

    //                     // console.log(depth);
    //                     // console.log(indentation);
    //                 }

    //                 indicator: Item {
    //                     x: 0

    //                     Text {
    //                         anchors.left: parent.left
    //                         text: "+"
    //                         font.pixelSize: 12
    //                     }
    //                 }

    //                 contentItem: Item {
    //                     anchors.fill: parent

    //                     // Column 2 is the GlobalId column
    //                     Rectangle {
    //                         anchors.fill: parent
    //                         color: "transparent"

    //                         function getWidth(level) {
    //                             if (level === 0) {
    //                                 return 600
    //                             } else if (level === 1) {
    //                                 return 400
    //                             } else if (level === 2) {
    //                                 return 350
    //                             } else if (level === 3) {
    //                                 return 300
    //                             } else if (level === 4) {
    //                                 return 150
    //                             } else {
    //                                 return 50
    //                             }
    //                         }

    //                         // Show a colored bar for the Gantt chart cell
    //                         Rectangle {
    //                             visible: model.column === 2
    //                             x: 200 + (depth * 20) + (model.row*2) // leftMargin + (depth * indentation) + 10 //x: 5
    //                             y: parent.height / 4
    //                             height: parent.height / 2
    //                             width: parent.getWidth(depth) //100   // you can bind this to some duration property
    //                             color: "skyblue"
    //                             radius: 1
    //                         }

    //                         // Fallback: normal text for other columns
    //                         Text {
    //                             visible: model.column === 0
    //                             anchors.verticalCenter: parent.verticalCenter
    //                             anchors.left: parent.left
    //                             leftPadding: 50 //leftMargin + (depth * indentation) + 10
    //                             text: model.display
    //                             font.pixelSize: 12
    //                         }
    //                     }
    //                 }
    //             }
    //             model: treeModel
    //         }

    //         TreeView {
    //             visible: plannedBIMRoot.pageType === "PlannedBIM"
    //             anchors.fill: parent
    //             anchors.margins: 10
    //             delegate: TreeViewDelegate {}
    //             model: treeModel
    //         }

    //     }
    // }

    Rectangle {
        width: parent.width/2
        height: 500

        GLScene {
            id: glscene
            anchors.fill: parent

            MouseArea {
                anchors.fill: parent
                // onWheel: function(wheel) { // Declare 'wheel' as a formal parameter
                //     if (wheel.modifiers & Qt.ControlModifier) {
                //         if (wheel.angleDelta.y > 0) {
                //             glscene.zoomIn();
                //         } else {
                //             glscene.zoomOut();
                //         }
                //         wheel.accepted = true;
                //     } else {
                //         wheel.accepted = false;
                //     }
                // }
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
                }
            }
        }
    }

    // Component.onCompleted: {
    //     ifcDetailList = ifcDetailRepository.getIFCDetails();
    // }
}
