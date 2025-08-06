import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { InsertableEntity, UpdateableEntity, ChangeHistory, ColumnValue } from "../../common/types/entity";
import { NotFoundError } from "../../common/utils/custom-errors";
import { DraftEntityService } from "./draft-entity.service";
import { IDraftEntity } from "./draft-entity.model";

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
          entitySchema: { type: 'object' },
          associatedApprovedEntity: { type: ['number', 'null'] },
          parentDraftEntity: { type: ['number', 'null'] },
          changeHistory: { type: 'object' },
          data: { type: 'object' }
        },
        required: ['id', 'tenant', 'createdOn', 'project', 'entity', 'createdByUser', 'entitySchema', 'changeHistory', 'data']
      },
      insertable: {
        type: 'object',
        properties: {
          tenant: { type: 'number' },
          project: { type: 'number' },
          entity: { type: 'string' },
          entitySchema: { type: 'object' },
          associatedApprovedEntity: { type: ['number', 'null'] },
          parentDraftEntity: { type: ['number', 'null'] },
          changeHistory: { type: 'object' },
          data: { type: 'object' }
        },
        required: ['tenant', 'project', 'entity', 'entitySchema', 'changeHistory', 'data']
      },
      updateable: {
        type: 'object',
        properties: {
          nextApprovingUser: { type: ['number', 'null'] },
          entitySchema: { type: 'object' },
          associatedApprovedEntity: { type: ['number', 'null'] },
          parentDraftEntity: { type: ['number', 'null'] },
          changeHistory: { type: 'object' },
          data: { type: 'object' }
        }
      }
    }
  });
  //Routes
  // findById
  fastify.get(
    '/draftEntity/:draftEntityId',
    {
      onRequest: fastify.authenticate,
      schema: {
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
    '/draftEntity',
    {
      onRequest: fastify.authenticate,
      schema: {
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
    '/draftEntity',
    {
      onRequest: fastify.authenticate,
      schema: {
        body: { $ref: 'draftEntity-object#/properties/insertable' },
        response: {
          201: { $ref: 'draftEntity-object#/properties/selectable' },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: Omit<InsertableEntity<IDraftEntity>, 'createdByUser' | 'changeHistory' | 'data' | 'nextApprovingUser'> & { 
            data: ColumnValue;
            changeHistory: ChangeHistory;
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
    '/draftEntity/:draftEntityId',
    {
      onRequest: fastify.authenticate,
      schema: {
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
          Body: Omit<InsertableEntity<IDraftEntity>, 'nextApprovingUser' | 'entitySchema' | 'associatedApprovedEntity' | 'parentDraftEntity'> & {changeHistory?: ChangeHistory; data?: ColumnValue;};
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
// Detele
  fastify.delete(
    '/draftEntity/:draftEntityId',
    {
      onRequest: fastify.authenticate,
      schema: {
        params: {
          type: 'object',
          properties: {
            draftEntityId: { type: 'integer', minimum: 1 }
          }
        },
        response: {
          204: { type: 'null' },
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
        const result = await request.getDecorator<DraftEntityService>('draftEntityService').delete(request.params.draftEntityId);

        return reply.code(204).send();
      }
    }
  );
}