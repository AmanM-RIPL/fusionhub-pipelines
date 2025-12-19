#include "database/database_manager.h"
#include <QDebug>
#include <QCoreApplication>

extern QString gEnvironmentPath;

std::shared_ptr<DatabaseManager> DatabaseManager::instance = nullptr;

std::shared_ptr<DatabaseManager> DatabaseManager::getInstance()
{
    if (!instance) {
        instance = std::shared_ptr<DatabaseManager>(new DatabaseManager());
    }
    return instance;
}

bool DatabaseManager::initializeDatabase(const QString& projectName)
{
    if (!createProjectFolder(projectName)) {
        return false;
    }
    
    database = QSqlDatabase::addDatabase("QSQLITE");
    databasePath = projectPath + "\\project.db";
    database.setDatabaseName(databasePath);

    
    if (!database.open()) {
        qDebug() << "Database Error:" << database.lastError().text();
        return false;
    }
    
    return createTables();
}

bool DatabaseManager::createProjectFolder(const QString& projectName)
{
   //QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
   //projectPath = documentsPath + "/ConstructionMgmt/" + projectName;

   //projectPath = "C:\\Users\\RIPL\\Documents\\FusionHubData\\" + projectName;
   projectPath = gEnvironmentPath + "\\" + projectName;
    
    QDir dir;
    if (!dir.mkpath(projectPath)) {
        qDebug() << "Failed to create project folder:" << projectPath;
        return false;
    }       
    return true;
}

QSqlDatabase DatabaseManager::getDatabase() const
{
    return database;
}

bool DatabaseManager::executeQuery(const QString& queryString)
{
    QSqlQuery query(database);
    if (!query.exec(queryString)) {
        qDebug() << "Query Error:" << query.lastError().text();
        qDebug() << "Query:" << queryString;
        return false;
    }
    return true;
}

bool DatabaseManager::beginTransaction()
{
    return database.transaction();
}

bool DatabaseManager::commitTransaction()
{
    return database.commit();
}

bool DatabaseManager::rollbackTransaction()
{
    return database.rollback();
}

QString DatabaseManager::getProjectPath() const
{
    return projectPath;
}

bool DatabaseManager::createTables()
{
    QStringList tableNames = {
        "User", /*"Project",*/ "UnitOfMeasurement", "Vendor", "Material", "BudgetHead",
        "ScheduleSetup", "Task", "TaskImage", "TaskMeasurement", "File",
        "FilePermission", "ScheduleOfRates", "ScheduleOfRatesLine",
        "BillOfQuantity", "BillOfQuantityLine", "ProjectBudget", "WorkOrder",
        "WorkOrderLine", "WorkBilling", "WorkBillingLine", "PurchaseOrder",
        "PurchaseOrderLine", "GoodReceivedNote", "MaterialIndent", "DraftEntity", "BIMElement", "BIMParameter"
    };
    
    for(const QString& tableName: tableNames) {
        QString createQuery = getCreateTableQuery(tableName);
        if (!executeQuery(createQuery)) {
            return false;
        }
    }
    
    return true;
}

