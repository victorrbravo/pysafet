
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;


CREATE TABLE architecture (
    id integer NOT NULL,
    name text,
    description text
);




CREATE SEQUENCE architecture_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;




ALTER SEQUENCE architecture_id_seq OWNED BY architecture.id;



CREATE TABLE attachment (
    type text NOT NULL,
    id text NOT NULL,
    filename text NOT NULL,
    size integer,
    "time" bigint,
    description text,
    author text,
    ipnr text
);




CREATE TABLE auth_cookie (
    cookie text NOT NULL,
    name text NOT NULL,
    ipnr text NOT NULL,
    "time" integer
);




CREATE TABLE cache (
    id text NOT NULL,
    generation integer
);




CREATE TABLE component (
    name text NOT NULL,
    owner text,
    description text
);




CREATE TABLE download (
    id integer NOT NULL,
    file text,
    description text,
    size integer,
    "time" integer,
    count integer,
    author text,
    tags text,
    component text,
    version text,
    architecture integer,
    platform integer,
    type integer
);




CREATE SEQUENCE download_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;




ALTER SEQUENCE download_id_seq OWNED BY download.id;



CREATE TABLE download_type (
    id integer NOT NULL,
    name text,
    description text
);




CREATE SEQUENCE download_type_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;




ALTER SEQUENCE download_type_id_seq OWNED BY download_type.id;



CREATE TABLE enum (
    type text NOT NULL,
    name text NOT NULL,
    value text
);




CREATE TABLE forum (
    id integer NOT NULL,
    name text,
    "time" integer,
    forum_group integer,
    author text,
    moderators text,
    subscribers text,
    subject text,
    description text
);




CREATE TABLE forum_group (
    id integer NOT NULL,
    name text,
    description text
);




CREATE SEQUENCE forum_group_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;




ALTER SEQUENCE forum_group_id_seq OWNED BY forum_group.id;



CREATE SEQUENCE forum_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;




ALTER SEQUENCE forum_id_seq OWNED BY forum.id;



CREATE TABLE message (
    id integer NOT NULL,
    forum integer,
    topic integer,
    replyto integer,
    "time" integer,
    author text,
    body text
);




CREATE SEQUENCE message_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;




ALTER SEQUENCE message_id_seq OWNED BY message.id;



CREATE TABLE milestone (
    name text NOT NULL,
    due bigint,
    completed bigint,
    description text
);




CREATE TABLE node_change (
    repos integer NOT NULL,
    rev text NOT NULL,
    path text NOT NULL,
    node_type text,
    change_type text NOT NULL,
    base_path text,
    base_rev text
);




CREATE TABLE permission (
    username text NOT NULL,
    action text NOT NULL
);




CREATE TABLE platform (
    id integer NOT NULL,
    name text,
    description text
);




CREATE SEQUENCE platform_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;




ALTER SEQUENCE platform_id_seq OWNED BY platform.id;



CREATE TABLE report (
    id integer NOT NULL,
    author text,
    title text,
    query text,
    description text
);




CREATE SEQUENCE report_id_seq
    START WITH 9
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;




ALTER SEQUENCE report_id_seq OWNED BY report.id;



CREATE TABLE repository (
    id integer NOT NULL,
    name text NOT NULL,
    value text
);




CREATE TABLE revision (
    repos integer NOT NULL,
    rev text NOT NULL,
    "time" bigint,
    author text,
    message text
);




CREATE TABLE session (
    sid text NOT NULL,
    authenticated integer NOT NULL,
    last_visit integer
);




CREATE TABLE session_attribute (
    sid text NOT NULL,
    authenticated integer NOT NULL,
    name text NOT NULL,
    value text
);




CREATE TABLE system (
    name text NOT NULL,
    value text
);




CREATE TABLE ticket (
    id integer NOT NULL,
    type text,
    "time" bigint,
    changetime bigint,
    component text,
    severity text,
    priority text,
    owner text,
    reporter text,
    cc text,
    version text,
    milestone text,
    status text,
    resolution text,
    summary text,
    description text,
    keywords text
);




CREATE TABLE ticket_change (
    ticket integer NOT NULL,
    "time" bigint NOT NULL,
    author text,
    field text NOT NULL,
    oldvalue text,
    newvalue text
);




CREATE TABLE ticket_custom (
    ticket integer NOT NULL,
    name text NOT NULL,
    value text
);




CREATE SEQUENCE ticket_id_seq
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;




ALTER SEQUENCE ticket_id_seq OWNED BY ticket.id;



CREATE TABLE topic (
    id integer NOT NULL,
    forum integer,
    "time" integer,
    author text,
    subscribers text,
    subject text,
    body text
);




CREATE SEQUENCE topic_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;




ALTER SEQUENCE topic_id_seq OWNED BY topic.id;



CREATE TABLE version (
    name text NOT NULL,
    "time" bigint,
    description text
);




CREATE TABLE wiki (
    name text NOT NULL,
    version integer NOT NULL,
    "time" bigint,
    author text,
    ipnr text,
    text text,
    comment text,
    readonly integer
);




ALTER TABLE architecture ALTER COLUMN id SET DEFAULT nextval('architecture_id_seq'::regclass);



