//
// Created by my_fl on 2021-02-16.
//

#ifndef CHATTY_CHATTY_DEFS_H
#define CHATTY_CHATTY_DEFS_H

//PRE-BUILD CONFIGURE DEFINITIONS
#define CHATTY_VERSION "0.1.0.5"
#define CHATTY_DB_NAME "chatty_database_storage.db"

//DATABASE TABLE NAME DEFINITIONS
#define CHATTY_T_CONN "CHATTY_CONNECTION"
#define CHATTY_T_REGIN "CHATTY_REGISTER_INFO"
#define CHATTY_T_TOWER "CHATTY_TOWER_STATUS"

#define CHATTY_T_CREATION_QUERY_CONN "CREATE TABLE \"CHATTY_CONNECTION\" (\"idx\"	INTEGER,\"id\"	INTEGER NOT NULL UNIQUE,\"latest_connect_date\"	INTEGER NOT NULL DEFAULT 0,\"is_online\"	BLOB DEFAULT 0,PRIMARY KEY(\"idx\" AUTOINCREMENT));"
#define CHATTY_T_CREATION_QUERY_REGIN "CREATE TABLE \"CHATTY_REGITER_INFO\" (\"chatty_id\"	INTEGER,\"user_id\"	TEXT NOT NULL UNIQUE,\"is_unsign\"	BLOB DEFAULT 0,\"is_activated\"	BLOB DEFAULT 0,\"reg_date\"	INTEGER DEFAULT 0,PRIMARY KEY(\"chatty_id\" AUTOINCREMENT));"
#define CHATTY_T_CREATION_QUERY_TOWER "CREATE TABLE \"CHATTY_TOWER_STATUS\" (\"idx\"	INTEGER,\"hash\"	TEXT NOT NULL UNIQUE,\"id\"	INTEGER NOT NULL,\"is_open\"	BLOB DEFAULT 0,\"reg_date\"	INTEGER DEFAULT 0,PRIMARY KEY(\"idx\" AUTOINCREMENT));"
/**
 * TITLE    : SQLITE3 ERROR-CODE 에서 가져옴
 * DATE     : 2021. 02. 22
 * */
#define CHATTY_STATUS_OK           0   /* Successful result */
/* beginning-of-error-codes */
#define CHATTY_STATUS_ERROR        1   /* Generic error */
#define CHATTY_STATUS_INTERNAL     2   /* Internal logic error in SQLite */
#define CHATTY_STATUS_PERM         3   /* Access permission denied */
#define CHATTY_STATUS_ABORT        4   /* Callback routine requested an abort */
#define CHATTY_STATUS_BUSY         5   /* The database file is locked */
#define CHATTY_STATUS_LOCKED       6   /* A table in the database is locked */
#define CHATTY_STATUS_NOMEM        7   /* A malloc() failed */
#define CHATTY_STATUS_READONLY     8   /* Attempt to write a readonly database */
#define CHATTY_STATUS_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define CHATTY_STATUS_IOERR       10   /* Some kind of disk I/O error occurred */
#define CHATTY_STATUS_CORRUPT     11   /* The database disk image is malformed */
#define CHATTY_STATUS_NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
#define CHATTY_STATUS_FULL        13   /* Insertion failed because database is full */
#define CHATTY_STATUS_CANTOPEN    14   /* Unable to open the database file */
#define CHATTY_STATUS_PROTOCOL    15   /* Database lock protocol error */
#define CHATTY_STATUS_EMPTY       16   /* Internal use only */
#define CHATTY_STATUS_SCHEMA      17   /* The database schema changed */
#define CHATTY_STATUS_TOOBIG      18   /* String or BLOB exceeds size limit */
#define CHATTY_STATUS_CONSTRAINT  19   /* Abort due to constraint violation */
#define CHATTY_STATUS_MISMATCH    20   /* Data type mismatch */
#define CHATTY_STATUS_MISUSE      21   /* Library used incorrectly */
#define CHATTY_STATUS_NOLFS       22   /* Uses OS features not supported on host */
#define CHATTY_STATUS_AUTH        23   /* Authorization denied */
#define CHATTY_STATUS_FORMAT      24   /* Not used */
#define CHATTY_STATUS_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define CHATTY_STATUS_NOTADB      26   /* File opened that is not a database file */
#define CHATTY_STATUS_NOTICE      27   /* Notifications from sqlite3_log() */
#define CHATTY_STATUS_WARNING     28   /* Warnings from sqlite3_log() */
#define CHATTY_STATUS_ROW         100  /* sqlite3_step() has another row ready */
#define CHATTY_STATUS_DONE        101  /* sqlite3_step() has finished executing */


#endif //CHATTY_CHATTY_DEFS_H
