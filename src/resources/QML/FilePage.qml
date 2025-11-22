import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.platform 1.0
import com.fh.models 1.0
import com.fh.controllers 1.0

Column {
    id: fileRoot
    width: parent.width
    padding: 10

    property var fileList: []
    property var fileListForTable: []
    property bool isApproved: false

    FileController {
        id: fileController
    }

    /* --------------------------------------------------
       FILE SELECTOR
    -----------------------------------------------------*/
    FileDialog {
        id: fileChooser
        title: "Select File"
        onAccepted: {
            let fileUrl = fileChooser.currentFile.toString();
            let localPath = fileUrl.replace("file:///", "").replace("file://", "");

            // Copy file through C++
            let savedPath = fileController.copyFileToFusionHub(localPath);

            if (savedPath !== "") {
                fileUrlTextBox.text = fileUrl;
                fileLocalPathTextBox.text = savedPath;   // final saved location
            } else {
                console.log("Copy failed");
            }
        }

    }

    /* --------------------------------------------------
       NEW FILE POPUP
    -----------------------------------------------------*/
    FHPopup {
        id: newFilePopup
        popupWidth: 500
        popupHeight: 380
        title: "New File"

        onAcceptCallback: function () {
            fileController.create(
                descriptionTextBox.text,
                fileUrlTextBox.text,
                fileLocalPathTextBox.text
            );

            descriptionTextBox.text = "";
            fileUrlTextBox.text = "";
            fileLocalPathTextBox.text = "";
            showList();
        }

        onCancelCallback: function () {
            descriptionTextBox.text = "";
            fileUrlTextBox.text = "";
            fileLocalPathTextBox.text = "";
        }

        Column {
            width: parent.width
            spacing: 10

            Text {
                text: "Description"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomTextBox {
                id: descriptionTextBox
                placeholderText: "File Description"
                color: "#323130"
            }

            Text {
                text: "File URL"
                color: "#323130"
                font.weight: 700
                font.pixelSize: 14
                font.family: "Segoe UI"
            }

            CustomTextBox {
                id: fileUrlTextBox
                placeholderText: "File URL"
                readOnly: true
                color: "#323130"
            }

            // Text {
            //     text: "File Local Path"
            //     color: "#323130"
            //     font.weight: 700
            //     font.pixelSize: 14
            //     font.family: "Segoe UI"
            // }

            CustomTextBox {
                id: fileLocalPathTextBox
                placeholderText: "Local File Path"
                readOnly: true
                visible: false
            }

            // ------------- Choose File Button -------------------
            CustomButton {
                color: "#007AFF"
                width: 150
                height: 36
                radius: 4
                btnName: "Choose File"
                btnNameColor: "white"

                MouseArea {
                    anchors.fill: parent
                    onClicked: fileChooser.open()
                }
            }
        }
    }

    /* --------------------------------------------------
       HEADER
    -----------------------------------------------------*/
    Row {
        spacing: 20
        Text {
            text: "File List"
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
                onClicked: newFilePopup.open()
            }
        }
    }

    Rectangle { width: 100; height: 40; color: "#EDF1F4" }

    /* --------------------------------------------------
       APPROVAL TYPE
    -----------------------------------------------------*/
    Row {
        spacing: 20
        Text {
            text: "Choose Approval Type"
            color: "#323130"
            font.weight: 700
            font.pixelSize: 14
            font.family: "Segoe UI"
            leftPadding: 20
        }

        CustomComboBox {
            id: approvalTypeComboBox
            model: ["Approved", "Draft"]
            width: 200

            onCurrentTextChanged: {
                isApproved = (currentText === "Approved");
                showList();
            }
        }
    }

    Rectangle { width: 100; height: 40; color: "#EDF1F4" }

    /* --------------------------------------------------
       TABLE
    -----------------------------------------------------*/
    FHTable {
        leftPadding: 20
        height: 300
        model: fileRoot.fileListForTable
        columns: [
            { label: "Id", width: 100, key: "id" },
            { label: "Description", width: 500, key: "description" },
            { label: "File URL", width: 500, key: "fileUrl" },
            { label: "Action", width: 150, key: "action" }

        ]
    }

    /* --------------------------------------------------
       LOAD FILE LIST
    -----------------------------------------------------*/
    Component.onCompleted: showList()
    onVisibleChanged: showList()

    function showList() {

        if (!fileRoot.visible)
            return;

        // Get C++ list
        fileList = fileController.getFileList(isApproved);

        // Convert to table data
        fileListForTable = fileList.map(file => {
            let fileName = file.fileUrl.split('/').pop();
            return {
                id: file.id,
                description: file.description,
                fileUrl: fileName,
                fileLocalPath: file.fileLocalPath,
                action: "View"
            }
        });
    }
}
