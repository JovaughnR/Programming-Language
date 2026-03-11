#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

#include "./lib/db.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/dict.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo DB_MODULE[] = {
    {MODULE_DB, DB_OPEN, "__db_open__", 1, 1},
    {MODULE_DB, DB_CLOSE, "__db_close__", 1, 1},
    {MODULE_DB, DB_EXEC, "__db_exec__", 2, 2},
    {MODULE_DB, DB_QUERY, "__db_query__", 2, 3},
    {MODULE_DB, DB_PREPARE, "__db_prepare__", 2, 2},
    {MODULE_DB, DB_BIND, "__db_bind__", 3, 3},
    {MODULE_DB, DB_STEP, "__db_step__", 1, 1},
    {MODULE_DB, DB_FINALIZE, "__db_finalize__", 1, 1},
    {MODULE_DB, DB_LASTID, "__db_lastid__", 1, 1},
    {MODULE_DB, DB_CHANGES, "__db_changes__", 1, 1},
    {MODULE_DB, DB_BEGIN, "__db_begin__", 1, 1},
    {MODULE_DB, DB_COMMIT, "__db_commit__", 1, 1},
    {MODULE_DB, DB_ROLLBACK, "__db_rollback__", 1, 1},
    {MODULE_DB, DB_TABLES, "__db_tables__", 1, 1},
    {MODULE_DB, DB_SCHEMA, "__db_schema__", 2, 2},
};

const int DB_MODULE_COUNT = sizeof(DB_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  TYPE_DB and TYPE_STMT — stored as TYPE_ANY with tag
//=========================================================

// We store sqlite3* and sqlite3_stmt* as opaque pointers
// wrapped in a Data with a special type tag

static const char *get_str(Data *d, const char *funcname)
{
   if (!d || d->type == TYPE_NONE || d->type != TYPE_STR)
   {
      throw_error(ERROR_TYPE, "%s() argument must be a string", funcname);
      return NULL;
   }
   return d->str;
}

static sqlite3 *get_db(Data *d, const char *funcname)
{
   if (!d || d->type != TYPE_DB || !d->ref || !d->ref->object)
   {
      throw_error(ERROR_TYPE, "%s() argument must be a database handle", funcname);
      return NULL;
   }
   return (sqlite3 *)d->ref->object;
}

static sqlite3_stmt *get_stmt(Data *d, const char *funcname)
{
   if (!d || d->type != TYPE_STMT || !d->any)
   {
      throw_error(ERROR_TYPE, "%s() argument must be a prepared statement", funcname);
      return NULL;
   }
   return (sqlite3_stmt *)d->ref->object;
}

// Convert a sqlite3 row column to Data
static Data *column_to_data(sqlite3_stmt *stmt, int col)
{
   switch (sqlite3_column_type(stmt, col))
   {
   case SQLITE_INTEGER:
   {
      int v = sqlite3_column_int(stmt, col);
      return createData(TYPE_INT, &v);
   }
   case SQLITE_FLOAT:
   {
      double v = sqlite3_column_double(stmt, col);
      return createData(TYPE_FLOAT, &v);
   }
   case SQLITE_TEXT:
   {
      const char *v = (const char *)sqlite3_column_text(stmt, col);
      return createData(TYPE_STR, strdup(v ? v : ""));
   }
   case SQLITE_NULL:
      return createData(TYPE_NONE, NULL);
   case SQLITE_BLOB:
   {
      // Return blob as string for now
      const void *blob = sqlite3_column_blob(stmt, col);
      int size = sqlite3_column_bytes(stmt, col);
      char *s = malloc(size + 1);
      memcpy(s, blob, size);
      s[size] = '\0';
      return createData(TYPE_STR, s);
   }
   default:
      return createData(TYPE_NONE, NULL);
   }
}

// Bind a list of params to a prepared statement
static int bind_params(sqlite3_stmt *stmt, Data *params)
{
   if (!params || params->type == TYPE_NONE)
      return 1;

   if (params->type != TYPE_LIST)
   {
      throw_error(ERROR_TYPE, "db query params must be a list");
      return 0;
   }

   List *list = LIST_PTR(params);
   for (int i = 0; i < list->length; i++)
   {
      Data *item = (Data *)list->items[i];
      int idx = i + 1; // sqlite bind index is 1-based

      if (!item || item->type == TYPE_NONE)
         sqlite3_bind_null(stmt, idx);
      else if (item->type == TYPE_INT)
         sqlite3_bind_int(stmt, idx, *(int *)item->atom);
      else if (item->type == TYPE_FLOAT)
         sqlite3_bind_double(stmt, idx, *(double *)item->real);
      else if (item->type == TYPE_BOOL)
         sqlite3_bind_int(stmt, idx, *(int *)item->atom);
      else if (item->type == TYPE_STR)
         sqlite3_bind_text(stmt, idx, item->str, -1, SQLITE_STATIC);
      else
      {
         throw_error(ERROR_TYPE, "db: unsupported param type at index %d", i);
         return 0;
      }
   }
   return 1;
}

//=========================================================
//  Implementations
//=========================================================

// open(path) -> db
// Opens a SQLite database file. Creates it if it doesn't exist.
// Use ":memory:" for an in-memory database.
// Example: db = db.open("myapp.db")
// Example: db = db.open(":memory:")
Data *__db_open__(Data *path)
{
   const char *p = get_str(path, "db.open");
   if (!p)
      return createData(TYPE_NONE, NULL);

   sqlite3 *db;
   int rc = sqlite3_open(p, &db);

   if (rc != SQLITE_OK)
   {
      throw_error(ERROR_RUNTIME, "db.open(): %s", sqlite3_errmsg(db));
      sqlite3_close(db);
      return createData(TYPE_NONE, NULL);
   }

   sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);
   sqlite3_exec(db, "PRAGMA foreign_keys=ON;", NULL, NULL, NULL);

   return createData(TYPE_DB, db);
}

