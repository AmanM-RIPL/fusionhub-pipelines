import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Column {
    id: root
    width: parent.width
    spacing: 10

    property var model: []
    property var columns: []

    property string searchText: ""
    property int rowsPerPage: 10
    property int currentPage: 1

    // Filtered list according to search
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

    // model length
    property int totalRows: filteredModel.length

    property int pageCount: totalRows > 0 ? Math.ceil(totalRows / rowsPerPage) : 0

    // Paginated data
    property var pageData: {
        if (filteredModel.length === 0)
            return []

        let start = (currentPage - 1) * rowsPerPage
        let end = currentPage * rowsPerPage
        return filteredModel.slice(start, end)
    }

    /*------------------------------------------------------
     Header row
    ------------------------------------------------------*/
    Row {
        spacing: 1
        height: 35
        Repeater {
            model: columns
            delegate: Rectangle {
                width: modelData.width
                height: 35
                color: "#e5e5e5"
                radius: 4

                Text {
                    anchors.centerIn: parent
                    text: modelData.label
                    font.bold: true
                }
            }
        }
    }

    /*------------------------------------------------------
     Table body
    ------------------------------------------------------*/
    ListView {
        id: listView
        width: parent.width
        height: 260
        clip: true
        spacing: 0

        model: pageData

        delegate: Row {
            height: 38
            property var row: modelData ? modelData : ({})

            Repeater {
                model: columns
                delegate: Rectangle {
                    width: modelData.width
                    height: 38
                    color: "#ffffff"
                    border.width: 0

                    Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 0.5
                        color: "#dddddd"
                        anchors.bottom: parent.bottom
                    }

                    Text {
                        anchors.centerIn: parent
                        text: row[modelData.key] !== undefined ? row[modelData.key] : ""
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
        padding: 20
        color: "#666"
    }

    /*------------------------------------------------------
    Pagination  Bar
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
                delegate: Button {
                    width: 32
                    height: 32

                    property int pageNumber: {
                        if (pageCount <= 5) {
                            return index + 1
                        }
                        let start = Math.max(1, currentPage - 2)
                        let end = Math.min(pageCount, start + 4)
                        start = Math.max(1, end - 4)
                        return start + index
                    }

                    text: pageNumber

                    contentItem: Text {
                        text: parent.text
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: currentPage === pageNumber ? "white" : "black"
                        font.pixelSize: 13
                    }

                    background: Rectangle {
                        radius: 2
                        color: currentPage === pageNumber ? "#4a90e2" : "white"
                        border.color: currentPage === pageNumber ? "#4a90e2" : "#ddd"
                        border.width: 1
                    }

                    onClicked: currentPage = pageNumber
                }
            }
            Button {
                width: 32
                height: 32
                text: "▶"
                enabled: currentPage < pageCount
                onClicked: currentPage++
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
                text: "⏭"
                enabled: currentPage < pageCount
                onClicked: currentPage = pageCount
                background: Rectangle {
                    radius: 2
                    color: parent.enabled ? (parent.pressed ? "#e0e0e0" : "white") : "#f5f5f5"
                    border.color: "#ddd"
                    border.width: 1
                }
            }
        }

        // Spacer to push right elements to the right
        Item {
            Layout.fillWidth: true
        }

        // RIGHT SIDE - ROWS PER PAGE DROPDOWN
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
    }
}
