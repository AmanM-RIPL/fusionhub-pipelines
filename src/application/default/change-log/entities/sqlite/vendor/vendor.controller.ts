import { Selectable } from "kysely";
import { IChangeLog } from "../../../change-log.model";
import { IVendor } from "./vendor.model";
import { IProjectEntityBaseController } from "../../../../../common/repositories/project-entity.repository";

export class VendorController implements IProjectEntityBaseController {
    validate(changeLog: Selectable<IChangeLog>): void {

        if (!changeLog.entitySchema || typeof changeLog.entitySchema !== "object") {
            const newEntitySchema: Selectable<IVendor> = {
                id: 0,
                global_id: 0,
                vendor_name: "",
                vendor_address: "",
                vendor_contact_person: "",
                vendor_mobile: "",
                vendor_email: "",
                approval_status: false,
                change_history: {
                    user: 0,
                    changeType: "create",
                    description: "",
                    timestamp: new Date(),
                    approvalHistory: []
                }
            };
        }

        const entitySchema = changeLog.entitySchema as Selectable<IVendor>;

        if (typeof entitySchema.vendor_name !== "string") {
            entitySchema.vendor_name = "";
        }

        if (typeof entitySchema.vendor_address !== "string") {
            entitySchema.vendor_address = "";
        }

        if (typeof entitySchema.approval_status !== "boolean") {
            entitySchema.approval_status = false;
        }

        if (typeof entitySchema.vendor_contact_person !== "string") {
            entitySchema.vendor_contact_person = "";
        }

        if (typeof entitySchema.vendor_mobile !== "string") {
            entitySchema.vendor_mobile = "";
        }

        if (typeof entitySchema.vendor_email !== "string") {
            entitySchema.vendor_email = "";
        }

        changeLog.entitySchema = entitySchema
    }

}