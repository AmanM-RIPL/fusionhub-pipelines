import { Insertable, Selectable, Transaction } from "kysely";
import { IUser } from "./user.model";
import { UpdateableEntity } from "../../common/types/entity";
import { IUserRepository } from "./user.repository";

export class UserService {
  constructor(protected readonly userRepository: IUserRepository) {}

  async findById(id: number): Promise<Selectable<IUser> | undefined> {
    return await this.userRepository.findById(id);
  }

  async findAll(limit: number, offset: number): Promise<Selectable<IUser>[]> {
    return await this.userRepository.findAll(limit, offset);
  }

  async create(user: Insertable<IUser>): Promise<Selectable<IUser>> {
    // find a user with that username
    const existingUser = await this.userRepository.findByUsername(user.username);
    if (existingUser) {
      throw new Error('User already exists');
    }

    return await this.userRepository.create(user);
  }

  async update(id: number, updatedUser: UpdateableEntity<IUser>): Promise<Selectable<IUser> | undefined> {
    // if updated user has a new username then check if that username exists first
    if (updatedUser.username) {
      const existingUser = await this.userRepository.findByUsername(updatedUser.username);
      if (existingUser && existingUser.id !== id) {
        throw new Error('Username already exists');
      }
    }

    return await this.userRepository.update(id, updatedUser);
  }

  async authenticate(username: string, password: string): Promise<Selectable<IUser> | undefined> {
    return await this.userRepository.findByUsernameAndPassword(username, password);
  }
}