import Database, { Database as DatabaseType } from "better-sqlite3";
import { IChangeLog } from "../../change-log.model";
import { Selectable } from "kysely";
import { IProjectEntityBaseController, IProjectEntityBaseRepository } from "../../../../common/repositories/project-entity.repository";
import { getSqLiteController, getSQLiteDao } from "./index.factory";

export class SQLiteSync {
  constructor(protected readonly db: DatabaseType) {}

  public async syncSqlite(changeLogs: Selectable<IChangeLog>[]) {
    
    const sqliteTransactions = this.db.transaction(() => {
      for (const changeLog of changeLogs) {
        const dao: IProjectEntityBaseRepository | undefined = getSQLiteDao(changeLog.entity, this.db);
        if (!dao) {
          // entity is not in sqlite, skip
          continue;
        }

        // validate the entitySchema
        const controller: IProjectEntityBaseController | undefined = getSqLiteController(changeLog.entity);
        if (!controller) {
          throw Error(`Dao exists but Controller does not exist for ${changeLog.entity}`);
        }

        controller.validate(changeLog);

        if (changeLog.associatedApprovedEntity === null) {
          dao.create(changeLog);
        } else {
          dao.update(changeLog);
        }
      }
    });

    try {
      sqliteTransactions();
    } catch (error) {
      console.error("Error during SQLite sync transaction:", error);
      throw error;
    }
  }
}

