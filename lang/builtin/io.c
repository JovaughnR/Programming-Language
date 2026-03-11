#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./lib/io.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo IO_MODULE[] = {
    {MODULE_IO, IO_OPEN, "__io_open__", 1, 2},
    {MODULE_IO, IO_CLOSE, "__io_close__", 1, 1},
    {MODULE_IO, IO_READ, "__io_read__", 1, 2},
    {MODULE_IO, IO_READLINE, "__io_readline__", 1, 1},
    {MODULE_IO, IO_READLINES, "__io_readlines__", 1, 1},
    {MODULE_IO, IO_WRITE, "__io_write__", 2, 2},
    {MODULE_IO, IO_WRITELINE, "__io_writeline__", 2, 2},
    {MODULE_IO, IO_WRITELINES, "__io_writelines__", 2, 2},
    {MODULE_IO, IO_SEEK, "__io_seek__", 2, 2},
    {MODULE_IO, IO_TELL, "__io_tell__", 1, 1},
    {MODULE_IO, IO_FLUSH, "__io_flush__", 1, 1},
};

const int IO_MODULE_COUNT = sizeof(IO_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Helpers
//=========================================================

static const char *get_str(Data *d, const char *funcname)
{
   if (!d || d->type != TYPE_STR)
   {
      throw_error(ERROR_TYPE, "%s() argument must be a string", funcname);
      return NULL;
   }
   return d->str;
}

// File handles are stored as TYPE_FILE — a thin wrapper around FILE*
// We store FILE* directly in Data->any
static FILE *get_file(Data *d, const char *funcname)
{
   if (!d || d->type != TYPE_FILE)
   {
      throw_error(ERROR_TYPE, "%s() argument must be a file handle", funcname);
      return NULL;
   }
   return (FILE *)d->any;
}

//=========================================================
//  Implementations
//=========================================================

// open(path, mode="r") -> file
// Opens a file and returns a file handle.
// Modes: "r" read, "w" write, "a" append, "r+" read/write
// Example: f = io.open("data.txt")
// Example: f = io.open("out.txt", "w")
Data *__io_open__(Data *path, Data *mode)
{
   const char *p = get_str(path, "io.open");
   if (!p)
      return createData(TYPE_NONE, NULL);

   const char *m = "r";
   if (mode && mode->type == TYPE_STR)
      m = mode->str;

   FILE *f = fopen(p, m);
   if (!f)
   {
      throw_error(ERROR_RUNTIME, "io.open(): cannot open file '%s'", p);
      return createData(TYPE_NONE, NULL);
   }

   return createData(TYPE_FILE, f);
}

// close(file) -> none
// Closes an open file handle.
// Example: io.close(f)
Data *__io_close__(Data *file)
{
   FILE *f = get_file(file, "io.close");
   if (!f)
      return createData(TYPE_NONE, NULL);
   fclose(f);
   file->any = NULL; // prevent double-close
   return createData(TYPE_NONE, NULL);
}

// read(file, n=-1) -> str
// Reads the entire file or up to n bytes.
// Example: content = io.read(f)
// Example: chunk   = io.read(f, 128)
Data *__io_read__(Data *file, Data *n)
{
   FILE *f = get_file(file, "io.read");
   if (!f)
      return createData(TYPE_NONE, NULL);

   // Read n bytes
   if (n && n->type == TYPE_INT)
   {
      int count = *(int *)n->atom;
      char *buf = malloc(count + 1);
      int bytes = (int)fread(buf, 1, count, f);
      buf[bytes] = '\0'; // use actual bytes read not count
      return createData(TYPE_STR, buf);
   }

   // Read from current position to end of file
   long cur = ftell(f);
   fseek(f, 0, SEEK_END);
   long end = ftell(f);
   fseek(f, cur, SEEK_SET); // restore to current position not start

   long size = end - cur; // only remaining bytes
   char *buf = malloc(size + 1);
   long read = (long)fread(buf, 1, size, f);
   buf[read] = '\0';

   return createData(TYPE_STR, buf);
}

// readline(file) -> str
// Reads one line from the file including the newline character.
// Returns empty string at end of file.
// Example: line = io.readline(f)
Data *__io_readline__(Data *file)
{
   FILE *f = get_file(file, "io.readline");
   if (!f)
      return createData(TYPE_STR, strdup(""));

   char buf[4096];
   if (!fgets(buf, sizeof(buf), f))
      return createData(TYPE_STR, strdup(""));

   return createData(TYPE_STR, strdup(buf));
}

// readlines(file) -> list
// Reads all lines from the file and returns them as a list of strings.
// Example: lines = io.readlines(f)
Data *__io_readlines__(Data *file)
{
   FILE *f = get_file(file, "io.readlines");
   if (!f)
      return createData(TYPE_NONE, NULL);

   List *lines = list_create(__size__);
   char buf[4096];

   while (fgets(buf, sizeof(buf), f))
   {
      // Strip trailing newline
      int len = strlen(buf);
      if (len > 0 && buf[len - 1] == '\n')
         buf[len - 1] = '\0';
      list_append(createData(TYPE_STR, strdup(buf)), lines);
   }

   return createData(TYPE_LIST, lines);
}

// write(file, content) -> int
// Writes a string to the file. Returns number of bytes written.
// Example: io.write(f, "hello world")
Data *__io_write__(Data *file, Data *content)
{
   FILE *f = get_file(file, "io.write");
   if (!f)
      return createData(TYPE_INT, &(int){0});

   const char *s = get_str(content, "io.write");
   if (!s)
      return createData(TYPE_INT, &(int){0});

   int written = fputs(s, f);
   return createData(TYPE_INT, &written);
}

// writeline(file, content) -> int
// Writes a string followed by a newline to the file.
// Example: io.writeline(f, "hello world")
Data *__io_writeline__(Data *file, Data *content)
{
   FILE *f = get_file(file, "io.writeline");
   if (!f)
      return createData(TYPE_INT, &(int){0});

   const char *s = get_str(content, "io.writeline");
   if (!s)
      return createData(TYPE_INT, &(int){0});

   fprintf(f, "%s\n", s);
   return createData(TYPE_NONE, NULL);
}

// writelines(file, lines) -> none
// Writes a list of strings to the file, each followed by a newline.
// Example: io.writelines(f, ["line1", "line2", "line3"])
Data *__io_writelines__(Data *file, Data *lines)
{
   FILE *f = get_file(file, "io.writelines");
   if (!f)
      return createData(TYPE_NONE, NULL);

   if (!lines || lines->type != TYPE_LIST)
   {
      throw_error(ERROR_TYPE, "io.writelines() second argument must be a list");
      return createData(TYPE_NONE, NULL);
   }

   List *list = LIST_PTR(lines);
   for (int i = 0; i < list->length; i++)
   {
      Data *item = (Data *)list->items[i];
      if (item->type != TYPE_STR)
      {
         throw_error(ERROR_TYPE, "io.writelines() list items must be strings");
         return createData(TYPE_NONE, NULL);
      }
      fprintf(f, "%s\n", item->str);
   }

   return createData(TYPE_NONE, NULL);
}

// seek(file, offset) -> none
// Moves the file cursor to the given byte offset from the start.
// Example: io.seek(f, 0)  # rewind to start
Data *__io_seek__(Data *file, Data *offset)
{
   FILE *f = get_file(file, "io.seek");
   if (!f)
      return createData(TYPE_NONE, NULL);

   if (!offset || offset->type != TYPE_INT)
   {
      throw_error(ERROR_TYPE, "io.seek() offset must be an integer");
      return createData(TYPE_NONE, NULL);
   }

   fseek(f, *(int *)offset->atom, SEEK_SET);
   return createData(TYPE_NONE, NULL);
}

// tell(file) -> int
// Returns the current byte position of the file cursor.
// Example: pos = io.tell(f)
Data *__io_tell__(Data *file)
{
   FILE *f = get_file(file, "io.tell");
   if (!f)
      return createData(TYPE_INT, &(int){0});
   int pos = (int)ftell(f);
   return createData(TYPE_INT, &pos);
}

// flush(file) -> none
// Flushes the file write buffer to disk.
// Example: io.flush(f)
Data *__io_flush__(Data *file)
{
   FILE *f = get_file(file, "io.flush");
   if (!f)
      return createData(TYPE_NONE, NULL);
   fflush(f);
   return createData(TYPE_NONE, NULL);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeIoModule(Runtime *mod_rt)
{
   for (int i = 0; i < IO_MODULE_COUNT; i++)
   {
      Data *key = createData(TYPE_STR, (void *)IO_MODULE[i].name);
      Data *val = createData(TYPE_MODULE, (void *)&IO_MODULE[i]);
      env_save(key, val, mod_rt->env);
      data_free(key);
   }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchIoBuiltin(IoType type, List *args, Dict *kwargs, Runtime *rt)
{
   UNUSED(kwargs);
   UNUSED(rt);

   switch (type)
   {
   case IO_OPEN:
      return __io_open__(ARG(0), ARG(1));
   case IO_CLOSE:
      return __io_close__(ARG(0));
   case IO_READ:
      return __io_read__(ARG(0), ARG(1));
   case IO_READLINE:
      return __io_readline__(ARG(0));
   case IO_READLINES:
      return __io_readlines__(ARG(0));
   case IO_WRITE:
      return __io_write__(ARG(0), ARG(1));
   case IO_WRITELINE:
      return __io_writeline__(ARG(0), ARG(1));
   case IO_WRITELINES:
      return __io_writelines__(ARG(0), ARG(1));
   case IO_SEEK:
      return __io_seek__(ARG(0), ARG(1));
   case IO_TELL:
      return __io_tell__(ARG(0));
   case IO_FLUSH:
      return __io_flush__(ARG(0));
   default:
      throw_error(ERROR_SYNTAX, "unknown io builtin type %d", type);
      return createData(TYPE_NONE, NULL);
   }
}