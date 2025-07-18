import { FastifyInstance, FastifyReply, FastifyRequest } from "fastify";
import { ControlledTransaction } from "kysely";
import { IDatabase } from "../../infrastructure/db/kysely/types";
import { CustomError, ForbiddenError, NotFoundError, ValidationError } from "../../application/common/utils/custom-errors";

export default async function dbTransactionPlugin(fastify: FastifyInstance) {

  // Registering the database connection
  fastify.decorateRequest('dbTransaction', null);

  // Registering services and dao
  fastify.addHook('preHandler', async (request: FastifyRequest, reply: FastifyReply) => {
    // starting db transaction
    const trx = await fastify.db.startTransaction().execute();
    request.setDecorator<ControlledTransaction<IDatabase>>('dbTransaction', trx);
  });

  // commit transaction before sending response
  fastify.addHook('onSend', async (request: FastifyRequest, reply: FastifyReply) => {
    const trx = request.getDecorator<ControlledTransaction<IDatabase>>('dbTransaction');
    if (trx && reply.statusCode < 400) {
      try {
        await trx.commit().execute();
      } catch (error) {
        console.log(error);
      }
    }
  });

  // Rollback transaction on error
  fastify.setErrorHandler(async (error: Error, request: FastifyRequest, reply: FastifyReply) => {
    const trx = request.getDecorator<ControlledTransaction<IDatabase>>('dbTransaction');
    if (trx) {
      try {
        await trx.rollback().execute();
      } catch (rollbackError) {
        console.error('Rollback failed:', rollbackError);
      }
    }

    let statusCode = 500;
    let errorMessage = 'Internal Server Error';

    if (error instanceof ForbiddenError) {
      statusCode = error.statusCode;
      errorMessage = error.message;
    } else if (error instanceof NotFoundError) {
      statusCode = error.statusCode;
      errorMessage = error.message;
    } else if (error instanceof ValidationError) {
      statusCode = error.statusCode;
      errorMessage = `${error.message}: ${error.details.join(', ')}`;
    } else if (error instanceof CustomError) {
      statusCode = error.statusCode;
      errorMessage = error.message;
    } else {
      errorMessage = error.message;
    }

    reply.code(statusCode).send({ error: errorMessage });
  });
}