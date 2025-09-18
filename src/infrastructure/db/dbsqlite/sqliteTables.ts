// Array of CREATE TABLE statements
export const sqliteTable: string[] = [
    // Unit of Measurement
    `CREATE TABLE UnitOfMeasurement (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    uom_name TEXT NOT NULL,
                    unit_type TEXT NOT NULL,
                    conversion_to_sqm REAL,
                    conversion_to_cubic_meter REAL,
                    conversion_to_meter REAL,
                    conversion_to_kilogram REAL,
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    // Vendor Management
    `CREATE TABLE Vendor (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    vendor_name TEXT NOT NULL,
                    vendor_address TEXT,
                    vendor_contact_person TEXT,
                    vendor_mobile TEXT,
                    vendor_email TEXT,
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    // Material Management
    `CREATE TABLE Material (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    material_name TEXT NOT NULL,
                    category TEXT,
                    unit_of_measurement_id INTEGER,
                    FOREIGN KEY (unit_of_measurement_id) REFERENCES UnitOfMeasurement(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    // Budget Management
    `CREATE TABLE BudgetHead (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    description TEXT NOT NULL,
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    `CREATE TABLE ProjectBudget (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    budget_head_id INTEGER NOT NULL,
                    dollar_value REAL,
                    FOREIGN KEY (budget_head_id) REFERENCES BudgetHead(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    // Schedule Setup
    `CREATE TABLE ScheduleSetup (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    schedule_name TEXT NOT NULL,
                    description TEXT,
                    cost_parameter TEXT,
                    resource_parameter TEXT,
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    `CREATE TABLE ScheduleOfRates (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    schedule_name TEXT NOT NULL,
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    `CREATE TABLE ScheduleOfRatesLine (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    schedule_id INTEGER NOT NULL,
                    schedule_type_id INTEGER NOT NULL,
                    cost TEXT,
                    resource TEXT,
                    FOREIGN KEY (schedule_id) REFERENCES ScheduleOfRates(id),
                    FOREIGN KEY (schedule_type_id) REFERENCES ScheduleSetup(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    // Task Management
    `CREATE TABLE Task (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    task_name TEXT NOT NULL,
                    description TEXT,
                    bim_element TEXT,
                    start_date TEXT,
                    end_date TEXT,
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    `CREATE TABLE TaskImage (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    task_id INTEGER NOT NULL,
                    image_url TEXT,
                    image_local_path TEXT,
                    FOREIGN KEY (task_id) REFERENCES Task(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    `CREATE TABLE TaskMeasurement (
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
                    FOREIGN KEY (surface_area_unit_id) REFERENCES UnitOfMeasurement(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    // Bill of Quantity
    `CREATE TABLE BillOfQuantity (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    schedule_id INTEGER NOT NULL,
                    description TEXT,
                    FOREIGN KEY (schedule_id) REFERENCES ScheduleOfRates(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    `CREATE TABLE BillOfQuantityLine (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    bill_of_quantity_id INTEGER NOT NULL,
                    description TEXT,
                    dollar_value REAL,
                    task_id INTEGER,
                    FOREIGN KEY (bill_of_quantity_id) REFERENCES BillOfQuantity(id),
                    FOREIGN KEY (task_id) REFERENCES Task(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    // File Management
    `CREATE TABLE File (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    description TEXT,
                    file_url TEXT,
                    file_local_path TEXT,
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    `CREATE TABLE FilePermission (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    user_id INTEGER NOT NULL,
                    file_id INTEGER NOT NULL,
                    permission_type TEXT NOT NULL,
                    FOREIGN KEY (user_id) REFERENCES User(id),
                    FOREIGN KEY (file_id) REFERENCES File(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    // Purchase Management
    `CREATE TABLE PurchaseOrder (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    vendor_id INTEGER NOT NULL,
                    FOREIGN KEY (vendor_id) REFERENCES Vendor(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    `CREATE TABLE PurchaseOrderLine (
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
                    FOREIGN KEY (unit_of_measurement_id) REFERENCES UnitOfMeasurement(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    `CREATE TABLE GoodReceivedNote (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    purchase_order_line_id INTEGER NOT NULL,
                    amount_of_material_received REAL,
                    FOREIGN KEY (purchase_order_line_id) REFERENCES PurchaseOrderLine(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    // Material Indent
    `CREATE TABLE MaterialIndent (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    material_id INTEGER NOT NULL,
                    total_quantity REAL,
                    task_id INTEGER,
                    FOREIGN KEY (material_id) REFERENCES Material(id),
                    FOREIGN KEY (task_id) REFERENCES Task(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    // Work Order Management
    `CREATE TABLE WorkOrder (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    vendor_id INTEGER NOT NULL,
                    description TEXT,
                    FOREIGN KEY (vendor_id) REFERENCES Vendor(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    `CREATE TABLE WorkOrderLine (
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
                    FOREIGN KEY (task_id) REFERENCES Task(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    // Billing Management
    `CREATE TABLE WorkBilling (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    work_order_id INTEGER NOT NULL,
                    FOREIGN KEY (work_order_id) REFERENCES WorkOrder(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`,

    `CREATE TABLE WorkBillingLine (
                    id INTEGER PRIMARY KEY AUTOINCREMENT,
                    global_id TEXT NOT NULL,
                    approval_status BOOLEAN DEFAULT 1,
                    work_order_line_id INTEGER NOT NULL,
                    dollar_value REAL,
                    tax_amount REAL,
                    tax_withholding_amount REAL,
                    retention_amount REAL,
                    FOREIGN KEY (work_order_line_id) REFERENCES WorkOrderLine(id),
                    change_history TEXT CHECK (change_history IS NULL OR json_valid(change_history))
                );`
];
