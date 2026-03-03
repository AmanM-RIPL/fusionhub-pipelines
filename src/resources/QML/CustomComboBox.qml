import QtQuick
import QtQuick.Controls.Basic

ComboBox {
    id: control
    model: ["First", "Second", "Third"]
    leftPadding: 9
    font.weight: 400
    font.pixelSize: 14    
    width: 450
    height: 34



    delegate: ItemDelegate {
        id: delegate

        required property var model
        required property int index

        width: control.width
        contentItem: Text {
            text: delegate.model[control.textRole]
            color: "#323130"
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: control.highlightedIndex === index
    }


    indicator: Canvas {
        id: canvas
        x: control.width - width - control.rightPadding
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 12
        height: 8
        contextType: "2d"

        Connections {
            target: control
            function onPressedChanged() { canvas.requestPaint(); }
        }


        onPaint: {
            var context = getContext("2d");//This line was not added before
            context.reset();
            context.moveTo(0, 0);
            context.lineTo(width, 0);
            context.lineTo(width / 2, height);
            context.closePath();
            context.fillStyle = control.pressed ? "#8A8886" : "#8A8886";
            context.fill();
        }
    }

    contentItem: Text {
        leftPadding: 0
        rightPadding: control.indicator.width + control.spacing

        text: control.displayText
        font: control.font
        color: control.pressed ? "#323130" : "#323130"
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
    }

    background: Rectangle {
        implicitWidth: 200
        implicitHeight: 40
        border.color: control.pressed ? "#8A888629" : "#8A888629"
        border.width: control.visualFocus ? 2 : 1
        radius: 2
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.color: "#8A888629"
            radius: 2
        }
    }
}
