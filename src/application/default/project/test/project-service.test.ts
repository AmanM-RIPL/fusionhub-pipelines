import { DeepMockProxy, mockDeep } from "jest-mock-extended";
import { ProjectService } from "../project.service";
import { IUnDeletableRepository } from "../../../common/repositories/base.repository";
import { IProject } from "../project.model";
import { Insertable, Selectable, Transaction } from "kysely";
import { UpdateableEntity } from "../../../common/types/entity";

describe('Default -> Project -> ProjectService', () => {
  let projectService: ProjectService;
  let mockProjectRepository: DeepMockProxy<IUnDeletableRepository<IProject>>;

  beforeEach(() => {
    mockProjectRepository = mockDeep<IUnDeletableRepository<IProject>>();
    projectService = new ProjectService(mockProjectRepository, mockProjectRepository);
  });

  test('findById should call repository method with correct parameters', async () => {
    const mockProject = { id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210' } as Selectable<IProject>;
    mockProjectRepository.findById.mockResolvedValue(mockProject);

    const result = await projectService.findById(1);

    expect(mockProjectRepository.findById).toHaveBeenCalledWith(1);
    expect(result).toEqual(mockProject);
  });

  test('findAll should call repository method with correct parameters', async () => {
    const mockProjects = [{
        name: 'HR Management System',
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210' }] as Selectable<IProject>[];
    mockProjectRepository.findAll.mockResolvedValue(mockProjects);

    const result = await projectService.findAll(10, 0);

    expect(mockProjectRepository.findAll).toHaveBeenCalledWith(10, 0);
    expect(result).toEqual(mockProjects);
  });

  test('create should call repository method with correct parameters', async () => {
    const newProject = {
        name: 'HR Management System',
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210' } as (Insertable<IProject>);
    
    const insertableProject = { name: newProject.name,
  description: newProject.description,
  projectCode: newProject.projectCode,
  startDate: newProject.startDate,
  endDate: newProject.endDate,
  status: newProject.status,
  contactEmail: newProject.contactEmail,
  contactMobile: newProject.contactMobile,
  tenant: newProject.tenant,
  isBlocked: newProject.isBlocked
 } as Insertable<IProject>;
    const createdProject = { name: newProject.name,
  description: newProject.description,
  projectCode: newProject.projectCode,
  startDate: newProject.startDate,
  endDate: newProject.endDate,
  status: newProject.status,
  contactEmail: newProject.contactEmail,
  contactMobile: newProject.contactMobile,
  tenant: newProject.tenant,
  isBlocked: newProject.isBlocked
 } as Selectable<IProject>;
    mockProjectRepository.create.mockResolvedValue(createdProject);

    const result = await projectService.create(newProject);

    expect(mockProjectRepository.create).toHaveBeenCalledWith(insertableProject);
    expect(result).toEqual(createdProject);
  });

  test('update should call repository method with correct parameters', async () => {
    const updatedData = { name: 'Updated Project' } as UpdateableEntity<IProject>;
    const updatedProject = { id: 1, ...updatedData } as Selectable<IProject>;
    mockProjectRepository.update.mockResolvedValue(updatedProject);

    const result = await projectService.update(1, updatedData);

    expect(mockProjectRepository.update).toHaveBeenCalledWith(1, updatedData);
    expect(result).toEqual(updatedProject);
  });
});