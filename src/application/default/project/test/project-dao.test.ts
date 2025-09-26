import { Kysely, Selectable } from "kysely";
import { ProjectDao } from "../dao/project.dao";
import { IDatabase } from "../../../../infrastructure/db/kysely/types";
import { InsertableEntity, UpdateableEntity } from "../../../common/types/entity";
import { MockKysely, MockKyselyDeleteQueryBuilder, MockKyselyInsertQueryBuilder, MockKyselySelectQueryBuilder, MockKyselyUpdateQueryBuilder } from "../../../common/types/test";



describe('Default -> Project -> ProjectDao', () => {
     let mockKysely: MockKysely;
      let projectDao: ProjectDao;
      let tenant = 2;
      let user = 2;

  beforeEach(() => {
      mockKysely = {
      selectFrom: jest.fn(),
      deleteFrom: jest.fn(),
      insertInto: jest.fn(),
      updateTable: jest.fn(),
    };

     projectDao = new ProjectDao(mockKysely  as unknown as Kysely<IDatabase>, tenant);
  });

  test('findById should call correct methods', async () => {
    // test data
    const mockProjects = {
        id: 1, 
        projectName: 'Navyug',
        description: 'Navyug Smart Mini City'
      };

    // Mock the Kysely methods to return the expected results
    const mockSelect: MockKyselySelectQueryBuilder = {
      selectAll: jest.fn(),
      select: jest.fn(),
      where: jest.fn(),
      limit: jest.fn(),
      offset: jest.fn(),
      executeTakeFirst: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      execute: jest.fn(),
    };
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.findById(1);

    // Add assertions.
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.project");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockSelect.executeTakeFirst).toHaveBeenCalled();
  });

  test('findById should return correct value', async () => {
    // test data
    const mockProjects = {
        id: 1, 
        projectName: 'Navyug',
        description: 'Navyug Smart Mini City'
      };

    // Mock the Kysely methods to return the expected results
    const mockSelect: MockKyselySelectQueryBuilder = {
      selectAll: jest.fn(),
      select: jest.fn(),
      where: jest.fn(),
      limit: jest.fn(),
      offset: jest.fn(),
      executeTakeFirst: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      execute: jest.fn(),
    };
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.executeTakeFirst.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.findById(1);

    // Add assertions.
    expect(projectResult).toEqual(mockProjects);
  });

  test('findAll should call correct methods', async () => {
    // test data
    const mockProjects = [
    {id: 1, projectName: 'Navyug',description: 'Navyug Smart Mini City' },
    {id: 2, projectName: 'CRM Management System',description: 'A system to manage records.' }
  ];

    // Mock the Kysely methods to return the expected results
    const mockSelect: MockKyselySelectQueryBuilder = {
      selectAll: jest.fn(),
      select: jest.fn(),
      where: jest.fn(),
      limit: jest.fn(),
      offset: jest.fn(),
      executeTakeFirst: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      execute: jest.fn(),
    };
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.limit.mockReturnThis();
    mockSelect.offset.mockReturnThis();
    mockSelect.execute.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.findAll(10, 0);

    // Add assertions.
    expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.project");
    expect(mockSelect.selectAll).toHaveBeenCalled();
    expect(mockSelect.limit).toHaveBeenCalledWith(10);
    expect(mockSelect.offset).toHaveBeenCalledWith(0);
    expect(mockSelect.execute).toHaveBeenCalled();
  });

  test('findAll should return correct value', async () => {
    // test data
    const mockProjects = [
    {id: 1, projectName: 'Navyug',description: 'Navyug Smart Mini City' },
    {id: 2, projectName: 'CRM Management System',description: 'A system to manage records.' }];

    // Mock the Kysely methods to return the expected results
    const mockSelect: MockKyselySelectQueryBuilder = {
      selectAll: jest.fn(),
      select: jest.fn(),
      where: jest.fn(),
      limit: jest.fn(),
      offset: jest.fn(),
      executeTakeFirst: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      execute: jest.fn(),
    };
    mockKysely.selectFrom.mockReturnValue(mockSelect);
    mockSelect.selectAll.mockReturnThis();
    mockSelect.where.mockReturnThis();
    mockSelect.limit.mockReturnThis();
    mockSelect.offset.mockReturnThis();
    mockSelect.execute.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.findAll(10, 0);

    // Add assertions.
    expect(projectResult).toEqual(mockProjects);
  });

  test('create should call correct methods', async () => {
    // test data
    const mockProjects: Selectable<IDatabase['public.project']> = {
        id: 1, 
        tenant: tenant,
        projectName: 'Navyug',
        createdOn: new Date(),
        description: 'Navyug Smart Mini City',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        customerName: 'Company',
        customerEmail: 'project.manager@example.com',
        customerMobile: '9876543210',
        lastChangeLogId: 1,
        isBlocked: false
    };

    const mockProjectInsertable: InsertableEntity<IDatabase['public.project']> = {
        projectName: 'Navyug',
        createdOn: new Date(),
        description: 'Navyug Smart Mini City',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        customerName: 'Company',
        customerEmail: 'project.manager@example.com',
        customerMobile: '9876543210',
        lastChangeLogId: 1,
        isBlocked: false
    };

    // Mock the Kysely methods to return the expected results
    const mockInsert: MockKyselyInsertQueryBuilder = {
      values: jest.fn(),
      returningAll: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
    };
     mockKysely.insertInto.mockReturnValue(mockInsert);
    mockInsert.values.mockReturnThis();
    mockInsert.returningAll.mockReturnThis();
    mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.create(mockProjects);

    // Add assertions.
    expect(mockKysely.insertInto).toHaveBeenCalledWith("public.project");
    expect(mockInsert.values).toHaveBeenCalledWith(mockProjects);
    expect(mockInsert.returningAll).toHaveBeenCalled();
  });

  test('create should return correct value', async () => {
    // test data
    const mockProjects: Selectable<IDatabase['public.project']> = {
        id: 1, 
        tenant: tenant,
        projectName: 'Navyug',
        createdOn: new Date(),
        description: 'Navyug Smart Mini City',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        customerName: 'Company',
        customerEmail: 'project.manager@example.com',
        customerMobile: '9876543210',
        lastChangeLogId: 1,
        isBlocked: false
    };

    const mockProjectInsertable: InsertableEntity<IDatabase['public.project']> = {
        projectName: 'Navyug',
        createdOn: new Date(),
        description: 'Navyug Smart Mini City',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        customerName: 'Company',
        customerEmail: 'project.manager@example.com',
        customerMobile: '9876543210',
        lastChangeLogId: 1,
        isBlocked: false
    };

   // Mock the Kysely methods to return the expected results
    const mockInsert: MockKyselyInsertQueryBuilder = {
      values: jest.fn(),
      returningAll: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
    };
    mockKysely.insertInto.mockReturnValue(mockInsert);
    mockInsert.values.mockReturnThis();
    mockInsert.returningAll.mockReturnThis();
    mockInsert.executeTakeFirstOrThrow.mockResolvedValueOnce(mockProjects);


    // Call the method under test
    const projectResult = await projectDao.create(mockProjects);

    // Add assertions or method calls here
    expect(projectResult).toEqual(mockProjects);
  });

   test('update should call correct methods', async () => {
    // test data
    const mockProjects = { 
        id: 1, 
        tenant: tenant,
        name: 'HR Management System',
        createdOn: new Date(),
        description: 'A system to manage employee records, attendance, and payroll.',
        projectCode: 'HRMS-001',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        contactEmail: 'project.manager@example.com',
        contactMobile: '9876543210',
        isBlocked: false
    };

    // Mock the Kysely methods to return the expected results
    const mockUpdate: MockKyselyUpdateQueryBuilder = {
      set: jest.fn(),
      where: jest.fn(),
      returningAll: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      executeTakeFirst: jest.fn(),
    };
    mockKysely.updateTable.mockReturnValue(mockUpdate);
    mockUpdate.set.mockReturnThis();
    mockUpdate.where.mockReturnThis();
    mockUpdate.returningAll.mockReturnThis();
    mockUpdate.executeTakeFirst.mockResolvedValueOnce(mockProjects);   

    // Call the method under test
    const projectResult = await projectDao.update(1, mockProjects);

    // Add assertions.
    expect(mockKysely.updateTable).toHaveBeenCalledWith("public.project");
    expect(mockUpdate.set).toHaveBeenCalledWith(mockProjects);
    expect(mockUpdate.where).toHaveBeenCalledWith("id", "=", 1);
    expect(mockUpdate.returningAll).toHaveBeenCalled();
  });

  test('update should return correct value', async () => {
    // test data
    const mockProjects: Selectable<IDatabase['public.project']> = {
        id: 1, 
        tenant: tenant,
        projectName: 'Navyug',
        createdOn: new Date(),
        description: 'Navyug Smart Mini City',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        customerName: 'Company',
        customerEmail: 'project.manager@example.com',
        customerMobile: '9876543210',
        lastChangeLogId: 1,
        isBlocked: false
    };

    const mockProjectUpdateable:
      Omit<
        UpdateableEntity<IDatabase['public.project']>,
        "entitySchema" | "createdByUser"
      > = {
        projectName: 'Navyug',
        createdOn: new Date(),
        description: 'Navyug Smart Mini City',
        startDate: new Date('2025-01-01'),
        endDate: new Date('2025-12-31'),
        status: 'active',
        customerName: 'Company',
        customerEmail: 'project.manager@example.com',
        customerMobile: '9876543210',
        lastChangeLogId: 1,
    };

     // Mock the Kysely methods to return the expected results
    const mockUpdate: MockKyselyUpdateQueryBuilder = {
      set: jest.fn(),
      where: jest.fn(),
      returningAll: jest.fn(),
      executeTakeFirstOrThrow: jest.fn(),
      executeTakeFirst: jest.fn(),
    };
    mockKysely.updateTable.mockReturnValue(mockUpdate);
    mockUpdate.set.mockReturnThis();
    mockUpdate.where.mockReturnThis();
    mockUpdate.returningAll.mockReturnThis();
    mockUpdate.executeTakeFirst.mockResolvedValueOnce(mockProjects);

    // Call the method under test
    const projectResult = await projectDao.update(1, mockProjectUpdateable);

    // Add assertions.
    expect(projectResult).toEqual(mockProjects);
  });
});