import fp from 'fastify-plugin';

export default fp(async (fastify) => {

  // Schema for ChangeHistory
  fastify.addSchema({
    $id: 'change-history-schema',
    type: 'array',
    items: {
      type: 'object',
      properties: {
        user: { type: 'integer' },
        changeType: { type: 'string', enum: ['create', 'update'] },
        description: { type: 'string' },
        timestamp: { type: 'string', format: 'date-time' },
        approvalHistory: {
          type: 'array',
          items: {
            type: 'object',
            properties: {
              user: { type: 'integer' },
              timestamp: { type: 'string', format: 'date-time' },
              description: { type: 'string' },
              status: { type: 'string', enum: ['approved', 'rejected'] },
            },
            required: ['user', 'timestamp', 'description', 'status'],
          },
        },
      }
    }
  });
});