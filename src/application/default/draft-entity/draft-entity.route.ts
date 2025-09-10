import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { InsertableEntity, UpdateableEntity, ChangeHistory, ColumnValue, ChangeHistoryPayload } from "../../common/types/entity";
import { NotFoundError } from "../../common/utils/custom-errors";
import { DraftEntityService } from "./draft-entity.service";
import { IDraftEntity } from "./draft-entity.model";
import { timeStamp } from "console";

export default async function draftEntityRoutes(fastify: FastifyInstance) {

  // adding schema
  fastify.addSchema({
    $id: 'draftEntity-object',
    type: 'object',
    properties: {
      selectable: {
        type: 'object',
        properties: {
          id: { type: 'integer' },
          tenant: { type: 'number' },
          createdOn: { type: 'string', format: 'date-time' },
          project: { type: 'number' },
          entity: { type: 'string' },
          createdByUser: { type: 'number' },
          nextApprovingUser: { type: ['number', 'null'] },
          entitySchema: { 
            type: 'object',
            additionalProperties: true
          },
          associatedApprovedEntity: { type: ['number', 'null'] },
          changeHistory: {
            type: 'object',
            properties: {
              user: { type: 'number' },
              changeType: { type: 'string' },
              description: { type: 'string' },
              timestamp: { type: 'string', format: 'date-time' },
              approvalHistory: { type: 'array' }
            },
            required:['user', 'changeType', 'timestamp', 'approvalHistory']
          }
        },
        required: ['id', 'tenant', 'createdOn', 'project', 'entity', 'createdByUser', 'entitySchema', 'changeHistory']
      },
      insertable: {
        type: 'object',
        properties: {
          project: { type: 'number' },
          entity: { type: 'string' },
          entitySchema: { type: 'object' },
          associatedApprovedEntity: { type: ['number', 'null'] },
          changeHistory: {
            type: 'object',
            properties: {
              user: { type: 'number' },
              changeType: { type: 'string' },
              description: { type: 'string' },
              timestamp: { type: 'string', format: 'date-time' },
              approvalHistory: { type: 'array' }
            },
            required:['user', 'changeType', 'timestamp', 'approvalHistory']
          }
        },
        required: ['project', 'entity', 'entitySchema', 'changeHistory', 'associatedApprovedEntity']
      },
      updateable: {
        type: 'object'
      }
    }
  });

  //Routes
  // findById
  fastify.get(
    '/draft-entity/:draftEntityId',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Get draftEntity by id field',
        tags: ['draft-entity'],
        summary: 'Get draftEntity By ID',
        params: {
          type: 'object',
          properties: {
            draftEntityId: { type: 'integer', minimum: 1 }
          }
        },
        response: {
          200: { $ref: 'draftEntity-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Params: { draftEntityId: number };
        }>,
        reply: FastifyReply
      ) => {
        const draftEntity = await request.getDecorator<DraftEntityService>('draftEntityService').findById(request.params.draftEntityId);

        if (!draftEntity) {
          throw new NotFoundError('Draft entity not found');
        }

        return reply.code(200).send(draftEntity);
      }
    }
  );

  //Find All
  fastify.get(
    '/draft-entity',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Get draftEntity all field',
        tags: ['draft-entity'],
        summary: 'Get draftEntity all',
        querystring: {
          type: 'object',
          properties: {
            limit: { type: 'integer', default: 10, minimum: 1, maximum: 20 },
            offset: { type: 'integer', default: 0, minimum: 0 }
          }
        },
        response: {
          200: {
            type: 'array',
            items: { $ref: 'draftEntity-object#/properties/selectable' }
          },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Querystring: { limit: number; offset: number };
        }>,
        reply: FastifyReply
      ) => {
        const draftEntities = await request.getDecorator<DraftEntityService>('draftEntityService').findAll(request.query.limit, request.query.offset);
        return reply.code(200).send(draftEntities);
      }
    }
  );

  // Create
  fastify.post(
    '/draft-entity',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Create draftEntity field',
        tags: ['draft-entity'],
        summary: 'Create draftEntity',
        body: { $ref: 'draftEntity-object#/properties/insertable' },
        response: {
          201: { $ref: 'draftEntity-object#/properties/selectable' },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: Omit<InsertableEntity<IDraftEntity>, 'createdByUser' | 'changeHistory' | 'entitySchema' | 'nextApprovingUser'> & {
            changeHistory: ChangeHistoryPayload;
            entitySchema: any
          };
        }>,
        reply: FastifyReply
      ) => {
        const newDraftEntity = request.body;

        const draftEntity = await request.getDecorator<DraftEntityService>('draftEntityService').create(newDraftEntity);

        return reply.code(201).send(draftEntity);
      }
    }
  );

  //Update
  fastify.patch(
    '/draft-entity/:draftEntityId',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Update draftEntity by id field',
        tags: ['draft-entity'],
        summary: 'Update draftEntity By ID',
        body: { $ref: 'draftEntity-object#/properties/updateable' },
        params: {
          type: 'object',
          properties: {
            draftEntityId: { type: 'integer', minimum: 1 }
          }
        },
        response: {
          200: { $ref: 'draftEntity-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: { [key: string]: unknown };
          Params: { draftEntityId: number };
        }>,
        reply: FastifyReply
      ) => {
        const draftEntity = await request.getDecorator<DraftEntityService>('draftEntityService').update(request.params.draftEntityId, request.body);

        if (!draftEntity) {
          throw new NotFoundError('Draft entity not found');
        }

        return reply.code(200).send(draftEntity);
      }
    }
  );

  fastify.delete(
    '/draft-entity/:draftEntityId',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Delete draft-entity by id field',
        tags: ['draft-entity'],
        summary: 'Delete draft-entity By ID',
        params: {
          type: 'object',
          properties: {
            draftEntityId: { type: 'integer', minimum: 1 }
          }
        },
        response: {
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Params: { draftEntityId: number };
        }>,
        reply: FastifyReply
      ) => {
        await request.getDecorator<DraftEntityService>('draftEntityService').delete(request.params.draftEntityId);

        return reply.code(204).send();
      }
    }
  );
}