// close(db) -> none
// Closes a database connection.
// Example: db.close(conn)
Data *__db_close__(Data *db_data)
{
   sqlite3 *db = get_db(db_data, "db.close");
   if (!db)
      return createData(TYPE_NONE, NULL);

   sqlite3_close(db);
   db_data->any = NULL;
   return createData(TYPE_NONE, NULL);
}

// exec(db, sql) -> bool
// Executes a SQL statement that returns no results.
// Use for CREATE TABLE, INSERT, UPDATE, DELETE, DROP.
// Example: db.exec(conn, "CREATE TABLE users (id INTEGER PRIMARY KEY, name TEXT)")
// Example: db.exec(conn, "INSERT INTO users VALUES (1, 'Alice')")
Data *__db_exec__(Data *db_data, Data *sql)
{
   sqlite3 *db = get_db(db_data, "db.exec");
   const char *s = get_str(sql, "db.exec");
   if (!db || !s)
      return createData(TYPE_BOOL, &(int){0});

   char *errmsg = NULL;
   int rc = sqlite3_exec(db, s, NULL, NULL, &errmsg);

   if (rc != SQLITE_OK)
   {
      throw_error(ERROR_RUNTIME, "db.exec(): %s", errmsg);
      sqlite3_free(errmsg);
      return createData(TYPE_BOOL, &(int){0});
   }

   return createData(TYPE_BOOL, &(int){1});
}