QString DatabaseManager::getCreateTableQuery(const QString& tableName)
{
    if (tableName == "User") {
        return R"(
            CREATE TABLE IF NOT EXISTS User (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                user_id TEXT NOT NULL,
                user_fullname TEXT NOT NULL,
                user_name TEXT NOT NULL,
                user_mobile1 TEXT NOT NULL,
                user_mobile2 TEXT NOT NULL,
                user_email1 TEXT NOT NULL,
                user_email2 TEXT NOT NULL,
                user_jobTitle TEXT NOT NULL,
                user_startDate TEXT NOT NULL,
                user_endDate TEXT NOT NULL,
                user_monthlyDeskCostValue TEXT NOT NULL,
                user_password TEXT NOT NULL
            )
        )";
    }
    /*else if (tableName == "Project") {
        return R"(
            CREATE TABLE IF NOT EXISTS Project (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                projectname TEXT NOT NULL,
                customername TEXT NOT NULL,
                contactname TEXT NOT NULL,
                phonenumber TEXT NOT NULL,
                emailid TEXT NOT NULL,
                totaldollarvalue TEXT NOT NULL,
                description TEXT NOT NULL,
                isBlocked BOOLEAN DEFAULT 0
            )
        )";
    }*/

    else if (tableName == "UnitOfMeasurement") {
        return R"(
            CREATE TABLE IF NOT EXISTS UnitOfMeasurement (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                uom_name TEXT NOT NULL,
                unit_type TEXT NOT NULL,
                conversion_to_sqm REAL,
                conversion_to_cubic_meter REAL,
                conversion_to_meter REAL,
                conversion_to_kilogram REAL
            )
        )";
    }
    else if (tableName == "Vendor") {
        return R"(
            CREATE TABLE IF NOT EXISTS Vendor (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                vendor_name TEXT NOT NULL,
                vendor_address TEXT,
                vendor_contact_person TEXT,
                vendor_mobile TEXT,
                vendor_email TEXT
            )
        )";
    }
    else if (tableName == "Material") {
        return R"(
            CREATE TABLE IF NOT EXISTS Material (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                material_name TEXT NOT NULL,
                category TEXT,
                unit_of_measurement_id INTEGER,
                FOREIGN KEY (unit_of_measurement_id) REFERENCES UnitOfMeasurement(id)
            )
        )";
    }
    else if (tableName == "BudgetHead") {
        return R"(
            CREATE TABLE IF NOT EXISTS BudgetHead (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                description TEXT NOT NULL
            )
        )";
    }
    else if (tableName == "ScheduleSetup") {
        return R"(
            CREATE TABLE IF NOT EXISTS ScheduleSetup (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                schedule_name TEXT NOT NULL,
                description TEXT,
                cost_parameter TEXT,
                resource_parameter TEXT
            )
        )";
    }
    else if (tableName == "Task") {
        return R"(
            CREATE TABLE IF NOT EXISTS Task (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                task_name TEXT NOT NULL,
                description TEXT,
                bim_element TEXT,
                start_date TEXT,
                end_date TEXT,
                pid INTEGER
            )
        )";
    }
    else if (tableName == "TaskImage") {
        return R"(
            CREATE TABLE IF NOT EXISTS TaskImage (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                task_id INTEGER NOT NULL,
                image_url TEXT,
                image_local_path TEXT,
                FOREIGN KEY (task_id) REFERENCES Task(id)
            )
        )";
    }
    else if (tableName == "TaskMeasurement") {
        return R"(
            CREATE TABLE IF NOT EXISTS TaskMeasurement (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                task_id INTEGER NOT NULL,
                date TEXT,
                length REAL,
                length_unit_id INTEGER,
                width REAL,
                width_unit_id INTEGER,
                height REAL,
                height_unit_id INTEGER,
                diameter REAL,
                diameter_unit_id INTEGER,
                volume REAL,
                volume_unit_id INTEGER,
                surface_area REAL,
                surface_area_unit_id INTEGER,
                FOREIGN KEY (task_id) REFERENCES Task(id),
                FOREIGN KEY (length_unit_id) REFERENCES UnitOfMeasurement(id),
                FOREIGN KEY (width_unit_id) REFERENCES UnitOfMeasurement(id),
                FOREIGN KEY (height_unit_id) REFERENCES UnitOfMeasurement(id),
                FOREIGN KEY (diameter_unit_id) REFERENCES UnitOfMeasurement(id),
                FOREIGN KEY (volume_unit_id) REFERENCES UnitOfMeasurement(id),
                FOREIGN KEY (surface_area_unit_id) REFERENCES UnitOfMeasurement(id)
            )
        )";
    }
    else if (tableName == "File") {
        return R"(
            CREATE TABLE IF NOT EXISTS File (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                description TEXT,
                file_url TEXT,
                file_local_path TEXT
            )
        )";
    }
    else if (tableName == "FilePermission") {
        return R"(
            CREATE TABLE IF NOT EXISTS FilePermission (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                user_id INTEGER NOT NULL,
                file_id INTEGER NOT NULL,
                permission_type TEXT NOT NULL,
                FOREIGN KEY (user_id) REFERENCES User(id),
                FOREIGN KEY (file_id) REFERENCES File(id)
            )
        )";
    }
    else if (tableName == "ScheduleOfRates") {
        return R"(
            CREATE TABLE IF NOT EXISTS ScheduleOfRates (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                schedule_name TEXT NOT NULL
            )
        )";
    }
    else if (tableName == "ScheduleOfRatesLine") {
        return R"(
            CREATE TABLE IF NOT EXISTS ScheduleOfRatesLine (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                schedule_id INTEGER NOT NULL,
                schedule_type_id INTEGER NOT NULL,
                cost TEXT,
                resource TEXT,
                FOREIGN KEY (schedule_id) REFERENCES ScheduleOfRates(id),
                FOREIGN KEY (schedule_type_id) REFERENCES ScheduleSetup(id)
            )
        )";
    }
    else if (tableName == "BillOfQuantity") {
        return R"(
            CREATE TABLE IF NOT EXISTS BillOfQuantity (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                schedule_id INTEGER NOT NULL,
                description TEXT,
                FOREIGN KEY (schedule_id) REFERENCES ScheduleOfRates(id)
            )
        )";
    }
    else if (tableName == "BillOfQuantityLine") {
        return R"(
            CREATE TABLE IF NOT EXISTS BillOfQuantityLine (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                bill_of_quantity_id INTEGER NOT NULL,
                description TEXT,
                dollar_value REAL,
                task_id INTEGER,
                FOREIGN KEY (bill_of_quantity_id) REFERENCES BillOfQuantity(id),
                FOREIGN KEY (task_id) REFERENCES Task(id)
            )
        )";
    }
    else if (tableName == "ProjectBudget") {
        return R"(
            CREATE TABLE IF NOT EXISTS ProjectBudget (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                budget_head_id INTEGER NOT NULL,
                dollar_value REAL,
                FOREIGN KEY (budget_head_id) REFERENCES BudgetHead(id)
            )
        )";
    }
    else if (tableName == "WorkOrder") {
        return R"(
            CREATE TABLE IF NOT EXISTS WorkOrder (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                vendor_id INTEGER NOT NULL,
                description TEXT,
                FOREIGN KEY (vendor_id) REFERENCES Vendor(id)
            )
        )";
    }
    else if (tableName == "WorkOrderLine") {
        return R"(
            CREATE TABLE IF NOT EXISTS WorkOrderLine (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                work_order_id INTEGER NOT NULL,
                description TEXT,
                dollar_value REAL,
                tax_amount REAL,
                tax_withholding_amount REAL,
                task_id INTEGER,
                retention_amount REAL,
                FOREIGN KEY (work_order_id) REFERENCES WorkOrder(id),
                FOREIGN KEY (task_id) REFERENCES Task(id)
            )
        )";
    }
    else if (tableName == "WorkBilling") {
        return R"(
            CREATE TABLE IF NOT EXISTS WorkBilling (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                work_order_id INTEGER NOT NULL,
                FOREIGN KEY (work_order_id) REFERENCES WorkOrder(id)
            )
        )";
    }
    else if (tableName == "WorkBillingLine") {
        return R"(
            CREATE TABLE IF NOT EXISTS WorkBillingLine (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                work_order_line_id INTEGER NOT NULL,
                dollar_value REAL,
                tax_amount REAL,
                tax_withholding_amount REAL,
                retention_amount REAL,
                FOREIGN KEY (work_order_line_id) REFERENCES WorkOrderLine(id)
            )
        )";
    }
    else if (tableName == "PurchaseOrder") {
        return R"(
            CREATE TABLE IF NOT EXISTS PurchaseOrder (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                vendor_id INTEGER NOT NULL,
                FOREIGN KEY (vendor_id) REFERENCES Vendor(id)
            )
        )";
    }
    else if (tableName == "PurchaseOrderLine") {
        return R"(
            CREATE TABLE IF NOT EXISTS PurchaseOrderLine (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                purchase_order_id INTEGER NOT NULL,
                material_id INTEGER NOT NULL,
                quantity REAL,
                unit_of_measurement_id INTEGER,
                dollar_value REAL,
                tax_amount REAL,
                tax_withholding REAL,
                FOREIGN KEY (purchase_order_id) REFERENCES PurchaseOrder(id),
                FOREIGN KEY (material_id) REFERENCES Material(id),
                FOREIGN KEY (unit_of_measurement_id) REFERENCES UnitOfMeasurement(id)
            )
        )";
    }
    else if (tableName == "GoodReceivedNote") {
        return R"(
            CREATE TABLE IF NOT EXISTS GoodReceivedNote (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                purchase_order_line_id INTEGER NOT NULL,
                amount_of_material_received REAL,
                FOREIGN KEY (purchase_order_line_id) REFERENCES PurchaseOrderLine(id)
            )
        )";
    }
    else if (tableName == "MaterialIndent") {
        return R"(
            CREATE TABLE IF NOT EXISTS MaterialIndent (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                material_id INTEGER NOT NULL,
                total_quantity REAL,
                task_id INTEGER,
                FOREIGN KEY (material_id) REFERENCES Material(id),
                FOREIGN KEY (task_id) REFERENCES Task(id)
            )
        )";
    }
    else if (tableName == "BIMElement") {
        return R"(
            CREATE TABLE IF NOT EXISTS BIMElement (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                type TEXT NOT NULL,
                name TEXT NOT NULL,
                level INTEGER NOT NULL,
                host_id INTEGER NOT NULL
            )
        )";
    }
    else if (tableName == "BIMParameter") {
        return R"(
            CREATE TABLE IF NOT EXISTS BIMParameter (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                global_id TEXT NOT NULL,
                approval_status BOOLEAN DEFAULT 1,
                bim_element_id INTEGER NOT NULL,
                key TEXT NOT NULL,
                value TEXT NOT NULL,
                FOREIGN KEY (bim_element_id) REFERENCES BIMElement(id)
             )
        )";
    }
    else if (tableName == "DraftEntity") {
        return R"(
            CREATE TABLE IF NOT EXISTS DraftEntity (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                tenant INTEGER,
                createdOn DATE,
                project INTEGER NOT NULL,
                entity TEXT NOT NULL,
                createdByUser INTEGER NOT NULL,
                nextApprovingUser INTEGER,
                entitySchema TEXT NOT NULL,
                associatedApprovedEntity INTEGER,
                changeHistory TEXT NOT NULL
            )
        )";
    }
    
    return QString();
}
