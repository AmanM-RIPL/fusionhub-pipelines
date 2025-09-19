import { Generated, JSONColumnType } from "kysely";
import { ChangeHistory } from "../../../../common/types/entity";

// ---- Base interfaces ----
export interface IBaseEntity {
  id: Generated<number>;
  global_id: string;
  approval_status: boolean;
  change_history: JSONColumnType<ChangeHistory | null>;
}

// ---- Unit of Measurement ----
export interface IUnitOfMeasurement extends IBaseEntity {
  uom_name: string;
  unit_type: string;
  conversion_to_sqm: number | null;
  conversion_to_cubic_meter: number | null;
  conversion_to_meter: number | null;
  conversion_to_kilogram: number | null;
}

// ---- Vendor ----
export interface IVendor extends IBaseEntity {
  vendor_name: string;
  vendor_address: string | null;
  vendor_contact_person: string | null;
  vendor_mobile: string | null;
  vendor_email: string | null;
}

// ---- Material ----
export interface IMaterial extends IBaseEntity {
  material_name: string;
  category: string | null;
  unit_of_measurement_id: number | null;
}

// ---- Budget ----
export interface IBudgetHead extends IBaseEntity {
  description: string;
}

export interface IProjectBudget extends IBaseEntity {
  budget_head_id: number;
  dollar_value: number | null;
}

// ---- Schedule ----
export interface IScheduleSetup extends IBaseEntity {
  schedule_name: string;
  description: string | null;
  cost_parameter: string | null;
  resource_parameter: string | null;
}

export interface IScheduleOfRates extends IBaseEntity {
  schedule_name: string;
}

export interface IScheduleOfRatesLine extends IBaseEntity {
  schedule_id: number;
  schedule_type_id: number;
  cost: string | null;
  resource: string | null;
}

// ---- Task Management ----
export interface ITask extends IBaseEntity {
  task_name: string;
  description: string | null;
  bim_element: string | null;
  start_date: string | null;
  end_date: string | null;
}

export interface ITaskImage extends IBaseEntity {
  task_id: number;
  image_url: string | null;
  image_local_path: string | null;
}

export interface ITaskMeasurement extends IBaseEntity {
  task_id: number;
  date: string | null;
  length: number | null;
  length_unit_id: number | null;
  width: number | null;
  width_unit_id: number | null;
  height: number | null;
  height_unit_id: number | null;
  diameter: number | null;
  diameter_unit_id: number | null;
  volume: number | null;
  volume_unit_id: number | null;
  surface_area: number | null;
  surface_area_unit_id: number | null;
}

// ---- Bill of Quantity ----
export interface IBillOfQuantity extends IBaseEntity {
  schedule_id: number;
  description: string | null;
}

export interface IBillOfQuantityLine extends IBaseEntity {
  bill_of_quantity_id: number;
  description: string | null;
  dollar_value: number | null;
  task_id: number | null;
}

// ---- File Management ----
export interface IFile extends IBaseEntity {
  description: string | null;
  file_url: string | null;
  file_local_path: string | null;
}

export interface IFilePermission extends IBaseEntity {
  user_id: number;
  file_id: number;
  permission_type: string;
}

// ---- Purchase Management ----
export interface IPurchaseOrder extends IBaseEntity {
  vendor_id: number;
}

export interface IPurchaseOrderLine extends IBaseEntity {
  purchase_order_id: number;
  material_id: number;
  quantity: number | null;
  unit_of_measurement_id: number | null;
  dollar_value: number | null;
  tax_amount: number | null;
  tax_withholding: number | null;
}

export interface IGoodReceivedNote extends IBaseEntity {
  purchase_order_line_id: number;
  amount_of_material_received: number | null;
}

// ---- Material Indent ----
export interface IMaterialIndent extends IBaseEntity {
  material_id: number;
  total_quantity: number | null;
  task_id: number | null;
}

// ---- Work Order ----
export interface IWorkOrder extends IBaseEntity {
  vendor_id: number;
  description: string | null;
}

export interface IWorkOrderLine extends IBaseEntity {
  work_order_id: number;
  description: string | null;
  dollar_value: number | null;
  tax_amount: number | null;
  tax_withholding_amount: number | null;
  task_id: number | null;
  retention_amount: number | null;
}

// ---- Billing ----
export interface IWorkBilling extends IBaseEntity {
  work_order_id: number;
}

export interface IWorkBillingLine extends IBaseEntity {
  work_order_line_id: number;
  dollar_value: number | null;
  tax_amount: number | null;
  tax_withholding_amount: number | null;
  retention_amount: number | null;
}