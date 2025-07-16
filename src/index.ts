import 'dotenv/config';
import Fastify from 'fastify';
import fastifyJwt from '@fastify/jwt';
import jwtAuth from './infrastructure/plugins/jwt-auth';
import kyselyPlugin from './infrastructure/plugins/kysely-plugin';
import defaultJsonSchema from './infrastructure/plugins/default-json-schema';
import defaultRoutes from './application/default/_main/default.routes';
import { PinoLoggerOptions } from 'fastify/types/logger';

// logger configuration
const envToLogger: { [key: string]: boolean | PinoLoggerOptions } = {
  development: {
    transport: {
      target: 'pino-pretty',
      options: {
        translateTime: 'HH:MM:ss Z',
        ignore: 'pid,hostname',
      },
    },
  },
  production: true,
  test: false,
}

// registering the main application
const app = Fastify({
  logger: envToLogger[process.env.NODE_ENV || 'development'],
  // ajv: {
  //   customOptions: {
  //     coerceTypes: false  // Ensure types are not coerced
  //   }
  // }
});

// Register JWT
app.register(fastifyJwt, {
  secret: process.env.JWT_SECRET_KEY as string
});
app.register(jwtAuth);

// Register kysely for connection pooling
app.register(kyselyPlugin);

// Registering the default JSON schema
app.register(defaultJsonSchema);

// registering the routes for action
app.register(defaultRoutes, { prefix: '/api/default' });

// Run the server!
app.listen({ 
  port: process.env.FASTIFY_PORT !== undefined ? parseInt(process.env.FASTIFY_PORT) : 3000, 
  host: process.env.FASTIFY_HOST 
}, (err: any, address: any) => {
  if (err) {
    app.log.error(err)
    process.exit(1)
  }
  app.log.info(`server listening on ${address}`)
})