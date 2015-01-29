--
-- PostgreSQL database dump
--

SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

--
-- Name: SCHEMA public; Type: COMMENT; Schema: -; Owner: postgres
--

COMMENT ON SCHEMA public IS 'Standard public schema';


SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: cheque; Type: TABLE; Schema: public; Owner: vbravo; Tablespace: 
--

CREATE TABLE cheque (
    cheque_monto numeric(8,2),
    cheque_numero bigint NOT NULL,
    fecha date,
    solicitud_numero bigint
);


ALTER TABLE public.cheque OWNER TO vbravo;

--
-- Name: factura; Type: TABLE; Schema: public; Owner: vbravo; Tablespace: 
--

CREATE TABLE factura (
    factura_numero bigint NOT NULL,
    factura_monto numeric(8,2),
    factura_cedulaproveedor character(10)
);


ALTER TABLE public.factura OWNER TO vbravo;

--
-- Name: presupuesto; Type: TABLE; Schema: public; Owner: vbravo; Tablespace: 
--

CREATE TABLE presupuesto (
    presupuesto_tipo character(50),
    presupuesto_id bigint NOT NULL
);


ALTER TABLE public.presupuesto OWNER TO vbravo;

--
-- Name: proveedor; Type: TABLE; Schema: public; Owner: vbravo; Tablespace: 
--

CREATE TABLE proveedor (
    proveedor_cedula character(10) NOT NULL,
    apellidos character(150),
    nombres character(150),
    activo boolean,
    proveedor_ingreso date
);


ALTER TABLE public.proveedor OWNER TO vbravo;

--
-- Name: solicitud; Type: TABLE; Schema: public; Owner: vbravo; Tablespace: 
--

CREATE TABLE solicitud (
    solicitud_numero bigint NOT NULL,
    solicitud_consignado boolean,
    solicitud_numerofactura bigint,
    solicitud_tipo character(12) DEFAULT 'almacen'::bpchar,
    solicitud_presupuesto boolean,
    solicitud_gerencia boolean,
    solicitud_presupuestotipo bigint
);


ALTER TABLE public.solicitud OWNER TO vbravo;

--
-- Name: solicitudcheque; Type: TABLE; Schema: public; Owner: vbravo; Tablespace: 
--

CREATE TABLE solicitudcheque (
    relacion bigint NOT NULL,
    numero_cheque bigint,
    numero_solicitud bigint
);


ALTER TABLE public.solicitudcheque OWNER TO vbravo;

--
-- Data for Name: cheque; Type: TABLE DATA; Schema: public; Owner: vbravo
--

COPY cheque (cheque_monto, cheque_numero, fecha, solicitud_numero) FROM stdin;
200.00	10	2008-02-22	1
300.00	11	2008-02-22	2
720.00	12	2008-03-13	3
\.


--
-- Data for Name: factura; Type: TABLE DATA; Schema: public; Owner: vbravo
--

COPY factura (factura_numero, factura_monto, factura_cedulaproveedor) FROM stdin;
1	200.00	V12797664 
2	300.00	V12643114 
3	150.00	V4328524  
4	500.00	V12797664 
5	600.00	V12643114 
\.


--
-- Data for Name: presupuesto; Type: TABLE DATA; Schema: public; Owner: vbravo
--

COPY presupuesto (presupuesto_tipo, presupuesto_id) FROM stdin;
alterno                                           	2
externo                                           	3
corriente                                         	1
\.


--
-- Data for Name: proveedor; Type: TABLE DATA; Schema: public; Owner: vbravo
--

COPY proveedor (proveedor_cedula, apellidos, nombres, activo, proveedor_ingreso) FROM stdin;
V4328524  	Bravo A.                                                                                                                                              	Víctor A.                                                                                                                                             	f	2008-08-15
V12797664 	Bravo B.                                                                                                                                              	Victor R.                                                                                                                                             	t	2008-07-15
V12643114 	Solé                                                                                                                                                  	Solazver                                                                                                                                              	t	2008-07-15
\.


--
-- Data for Name: solicitud; Type: TABLE DATA; Schema: public; Owner: vbravo
--

COPY solicitud (solicitud_numero, solicitud_consignado, solicitud_numerofactura, solicitud_tipo, solicitud_presupuesto, solicitud_gerencia, solicitud_presupuestotipo) FROM stdin;
6	f	1	presupuesto 	t	f	1
1	t	1	presupuesto 	t	f	2
5	f	1	gerencia    	f	t	1
3	t	1	corriente   	t	f	1
4	t	2	almacen     	f	f	1
2	f	1	rrhh        	f	t	3
7	f	3	gerencia    	f	t	1
8	f	5	gerencia    	f	t	1
\.


--
-- Data for Name: solicitudcheque; Type: TABLE DATA; Schema: public; Owner: vbravo
--

COPY solicitudcheque (relacion, numero_cheque, numero_solicitud) FROM stdin;
1	10	1
2	10	5
3	10	6
\.


--
-- Name: cheque_pkey; Type: CONSTRAINT; Schema: public; Owner: vbravo; Tablespace: 
--

ALTER TABLE ONLY cheque
    ADD CONSTRAINT cheque_pkey PRIMARY KEY (cheque_numero);


--
-- Name: factura_pkey; Type: CONSTRAINT; Schema: public; Owner: vbravo; Tablespace: 
--

ALTER TABLE ONLY factura
    ADD CONSTRAINT factura_pkey PRIMARY KEY (factura_numero);


--
-- Name: presupuesto_pkey; Type: CONSTRAINT; Schema: public; Owner: vbravo; Tablespace: 
--

ALTER TABLE ONLY presupuesto
    ADD CONSTRAINT presupuesto_pkey PRIMARY KEY (presupuesto_id);


--
-- Name: proveedor_pkey; Type: CONSTRAINT; Schema: public; Owner: vbravo; Tablespace: 
--

ALTER TABLE ONLY proveedor
    ADD CONSTRAINT proveedor_pkey PRIMARY KEY (proveedor_cedula);


--
-- Name: solicitud_pkey; Type: CONSTRAINT; Schema: public; Owner: vbravo; Tablespace: 
--

ALTER TABLE ONLY solicitud
    ADD CONSTRAINT solicitud_pkey PRIMARY KEY (solicitud_numero);


--
-- Name: solicitudcheque_pkey; Type: CONSTRAINT; Schema: public; Owner: vbravo; Tablespace: 
--

ALTER TABLE ONLY solicitudcheque
    ADD CONSTRAINT solicitudcheque_pkey PRIMARY KEY (relacion);


--
-- Name: factura_factura_cedulaproveedor_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vbravo
--

ALTER TABLE ONLY factura
    ADD CONSTRAINT factura_factura_cedulaproveedor_fkey FOREIGN KEY (factura_cedulaproveedor) REFERENCES proveedor(proveedor_cedula);


--
-- Name: solicitud_solicitud_numerofactura_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vbravo
--

ALTER TABLE ONLY solicitud
    ADD CONSTRAINT solicitud_solicitud_numerofactura_fkey FOREIGN KEY (solicitud_numerofactura) REFERENCES factura(factura_numero);


--
-- Name: solicitud_solicitud_presupuestotipo_fkey; Type: FK CONSTRAINT; Schema: public; Owner: vbravo
--

ALTER TABLE ONLY solicitud
    ADD CONSTRAINT solicitud_solicitud_presupuestotipo_fkey FOREIGN KEY (solicitud_presupuestotipo) REFERENCES presupuesto(presupuesto_id);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

