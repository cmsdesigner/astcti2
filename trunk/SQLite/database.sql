CREATE TABLE services (
    "ID_SERVICE" INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
    "SERVICE_NAME" VARCHAR(100),
    "SERVICE_CONTEXT_TYPE" VARCHAR(10) DEFAULT ("INBOUND"),
    "SERVICE_IS_QUEUE" BOOLEAN,
    "SERVICE_QUEUE_NAME" VARCHAR(100),
    "SERVICE_TRIGGER_TYPE" VARCHAR(20) DEFAULT ("APPLICATION"),
    "ENABLED" BOOLEAN
);

CREATE TABLE services_variables(
ID_VARIABLE INTEGER PRIMARY KEY AUTOINCREMENT,
ID_SERVICE INTEGER NOT NULL,
VARNAME TEXT NOT NULL,
FOREIGN KEY (ID_SERVICE) REFERENCES services(ID_SERVICE)
);
CREATE TABLE sqlite_sequence(name,seq);
CREATE TABLE [operators] (
[ID_OPERATOR] INTEGER  PRIMARY KEY AUTOINCREMENT NULL,
[FULL_NAME] VARCHAR(100)  NOT NULL,
[USERNAME] VARCHAR(100)  UNIQUE NOT NULL,
[PASS_WORD] VARCHAR(32)  NOT NULL,
[LAST_SEAT] INTEGER  NULL,
[BEGIN_IN_PAUSE] BOOLEAN DEFAULT 'false' NULL,
[ENABLED] BOOLEAN DEFAULT 'true' NULL
);
;
CREATE TABLE SQLITEADMIN_QUERIES(ID INTEGER PRIMARY KEY,NAME VARCHAR(100),SQL TEXT);
CREATE TRIGGER [on_operator_delete] 
AFTER DELETE ON [operators] 
FOR EACH ROW 
BEGIN 

DELETE FROM services_operators WHERE ID_OPERATOR=OLD.ID_OPERATOR;

END;
CREATE TABLE [seats] (
[ID_SEAT] INTEGER  PRIMARY KEY AUTOINCREMENT NOT NULL,
[SEAT_MAC] VARCHAR(17) DEFAULT '00:00:00:00:00:00' UNIQUE NOT NULL,
[SEAT_EXTEN] VARCHAR(50)  UNIQUE NOT NULL,
[DESCRIPTION] VARCHAR(255)  NULL
);
;
;
CREATE TRIGGER [on_seats_delete] 
AFTER DELETE ON [seats] 
FOR EACH ROW 
BEGIN 

UPDATE operators SET LAST_SEAT=NULL WHERE LAST_SEAT=OLD.ID_SEAT;

END;
CREATE TABLE [services_operators] (
[ID_SERVICE] INTEGER  NOT NULL,
[ID_OPERATOR] INTEGER  NOT NULL,
[PENALTY] INTEGER DEFAULT '0' NULL
);
CREATE TRIGGER [on_seats_update]
AFTER UPDATE ON [seats]
FOR EACH ROW 
BEGIN 

UPDATE operators SET LAST_SEAT=NEW.ID_SEAT WHERE LAST_SEAT=OLD.ID_SEAT;

END;
CREATE TABLE [Applications] (
[ID_APPLICATION] INTEGER  NOT NULL PRIMARY KEY AUTOINCREMENT,
[ID_SERVICE] INTEGER  NOT NULL,
[APPLICATION_OS_TYPE] VARCHAR(3)  NOT NULL,
[APPLICATION_PATH] VARCHAR(255)  NOT NULL,
[PARAMETERS] TEXT  NULL
);
CREATE TABLE [OperatingSystemsEnum] (
[OS] VARCHAR(3)  UNIQUE NOT NULL
);
;
CREATE TRIGGER [check_osenum] 
BEFORE INSERT ON [Applications] 
FOR EACH ROW WHEN (SELECT 1 FROM OPERATINGSYSTEMSENUM WHERE OS = NEW.APPLICATION_OS_TYPE LIMIT 1) IS NULL 
BEGIN 

SELECT raise(rollback, 'foreign-key violation: Applications.APPLICATION_OS_TYPE');

