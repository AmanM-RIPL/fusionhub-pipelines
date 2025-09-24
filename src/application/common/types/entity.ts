import { Insertable, Updateable } from "kysely";
import { IProjectEntityBaseRepository } from "../repositories/project-entity.repository";

export type EntityType = "draft" | "approved" | "updated-draft";

// To prevent updating/inserting id or tenant by mistake
export type UpdateableEntity<T> = Omit<Updateable<T>, "id" | "tenant">;
export type InsertableEntity<T> = Omit<Insertable<T>, "id" | "tenant">;

export interface IColumnSchema {
  name: string;
  type: "integer" | "float" | "string" | "boolean" | "date";
  relatedEntity?: number; // id of another entity in entity schema (fk is always to the id column)
  isRequired?: boolean;
};

export type ColumnValue = Record<string, number | string | boolean | Date | null>;
export type FilterPermission = { 
  project?: number[];
  subProject?: number[];
  businessEntity?: number[];
  businessEntityBranch?: number[];
  financialYear?: number[];
};
export type AccessPermission = { create: boolean; read: boolean | "self"; update: boolean };

export type ApprovalHistory = {
  user: number;
  timestamp: Date;
  description: string;
  status: "approved" | "rejected";
};

export type ChangeHistory = {
  user: number;
  changeType: "create" | "update";
  description: string;
  timestamp: Date;
  approvalHistory: ApprovalHistory[];
};

// for ApprovalHistory sent back in draft-entity endpoints
export type ApprovalHistoryPayload = {
  user: number;
  timestamp: string;
  description: string;
  status: "approved" | "rejected";
};

// for ChangeHistory sent back in draft-entity endpoints
export type ChangeHistoryPayload = {
  user: number;
  changeType: "create" | "update";
  description: string;
  timestamp: string;
  approvalHistory: ApprovalHistoryPayload[];
};