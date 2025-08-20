import Fastify, { FastifyInstance } from "fastify";
import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { UserService } from "../user.service";
import { RequestUser } from "../../../../infrastructure/types/fastify.types";
import { CustomError } from "../../../common/utils/custom-errors";
import userRoutes from "../user.route";
import { IUser } from "../user.model";
import { Insertable, Selectable } from "kysely";
import { JWT } from "@fastify/jwt";
import { UpdateableEntity } from "../../../common/types/entity";

describe('Default -> User -> UserRoute', () => {
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

    await userRoutes(fastify as FastifyInstance);
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

  test('should handle get user by id', async () => {
    const mockUser = { 
      id: 1, 
      username: 'testuser',
      email: 'sample@gmail.com',
      createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
      firstName: 'Test',
      lastName: 'User',
      mobile: '1234567890'
    } as Selectable<IUser>;

    userService.findById.mockResolvedValue(mockUser);

    const response = await fastify.inject({
      method: 'GET',
      url: '/user/1',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual({
      ...mockUser, 
      createdOn: mockUser.createdOn.toISOString()
    });
    expect(userService.findById).toHaveBeenCalledWith(1);
  });

  test('should handle get user by id not found error', async () => {
    // Mock the userService to return undefined for a non-existent user
    userService.findById.mockResolvedValue(undefined);

    const response = await fastify.inject({
      method: 'GET',
      url: '/user/999',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(404);
    expect(JSON.parse(response.body)).toEqual({ error: 'User not found' });
  });

  test('should handle get all users', async () => {
    const mockUser: Selectable<IUser>[] = [
      { 
        id: 1, 
        username: 'testuser',
        password: 'sample',
        tenant: undefined,
        email: 'sample@gmail.com',
        createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
        firstName: 'Test',
        lastName: 'User',
        mobile: '1234567890'
      }
    ];

    const mockUserResponse: Omit<Selectable<IUser>, 'password' | 'tenant'>[] = [
      { 
        id: 1, 
        username: 'testuser',
        email: 'sample@gmail.com',
        createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
        firstName: 'Test',
        lastName: 'User',
        mobile: '1234567890'
      }
    ];

    userService.findAll.mockResolvedValue(mockUser);

    const response = await fastify.inject({
      method: 'GET',
      url: '/user',
      headers: { authorization: 'Bearer test-token' }
    });

    // Check the response
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual(
      mockUserResponse.map(user => ({ 
        ...user, 
        createdOn: user.createdOn.toISOString() 
      }))
    );
    expect(userService.findAll).toHaveBeenCalled();
  });

  test('should handle create users', async () => {
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

    const mockUserRequest: Insertable<IUser> = {
      username: 'testuser',
      password: 'sample',
      email: 'sample@gmail.com',
      firstName: 'Test',
      lastName: 'User',
      mobile: '1234567890'
    };

    const mockUserResponse: Omit<Selectable<IUser>, 'password' | 'tenant'> = { 
      id: 1, 
      username: 'testuser',
      email: 'sample@gmail.com',
      createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
      firstName: 'Test',
      lastName: 'User',
      mobile: '1234567890'
    };

    userService.create.mockResolvedValue(mockUser);

    const response = await fastify.inject({
      method: 'POST',
      url: '/user',
      headers: { authorization: 'Bearer test-token' },
      payload: mockUserRequest
    });

    // Check the response - user creation should return 201 (Created)
    expect(response.statusCode).toBe(201);
    expect(JSON.parse(response.body)).toEqual({ 
      ...mockUserResponse, 
      createdOn: mockUserResponse.createdOn.toISOString() 
    });
    expect(userService.create).toHaveBeenCalled();
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
      url: '/user/login',
      headers: { authorization: 'Bearer test-token' },
      payload: mockUserRequest
    });

    // Check the response - login typically returns 200 (OK), not 201
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
      url: '/auth/login',
      headers: { authorization: 'Bearer test-token' },
      payload: mockUserRequest
    });

    // Check the response - could be 401 (Unauthorized) instead of 404
    expect(response.statusCode).toBe(401);
    expect(JSON.parse(response.body)).toEqual({ error: 'Invalid credentials' });
  });

  test('should handle user update', async () => {
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

    const mockUserRequest: UpdateableEntity<IUser> = {
      username: 'testuser',
      password: 'sample'
    };

    const mockUserResponse: Omit<Selectable<IUser>, 'password' | 'tenant'> = { 
      id: 1, 
      username: 'testuser',
      email: 'sample@gmail.com',
      createdOn: new Date(Date.parse('2025-04-11T10:00:00Z')),
      firstName: 'Test',
      lastName: 'User',
      mobile: '1234567890'
    };

    userService.update.mockResolvedValue(mockUser);

    const response = await fastify.inject({
      method: 'PATCH',
      url: '/user/1',
      headers: { authorization: 'Bearer test-token' },
      payload: mockUserRequest
    });

    // Check the response
    expect(response.statusCode).toBe(200);
    expect(JSON.parse(response.body)).toEqual({ 
      ...mockUserResponse, 
      createdOn: mockUserResponse.createdOn.toISOString() 
    });
    expect(userService.update).toHaveBeenCalledWith(1, mockUserRequest);
  });
});