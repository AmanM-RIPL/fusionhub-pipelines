// import { InsertQueryBuilder, InsertResult, Kysely, Selectable, SelectQueryBuilder, Transaction, UpdateQueryBuilder } from "kysely";
// import { DraftEntityDao } from "../dao/draft-entity.dao";
// import { mockDeep, DeepMockProxy } from 'jest-mock-extended';
// import { IDatabase } from "../../../../infrastructure/db/kysely/types";

// // Helper types for the chained query builder objects
// type DraftEntitySelectQueryBuilder = SelectQueryBuilder<IDatabase, 'public.draft_entity', {}>;
// type DraftEntityInsertQueryBuilder = InsertQueryBuilder<IDatabase, keyof IDatabase, InsertResult>;
// type DraftEntityInsertReturningAllQueryBuilder = InsertQueryBuilder<IDatabase, keyof IDatabase, Selectable<IDatabase['public.draft_entity']>>;
// type DraftEntityUpdateReturningAllQueryBuilder = UpdateQueryBuilder<IDatabase, 'public.draft_entity', 'public.draft_entity', Selectable<IDatabase['public.draft_entity']>>;
// type DraftEntityUpdateQueryBuilder = UpdateQueryBuilder<IDatabase, 'public.draft_entity', 'public.draft_entity', {}>;

// describe('Default -> DraftEntity -> DraftEntityDao', () => {
//     let mockKysely: DeepMockProxy<Kysely<IDatabase>>;
//     let draftEntityDao: DraftEntityDao;
//     let draftEntityDaoNoTenant: DraftEntityDao;
//     const tenantId = 1;

//   beforeEach(() => {
//      mockKysely = mockDeep<Kysely<IDatabase>>();
//      draftEntityDao = new DraftEntityDao(mockKysely, tenantId);
//      draftEntityDaoNoTenant = new DraftEntityDao(mockKysely, null);
//   });

//   test('findById should call correct methods', async () => {
//     // test data
//     const mockDraftEntity = {
//         id: 1,
//         tenant: 1,
//         project: 101,
//         entity: "employee",
//         createdByUser: 501
//       };

//     // Mock the Kysely methods to return the expected results
//     const mockSelect = mockDeep<DraftEntitySelectQueryBuilder>();
//     mockKysely.selectFrom.mockReturnValue(mockSelect);
//     mockSelect.selectAll.mockReturnThis();
//     mockSelect.where.mockReturnThis();
//     mockSelect.executeTakeFirst.mockResolvedValueOnce(mockDraftEntity);


//     // Call the method under test
//     const draftEntityResult = await draftEntityDao.findById(1);

//     // Add your assertions or method calls here
//     expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.draftEntity");
//     expect(mockSelect.selectAll).toHaveBeenCalled();
//     expect(mockSelect.where).toHaveBeenCalledWith("id", "=", 1);
//     expect(mockSelect.executeTakeFirst).toHaveBeenCalled();
//   });

//   test('findById should return correct value', async () => {
//     // test data
//     const mockDraftEntity = {
//        id: 1,
//         tenant: 1,
//         project: 101,
//         entity: "employee",
//         createdByUser: 501
//       };

//     // Mock the Kysely methods to return the expected results
//     const mockSelect = mockDeep<DraftEntitySelectQueryBuilder>();
//     mockKysely.selectFrom.mockReturnValue(mockSelect);
//     mockSelect.selectAll.mockReturnThis();
//     mockSelect.where.mockReturnThis();
//     mockSelect.executeTakeFirst.mockResolvedValueOnce(mockDraftEntity);


//     // Call the method under test
//     const draftEntityResult = await draftEntityDao.findById(1);

//     // Add your assertions or method calls here
//     expect(draftEntityResult).toEqual(mockDraftEntity);
//   });

//   test('findAll should call correct methods', async () => {
//     // test data
//     const mockDraftEntity = [
//     {id: 1,
//         tenant: 1,
//         project: 101,
//         entity: "employee",
//         createdByUser: 501},
//     {id: 1,
//         tenant: 1,
//         project: 2,
//         entity: "employee",
//         createdByUser: 501 }
//   ];

