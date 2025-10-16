import { Selectable } from "kysely";
import { IBaseRepository } from "../../common/repositories/base.repository";
import { IChangeLog } from "./change-log.model";
import { ChangeHistory } from "../../common/types/entity";

export interface IChangeLogRepository extends IBaseRepository<IChangeLog> {
    findAllWithChangeLogGreaterThan(projectId: number, lastChangeLogId: number, limit:number): Promise<Selectable<IChangeLog>[]>;
}