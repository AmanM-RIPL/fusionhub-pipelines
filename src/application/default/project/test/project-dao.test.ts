import { InsertQueryBuilder, InsertResult, Kysely, Selectable, SelectQueryBuilder, Transaction, UpdateQueryBuilder } from "kysely";
import { ProjectDao } from "../dao/project.dao";
import { mockDeep, DeepMockProxy } from 'jest-mock-extended';
import { IDatabase } from "../../../../infrastructure/db/kysely/types";

// Helper types for the chained query builder objects
type ProjectsSelectQueryBuilder = SelectQueryBuilder<IDatabase, 'public.project', {}>;
type ProjectsInsertQueryBuilder = InsertQueryBuilder<IDatabase, keyof IDatabase, InsertResult>;
type ProjectsInsertReturningAllQueryBuilder = InsertQueryBuilder<IDatabase, keyof IDatabase, Selectable<IDatabase['public.project']>>;
type ProjectsUpdateReturningAllQueryBuilder = UpdateQueryBuilder<IDatabase, 'public.project', 'public.project', Selectable<IDatabase['public.project']>>;
type ProjectsUpdateQueryBuilder = UpdateQueryBuilder<IDatabase, 'public.project', 'public.project', {}>;

describe('Default -> Project -> ProjectDao', () => {
  let mockKysely: DeepMockProxy<Kysely<IDatabase>>;
  let projectDao: ProjectDao;

  beforeEach(() => {
    mockKysely = mockDeep<Kysely<IDatabase>>();
    projectDao = new ProjectDao(mockKysely);
  });

  test('findById should call correct methods', async () => {
    // test data
    const mockProjects = {
        id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210'
      };

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<ProjectsSelectQueryBuilder>();
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.findById(1);

    // Add your assertions or method calls here
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.project");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockSelect.executeTakeFirst).toHaveBeenCalled();
  });

  test('findById should return correct value', async () => {
    // test data
    const mockProjects = {
        id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210'
      };

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<ProjectsSelectQueryBuilder>();
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.findById(1);

    // Add your assertions or method calls here
    expect(projectResult).toEqual(mockProjects);
  });

  test('findAll should call correct methods', async () => {
    // test data
    const mockProjects = [
    {
      id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210'
    },
    {
      id: 1,
        name: 'Inventory Tractor',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'INVT-002',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210'
    }
  ];

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<ProjectsSelectQueryBuilder>();
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.limit.mockReturnThis();
    mockSelect.offset.mockReturnThis();
    mockSelect.execute.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.findAll(10, 0);

    // Add your assertions or method calls here
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.project");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.limit).toHaveBeenCalledWith(10);
    expect(mockSelect.offset).toHaveBeenCalledWith(0);
    expect(mockSelect.execute).toHaveBeenCalled();
  });

  test('findAll should return correct value', async () => {
    // test data
    const mockProjects = [{
      id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210'
    },
    {
      id: 1,
        name: 'Inventory Tractor',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'INVT-002',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210'
    }];

    // Mock the Kysely methods to return the expected results
    const mockSelect = mockDeep<ProjectsSelectQueryBuilder>();
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.limit.mockReturnThis();
    mockSelect.offset.mockReturnThis();
    mockSelect.execute.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.findAll(10, 0);

    // Add your assertions or method calls here
    expect(projectResult).toEqual(mockProjects);
  });

  test('create should call correct methods', async () => {
    // test data
    const mockProjects = { 
      id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210',
        isBlocked: false,
        tenantId: 1
    };

    // Mock the Kysely methods to return the expected results
    const mockInsert = mockDeep<ProjectsInsertQueryBuilder>();
    mockKysely.insertInto.mockReturnValue(mockInsert);
    mockInsert.values.mockReturnThis();
    const mockReturningAll = mockDeep<ProjectsInsertReturningAllQueryBuilder>();
    mockInsert.returningAll.mockReturnValue(mockReturningAll);
    mockReturningAll.executeTakeFirstOrThrow.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.create(mockProjects);

    // Add your assertions or method calls here
    expect(mockKysely.insertInto).toHaveBeenCalledWith("public.project");
    expect(mockInsert.values).toHaveBeenCalledWith(mockProjects);
    expect(mockInsert.returningAll).toHaveBeenCalled();
    expect(mockReturningAll.executeTakeFirstOrThrow).toHaveBeenCalled();
  });

  test('create should return correct value', async () => {
    // test data
    const mockProjects = { 
      id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210',
        isBlocked: false,
        tenantId: 1
    };

    // Mock the Kysely methods to return the expected results
    const mockInsert = mockDeep<ProjectsInsertQueryBuilder>();
    mockKysely.insertInto.mockReturnValue(mockInsert);
    mockInsert.values.mockReturnThis();
    const mockReturningAll = mockDeep<ProjectsInsertReturningAllQueryBuilder>();
    mockInsert.returningAll.mockReturnValue(mockReturningAll);
    mockReturningAll.executeTakeFirstOrThrow.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.create(mockProjects);

    // Add your assertions or method calls here
    expect(projectResult).toEqual(mockProjects);
  });

  test('update should call correct methods', async () => {
    // test data
    const mockProjects = { 
      id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210',
        isBlocked: false,
        tenantId: 1
    };

    // Mock the Kysely methods to return the expected results
    const mockUpdate = mockDeep<ProjectsUpdateQueryBuilder>();
    //@ts-ignore
    mockKysely.updateTable.mockReturnValue(mockUpdate);
    mockUpdate.set.mockReturnThis();
    mockUpdate.where.mockReturnThis();
    const mockReturningAll = mockDeep<ProjectsUpdateReturningAllQueryBuilder>();
    mockUpdate.returningAll.mockReturnValue(mockReturningAll);
    mockReturningAll.executeTakeFirst.mockResolvedValueOnce(mockProjects);    

    // Call the method under test
    const projectResult = await projectDao.update(1, mockProjects);

    // Add your assertions or method calls here
    expect(mockKysely.updateTable).toHaveBeenCalledWith("public.project");
    expect(mockUpdate.set).toHaveBeenCalledWith(mockProjects);
    expect(mockUpdate.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockUpdate.returningAll).toHaveBeenCalled();
    expect(mockReturningAll.executeTakeFirst).toHaveBeenCalled();
  });

  test('update should return correct value', async () => {
    // test data
    const mockProjects = { 
      id: 1,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210',
        isBlocked: false,
        tenantId: 1
        
    };

    // Mock the Kysely methods to return the expected results
    const mockUpdate = mockDeep<ProjectsUpdateQueryBuilder>();
    mockKysely.updateTable.mockReturnValue(mockUpdate);
    mockUpdate.set.mockReturnThis();
    mockUpdate.where.mockReturnThis();
    const mockReturningAll = mockDeep<ProjectsUpdateReturningAllQueryBuilder>();
    mockUpdate.returningAll.mockReturnValue(mockReturningAll);
    mockReturningAll.executeTakeFirst.mockResolvedValueOnce(mockProjects);    

    // Call the method under test
    const projectResult = await projectDao.update(1, mockProjects);

    // Add your assertions or method calls here
    expect(projectResult).toEqual(mockProjects);
  });
});