ALTER TABLE download ALTER COLUMN id SET DEFAULT nextval('download_id_seq'::regclass);



ALTER TABLE download_type ALTER COLUMN id SET DEFAULT nextval('download_type_id_seq'::regclass);



ALTER TABLE forum ALTER COLUMN id SET DEFAULT nextval('forum_id_seq'::regclass);



ALTER TABLE forum_group ALTER COLUMN id SET DEFAULT nextval('forum_group_id_seq'::regclass);



ALTER TABLE message ALTER COLUMN id SET DEFAULT nextval('message_id_seq'::regclass);



ALTER TABLE platform ALTER COLUMN id SET DEFAULT nextval('platform_id_seq'::regclass);



ALTER TABLE report ALTER COLUMN id SET DEFAULT nextval('report_id_seq'::regclass);



ALTER TABLE ticket ALTER COLUMN id SET DEFAULT nextval('ticket_id_seq'::regclass);



ALTER TABLE topic ALTER COLUMN id SET DEFAULT nextval('topic_id_seq'::regclass);



ALTER TABLE ONLY architecture
    ADD CONSTRAINT architecture_pkey PRIMARY KEY (id);



ALTER TABLE ONLY attachment
    ADD CONSTRAINT attachment_pk PRIMARY KEY (type, id, filename);



ALTER TABLE ONLY auth_cookie
    ADD CONSTRAINT auth_cookie_pk PRIMARY KEY (cookie, ipnr, name);



ALTER TABLE ONLY cache
    ADD CONSTRAINT cache_pkey PRIMARY KEY (id);



ALTER TABLE ONLY component
    ADD CONSTRAINT component_pkey PRIMARY KEY (name);



ALTER TABLE ONLY download
    ADD CONSTRAINT download_pkey PRIMARY KEY (id);



ALTER TABLE ONLY download_type
    ADD CONSTRAINT download_type_pkey PRIMARY KEY (id);



ALTER TABLE ONLY enum
    ADD CONSTRAINT enum_pk PRIMARY KEY (type, name);



ALTER TABLE ONLY forum_group
    ADD CONSTRAINT forum_group_pkey PRIMARY KEY (id);



ALTER TABLE ONLY forum
    ADD CONSTRAINT forum_pkey PRIMARY KEY (id);



ALTER TABLE ONLY message
    ADD CONSTRAINT message_pkey PRIMARY KEY (id);



ALTER TABLE ONLY milestone
    ADD CONSTRAINT milestone_pkey PRIMARY KEY (name);



ALTER TABLE ONLY node_change
    ADD CONSTRAINT node_change_pk PRIMARY KEY (repos, rev, path, change_type);



ALTER TABLE ONLY permission
    ADD CONSTRAINT permission_pk PRIMARY KEY (username, action);



ALTER TABLE ONLY platform
    ADD CONSTRAINT platform_pkey PRIMARY KEY (id);



ALTER TABLE ONLY report
    ADD CONSTRAINT report_pkey PRIMARY KEY (id);



ALTER TABLE ONLY repository
    ADD CONSTRAINT repository_pk PRIMARY KEY (id, name);



ALTER TABLE ONLY revision
    ADD CONSTRAINT revision_pk PRIMARY KEY (repos, rev);



ALTER TABLE ONLY session_attribute
    ADD CONSTRAINT session_attribute_pk PRIMARY KEY (sid, authenticated, name);



ALTER TABLE ONLY session
    ADD CONSTRAINT session_pk PRIMARY KEY (sid, authenticated);



ALTER TABLE ONLY system
    ADD CONSTRAINT system_pkey PRIMARY KEY (name);



ALTER TABLE ONLY ticket_change
    ADD CONSTRAINT ticket_change_pk PRIMARY KEY (ticket, "time", field);



ALTER TABLE ONLY ticket_custom
    ADD CONSTRAINT ticket_custom_pk PRIMARY KEY (ticket, name);



ALTER TABLE ONLY ticket
    ADD CONSTRAINT ticket_pkey PRIMARY KEY (id);



ALTER TABLE ONLY topic
    ADD CONSTRAINT topic_pkey PRIMARY KEY (id);



ALTER TABLE ONLY version
    ADD CONSTRAINT version_pkey PRIMARY KEY (name);



ALTER TABLE ONLY wiki
    ADD CONSTRAINT wiki_pk PRIMARY KEY (name, version);



CREATE INDEX node_change_repos_rev_idx ON node_change USING btree (repos, rev);



CREATE INDEX revision_repos_time_idx ON revision USING btree (repos, "time");



CREATE INDEX session_authenticated_idx ON session USING btree (authenticated);



CREATE INDEX session_last_visit_idx ON session USING btree (last_visit);



CREATE INDEX ticket_change_ticket_idx ON ticket_change USING btree (ticket);



CREATE INDEX ticket_change_time_idx ON ticket_change USING btree ("time");



CREATE INDEX ticket_status_idx ON ticket USING btree (status);



CREATE INDEX ticket_time_idx ON ticket USING btree ("time");



CREATE INDEX wiki_time_idx ON wiki USING btree ("time");



REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;

