import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { InsertableEntity, UpdateableEntity, ChangeHistory, ColumnValue, ChangeHistoryPayload } from "../../common/types/entity";
import { NotFoundError } from "../../common/utils/custom-errors";
import { ChangeLogService } from "./change-log.service";
import { IChangeLog } from "./change-log.model";
import { timeStamp } from "console";

export default async function changeLogRoutes(fastify: FastifyInstance) {

  // adding schema
  fastify.addSchema({
    $id: 'changeLog-object',
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
    '/change-log/:changeLogId',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Get changeLog by id field',
        tags: ['change-log'],
        summary: 'Get changeLog By ID',
        params: {
          type: 'object',
          properties: {
            changeLogId: { type: 'integer', minimum: 1 }
          }
        },
        response: {
          200: { $ref: 'changeLog-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Params: { changeLogId: number };
        }>,
        reply: FastifyReply
      ) => {
        const changeLog = await request.getDecorator<ChangeLogService>('changeLogService').findById(request.params.changeLogId);

        if (!changeLog) {
          throw new NotFoundError('Change log not found.');
        }

        return reply.code(200).send(changeLog);
      }
    }
  );

  //Find All
  fastify.get(
    '/change-log',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Get changeLog all field',
        tags: ['change-log'],
        summary: 'Get changeLog all',
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
            items: { $ref: 'changeLog-object#/properties/selectable' }
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
        const changeLogs = await request.getDecorator<ChangeLogService>('changeLogService').findAll(request.query.limit, request.query.offset);
        return reply.code(200).send(changeLogs);
      }
    }
  );

  // Create
  fastify.post(
    '/change-log',
    {
      onRequest: fastify.authenticate,
      schema: {
        description: 'Create changeLog field',
        tags: ['change-log'],
        summary: 'Create changeLog',
        body: { $ref: 'changeLog-object#/properties/insertable' },
        response: {
          201: { $ref: 'changeLog-object#/properties/selectable' },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: Omit<InsertableEntity<IChangeLog>, 'createdByUser' | 'changeHistory' | 'entitySchema'> & {
            changeHistory: ChangeHistoryPayload;
            entitySchema: any
          };
        }>,
        reply: FastifyReply
      ) => {
        const newChangeLog = request.body;

        const changeLog = await request.getDecorator<ChangeLogService>('changeLogService').create(newChangeLog);

        return reply.code(201).send(changeLog);
      }
    }
  );

}