// query(db, sql, params=[]) -> list
// Executes a SELECT and returns all rows as a list of dicts.
// Use ? placeholders for parameters to prevent SQL injection.
// Example: rows = db.query(conn, "SELECT * FROM users")
// Example: rows = db.query(conn, "SELECT * FROM users WHERE id = ?", [1])
// Example: row = rows[0]; print(row["name"])
Data *__db_query__(Data *db_data, Data *sql, Data *params)
{
   sqlite3 *db = get_db(db_data, "db.query");
   const char *s = get_str(sql, "db.query");
   if (!db || !s)
      return createData(TYPE_NONE, NULL);

   sqlite3_stmt *stmt;
   int rc = sqlite3_prepare_v2(db, s, -1, &stmt, NULL);
   if (rc != SQLITE_OK)
   {
      throw_error(ERROR_RUNTIME, "db.query(): %s", sqlite3_errmsg(db));
      return createData(TYPE_NONE, NULL);
   }

   if (!bind_params(stmt, params))
   {
      sqlite3_finalize(stmt);
      return createData(TYPE_NONE, NULL);
   }

   List *rows = list_create(__size__);
   int ncols = sqlite3_column_count(stmt);

   while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
   {
      Dict *row = dict_create(__size__);

      for (int i = 0; i < ncols; i++)
      {
         const char *col_name = sqlite3_column_name(stmt, i);
         Data *key = createData(TYPE_STR, strdup(col_name));
         Data *val = column_to_data(stmt, i);
         dict_insert(key, val, row);
      }

      list_append(createData(TYPE_DICT, row), rows);
   }

   if (rc != SQLITE_DONE)
      throw_error(ERROR_RUNTIME, "db.query(): %s", sqlite3_errmsg(db));

   sqlite3_finalize(stmt);
   return createData(TYPE_LIST, rows);
}

// prepare(db, sql) -> stmt
// Prepares a SQL statement for repeated execution.
// More efficient than query() when running the same SQL many times.
// Example: stmt = db.prepare(conn, "INSERT INTO users (name, age) VALUES (?, ?)")
// Example: db.bind(stmt, [name, age]); db.step(stmt); db.finalize(stmt)
Data *__db_prepare__(Data *db_data, Data *sql)
{
   sqlite3 *db = get_db(db_data, "db.prepare");
   const char *s = get_str(sql, "db.prepare");
   if (!db || !s)
      return createData(TYPE_NONE, NULL);

   sqlite3_stmt *stmt;
   int rc = sqlite3_prepare_v2(db, s, -1, &stmt, NULL);
   if (rc != SQLITE_OK)
   {
      throw_error(ERROR_RUNTIME, "db.prepare(): %s", sqlite3_errmsg(db));
      return createData(TYPE_NONE, NULL);
   }

   return createData(TYPE_STMT, stmt);
}

// bind(stmt, params) -> bool
// Binds parameters to a prepared statement.
// Call before each step() when reusing a prepared statement.
// Example: db.bind(stmt, ["Alice", 30])
Data *__db_bind__(Data *stmt_data, Data *params, Data *unused)
{
   UNUSED(unused);

   if (!stmt_data || stmt_data->type != TYPE_STMT ||
       !stmt_data->ref || !stmt_data->ref->object)
   {
      throw_error(ERROR_RUNTIME, "db.bind(): invalid or finalized statement");
      return createData(TYPE_BOOL, &(int){0});
   }

   sqlite3_stmt *stmt = (sqlite3_stmt *)stmt_data->ref->object;

   sqlite3_reset(stmt);
   sqlite3_clear_bindings(stmt);

   int result = bind_params(stmt, params);
   return createData(TYPE_BOOL, &result);
}

// step(stmt) -> dict | none
// Executes one step of a prepared statement.
// Returns the next row as a dict, or none when done.
// Example: row = db.step(stmt)
// Example: while (row != none) { print(row); row = db.step(stmt); }
Data *__db_step__(Data *stmt_data)
{
   if (!stmt_data || stmt_data->type != TYPE_STMT ||
       !stmt_data->ref || !stmt_data->ref->object)
   {
      throw_error(ERROR_RUNTIME, "db.step(): invalid or finalized statement");
      return createData(TYPE_NONE, NULL);
   }

   sqlite3_stmt *stmt = get_stmt(stmt_data, "db.step");
   if (!stmt)
      return createData(TYPE_NONE, NULL);

   int rc = sqlite3_step(stmt);

   if (rc == SQLITE_ROW)
   {
      int ncols = sqlite3_column_count(stmt);
      Dict *row = dict_create(__size__);

      for (int i = 0; i < ncols; i++)
      {
         const char *col_name = sqlite3_column_name(stmt, i);
         Data *key = createData(TYPE_STR, strdup(col_name));
         Data *val = column_to_data(stmt, i);
         dict_insert(key, val, row);
      }

      return createData(TYPE_DICT, row);
   }

   if (rc != SQLITE_DONE)
      throw_error(ERROR_RUNTIME, "db.step(): %s",
                  sqlite3_errmsg(sqlite3_db_handle(stmt)));

   return createData(TYPE_NONE, NULL);
}

