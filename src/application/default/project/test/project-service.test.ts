import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { ProjectService } from "../project.service";
import { IUnDeletableRepository } from "../../../common/repositories/base.repository";
import { IProject } from "../project.model";
import { Insertable, Selectable, Transaction } from "kysely";
import { UpdateableEntity } from "../../../common/types/entity";
import { IProjectRepository } from "../project.repository";

describe('Default -> Project -> ProjectService', () => {
  let projectService: ProjectService;
  let mockProjectRepository: DeepMockProxy<IProjectRepository>;

  beforeEach(() => {
      mockProjectRepository = mockDeep<IProjectRepository>();
      projectService = new ProjectService(mockProjectRepository);
  });

  test('findById should call repository method with correct parameters', async () => {
    const mockProject = {
      id: 1,
      projectName: 'HRMS System',
      description: 'A human resource management system for internal use.'
      } as Selectable<IProject>;
    mockProjectRepository.findById.mockResolvedValue(mockProject);

    const result = await projectService.findById(1);

    expect(mockProjectRepository.findById).toHaveBeenCalledWith(1);
    expect(result).toEqual(mockProject);
  });

  test('findAll should call repository method with correct parameters', async () => {
    const mockProjects = [{
      id: 1,
      projectName: 'HRMS System',
      description: 'A human resource management system for internal use.'
      }] as Selectable<IProject>[];
    mockProjectRepository.findAll.mockResolvedValue(mockProjects);

    const result = await projectService.findAll(10, 0);

    expect(mockProjectRepository.findAll).toHaveBeenCalledWith(10, 0);
    expect(result).toEqual(mockProjects);
  });

  test('create should call repository method with correct parameters', async () => {
    const newProject = {
      projectName: 'HRMS System',
      description: 'A human resource management system for internal use.'
      } as (Insertable<IProject>);
    
    const insertableProject = { projectName: newProject.projectName,description: newProject.description} as Insertable<IProject>;
    const createdProject = { id: 2, projectName: newProject.projectName,description: newProject.description } as Selectable<IProject>;
    mockProjectRepository.create.mockResolvedValue(createdProject);

    const result = await projectService.create(newProject);

    expect(mockProjectRepository.create).toHaveBeenCalledWith(insertableProject);
    expect(result).toEqual(createdProject);
  });

  test('update should call repository method with correct parameters', async () => {
    const updatedData = { projectName: 'Updated Project' } as UpdateableEntity<IProject>;
    const updatedProject = { id: 1, ...updatedData } as Selectable<IProject>;
    mockProjectRepository.update.mockResolvedValue(updatedProject);

    const result = await projectService.update(1, updatedData);

    expect(mockProjectRepository.update).toHaveBeenCalledWith(1, updatedData);
    expect(result).toEqual(updatedProject);
  });
});