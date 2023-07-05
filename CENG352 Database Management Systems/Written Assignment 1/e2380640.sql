--
-- PostgreSQL database cluster dump
--

-- Started on 2023-04-20 01:52:36

SET default_transaction_read_only = off;

SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;

--
-- Roles
--

CREATE ROLE postgres;
ALTER ROLE postgres WITH SUPERUSER INHERIT CREATEROLE CREATEDB LOGIN REPLICATION BYPASSRLS;

--
-- User Configurations
--








--
-- Databases
--

--
-- Database "template1" dump
--

\connect template1

--
-- PostgreSQL database dump
--

-- Dumped from database version 15.2
-- Dumped by pg_dump version 15.2

-- Started on 2023-04-20 01:52:36

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

-- Completed on 2023-04-20 01:52:36

--
-- PostgreSQL database dump complete
--

--
-- Database "db1" dump
--

--
-- PostgreSQL database dump
--

-- Dumped from database version 15.2
-- Dumped by pg_dump version 15.2

-- Started on 2023-04-20 01:52:36

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
-- TOC entry 3338 (class 1262 OID 16398)
-- Name: db1; Type: DATABASE; Schema: -; Owner: postgres
--

CREATE DATABASE db1 WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'English_United States.1252';


ALTER DATABASE db1 OWNER TO postgres;

\connect db1

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

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 214 (class 1259 OID 17976)
-- Name: r1; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.r1 (
    a character varying(50) NOT NULL,
    d character varying(2)
);


ALTER TABLE public.r1 OWNER TO postgres;

--
-- TOC entry 215 (class 1259 OID 17981)
-- Name: r2; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.r2 (
    c character varying(5) NOT NULL,
    b character varying(10)
);


ALTER TABLE public.r2 OWNER TO postgres;

--
-- TOC entry 216 (class 1259 OID 17986)
-- Name: r3; Type: TABLE; Schema: public; Owner: postgres
--

CREATE TABLE public.r3 (
    a character varying(50) NOT NULL,
    c character varying(5) NOT NULL
);


ALTER TABLE public.r3 OWNER TO postgres;

--
-- TOC entry 3330 (class 0 OID 17976)
-- Dependencies: 214
-- Data for Name: r1; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.r1 (a, d) FROM stdin;
mouse5	89
bar7	29
bar1	19
gizmo1	49
click2	39
bar5	89
gizmo4	29
click5	99
gizmo3	19
bar4	29
gizmo2	99
gizmo9	79
gizmo6	29
bar2	59
mouse1	19
click1	39
mouse8	59
mouse6	99
bar8	19
click6	89
mouse7	29
bar6	99
gizmo5	79
mouse4	69
click4	49
gizmo7	19
bar9	39
gizmo8	89
click7	29
mouse2	49
click9	49
mouse9	69
click8	39
bar3	59
click3	39
mouse3	29
\.


--
-- TOC entry 3331 (class 0 OID 17981)
-- Dependencies: 215
-- Data for Name: r2; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.r2 (c, b) FROM stdin;
may	10%
jan	33%
jun	10%
dec	33%
aug	15%
apr	15%
mar	15%
feb	10%
sep	15%
oct	10%
jul	33%
nov	15%
\.


