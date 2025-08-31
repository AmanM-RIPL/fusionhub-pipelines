import Fastify, { FastifyInstance } from "fastify";
import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { Insertable, Selectable } from "kysely";
import { JWT } from "@fastify/jwt";
import { UserService } from "../../default/user/user.service";
import { CustomError } from "../../common/utils/custom-errors";
import { RequestUser } from "../../../infrastructure/types/fastify.types";
import { userAuthRoutes } from "../_main/auth.route";
import { IUser } from "../../default/user/user.model";

describe('Default -> Auth -> AuthRoute', () => {
  let fastify: FastifyInstance;
  let userService: DeepMockProxy<UserService>;

  beforeAll(async () => {
    fastify = Fastify() as FastifyInstance;
    userService = mockDeep<UserService>();
    const mockJwt = mockDeep<JWT>();

    // Mock the userService methods
    fastify.jwt = mockJwt;
    fastify.decorateRequest('userService', null);
    fastify.addHook('onRequest', async (request, reply) => {
      request.user = { user: 1, tenant: 1 } as RequestUser;
      // Use direct assignment instead of setDecorator
      (request as any).userService = userService;
    });
    fastify.setErrorHandler(async (error: Error, request, reply) => {
      let statusCode = 500;
      let errorMessage = 'Internal Server Error';

      if (error instanceof CustomError) {
        statusCode = error.statusCode;
        errorMessage = error.message;
      }

      reply.code(statusCode).send({ error: errorMessage });
    });

    await userAuthRoutes(fastify as FastifyInstance);
    await fastify.ready();
  });

  afterAll(async () => {
    await fastify.close();
    jest.clearAllMocks();
  });

  beforeEach(() => {
    // Reset all mocks before each test
    jest.clearAllMocks();
  });

  test('should handle user login', async () => {
    const mockUser: Selectable<IUser> = {
      id: 1,
      username: 'testuser',
      password: 'sample',
      tenant: undefined,
      email: 'sample@gmail.com',
      createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
      firstName: 'Test',
      lastName: 'User',
      mobile: '1234567890'
    };

    const mockUserRequest: { username: string, password: string } = {
      username: 'testuser',
      password: 'sample'
    };

    const mockUserResponse: Omit<Selectable<IUser>, 'password' | 'tenant'> & { token: string } = { 
      id: 1, 
      username: 'testuser',
      email: 'sample@gmail.com',
      createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
      firstName: 'Test',
      lastName: 'User',
      mobile: '1234567890',
      token: 'test-token'
    };

    userService.authenticate.mockResolvedValue(mockUser);
    fastify.jwt.sign = jest.fn().mockReturnValue('test-token');

    const response = await fastify.inject({
      method: 'POST',
      url: '/login',
      payload: mockUserRequest
    });

    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual({ 
      ...mockUserResponse, 
      createdOn: mockUserResponse.createdOn.toISOString() 
    });
    // Fix: should call authenticate, not create
    expect(userService.authenticate).toHaveBeenCalledWith(mockUserRequest.username, mockUserRequest.password);
  });

  test('should handle incorrect user login', async () => {
    const mockUserRequest: { username: string, password: string } = {
      username: 'testuser',
      password: 'wrongpassword'
    };

    userService.authenticate.mockResolvedValue(undefined);
    fastify.jwt.sign = jest.fn().mockReturnValue('test-token');

    const response = await fastify.inject({
      method: 'POST',
      url: '/login',
      payload: mockUserRequest
    });

    expect(response.statusCode).toBe(401);
    expect(JSON.parse(response.body)).toEqual({ error: 'Invalid username or password' });
  });
});