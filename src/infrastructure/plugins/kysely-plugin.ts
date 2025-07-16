import fp from 'fastify-plugin';
import { Kysely, PostgresDialect, sql } from 'kysely';
import { IDatabase } from '../db/kysely/types';
import pg from 'pg';

export default fp(async (fastify) => {
  // updating node-postgres type for NUMERIC(10,2)
  pg.types.setTypeParser(pg.types.builtins.NUMERIC, (value: any) => parseFloat(value));
  pg.types.setTypeParser(pg.types.builtins.INT8, (value: any) => parseInt(value)); // COUNT is bigint in postgres so we need to map to integer.

  let db: Kysely<IDatabase>;
  if (process.env.NODE_ENV === 'development' || process.env.NODE_ENV === 'test') {
    db = new Kysely<IDatabase>({
      dialect: new PostgresDialect({
        pool: new pg.Pool({
          connectionString:`postgres://${process.env.POSTGRESQL_USER}:${process.env.POSTGRESQL_PASSWORD}@${process.env.POSTGRESQL_HOST}:${process.env.POSTGRESQL_PORT}/${process.env.POSTGRESQL_DATABASE}`,
          max: 10,
        }),
      }),
    });


  } else {
    db = new Kysely<IDatabase>({
      dialect: new PostgresDialect({
        pool: new pg.Pool({
          max: 10,
          host: process.env.POSTGRESQL_HOST,
          user: process.env.POSTGRESQL_USER,
          password: process.env.POSTGRESQL_PASSWORD,
          database: process.env.POSTGRESQL_DATABASE,
        }),
      }),
    });
  }

  fastify.decorate('db', db);
  fastify.addHook('onClose', () => db.destroy());
});