// finalize(stmt) -> none
// Releases a prepared statement's resources.
// Always call after you're done with a prepared statement.
// Example: db.finalize(stmt)
Data *__db_finalize__(Data *stmt_data)
{
   sqlite3_stmt *stmt = get_stmt(stmt_data, "db.finalize");
   if (!stmt)
      return createData(TYPE_NONE, NULL);

   sqlite3_finalize(stmt);
   stmt_data->any = NULL;
   return createData(TYPE_NONE, NULL);
}

// lastid(db) -> int
// Returns the rowid of the last successful INSERT.
// Example: id = db.lastid(conn)
Data *__db_lastid__(Data *db_data)
{
   sqlite3 *db = get_db(db_data, "db.lastid");
   if (!db)
      return createData(TYPE_INT, &(int){0});

   int id = (int)sqlite3_last_insert_rowid(db);
   return createData(TYPE_INT, &id);
}

// changes(db) -> int
// Returns the number of rows changed by the last INSERT/UPDATE/DELETE.
// Example: n = db.changes(conn)
Data *__db_changes__(Data *db_data)
{
   sqlite3 *db = get_db(db_data, "db.changes");
   if (!db)
      return createData(TYPE_INT, &(int){0});

   int n = sqlite3_changes(db);
   return createData(TYPE_INT, &n);
}

// begin(db) -> bool
// Begins a transaction. Use with commit() and rollback().
// Example: db.begin(conn)
Data *__db_begin__(Data *db_data)
{
   sqlite3 *db = get_db(db_data, "db.begin");
   if (!db)
      return createData(TYPE_BOOL, &(int){0});

   char *errmsg = NULL;
   int rc = sqlite3_exec(db, "BEGIN;", NULL, NULL, &errmsg);
   if (rc != SQLITE_OK)
   {
      throw_error(ERROR_RUNTIME, "db.begin(): %s", errmsg);
      sqlite3_free(errmsg);
      return createData(TYPE_BOOL, &(int){0});
   }
   return createData(TYPE_BOOL, &(int){1});
}

// commit(db) -> bool
// Commits the current transaction.
// Example: db.commit(conn)
Data *__db_commit__(Data *db_data)
{
   sqlite3 *db = get_db(db_data, "db.commit");
   if (!db)
      return createData(TYPE_BOOL, &(int){0});

   char *errmsg = NULL;
   int rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, &errmsg);
   if (rc != SQLITE_OK)
   {
      throw_error(ERROR_RUNTIME, "db.commit(): %s", errmsg);
      sqlite3_free(errmsg);
      return createData(TYPE_BOOL, &(int){0});
   }
   return createData(TYPE_BOOL, &(int){1});
}

// rollback(db) -> bool
// Rolls back the current transaction.
// Example: db.rollback(conn)
Data *__db_rollback__(Data *db_data)
{
   sqlite3 *db = get_db(db_data, "db.rollback");
   if (!db)
      return createData(TYPE_BOOL, &(int){0});

   char *errmsg = NULL;
   int rc = sqlite3_exec(db, "ROLLBACK;", NULL, NULL, &errmsg);
   if (rc != SQLITE_OK)
   {
      throw_error(ERROR_RUNTIME, "db.rollback(): %s", errmsg);
      sqlite3_free(errmsg);
      return createData(TYPE_BOOL, &(int){0});
   }
   return createData(TYPE_BOOL, &(int){1});
}

