import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Column {
    width: totalColumnWidth
    spacing: 5//10
    id: tableRoot

    property var model
    property var columns // array of { label, key, width }
    property int headerHeight: 30
    property int headerFontPixelSize: 12
    property int monthScale: 5
    property var rowDataForGantt: []

    property int rowNumber: 0
   // property int colNumber:-1
    property var month_arr:{"jan":"01", "feb":"02", "mar":"03", "apr":"04", "may":"05", "jun":"06", "jul":"07", "aug":"08", "sep":"09", "oct":"10", "nov":"11", "dec":"12"}

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
            delegate: Column {
                //padding: 10
                property var rowData: modelData

                Component.onCompleted:
                {
                  rowNumber++;                  
                }

                Row {                    
                    spacing: 2

                    Repeater {
                        model: tableRoot.columns

                        Rectangle {
                            id:idMainRect
                            width: modelData.width
                            height: 30

                            RowLayout{
                                width: modelData.width
                                height: 30                                
                                Rectangle {
                                    id:idRect
                                    width: rowData[modelData.days] !== undefined ? rowData[modelData.days]*monthScale : modelData.width //monthScale = 5
                                    Layout.leftMargin: rowData[modelData.startx] !== undefined ? rowData[modelData.startx]*monthScale : 0
                                    height: 15
                                    color:"blue"

                                    Component.onCompleted: {

                                        rowDataForGantt = [];
                                        //console.log("rowkey:", modelData.key, "value:", rowData[modelData.key], "id:", rowData[modelData.id], "pid:", rowData[modelData.pid], "x:",idRect.x, "y:",idRect.y ,"width:", idRect.width, "height:", idRect.height );

                                        var keyValue = String(rowData[modelData.key]).trim();
                                        //console.log("modelData.key: ", modelData.key, "keyvalue:", keyValue)


                                        var startDate = String(rowData[modelData.startDate]);
                                        //console.log("startDate:", startDate)

                                        var YearValue = String(rowData[modelData.year]);
                                        //console.log("YearValue:", YearValue)

                                        var monthno =  month_arr[modelData.key]
                                        //console.log("monthno:", monthno)

                                        var startDay = String(rowData[modelData.startx]);
                                        //console.log("startday:", startDay)


                                        const pad = (num) => num.toString().padStart(2, '0');
                                        const formattedDate = `${pad(startDay)}/${pad(monthno)}/${YearValue}`;

                                       // console.log("formattedDate:", formattedDate);
                                        //idRect.color ="blue"

                                        if(keyValue.length > 0 && keyValue !== "0" && startDate.localeCompare(formattedDate) === 0 ){
                                            //idRect.color = "red"
                                             //console.log("startDate:", startDate, "formattedDate:", formattedDate);

                                            /*Qt.callLater(function() {
                                                 // 1. Get the absolute position of idRect on the entire screen/window
                                                var globalPoint = idRect.mapToGlobal(0, 0);

                                                // 2. Map that global screen point *back* into the local coordinate system of the 'linesOverlay'
                                                // We use linesOverlay.mapFromGlobal(globalX, globalY)
                                                var canvasPoint = linesOverlay.mapFromGlobal(globalPoint.x, globalPoint.y);

                                                var data = {x:0, y:0, id:0, pid:0, width:0, height:0, month:""};
                                                data.x = canvasPoint.x
                                                data.y = canvasPoint.y;

                                                data.id = rowData[modelData.id];
                                                data.pid = rowData[modelData.pid];
                                                data.width = idRect.width;
                                                data.height = idRect.height;
                                                data.month = modelData.key;

                                                rowDataForGantt.push(data);
                                                //linesOverlay.requestPaint();
                                            });*/
                                        }                                        
                                    }

                                    Text {
                                        id:idText
                                        text:rowData[modelData.key] !== undefined ? rowData[modelData.key] : ""
                                        font.pixelSize: 13
                                        anchors.centerIn: parent
                                        topPadding: 1
                                        bottomPadding: 3
                                    }
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
            Component.onCompleted: {
                linesOverlay.requestPaint()
            }

            onVisibleChanged: {
               linesOverlay.requestPaint()

           }
        }
        Canvas
        {
            id: linesOverlay
            anchors.fill: parent
            // This ensures the canvas is transparent and above the ListView visually
            z: 1
            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height); // Clear previous frame

                function drawArrowhead(ctx, fromX, fromY, toX, toY, size) {
                    const angle = Math.atan2(toY - fromY, toX - fromX); // Calculate the angle of the line
                    const angle1 = angle - Math.PI / 6; // Wing 1 angle (30 degrees offset)
                    const angle2 = angle + Math.PI / 6; // Wing 2 angle (30 degrees offset)

                    // Move to the endpoint of the main line
                    ctx.moveTo(toX, toY);
                    // Draw the first wing of the arrow head
                    ctx.lineTo(toX - size * Math.cos(angle1), toY - size * Math.sin(angle1));
                    ctx.moveTo(toX, toY); // Move back to the tip
                    // Draw the second wing of the arrow head
                    ctx.lineTo(toX - size * Math.cos(angle2), toY - size * Math.sin(angle2));
                }

                ctx.lineWidth = 2;
                ctx.strokeStyle = "red";
                ctx.beginPath();

                for(var i = 0; i < rowDataForGantt.length; i++){
                    var modelData = rowDataForGantt[i];
                    var x = modelData.x;
                    var y = modelData.y;
                    var id = modelData.id;
                    var pid = modelData.pid;
                    var month = modelData.month;

                    for(var j = 0; j < rowDataForGantt.length; j++){
                        var modelDataNew = rowDataForGantt[j] ;
                        var xNew = modelDataNew.x;
                        var yNew = modelDataNew.y;
                        var idNew = modelDataNew.id;
                        var pidNew = modelDataNew.pid;

                        //console.log("id:", id, "pidNew:", pidNew, "xNew:", xNew, "x:", x );

                        /*var r = month.localeCompare("dec");
                        if(r === 0 )
                        {
                             valuesToRemove.push(id);
                        }*/

                        if(id === pidNew)// && xNew !== x)
                        {
                            valuesToRemove.push(pidNew);

                            const arrowHeadSize = 7; // Size of the arrowhead wings
                            ctx.moveTo(modelData.x + modelData.width, modelData.y + 6);
                            ctx.lineTo(modelData.x + modelData.width + 3, modelData.y + 6 )

                            ctx.moveTo(modelData.x + modelData.width + 3, modelData.y + 6);
                            ctx.lineTo(modelData.x + modelData.width + 3, modelData.y + 37 )

                            ctx.moveTo(modelData.x + modelData.width + 3, modelData.y + 37);                            
                            ctx.lineTo(xNew, modelDataNew.y+7);

                            ctx.stroke();

                            var fromX = modelData.x + modelData.width + 3;
                            var fromY = modelData.y+37;                           
                            var toX = xNew;
                            var toY = modelDataNew.y+7;

                            drawArrowhead(ctx, fromX, fromY, toX, toY, arrowHeadSize);
                            ctx.stroke();
                        }                       
                    }
                }
            }
        }
    }   
}