//     // Mock the Kysely methods to return the expected results
//     const mockSelect = mockDeep<DraftEntitySelectQueryBuilder>();
//     mockKysely.selectFrom.mockReturnValue(mockSelect);
//     mockSelect.selectAll.mockReturnThis();
//     mockSelect.where.mockReturnThis();
//     mockSelect.limit.mockReturnThis();
//     mockSelect.offset.mockReturnThis();
//     mockSelect.execute.mockResolvedValueOnce(mockDraftEntity);


//     // Call the method under test
//     const draftEntityResult = await draftEntityDao.findAll(10, 0);

//     // Add your assertions or method calls here
//     expect(mockKysely.selectFrom).toHaveBeenCalledWith("public.draftEntity");
//     expect(mockSelect.selectAll).toHaveBeenCalled();
//     expect(mockSelect.limit).toHaveBeenCalledWith(10);
//     expect(mockSelect.offset).toHaveBeenCalledWith(0);
//     expect(mockSelect.execute).toHaveBeenCalled();
//   });

//   test('findAll should return correct value', async () => {
//     // test data
//     const mockDraftEntity = [
//     {id: 1,
//         tenant: 1,
//         project: 101,
//         entity: "employee",
//         createdByUser: 501},
//     {id: 1,
//         tenant: 1,
//         project: 2,
//         entity: "employee",
//         createdByUser: 501 }];

//     // Mock the Kysely methods to return the expected results
//     const mockSelect = mockDeep<DraftEntitySelectQueryBuilder>();
//     mockKysely.selectFrom.mockReturnValue(mockSelect);
//     mockSelect.selectAll.mockReturnThis();
//     mockSelect.where.mockReturnThis();
//     mockSelect.limit.mockReturnThis();
//     mockSelect.offset.mockReturnThis();
//     mockSelect.execute.mockResolvedValueOnce(mockDraftEntity);


//     // Call the method under test
//     const draftEntityResult = await draftEntityDao.findAll(10, 0);

//     // Add your assertions or method calls here
//     expect(draftEntityResult).toEqual(mockDraftEntity);
//   });

//   test('create should call correct methods', async () => {
//     // test data
//     const mockDraftEntity = { 
//         id: 1,
//         tenant: 1,
//         createdOn: new Date(),
//         project: 101,
//         entity: "employee",
//         createdByUser: 501,
//         nextApprovingUser: 502,
//         entitySchema: {},
//         associatedApprovedEntity: null,
//         parentDraftEntity: null,
//         changeHistory: [],
//         data: {},
//         isBlocked: false
//     };

//     // Mock the Kysely methods to return the expected results
//     const mockInsert = mockDeep<DraftEntityInsertQueryBuilder>();
//     mockKysely.insertInto.mockReturnValue(mockInsert);
//     mockInsert.values.mockReturnThis();
//     const mockReturningAll = mockDeep<DraftEntityInsertReturningAllQueryBuilder>();
//     mockInsert.returningAll.mockReturnValue(mockReturningAll);
//     mockReturningAll.executeTakeFirstOrThrow.mockResolvedValueOnce(mockDraftEntity);


//     // Call the method under test
//     const draftEntityResult = await draftEntityDao.create(mockDraftEntity);

//     // Add your assertions or method calls here
//     expect(mockKysely.insertInto).toHaveBeenCalledWith("public.draftEntity");
//     expect(mockInsert.values).toHaveBeenCalledWith(mockDraftEntity);
//     expect(mockInsert.returningAll).toHaveBeenCalled();
//     expect(mockReturningAll.executeTakeFirstOrThrow).toHaveBeenCalled();
//   });

//   test('create should return correct value', async () => {
//     // test data
//     const mockDraftEntity = { 
//         id: 1,
//         tenant: 1,
//         createdOn: new Date(),
//         project: 101,
//         entity: "employee",
//         createdByUser: 501,
//         nextApprovingUser: 502,
//         entitySchema: {},
//         associatedApprovedEntity: null,
//         parentDraftEntity: null,
//         changeHistory: [],
//         data: {},
//         isBlocked: false
//     };

