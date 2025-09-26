import { Selectable } from "kysely";
import { IChangeLog } from "../../default/change-log/change-log.model";

/*

Basic CRUD operations in this repository interface.

The repository is not generic because we will need to do runtime key checking
in changeLog.entitySchema
*/
export interface IProjectEntityBaseRepository {
  create(changeLog: Selectable<IChangeLog>): void;
  update(changeLog: Selectable<IChangeLog>): void;
}

export interface IProjectEntityBaseController {
  validate(changeLog: Selectable<IChangeLog>): void;
}