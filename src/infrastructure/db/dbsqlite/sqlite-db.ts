import Database from "better-sqlite3";
import path from "path";
import fs from "fs";

const DB_FILE = path.join(
  __dirname,"C:/code/fhapi_service/src/infrastructure/sqlite/"
);

fs.mkdirSync(path.dirname(DB_FILE), { recursive: true });

// Open DB file connection
export const sqliteDb = new Database(DB_FILE);

// Enforce foreign keys
sqliteDb.pragma("foreign_keys = ON");

export default sqliteDb;