--
-- TOC entry 3332 (class 0 OID 17986)
-- Dependencies: 216
-- Data for Name: r3; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY public.r3 (a, c) FROM stdin;
bar3	jan
click4	nov
mouse1	sep
click8	jan
mouse2	nov
click6	nov
gizmo3	oct
bar3	apr
mouse7	jul
bar9	sep
click6	jun
mouse3	jul
gizmo9	apr
mouse6	sep
click3	may
mouse3	mar
mouse6	feb
gizmo9	nov
bar1	jul
gizmo1	apr
click3	dec
bar4	dec
bar7	may
click6	dec
gizmo4	jun
mouse8	apr
gizmo3	aug
click2	jul
bar6	jul
click8	oct
gizmo4	apr
mouse9	jul
mouse7	dec
gizmo2	feb
click1	mar
bar1	mar
bar1	aug
gizmo4	feb
click4	may
bar7	mar
gizmo6	jan
mouse7	mar
bar8	aug
click6	jan
mouse9	may
mouse6	dec
gizmo1	mar
click2	mar
click7	aug
mouse4	sep
mouse6	aug
mouse9	feb
click8	jun
click2	dec
bar5	oct
gizmo2	dec
gizmo4	nov
click3	feb
click8	apr
bar6	nov
gizmo8	may
gizmo7	aug
bar9	mar
bar6	aug
mouse4	feb
click5	aug
mouse9	nov
mouse9	sep
click3	jan
mouse8	jul
click4	mar
click2	jun
gizmo6	oct
mouse5	feb
gizmo4	aug
bar1	nov
bar3	nov
gizmo9	dec
click4	apr
gizmo7	may
gizmo8	mar
bar8	apr
bar2	may
bar4	jun
click9	oct
click9	sep
click2	jan
mouse6	apr
bar1	feb
bar6	oct
gizmo5	mar
click1	jun
mouse4	aug
click5	jun
gizmo1	sep
click8	mar
bar2	dec
bar7	sep
click6	sep
click2	sep
bar8	feb
gizmo6	apr
bar7	nov
mouse1	jun
mouse8	may
bar3	jul
gizmo1	dec
bar3	may
mouse3	may
click7	jan
bar2	aug
click3	oct
mouse3	oct
mouse1	oct
mouse4	oct
click8	sep
click7	may
gizmo5	nov
click3	nov
bar2	jun
gizmo7	mar
mouse7	jun
gizmo7	jun
mouse2	feb
gizmo2	aug
click3	aug
click5	sep
click6	apr
bar6	may
click4	jan
mouse5	jul
bar6	jun
mouse4	dec
click5	jan
click4	feb
gizmo8	jan
mouse9	jun
bar3	feb
bar3	aug
click7	feb
bar7	aug
click6	aug
click1	jan
click6	oct
bar1	oct
bar8	jun
mouse3	feb
bar9	dec
gizmo6	dec
gizmo2	may
gizmo9	jun
bar1	dec
click7	dec
bar5	jun
click7	nov
gizmo5	oct
gizmo3	jan
bar2	mar
gizmo5	may
click1	nov
click9	jun
mouse1	jul
mouse8	aug
gizmo9	aug
mouse9	jan
gizmo4	mar
mouse1	nov
gizmo2	apr
mouse1	mar
bar9	oct
click4	sep
mouse5	oct
gizmo9	feb
click4	dec
gizmo8	aug
bar4	apr
bar4	oct
mouse8	nov
mouse2	sep
bar8	jul
click5	mar
mouse8	mar
gizmo1	oct
mouse3	apr
mouse3	sep
bar8	sep
gizmo7	jul
mouse8	oct
click9	nov
mouse8	feb
bar2	sep
mouse6	may
click7	sep
mouse9	dec
bar7	feb
gizmo6	jul
click3	mar
gizmo7	oct
bar8	oct
bar6	jan
click4	jul
mouse5	nov
gizmo1	jan
gizmo1	may
bar2	feb
mouse5	dec
mouse4	jan
click9	mar
gizmo2	jul
bar9	jun
click1	may
gizmo8	jul
click8	dec
gizmo5	sep
click8	feb
bar4	jan
mouse8	jun
click6	jul
mouse8	dec
mouse8	sep
click9	feb
mouse4	may
click5	nov
gizmo3	feb
gizmo5	aug
bar2	nov
mouse7	oct
bar5	jul
gizmo5	jan
bar4	aug
click2	nov
bar5	aug
gizmo3	dec
bar5	nov
bar9	feb
click9	jul
click7	jul
bar2	jan
mouse9	mar
gizmo3	sep
gizmo8	jun
click4	oct
gizmo7	jan
bar7	oct
bar9	may
bar2	oct
click7	jun
gizmo6	feb
mouse7	may
click9	jan
bar7	jun
gizmo6	aug
click5	may
gizmo9	mar
click2	may
gizmo2	jan
mouse1	feb
click1	feb
mouse9	aug
gizmo6	nov
gizmo6	jun
gizmo4	sep
bar4	nov
bar6	feb
bar7	dec
bar2	jul
gizmo9	oct
gizmo5	feb
gizmo1	nov
click3	jun
gizmo8	dec
click4	jun
bar5	feb
bar6	dec
gizmo8	nov
mouse4	jul
click1	sep
gizmo9	jul
click6	mar
gizmo4	oct
click4	aug
click2	aug
gizmo6	mar
bar8	may
click8	jul
mouse6	nov
gizmo8	sep
click3	apr
gizmo9	may
mouse4	nov
click8	nov
gizmo4	jan
gizmo8	feb
gizmo2	nov
gizmo4	jul
gizmo8	apr
click7	apr
gizmo1	jul
mouse6	jan
bar7	apr
gizmo3	apr
gizmo2	jun
bar8	mar
mouse2	mar
mouse5	jan
mouse2	jun
gizmo1	jun
mouse7	feb
bar1	jun
gizmo1	aug
gizmo9	jan
bar3	sep
click6	may
gizmo3	mar
click2	apr
bar8	dec
click8	may
bar1	may
click5	oct
mouse6	oct
click9	dec
gizmo2	sep
mouse2	aug
gizmo9	sep
bar9	jan
gizmo4	dec
gizmo2	oct
bar6	apr
mouse1	may
mouse5	aug
gizmo6	may
bar1	sep
bar4	jul
mouse5	sep
click2	oct
bar9	nov
mouse7	apr
gizmo3	may
mouse2	may
click8	aug
mouse4	apr
gizmo3	nov
gizmo3	jun
mouse2	apr
gizmo4	may
click1	apr
mouse3	jan
mouse1	aug
click5	jul
click9	may
gizmo5	jul
bar5	dec
mouse7	sep
bar5	may
click5	dec
mouse2	jan
bar9	jul
gizmo6	sep
bar6	mar
mouse9	apr
bar8	nov
mouse3	nov
bar3	oct
bar3	dec
bar7	jul
bar8	jan
mouse5	jun
bar5	mar
bar4	sep
gizmo5	apr
gizmo2	mar
bar1	apr
mouse7	nov
mouse7	jan
mouse3	aug
mouse6	jul
click1	jul
bar9	apr
mouse5	may
bar7	jan
mouse4	jun
click5	apr
bar5	jan
gizmo7	sep
gizmo1	feb
gizmo7	nov
gizmo3	jul
click1	dec
bar3	mar
bar5	apr
bar9	aug
mouse3	dec
click2	feb
click7	mar
bar4	mar
click6	feb
gizmo7	apr
gizmo8	oct
click9	aug
mouse3	jun
bar3	jun
mouse8	jan
bar5	sep
bar1	jan
gizmo7	dec
bar6	sep
mouse5	apr
mouse2	oct
bar4	may
bar2	apr
mouse6	mar
mouse5	mar
gizmo5	jun
click7	oct
click1	oct
mouse6	jun
mouse7	aug
mouse2	dec
click3	sep
mouse4	mar
mouse2	jul
mouse1	dec
mouse9	oct
mouse1	apr
click5	feb
gizmo7	feb
\.


