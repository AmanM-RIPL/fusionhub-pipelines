import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { IProjectBudget } from "./project-budget.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";
import { ChangeHistory } from "../../../../../common/types/entity";

export class ProjectBudgetController implements IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void {

    if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
      const newEntitySchema: Selectable<IProjectBudget> = {
        id: 0,
        global_id: 0,
        approval_status: true,
        budget_head_id: 0,
        dollar_value: 0,
        change_history: {
          user: 0,
          changeType: "create",
          description: "",
          timestamp: new Date(),
          approvalHistory: []
        }
      };

      changeLog.entitySchema = newEntitySchema;
    }

    const entitySchema = changeLog.entitySchema as Selectable<IProjectBudget>;

    if (typeof entitySchema.global_id !== "number") {
      entitySchema.global_id = 0;
    }

    if (typeof entitySchema.approval_status !== "boolean") {
      entitySchema.approval_status = true;
    }

    if (typeof entitySchema.budget_head_id !== "number") {
      entitySchema.budget_head_id = 0;
    }

    if (typeof entitySchema.dollar_value !== "number") {
      entitySchema.dollar_value = 0;
    }

    if (typeof entitySchema.change_history !== "object" || entitySchema.change_history === null) {
        
      entitySchema.change_history = {
        user: 0,
        changeType: "create",
        description: "",
        timestamp: new Date(),
        approvalHistory: []
      } as ChangeHistory;
    }

    changeLog.entitySchema = entitySchema;
  }
}