END;
CREATE TRIGGER [check_osenum_update] 
BEFORE UPDATE ON [Applications] 
FOR EACH ROW WHEN (SELECT 1 FROM OPERATINGSYSTEMSENUM WHERE OS = NEW.APPLICATION_OS_TYPE LIMIT 1) IS NULL 
BEGIN 

SELECT raise(rollback, 'foreign-key violation: Applications.APPLICATION_OS_TYPE');

END;
CREATE TRIGGER fk_app2service_update
   BEFORE UPDATE ON Applications
   FOR EACH ROW
    WHEN (SELECT 1
             FROM Services
             WHERE ID_SERVICE = new.ID_SERVICE
             LIMIT 1) IS NULL
       BEGIN
         SELECT raise(rollback, 'foreign-key violation: Services.ID_SERVICE');
       END;
CREATE TRIGGER fk_app2service
   BEFORE INSERT ON Applications
   FOR EACH ROW
    WHEN (SELECT 1
             FROM Services
             WHERE ID_SERVICE = new.ID_SERVICE
             LIMIT 1) IS NULL
       BEGIN
         SELECT raise(rollback, 'foreign-key violation: Services.ID_SERVICE');
       END;
CREATE TABLE [ServicesTriggerTypeEnum] (
[TRIGGERTYPE] varCHAR(20)  UNIQUE NOT NULL
);
;
CREATE TABLE [ServiceContextTypeEnum] (
[CONTEXTTYPE] VARCHAR(20)  UNIQUE NOT NULL
);
;
CREATE TABLE "dbversion" (
    "VERSION" REAL
);
CREATE TRIGGER [on_operator_update] 
AFTER UPDATE OF ID_OPERATOR ON [operators] 
FOR EACH ROW 
BEGIN 

UPDATE services_operators SET ID_OPERATOR=NEW.ID_OPERATOR WHERE ID_OPERATOR=OLD.ID_OPERATOR;

END;
CREATE TRIGGER fk_services_update
   BEFORE UPDATE OF ID_SERVICE ON services_operators
   FOR EACH ROW
    WHEN (SELECT 1
             FROM Services
             WHERE ID_SERVICE = new.ID_SERVICE
             LIMIT 1) IS NULL
       BEGIN
         SELECT raise(rollback, 'foreign-key violation: Services.ID_SERVICE');
       END;
CREATE TRIGGER fk_operators_update
   BEFORE UPDATE OF ID_OPERATOR ON services_operators
   FOR EACH ROW
    WHEN (SELECT 1
             FROM Operators
             WHERE ID_OPERATOR= new.ID_OPERATOR
             LIMIT 1) IS NULL
       BEGIN
         SELECT raise(rollback, 'foreign-key violation: Services.ID_OPERATOR');
       END;
CREATE TRIGGER fk_operators
   BEFORE INSERT ON services_operators
   FOR EACH ROW
    WHEN (SELECT 1
             FROM operators
             WHERE ID_OPERATOR= new.ID_OPERATOR
             LIMIT 1) IS NULL
       BEGIN
         SELECT raise(rollback, 'foreign-key violation: Services.ID_OPERATOR');
       END;
CREATE TRIGGER fk_services
   BEFORE UPDATE ON services_operators
   FOR EACH ROW
    WHEN (SELECT 1
             FROM services
             WHERE ID_SERVICE = new.ID_SERVICE
             LIMIT 1) IS NULL
       BEGIN
         SELECT raise(rollback, 'foreign-key violation: Services.ID_SERVICE');
       END;
CREATE UNIQUE INDEX "unique_var_for_service" on services_variables (ID_SERVICE ASC, VARNAME ASC);
CREATE UNIQUE INDEX "unique_service_op" on services_operators (ID_SERVICE ASC, ID_OPERATOR ASC);
CREATE UNIQUE INDEX "unique_app_srvc" on applications (ID_SERVICE ASC, APPLICATION_OS_TYPE ASC);
CREATE UNIQUE INDEX "unique_username" on operators (USERNAME ASC);
CREATE UNIQUE INDEX "unique_mac_address" on seats (SEAT_MAC ASC);
CREATE UNIQUE INDEX "unique_seat_exten" on seats (SEAT_EXTEN ASC);

CREATE TABLE services_ID_SERVICE_seq (id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT, p4a CHAR);
