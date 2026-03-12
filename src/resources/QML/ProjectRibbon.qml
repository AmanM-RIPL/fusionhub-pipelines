import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.defaults 1.0

Column {
    id: projectRibbon

    signal logOutButtonClicked()
    signal projectPageToRedirect(string pageName)
    signal projectPageAction()

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
            onClicked: {
                projectPageToRedirect("TaskBoard")                
            }
        }
        TabButton {            
            text: qsTr("BIM")
            onClicked: {
                projectPageToRedirect("PlannedBIM")
            }
        }
        TabButton {
            text: qsTr("Analytics")
        }
        TabButton {
            text: qsTr("Master")
            onClicked: {
                projectPageToRedirect("ScheduleSetup")
            }
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
                    btnSource: "qrc:/resources/images/po_icon.png"
                    btnName: "Gantt"
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
                    btnSource: "qrc:/resources/images/po_icon.png"
                    btnName: "Schedule"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            projectPageToRedirect("Schedule")
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }


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
                              projectPageToRedirect("File")
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
                              projectPageToRedirect("ScheduleOfRates")
                        }
                    }
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/boq_icon.png"
                    btnName: "BillOfQuantity"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                             projectPageToRedirect("BillOfQuantity")
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
                             projectPageToRedirect("ProjectBudget")
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
                            projectPageToRedirect("WorkOrder")
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
                            projectPageToRedirect("WorkBilling")
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
                              projectPageToRedirect("PurchaseOrder")
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
                             projectPageToRedirect("GoodReceivedNote")
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
                             projectPageToRedirect("MaterialIndent")
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

                            EditOption.viewType = "ModelView";
                            projectPageAction();
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

                            EditOption.viewType = "PlanView";
                            projectPageAction();
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
                           // projectPageToRedirect("Gantt")
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

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }


                RibbonButton {
                    btnSource: "qrc:/resources/images/line.svg"
                    btnName: "Line"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            EditOption.editType = "line";
                            projectPageAction();
                        }
                    }
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/circle.svg"
                    btnName: "3PT Circle"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            EditOption.editType = "3pt-circle";
                            projectPageAction();
                        }
                    }
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/bezier.svg"
                    btnName: "Bezier"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            EditOption.editType = "bezier";
                            projectPageAction();
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/cube.svg"
                    btnName: "Cube"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            EditOption.editType = "cube";
                            projectPageAction();
                        }
                    }
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/sphere.svg"
                    btnName: "Sphere"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            EditOption.editType = "sphere";
                            projectPageAction();
                        }
                    }
                }

                Rectangle {
                    width: 1
                    height: 60
                    color: "#7676801F"
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/copy.svg"
                    btnName: "Copy"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            EditOption.editType = "copy";
                            projectPageAction();
                        }
                    }
                }

                RibbonButton {
                    btnSource: "qrc:/resources/images/move.svg"
                    btnName: "Move"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            EditOption.editType = "move";
                            projectPageAction();
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

                RibbonButton {
                    btnSource: "qrc:/resources/images/budget_icon.png"
                    btnName: "UOM"
                    btnNameColor: "#000000"
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        anchors.fill: parent

                        onClicked: {
                            projectPageToRedirect("UnitOfMeasurement")
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
