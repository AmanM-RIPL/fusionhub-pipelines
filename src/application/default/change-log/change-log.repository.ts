import { Selectable } from "kysely";
import { IBaseRepository } from "../../common/repositories/base.repository";
import { IChangeLog } from "./change-log.model";

export interface IChangeLogRepository extends IBaseRepository<IChangeLog> {

    findAllWithChangeLogGreaterThan(projectId: number, lastChangeLogId: number, limit:number, offset:number): Promise<Selectable<IChangeLog>[]>;

}