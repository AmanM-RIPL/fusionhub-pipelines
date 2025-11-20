import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.fh.models 1.0


Rectangle {
    // width: 1440
    // height: 1024
    id: baseLayout
    width: Screen.width
    height: Screen.height
    anchors.fill: parent
    color: "#EDF1F4"

    signal logOutClicked()
    property string pageName: "TaskBoard";
    property string projectName: "NA";


    Column {
        width: parent.width
        height: parent.height

        ProjectRibbon {
            onLogOutButtonClicked: {
                logOutClicked()
            }

            onProjectPageToRedirect: (pageName) => {
                console.log(pageName);
                baseLayout.pageName = pageName;

            }
        }


        BudgetHeadPage {            
            visible: pageName === "BudgetHead"

        }

        VendorPage {
            visible: pageName === "Vendor"
        }

        MaterialPage {
           visible: pageName === "Material"
        }

        ScheduleSetupPage {
            visible: pageName === "ScheduleSetup"
        }


        /*

          To be added once BIM related stuff is added

        */

         //PlannedBIMPage {
         //   visible: pageName === "PlannedBIM" || pageName === "Gantt" || pageName === "Collision"
         //   pageType: pageName
         //}

        Rectangle{
            id: taskBoard
            width: parent.width
            height: 1024 - (131 + 13)
            //color: parent.color
            // border.color: "red"
            //anchors.bottom: parent.bottom
            visible: pageName === "TaskBoard"

            TaskBoard{
                anchors.fill: parent
                txtProjectName: projectName
            }
        }

        PlannedBIMPage {
            //id:rootPlannedBIM
            pageType: pageName
            visible: pageType !== ""
            onPageTypeChanged: {
                switch (pageType) {
                case "PlannedBIM":
                    treeviewWidth = 200
                    glsceneWidth = parent.width - treeviewWidth - 30
                    glsceneVisible = true
                    break;
                case "Gantt":
                case "Collision":
                    treeviewWidth = parent.width - 20
                    glsceneWidth = parent.width - treeviewWidth
                    glsceneVisible = false
                    break;
                default:
                    treeviewWidth = 0
                    glsceneWidth = 0
                    glsceneVisible = false
                    break;
                }

            }
        }
    }
}
