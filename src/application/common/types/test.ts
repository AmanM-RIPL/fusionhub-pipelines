/*
  As a replacement of using jest-mock-extended's mockDeep which requires lot of memory
  and causes out of memory issues in CI/CD pipelines.
  This file contains custom mock types for Kysely and its query builders.
*/

export type MockKysely = { 
  selectFrom: jest.Mock;
  deleteFrom: jest.Mock;
  insertInto: jest.Mock;
  updateTable: jest.Mock;
};

// Modeled after Kysely's SelectQueryBuilder
export type MockKyselySelectQueryBuilder = {
  selectAll: jest.Mock;
  select: jest.Mock;
  where: jest.Mock;
  limit: jest.Mock;
  offset: jest.Mock;
  executeTakeFirst: jest.Mock;
  executeTakeFirstOrThrow: jest.Mock;
  execute: jest.Mock;
};


// Modeled after Kysely's InsertQueryBuilder
export type MockKyselyInsertQueryBuilder = {
  values: jest.Mock;
  returningAll: jest.Mock;
  executeTakeFirstOrThrow: jest.Mock;
};

// Modeled after Kysely's UpdateQueryBuilder
export type MockKyselyUpdateQueryBuilder = {
  set: jest.Mock;
  where: jest.Mock;
  returningAll: jest.Mock;
  executeTakeFirstOrThrow: jest.Mock;
  executeTakeFirst: jest.Mock;
};

// Modeled after Kysely's DeleteQueryBuilder
export type MockKyselyDeleteQueryBuilder = {
  where: jest.Mock;
  returningAll: jest.Mock;
  executeTakeFirstOrThrow: jest.Mock;
  executeTakeFirst: jest.Mock;
  execute: jest.Mock;
};