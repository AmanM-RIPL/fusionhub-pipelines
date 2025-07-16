import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { UserService } from "../user.service";
import { IUserRepository } from "../user.repository";
import { Selectable } from "kysely";
import { IUser } from "../user.model";

describe('Default -> User -> UserService', () => {
  let userService: UserService;
  let mockUserRepository: DeepMockProxy<IUserRepository>;

  beforeEach(() => {
    mockUserRepository = mockDeep<IUserRepository>();
    userService = new UserService(mockUserRepository);
  });

  afterEach(() => {
    jest.clearAllMocks();
  });

  test('create should create a new user if username does not exist', async () => {
    const newUser = {
      username: 'newuser',
      password: 'password',
      email: 'newuser@example.com',
      firstName: 'New',
      lastName: 'User',
      mobile: '1234567890'
    };

    const newCreatedUser: Selectable<IUser> = {
      id: 1,
      username: 'newuser',
      password: 'password',
      tenant: 1,
      email: 'newuser@example.com',
      createdOn: new Date(),
      firstName: 'New',
      lastName: 'User',
      mobile: '1234567890'
    };

    mockUserRepository.findByUsername.mockResolvedValue(undefined);
    mockUserRepository.create.mockResolvedValue(newCreatedUser);

    const result = await userService.create(newUser);

    expect(result).toEqual(newCreatedUser);
    expect(mockUserRepository.findByUsername).toHaveBeenCalledWith('newuser');
    expect(mockUserRepository.create).toHaveBeenCalledWith(newUser);
  });

  test('create should not create a new user if username already exists', async () => {
    const newUser = {
      username: 'newuser',
      password: 'password',
      email: 'newuser@example.com',
      firstName: 'New',
      lastName: 'User',
      mobile: '1234567890'
    };

    const existingUser: Selectable<IUser> = {
      id: 1,
      username: 'newuser',
      password: 'password',
      tenant: 1,
      email: 'newuser@example.com',
      createdOn: new Date(),
      firstName: 'New',
      lastName: 'User',
      mobile: '1234567890'
    };

    mockUserRepository.findByUsername.mockResolvedValue(existingUser);

    async function result() { 
      await userService.create(newUser);
    }

    await expect(result).rejects.toThrow('User already exists');
  });

  test('update should update user if username does not clash', async () => {
    const newUser = {
      username: 'newuser',
      password: 'password',
      email: 'newuser@example.com',
      firstName: 'New',
      lastName: 'User',
      mobile: '1234567890'
    };

    const newUpdatedUser: Selectable<IUser> = {
      id: 1,
      username: 'newuser',
      password: 'password',
      tenant: 1,
      email: 'newuser@example.com',
      createdOn: new Date(),
      firstName: 'New',
      lastName: 'User',
      mobile: '1234567890'
    };

    mockUserRepository.findByUsername.mockResolvedValue(undefined);
    mockUserRepository.update.mockResolvedValue(newUpdatedUser);

    const result = await userService.update(1, newUser);

    expect(result).toEqual(newUpdatedUser);
    expect(mockUserRepository.findByUsername).toHaveBeenCalledWith('newuser');
    expect(mockUserRepository.update).toHaveBeenCalledWith(1, newUser);
  });

  test('update should not update user if username already exists', async () => {
    const newUser = {
      username: 'newuser',
      password: 'password',
      email: 'newuser@example.com',
      firstName: 'New',
      lastName: 'User',
      mobile: '1234567890'
    };

    const newUpdatedUser: Selectable<IUser> = {
      id: 1,
      username: 'newuser',
      password: 'password',
      tenant: 1,
      email: 'newuser@example.com',
      createdOn: new Date(),
      firstName: 'New',
      lastName: 'User',
      mobile: '1234567890'
    };

    mockUserRepository.findByUsername.mockResolvedValue(newUpdatedUser);

    async function result() { 
      await userService.update(2, newUser);
    }

    await expect(result).rejects.toThrow('Username already exists');
  });
});
