-- migrate:up
CREATE TABLE tenant (
    -- default columns
    "id" SERIAL PRIMARY KEY,
    "createdOn" TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    -- custom columns
    "name" VARCHAR(50) NOT NULL,
    "defaultEmail" VARCHAR(50) NOT NULL, -- not unique because one person may have multiple tenants
    "defaultEmail1" VARCHAR(50),
    "defaultEmail2" VARCHAR(50),
    "defaultMobile" VARCHAR(20) NOT NULL, -- not unique because one person may have multiple tenants
    "defaultMobile1" VARCHAR(20),
    "defaultMobile2" VARCHAR(20),

    -- admin and access
    "isBlocked" BOOLEAN DEFAULT TRUE
);

CREATE TABLE project (
    -- default columns
    "id" SERIAL PRIMARY KEY,
    "tenant" INT NOT NULL,
    "createdOn" TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    -- custom columns
    "name" VARCHAR(100) NOT NULL,
    "description" TEXT,
    "projectCode" VARCHAR(50) UNIQUE NOT NULL,
    "startDate" DATE,
    "endDate" DATE,
    "status" VARCHAR(20) DEFAULT 'pending',

    -- contact information
    "contactEmail" VARCHAR(50),
    "contactMobile" VARCHAR(20),

    -- access control
    "isBlocked" BOOLEAN DEFAULT FALSE
    
);


CREATE TABLE fh_user (
    -- default columns
    "id" SERIAL PRIMARY KEY,
    "tenant" INT, -- does not have NOT NULL because it can be a super admin user
    "createdOn" TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    -- custom columns
    "username" VARCHAR(50) UNIQUE NOT NULL,
    "password" VARCHAR(100) NOT NULL,
    "email" VARCHAR(50) NOT NULL DEFAULT '',
    "firstName" VARCHAR(20) NOT NULL DEFAULT '',
    "lastName" VARCHAR(20) NOT NULL DEFAULT '',
    "mobile" VARCHAR(20) NOT NULL DEFAULT '',

    -- foreign keys
    CONSTRAINT fk_user_tenant FOREIGN KEY(tenant)
        REFERENCES tenant(id) ON DELETE CASCADE
);

CREATE TABLE permission (
    -- default columns
    "id" SERIAL PRIMARY KEY,
    "tenant" INT NOT NULL,
    "createdOn" TIMESTAMP DEFAULT CURRENT_TIMESTAMP,

    -- custom columns
    "user" INT NOT NULL,
    "entity" VARCHAR(50) NOT NULL,
    "approval" INT[] NOT NULL DEFAULT '{}', -- array of user IDs
    "filter" JSONB NOT NULL DEFAULT '{}', -- json of type {"field": integer[]}
    "access" JSONB NOT NULL DEFAULT '{}', -- json of type {"create": boolean, "read": boolean | "self", "update": boolean}

    -- foreign keys
    CONSTRAINT fk_permission_tenant FOREIGN KEY(tenant)
        REFERENCES tenant(id) ON DELETE CASCADE,
    CONSTRAINT fk_permission_user FOREIGN KEY("user")
        REFERENCES fh_user(id) ON DELETE CASCADE,
    
    -- unique constraints on user + entity + tenant
    CONSTRAINT uq_permission_user_entity_tenant UNIQUE("user", "entity", "tenant")
);

CREATE TABLE draft_entity (
    -- default columns
    "id" SERIAL PRIMARY KEY,
    "tenant" INT NOT NULL,
    "createdByUser" INT NOT NULL,

    -- custom columns
    "entitySchema" VARCHAR(50) NOT NULL, -- reference to the entity schema
    "data" JSONB NOT NULL, -- json data of format {"fieldName": value}
    "associatedApprovedEntity" INT, -- this is the id of the approved system entity
    "nextApprovingUser" INT, -- this is the id of the next user who will approve the draft entity
    "changeHistory" JSONB NOT NULL DEFAULT '{}', -- json of type ChangeHistory take a look at src/common/types

    -- foreign keys
    CONSTRAINT fk_draft_entity_tenant FOREIGN KEY(tenant)
        REFERENCES tenant(id) ON DELETE CASCADE,
    CONSTRAINT fk_draft_entity_created_by_user FOREIGN KEY("createdByUser")
        REFERENCES fh_user(id) ON DELETE CASCADE,
    CONSTRAINT fk_draft_entity_next_approving_user FOREIGN KEY("nextApprovingUser")
        REFERENCES fh_user(id) ON DELETE SET NULL
);

-- migrate:down
DROP TABLE draft_entity;
DROP TABLE permission;
DROP TABLE fh_user;
DROP TABLE project;
DROP TABLE tenant;