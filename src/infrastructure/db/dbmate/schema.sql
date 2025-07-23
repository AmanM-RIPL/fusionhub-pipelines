SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: public; Type: SCHEMA; Schema: -; Owner: -
--

-- *not* creating schema, since initdb creates it


--
-- Name: SCHEMA public; Type: COMMENT; Schema: -; Owner: -
--

COMMENT ON SCHEMA public IS '';


SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- Name: draft_entity; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.draft_entity (
    id integer NOT NULL,
    tenant integer NOT NULL,
    "createdByUser" integer NOT NULL,
    "entitySchema" character varying(50) NOT NULL,
    data jsonb NOT NULL,
    "associatedApprovedEntity" integer,
    "nextApprovingUser" integer,
    "changeHistory" jsonb DEFAULT '{}'::jsonb NOT NULL
);


--
-- Name: draft_entity_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.draft_entity_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: draft_entity_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.draft_entity_id_seq OWNED BY public.draft_entity.id;


--
-- Name: fh_user; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.fh_user (
    id integer NOT NULL,
    tenant integer,
    "createdOn" timestamp without time zone DEFAULT CURRENT_TIMESTAMP,
    username character varying(50) NOT NULL,
    password character varying(100) NOT NULL,
    email character varying(50) DEFAULT ''::character varying NOT NULL,
    "firstName" character varying(20) DEFAULT ''::character varying NOT NULL,
    "lastName" character varying(20) DEFAULT ''::character varying NOT NULL,
    mobile character varying(20) DEFAULT ''::character varying NOT NULL
);


--
-- Name: fh_user_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.fh_user_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: fh_user_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.fh_user_id_seq OWNED BY public.fh_user.id;


--
-- Name: permission; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.permission (
    id integer NOT NULL,
    tenant integer NOT NULL,
    "createdOn" timestamp without time zone DEFAULT CURRENT_TIMESTAMP,
    "user" integer NOT NULL,
    entity character varying(50) NOT NULL,
    approval integer[] DEFAULT '{}'::integer[] NOT NULL,
    filter jsonb DEFAULT '{}'::jsonb NOT NULL,
    access jsonb DEFAULT '{}'::jsonb NOT NULL
);


--
-- Name: permission_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.permission_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: permission_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.permission_id_seq OWNED BY public.permission.id;


--
-- Name: schema_migrations; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.schema_migrations (
    version character varying NOT NULL
);


--
-- Name: tenant; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.tenant (
    id integer NOT NULL,
    "createdOn" timestamp without time zone DEFAULT CURRENT_TIMESTAMP,
    name character varying(50) NOT NULL,
    "defaultEmail" character varying(50) NOT NULL,
    "defaultEmail1" character varying(50),
    "defaultEmail2" character varying(50),
    "defaultMobile" character varying(20) NOT NULL,
    "defaultMobile1" character varying(20),
    "defaultMobile2" character varying(20),
    "isBlocked" boolean DEFAULT true
);


--
-- Name: tenant_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.tenant_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tenant_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.tenant_id_seq OWNED BY public.tenant.id;

--
-- Name: project id; Type: DEFAULT; Schema: public; Owner: -
--

CREATE TABLE public.project (
    id integer NOT NULL,
    "createdOn" timestamp without time zone DEFAULT CURRENT_TIMESTAMP,
    name character varying(100) NOT NULL,
    description text,
    "projectCode" character varying(50) NOT NULL,
    "startDate" date,
    "endDate" date,
    status character varying(20) DEFAULT 'pending',
    "contactEmail" character varying(50),
    "contactMobile" character varying(20),
    "tenantId" integer,
    "isBlocked" boolean DEFAULT false
);

-- Create the sequence for 'project.id'
CREATE SEQUENCE public.project_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;

-- Link the sequence to the 'project.id' column
ALTER SEQUENCE public.project_id_seq OWNED BY public.project.id;

-- Set default value for 'project.id' using the sequence
ALTER TABLE ONLY public.project ALTER COLUMN id SET DEFAULT nextval('public.project_id_seq'::regclass);

-- Optional: Add foreign key to tenant table
ALTER TABLE ONLY public.project
    ADD CONSTRAINT project_tenantId_fkey FOREIGN KEY ("tenantId") REFERENCES public.tenant(id) ON DELETE SET NULL;


--
-- Name: draft_entity id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.draft_entity ALTER COLUMN id SET DEFAULT nextval('public.draft_entity_id_seq'::regclass);


--
-- Name: fh_user id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.fh_user ALTER COLUMN id SET DEFAULT nextval('public.fh_user_id_seq'::regclass);


--
-- Name: permission id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.permission ALTER COLUMN id SET DEFAULT nextval('public.permission_id_seq'::regclass);


--
-- Name: tenant id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.tenant ALTER COLUMN id SET DEFAULT nextval('public.tenant_id_seq'::regclass);


--
-- Name: draft_entity draft_entity_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.draft_entity
    ADD CONSTRAINT draft_entity_pkey PRIMARY KEY (id);


--
-- Name: fh_user fh_user_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.fh_user
    ADD CONSTRAINT fh_user_pkey PRIMARY KEY (id);


--
-- Name: fh_user fh_user_username_key; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.fh_user
    ADD CONSTRAINT fh_user_username_key UNIQUE (username);


--
-- Name: permission permission_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.permission
    ADD CONSTRAINT permission_pkey PRIMARY KEY (id);


--
-- Name: schema_migrations schema_migrations_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.schema_migrations
    ADD CONSTRAINT schema_migrations_pkey PRIMARY KEY (version);


--
-- Name: tenant tenant_pkey; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.tenant
    ADD CONSTRAINT tenant_pkey PRIMARY KEY (id);


--
-- Name: permission uq_permission_user_entity_tenant; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.permission
    ADD CONSTRAINT uq_permission_user_entity_tenant UNIQUE ("user", entity, tenant);


--
-- Name: draft_entity fk_draft_entity_created_by_user; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.draft_entity
    ADD CONSTRAINT fk_draft_entity_created_by_user FOREIGN KEY ("createdByUser") REFERENCES public.fh_user(id) ON DELETE CASCADE;


--
-- Name: draft_entity fk_draft_entity_next_approving_user; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.draft_entity
    ADD CONSTRAINT fk_draft_entity_next_approving_user FOREIGN KEY ("nextApprovingUser") REFERENCES public.fh_user(id) ON DELETE SET NULL;


--
-- Name: draft_entity fk_draft_entity_tenant; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.draft_entity
    ADD CONSTRAINT fk_draft_entity_tenant FOREIGN KEY (tenant) REFERENCES public.tenant(id) ON DELETE CASCADE;


--
-- Name: permission fk_permission_tenant; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.permission
    ADD CONSTRAINT fk_permission_tenant FOREIGN KEY (tenant) REFERENCES public.tenant(id) ON DELETE CASCADE;


--
-- Name: permission fk_permission_user; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.permission
    ADD CONSTRAINT fk_permission_user FOREIGN KEY ("user") REFERENCES public.fh_user(id) ON DELETE CASCADE;


--
-- Name: fh_user fk_user_tenant; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.fh_user
    ADD CONSTRAINT fk_user_tenant FOREIGN KEY (tenant) REFERENCES public.tenant(id) ON DELETE CASCADE;


--
-- PostgreSQL database dump complete
--


--
-- Dbmate schema migrations
--

INSERT INTO public.schema_migrations (version) VALUES
    ('20250716065417');