// tables(db) -> list
// Returns a list of all table names in the database.
// Example: tables = db.tables(conn)
// Example: print(tables)  -> ["users", "posts", "comments"]
Data *__db_tables__(Data *db_data)
{
   sqlite3 *db = get_db(db_data, "db.tables");
   if (!db)
      return createData(TYPE_NONE, NULL);

   sqlite3_stmt *stmt;
   const char *sql = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";
   int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   if (rc != SQLITE_OK)
   {
      throw_error(ERROR_RUNTIME, "db.tables(): %s", sqlite3_errmsg(db));
      return createData(TYPE_NONE, NULL);
   }

   List *result = list_create(__size__);
   while (sqlite3_step(stmt) == SQLITE_ROW)
   {
      const char *name = (const char *)sqlite3_column_text(stmt, 0);
      list_append(createData(TYPE_STR, strdup(name)), result);
   }

   sqlite3_finalize(stmt);
   return createData(TYPE_LIST, result);
}

// schema(db, table) -> list
// Returns the schema of a table as a list of column dicts.
// Each dict has: {name, type, notnull, default, pk}
// Example: cols = db.schema(conn, "users")
// Example: for col in cols { print(col["name"], col["type"]); }
Data *__db_schema__(Data *db_data, Data *table)
{
   sqlite3 *db = get_db(db_data, "db.schema");
   const char *t = get_str(table, "db.schema");
   if (!db || !t)
      return createData(TYPE_NONE, NULL);

   char sql[256];
   snprintf(sql, sizeof(sql), "PRAGMA table_info(%s);", t);

   sqlite3_stmt *stmt;
   int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
   if (rc != SQLITE_OK)
   {
      throw_error(ERROR_RUNTIME, "db.schema(): %s", sqlite3_errmsg(db));
      return createData(TYPE_NONE, NULL);
   }

   List *result = list_create(__size__);
   while (sqlite3_step(stmt) == SQLITE_ROW)
   {
      Dict *col = dict_create(__size__);

      // cid, name, type, notnull, dflt_value, pk
      const char *name = (const char *)sqlite3_column_text(stmt, 1);
      const char *type = (const char *)sqlite3_column_text(stmt, 2);
      int notnull = sqlite3_column_int(stmt, 3);
      const char *dflt = (const char *)sqlite3_column_text(stmt, 4);
      int pk = sqlite3_column_int(stmt, 5);

      dict_insert(createData(TYPE_STR, "name"), createData(TYPE_STR, strdup(name ? name : "")), col);
      dict_insert(createData(TYPE_STR, "type"), createData(TYPE_STR, strdup(type ? type : "")), col);
      dict_insert(createData(TYPE_STR, "notnull"), createData(TYPE_BOOL, &notnull), col);
      dict_insert(createData(TYPE_STR, "default"), dflt ? createData(TYPE_STR, strdup(dflt)) : createData(TYPE_NONE, NULL), col);
      dict_insert(createData(TYPE_STR, "pk"), createData(TYPE_BOOL, &pk), col);

      list_append(createData(TYPE_DICT, col), result);
   }

   sqlite3_finalize(stmt);
   return createData(TYPE_LIST, result);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeDbModule(Runtime *mod_rt)
{
   for (int i = 0; i < DB_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)DB_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&DB_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchDbBuiltin(DbType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case DB_OPEN:
      return __db_open__(ARG(0));
   case DB_CLOSE:
      return __db_close__(ARG(0));
   case DB_EXEC:
      return __db_exec__(ARG(0), ARG(1));
   case DB_QUERY:
      return __db_query__(ARG(0), ARG(1), ARG(2));
   case DB_PREPARE:
      return __db_prepare__(ARG(0), ARG(1));
   case DB_BIND:
      return __db_bind__(ARG(0), ARG(1), ARG(2));
   case DB_STEP:
      return __db_step__(ARG(0));
   case DB_FINALIZE:
      return __db_finalize__(ARG(0));
   case DB_LASTID:
      return __db_lastid__(ARG(0));
   case DB_CHANGES:
      return __db_changes__(ARG(0));
   case DB_BEGIN:
      return __db_begin__(ARG(0));
   case DB_COMMIT:
      return __db_commit__(ARG(0));
   case DB_ROLLBACK:
      return __db_rollback__(ARG(0));
   case DB_TABLES:
      return __db_tables__(ARG(0));
   case DB_SCHEMA:
      return __db_schema__(ARG(0), ARG(1));
   default:
      throw_error(ERROR_SYNTAX, "unknown db builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}