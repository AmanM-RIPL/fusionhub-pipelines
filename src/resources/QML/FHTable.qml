import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Column {
    id: root
    width: parent.width
    spacing: 2

    property int tableBodyHeight: 260

    // Model and columns
    property var model: []
    property var columns: []

    // Search and pagination
    property string searchText: ""
    property int rowsPerPage: 10
    property int currentPage: 1

    // Signals
    signal viewRequested(var row)
    signal editRequested(var row)

    // UI properties
    property int activeRowIndex: -1
    property bool removeRow: false
    property int removedIndex: -1
    property int headerHeight: 35
    property int headerFontPixelSize: 12
    property int rowHeight: 38

    // Filtered model according to search
    property var filteredModel: {
        if (!model || model.length === 0)
            return []

        if (searchText === "")
            return model

        return model.filter(function(row) {
            for (var i = 0; i < columns.length; i++) {
                var col = columns[i]
                if (row[col.key] && String(row[col.key]).toLowerCase().includes(searchText.toLowerCase()))
                    return true
            }
            return false
        })
    }

    function appendRow(newRow) {
        let temp = root.model.slice()
        temp.push(newRow)
        root.model = temp
        currentPage = Math.ceil(root.model.length / rowsPerPage)
    }


    // Total rows and page count
    property int totalRows: filteredModel.length
    property int pageCount: totalRows > 0 ? Math.ceil(totalRows / rowsPerPage) : 0

    // Paginated data - Add explicit dependencies to prevent binding loops
    property var pageData: {
        // Reference dependencies explicitly
        var filtered = filteredModel
        var page = currentPage
        var rows = rowsPerPage

        if (filtered.length === 0)
            return []

        let start = (page - 1) * rows
        let end = page * rows
        return filtered.slice(start, end)
    }

    // Calculate total column width
    readonly property int totalColumnWidth: {
        var total = 0
        for (var i = 0; i < columns.length; i++) {
            total += columns[i].width
        }
        total = total + (spacing * (columns.length - 1))
        return total
    }

    /*------------------------------------------------------
     Header Row
    ------------------------------------------------------*/
    Row {
        id: headerRow
        spacing: 1
        height: headerHeight

        Repeater {
            model: columns
            delegate: Rectangle {
                width: modelData.width
                height: headerHeight
                color: "#E0E0E0"

                Text {
                    anchors.centerIn: parent
                    text: modelData.label
                    font.bold: true
                    font.pixelSize: headerFontPixelSize
                }
            }
        }
    }

    /*------------------------------------------------------
     Table Body
    ------------------------------------------------------*/
    Rectangle {
        width: headerRow.width
        //height: 260
        height: tableBodyHeight
        border.width: 1
        border.color: "#7676801F"
        color: "white"

        ListView {
            id: listView
            anchors.fill: parent
            model: pageData
            clip: true
            focus: true
            spacing: 1

            delegate: Column {
                property var rowData: modelData
                property int rowIndex: index

                Row {
                    spacing: 1
                    height: rowHeight

                    // Remove button (optional)
                    Rectangle {
                        id: removeRowId
                        width: 10
                        height: rowHeight
                        color: "transparent"
                        visible: removeRow
                        anchors.verticalCenter: parent.verticalCenter

                        Text {
                            text: "x"
                            font.pixelSize: 15
                            color: "blue"
                            anchors.centerIn: parent
                        }

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onClicked: {
                                if (listView.currentIndex !== -1) {
                                    removedIndex = index
                                    console.log("removedIndex:", removedIndex, "removed:", index)
                                    root.model.splice(index, 1)
                                    //listView.model = root.pageData
                                    listView.model.splice(index, 1)
                                }
                            }


                        }
                    }

                    // Column cells
                    Repeater {
                        model: columns

                        delegate: Rectangle {
                            width: modelData.width
                            height: rowHeight
                            color: "#ffffff"
                            property bool isIdColumn: modelData.key === "id"

                            Rectangle {
                                anchors.left: parent.left
                                anchors.right: parent.right
                                anchors.bottom: parent.bottom
                                height: 0.5
                                color: "#EDF1F4"
                            }

                            // Regular text cell
                            Text {
                                visible: !isIdColumn
                                anchors.centerIn: parent
                                text: rowData[modelData.key] !== undefined ? rowData[modelData.key] : ""
                                font.pixelSize: 15
                            }

                            // ID column with action buttons
                            Item {
                                anchors.fill: parent
                                visible: isIdColumn

                                Image {
                                    id: dots
                                    source: "qrc:/resources/images/dotMenu.svg"
                                    width: 16
                                    height: 16
                                    fillMode: Image.PreserveAspectFit
                                    anchors.left: parent.left
                                    anchors.leftMargin: 8
                                    anchors.verticalCenter: parent.verticalCenter

                                    MouseArea {
                                        anchors.fill: parent
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            activeRowIndex = activeRowIndex === rowIndex ? -1 : rowIndex
                                        }
                                    }
                                }

                                // Action buttons
                                Row {
                                    spacing: 6
                                    anchors.left: dots.right
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.leftMargin: 6
                                    visible: activeRowIndex === rowIndex

                                    Rectangle {
                                        width: 50
                                        height: 24
                                        radius: 4
                                        color: viewMouseArea.pressed ? "#0056b3" : (viewMouseArea.containsMouse ? "#0069d9" : "#007AFF")

                                        Text {
                                            anchors.centerIn: parent
                                            text: "View"
                                            color: "white"
                                            font.pixelSize: 12
                                        }

                                        MouseArea {
                                            id: viewMouseArea
                                            anchors.fill: parent
                                            hoverEnabled: true
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: {
                                                viewRequested(rowData)
                                                activeRowIndex = -1
                                            }
                                        }
                                    }

                                    Rectangle {
                                        width: 50
                                        height: 24
                                        radius: 4
                                        color: editMouseArea.pressed ? "#0056b3" : (editMouseArea.containsMouse ? "#0069d9" : "#007AFF")

                                        Text {
                                            anchors.centerIn: parent
                                            text: "Edit"
                                            color: "white"
                                            font.pixelSize: 12
                                        }

                                        MouseArea {
                                            id: editMouseArea
                                            anchors.fill: parent
                                            hoverEnabled: true
                                            cursorShape: Qt.PointingHandCursor
                                            onClicked: {
                                                editRequested(rowData)
                                                activeRowIndex = -1
                                            }
                                        }
                                    }
                                }

                                Text {
                                    anchors.left: dots.right
                                    anchors.leftMargin: activeRowIndex === rowIndex ? 80 : 10
                                    anchors.verticalCenter: parent.verticalCenter
                                    text: rowData && rowData.id !== undefined ? String(rowData.id) : ""
                                    visible: activeRowIndex !== rowIndex
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Empty state message
    Text {
        visible: pageData.length === 0
        text: searchText ? "No results found" : "No data available"
        anchors.horizontalCenter: parent.horizontalCenter
        padding: 10
        color: "#666"
    }

    /*------------------------------------------------------
     Pagination Bar
    ------------------------------------------------------*/
    RowLayout {
        width: parent.width
        height: 40
        spacing: 10

        Row {
            id: bottomPagination
            spacing: 5
            Layout.alignment: Qt.AlignVCenter

            Button {
                width: 32
                height: 32
                text: "⏮"
                enabled: currentPage > 1
                onClicked: currentPage = 1
                background: Rectangle {
                    radius: 2
                    color: parent.enabled ? (parent.pressed ? "#e0e0e0" : "white") : "#f5f5f5"
                    border.color: "#ddd"
                    border.width: 1
                }
            }

            Button {
                width: 32
                height: 32
                text: "◀"
                enabled: currentPage > 1
                onClicked: currentPage--
                background: Rectangle {
                    radius: 2
                    color: parent.enabled ? (parent.pressed ? "#e0e0e0" : "white") : "#f5f5f5"
                    border.color: "#ddd"
                    border.width: 1
                }
            }

            Repeater {
                model: Math.min(pageCount, 5)
                delegate: Rectangle {
                    width: 32
                    height: 32
                    radius: 2

                    property int pageNumber: {
                        if (pageCount <= 5) {
                            return index + 1
                        }
                        let start = Math.max(1, currentPage - 2)
                        let end = Math.min(pageCount, start + 4)
                        start = Math.max(1, end - 4)
                        return start + index
                    }

                    color: currentPage === pageNumber ? "#4a90e2" : "white"
                    border.color: currentPage === pageNumber ? "#4a90e2" : "#ddd"
                    border.width: 1

                    Text {
                        anchors.centerIn: parent
                        text: pageNumber
                        color: currentPage === pageNumber ? "white" : "black"
                        font.pixelSize: 13
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: currentPage = pageNumber
                    }
                }
            }

            Rectangle {
                width: 32
                height: 32
                radius: 2
                color: nextPageMouseArea.pressed ? "#e0e0e0" : "white"
                border.color: "#ddd"
                border.width: 1
                opacity: currentPage < pageCount ? 1.0 : 0.5

                Text {
                    anchors.centerIn: parent
                    text: "▶"
                    color: currentPage < pageCount ? "black" : "#999"
                }

                MouseArea {
                    id: nextPageMouseArea
                    anchors.fill: parent
                    enabled: currentPage < pageCount
                    cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                    onClicked: currentPage++
                }
            }

            Rectangle {
                width: 32
                height: 32
                radius: 2
                color: lastPageMouseArea.pressed ? "#e0e0e0" : "white"
                border.color: "#ddd"
                border.width: 1
                opacity: currentPage < pageCount ? 1.0 : 0.5

                Text {
                    anchors.centerIn: parent
                    text: "⏭"
                    color: currentPage < pageCount ? "black" : "#999"
                }

                MouseArea {
                    id: lastPageMouseArea
                    anchors.fill: parent
                    enabled: currentPage < pageCount
                    cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
                    onClicked: currentPage = pageCount
                }
            }
        }

        // Rows per page dropdown
        Row {
            spacing: 8
            Layout.alignment: Qt.AlignVCenter

            ComboBox {
                id: rowsDropdown
                width: 70
                height: 32
                model: ["5", "10", "20", "50"]
                currentIndex: 1
                onCurrentTextChanged: {
                    rowsPerPage = parseInt(currentText)
                    currentPage = 1
                }
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: "Items per page"
                color: "#666"
                font.pixelSize: 13
            }
        }

        Item {
            Layout.fillWidth: true
        }

        // Items count info
        Text {
            Layout.alignment: Qt.AlignVCenter
            text: {
                if (totalRows === 0) return "Items 0 to 0 of 0"
                let start = (currentPage - 1) * rowsPerPage + 1
                let end = Math.min(currentPage * rowsPerPage, totalRows)
                return "Items " + start + " to " + end + " of " + totalRows
            }
            color: "#666"
            font.pixelSize: 13
        }
        Rectangle {
            width: 50
            height: 5
            color: "#EDF1F4"
        }
    }
}
