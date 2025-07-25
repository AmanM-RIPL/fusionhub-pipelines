import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { Insertable, Selectable } from "kysely";
import { IProject } from "./project.model";
import { UpdateableEntity } from "../../common/types/entity";
import { RequestUser } from "../../../infrastructure/types/fastify.types";
import { ProjectService } from "./project.service";
import { ForbiddenError, NotFoundError } from "../../common/utils/custom-errors";

export default async function projectRoutes(fastify: FastifyInstance) {

  // adding schema
  fastify.addSchema({
    $id: 'project-object',
    type: 'object',
    properties: {
      selectable: {
          type: 'object',
         properties: {
            id: { type: 'integer' },
            name: { type: 'string' },
            createdOn: { type: 'string', format: 'date-time' },
            description: { type: 'string', nullable: true },
            projectCode: { type: 'string' },
            startDate: { type: 'string', format: 'date' },
            endDate: { type: 'string', format: 'date', nullable: true },
            status: { type: 'string' },
            contactEmail: { type: 'string', nullable: true },
            contactMobile: { type: 'string', nullable: true },
            tenant: { type: 'integer', nullable: true },
            isBlocked: { type: 'boolean' }
          },
          required: ['id', 'name', 'createdOn',  'projectCode', 'startDate', 'status', 'isBlocked']
        },

      insertable: {
        type: 'object',
        properties: {
            name: { type: 'string' },
            createdOn: { type: 'string', format: 'date-time' },
            description: { type: 'string', nullable: true },
            projectCode: { type: 'string' },
            startDate: { type: 'string', format: 'date' },
            endDate: { type: 'string', format: 'date', nullable: true },
            status: { type: 'string' },
            contactEmail: { type: 'string', nullable: true },
            contactMobile: { type: 'string', nullable: true },
            tenant: { type: 'integer', nullable: true },
            isBlocked: { type: 'boolean' }
          },
          required: ['name', 'createdOn',  'projectCode', 'startDate', 'status', 'isBlocked']

      },
     updateable: {
        type: 'object',
        properties: {
            name: { type: 'string' },
            createdOn: { type: 'string', format: 'date-time' },
            description: { type: 'string', nullable: true },
            projectCode: { type: 'string' },
            startDate: { type: 'string', format: 'date' },
            endDate: { type: 'string', format: 'date', nullable: true },
            status: { type: 'string' },
            contactEmail: { type: 'string', nullable: true },
            contactMobile: { type: 'string', nullable: true },
            tenant: { type: 'integer', nullable: true },
            isBlocked: { type: 'boolean' }
          }
      },
    }
  });


  // Routes
  fastify.get(
    '/project/:projectId',
    {
      onRequest: fastify.authenticate,
      schema: {
        params: { 
          type: 'object', 
          properties: { 
            projectId: { type: 'integer', minimum: 1 } 
          } 
        },
        response: {
          200: { $ref: 'project-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Params: { projectId: number };
        }>,
        reply: FastifyReply
      ) => {
         const projectDetails = await request.getDecorator<ProjectService>('projectService').findById(request.params.projectId);

        if (!projectDetails) {
          throw new NotFoundError('Project not found');
        }

        return reply.code(200).send(projectDetails);
      }
    }
  );

  fastify.get(
    '/project',
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
            items: { $ref: 'project-object#/properties/selectable' }
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
        const projectDetails = await request.getDecorator<ProjectService>('projectService').findAll(request.query.limit, request.query.offset);
        return reply.code(200).send(projectDetails);
      }
    }
  );

  fastify.post(
    '/project',
    {
      onRequest: fastify.authenticate,
      schema: {
        body: { $ref: 'project-object#/properties/insertable' },
        response: {
          201: { $ref: 'project-object#/properties/selectable' },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: Insertable<IProject>;
        }>,
        reply: FastifyReply
      ) => {
        const newProject: Insertable<IProject> = request.body;

        const projectDetails = await request.getDecorator<ProjectService>('projectService').create(newProject);

        return reply.code(201).send(projectDetails);
      }
    }
  );

  fastify.patch(
    '/project/:projectId',
    {
      onRequest: fastify.authenticate,
      schema: {
        body: { $ref: 'project-object#/properties/updateable' },
        params: { 
          type: 'object', 
          properties: { 
            projectId: { type: 'integer', minimum: 1 } 
          } 
        },
        response: {
          200: { $ref: 'project-object#/properties/selectable' },
          404: { type: 'object', properties: { error: { type: 'string' } } },
          500: { type: 'object', properties: { error: { type: 'string' } } }
        }
      },
      handler: async (
        request: FastifyRequest<{
          Body: Omit<UpdateableEntity<IProject>, "createdOn">;
          Params: { projectId: number };
        }>,
        reply: FastifyReply
      ) => {

        const projectDetails = await request.getDecorator<ProjectService>('projectService').update(request.params.projectId, request.body);

        if (!projectDetails) {
          throw new NotFoundError('Project not found');
        }

        return reply.code(200).send(projectDetails);
      }
    }
  );
}