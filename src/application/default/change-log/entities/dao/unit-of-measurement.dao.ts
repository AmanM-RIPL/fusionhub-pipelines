class UnitOfMeasurementDAO {
    async create(data: { uom_name: string, unit_type: string }): Promise<void>
    {
        sqliteTable.("INSERT INTO ")
    }
}