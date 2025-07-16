import { IDraftEntity } from "../draft-entity/draft-entity.model";
import { IPermission } from "../permission/permission.model";
import { ITenant } from "../tenant/tenant.model";
import { IUser } from "../user/user.model";


export interface IDefaultSchema {
  "public.tenant": ITenant;
  "public.fh_user": IUser;
  "public.permission": IPermission;
  "public.draft_entity": IDraftEntity;
}