--
-- TOC entry 3181 (class 2606 OID 17980)
-- Name: r1 r1_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.r1
    ADD CONSTRAINT r1_pkey PRIMARY KEY (a);


--
-- TOC entry 3183 (class 2606 OID 17985)
-- Name: r2 r2_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.r2
    ADD CONSTRAINT r2_pkey PRIMARY KEY (c);


--
-- TOC entry 3185 (class 2606 OID 17990)
-- Name: r3 r3_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.r3
    ADD CONSTRAINT r3_pkey PRIMARY KEY (a, c);


--
-- TOC entry 3186 (class 2606 OID 17991)
-- Name: r3 r3_a_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.r3
    ADD CONSTRAINT r3_a_fkey FOREIGN KEY (a) REFERENCES public.r1(a);


--
-- TOC entry 3187 (class 2606 OID 17996)
-- Name: r3 r3_c_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY public.r3
    ADD CONSTRAINT r3_c_fkey FOREIGN KEY (c) REFERENCES public.r2(c);


-- Completed on 2023-04-20 01:52:36

--
-- PostgreSQL database dump complete
--

--
-- Database "postgres" dump
--

\connect postgres

--
-- PostgreSQL database dump
--

-- Dumped from database version 15.2
-- Dumped by pg_dump version 15.2

-- Started on 2023-04-20 01:52:36

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
-- TOC entry 2 (class 3079 OID 16384)
-- Name: adminpack; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS adminpack WITH SCHEMA pg_catalog;


--
-- TOC entry 3316 (class 0 OID 0)
-- Dependencies: 2
-- Name: EXTENSION adminpack; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION adminpack IS 'administrative functions for PostgreSQL';


-- Completed on 2023-04-20 01:52:36

--
-- PostgreSQL database dump complete
--

-- Completed on 2023-04-20 01:52:36

--
-- PostgreSQL database cluster dump complete
--