//     // Mock the Kysely methods to return the expected results
//     const mockInsert = mockDeep<DraftEntityInsertQueryBuilder>();
//     mockKysely.insertInto.mockReturnValue(mockInsert);
//     mockInsert.values.mockReturnThis();
//     const mockReturningAll = mockDeep<DraftEntityInsertReturningAllQueryBuilder>();
//     mockInsert.returningAll.mockReturnValue(mockReturningAll);
//     mockReturningAll.executeTakeFirstOrThrow.mockResolvedValueOnce(mockDraftEntity);


//     // Call the method under test
//     const draftEntityResult = await draftEntityDao.create(mockDraftEntity);

//     // Add your assertions or method calls here
//     expect(draftEntityResult).toEqual(mockDraftEntity);
//   });

//   test('update should call correct methods', async () => {
//     // test data
//     const mockDraftEntity = { 
//          id: 1,
//             tenant: 1,
//             createdOn: new Date(),
//             project: 101,
//             entity: "employee",
//             createdByUser: 501,
//             nextApprovingUser: 502,
//             entitySchema: {},
//             associatedApprovedEntity: null,
//             parentDraftEntity: null,
//             changeHistory: [],
//             data: {},
//             isBlocked: false
//     };

//     // Mock the Kysely methods to return the expected results
//     const mockUpdate = mockDeep<DraftEntityUpdateQueryBuilder>();
//     //@ts-ignore
//     mockKysely.updateTable.mockReturnValue(mockUpdate);
//     mockUpdate.set.mockReturnThis();
//     mockUpdate.where.mockReturnThis();
//     const mockReturningAll = mockDeep<DraftEntityUpdateReturningAllQueryBuilder>();
//     mockUpdate.returningAll.mockReturnValue(mockReturningAll);
//     mockReturningAll.executeTakeFirst.mockResolvedValueOnce(mockDraftEntity);    

//     // Call the method under test
//     const draftEntityResult = await draftEntityDao.update(1, mockDraftEntity);

//     // Add your assertions or method calls here
//     expect(mockKysely.updateTable).toHaveBeenCalledWith("public.draftEntity");
//     expect(mockUpdate.set).toHaveBeenCalledWith(mockDraftEntity);
//     expect(mockUpdate.where).toHaveBeenCalledWith("id", "=", 1);
//     expect(mockUpdate.returningAll).toHaveBeenCalled();
//     expect(mockReturningAll.executeTakeFirst).toHaveBeenCalled();
//   });

//   test('update should return correct value', async () => {
//     // test data
//     const mockDraftEntity = { 
//          id: 1,
//         tenant: 1,
//         createdOn: new Date(),
//         project: 101,
//         entity: "employee",
//         createdByUser: 501,
//         nextApprovingUser: 502,
//         entitySchema: {},
//         associatedApprovedEntity: null,
//         parentDraftEntity: null,
//         changeHistory: [],
//         data: {},
//         isBlocked: false
//     };

//     // Mock the Kysely methods to return the expected results
//     const mockUpdate = mockDeep<DraftEntityUpdateQueryBuilder>();
//     mockKysely.updateTable.mockReturnValue(mockUpdate);
//     mockUpdate.set.mockReturnThis();
//     mockUpdate.where.mockReturnThis();
//     const mockReturningAll = mockDeep<DraftEntityUpdateReturningAllQueryBuilder>();
//     mockUpdate.returningAll.mockReturnValue(mockReturningAll);
//     mockReturningAll.executeTakeFirst.mockResolvedValueOnce(mockDraftEntity);    

//     // Call the method under test
//     const draftEntityResult = await draftEntityDao.update(1, mockDraftEntity);

//     // Add your assertions or method calls here
//     expect(draftEntityResult).toEqual(mockDraftEntity);
//   });
// });