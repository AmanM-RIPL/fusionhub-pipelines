import swagger from '@fastify/swagger';
import swaggerUi from '@fastify/swagger-ui';
import { FastifyInstance } from 'fastify';

export async function tenantSwagger(fastify: FastifyInstance) {
  await fastify.register(swagger, {
    openapi: {
      info: {
        title: 'Tenant API',
        description: 'Tenant module APIs',
        version: '1.0.0'
      }
    }
  });

  await fastify.register(swaggerUi, {
    routePrefix: '/docs/tenant',
    uiConfig: {
      docExpansion: 'full',
      deepLinking: true
    }
  });
}
