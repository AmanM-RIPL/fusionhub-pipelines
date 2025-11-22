import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

Column {

    signal logOutButtonClicked()
    signal projectPageToRedirect(string pageName)
    signal projectPageAction(string pageAction)


    width: parent.width
    height: 131
    // border.color: "black"
    // anchors.horizontalCenter: parent.horizontalCenter
    // anchors.top: parent.top
    // anchors.topMargin: 23
    TabBar {
        id: mainBar
        width: 300

        TabButton {
            text: qsTr("Project")
        }
        TabButton {
            text: qsTr("BIM")
        }
        TabButton {
            text: qsTr("Analytics")
        }
        TabButton {
            text: qsTr("Master")
        }
    }

    StackLayout {
        width: parent.width
        currentIndex: mainBar.currentIndex

        Rectangle {
            width: parent.width - 20
            height: 100
            radius: 8
            border.width: 1
            border.color: "#7676801F"//"#8A888629"
            color: "white"
            //anchors.horizontalCenter: parent.horizontalCenter
            Layout.alignment: parent.anchors.alignWhenCentered
            onVisibleChanged:   {
                projectPageToRedirect("TaskBoard")
            }


            Row {
                spacing: 7
                padding: 10

                RibbonButton {
                    btnSource: "qrc:/resources/images/task_board_icon.png"
                    btnName: "Task Board"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            projectPageToRedirect("TaskBoard")
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/files_icon.png"
                    btnName: "Files"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            // newProjectClicked()
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/sor_icon.png"
                    btnName: "Rates"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            // newProjectClicked()
                        }
                    }
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/boq_icon.png"
                    btnName: "BOQs"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            // newProjectClicked()
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }


                RibbonButton {
                    btnSource: "qrc:/resources/images/budget_icon.png"
                    btnName: "Budget"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            // newProjectClicked()
                        }
                    }
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/wo_icon.png"
                    btnName: "Work Order"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            // newProjectClicked()
                        }
                    }
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/billing_icon.png"
                    btnName: "Billing"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            // newProjectClicked()
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/po_icon.png"
                    btnName: "PO"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            // newProjectClicked()
                        }
                    }
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/grn_icon.png"
                    btnName: "GRN"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            // newProjectClicked()
                        }
                    }
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/indent_icon.png"
                    btnName: "Indent"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            // newProjectClicked()
                        }
                    }
                }
            }


            CustomButton {
                color: "#007AFF"
                width: 89
                height: 38
                radius: 4
                // border.color: "#007AFF"
                btnSource: "qrc:/resources/images/logout.svg"
                btnName: "Logout"
                btnNameColor: "white"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 13

                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        logOutButtonClicked()
                    }
                }
            }


        }

        Rectangle {
            width: parent.width - 20
            height: 100
            radius: 8
            border.width: 1
            border.color: "#7676801F"//"#8A888629"
            color: "white"
            //anchors.horizontalCenter: parent.horizontalCenter
            Layout.alignment: parent.anchors.alignWhenCentered

            onVisibleChanged:   {
                  projectPageToRedirect("PlannedBIM")
            }


            Row {
                spacing: 7
                padding: 10

                RibbonButton {
                    btnSource: "qrc:/resources/images/bim_icon.png"
                    btnName: "Model"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {                            
                            projectPageToRedirect("PlannedBIM")
                            projectPageAction("ModelView")
                        }
                    }
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/blueprint.png"
                    btnName: "Plan"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            projectPageToRedirect("PlannedBIM")
                            projectPageAction("PlanView")
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/measure_icon.png"
                    btnName: "Measure"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            projectPageToRedirect("Gantt")
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/screenshot_icon.png"
                    btnName: "Snapshot"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            //projectPageToRedirect("Vendor")
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }


                RibbonButton {
                    btnSource: "qrc:/resources/images/collision_icon.png"
                    btnName: "Collision"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            projectPageToRedirect("Collision")
                        }
                    }
                }
            }


            CustomButton {
                color: "#007AFF"
                width: 89
                height: 38
                radius: 4
                // border.color: "#007AFF"
                btnSource: "qrc:/resources/images/logout.svg"
                btnName: "Logout"
                btnNameColor: "white"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 13

                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        logOutButtonClicked()
                    }
                }
            }


        }


        Item {
            id: activityTab
            onVisibleChanged:   {
                //This will be changed after Implemention of Analytics
                  projectPageToRedirect("Analytics")
            }
        }

        Rectangle {
            width: parent.width - 20
            height: 100
            radius: 8
            border.width: 1
            border.color: "#7676801F"//"#8A888629"
            color: "white"
            //anchors.horizontalCenter: parent.horizontalCenter
            Layout.alignment: parent.anchors.alignWhenCentered
            onVisibleChanged:   {
                 projectPageToRedirect("ScheduleSetup")
            }


            Row {
                spacing: 7
                padding: 10

                RibbonButton {
                    btnSource: "qrc:/resources/images/task_board_icon.png"
                    btnName: "Setup"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            projectPageToRedirect("ScheduleSetup")
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/files_icon.png"
                    btnName: "Material"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            projectPageToRedirect("Material")
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/sor_icon.png"
                    btnName: "Vendor"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            projectPageToRedirect("Vendor")
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }


                RibbonButton {
                    btnSource: "qrc:/resources/images/budget_icon.png"
                    btnName: "Budget Head"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            projectPageToRedirect("BudgetHead")
                        }
                    }
                }
            }


            CustomButton {
                color: "#007AFF"
                width: 89
                height: 38
                radius: 4
                // border.color: "#007AFF"
                btnSource: "qrc:/resources/images/logout.svg"
                btnName: "Logout"
                btnNameColor: "white"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 13

                MouseArea{
                    anchors.fill: parent

                    onClicked: {
                        logOutButtonClicked()
                    }
                }
            }


        }

    }
}
