import { IDraftEntity } from "../draft-entity/draft-entity.model";
import { IPermission } from "../permission/permission.model";
import { ITenant } from "../tenant/tenant.model";
import { IProject } from "../project/project.model";
import { IUser } from "../user/user.model";
import { IChangeLog } from "../change-log/change-log.model";


export interface IDefaultSchema {
  "public.tenant": ITenant;
  "public.project": IProject;
  "public.fh_user": IUser;
  "public.permission": IPermission;
  "public.draft_entity": IDraftEntity;
  "public.change_log": IChangeLog;
}