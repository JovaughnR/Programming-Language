#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "./lib/csv.h"

#include "../core/lib/error.h"
#include "../core/lib/type.h"
#include "../core/lib/maloc.h"
#include "../core/lib/daloc.h"
#include "../core/lib/list.h"
#include "../core/lib/utils.h"

const ModuleFuncInfo CSV_MODULE[] = {
    {MODULE_CSV, CSV_PARSE, "__csv_parse__", 1, 2},
    {MODULE_CSV, CSV_STRINGIFY, "__csv_stringify__", 1, 2},
    {MODULE_CSV, CSV_READ, "__csv_read__", 1, 2},
    {MODULE_CSV, CSV_WRITE, "__csv_write__", 2, 3},
};

const int CSV_MODULE_COUNT = sizeof(CSV_MODULE) / sizeof(ModuleFuncInfo);

//=========================================================
//  Helpers
//=========================================================

static const char *get_str(Data *d, const char *funcname)
{
    if (!d || d->type == TYPE_NONE || d->type != TYPE_STR)
    {
        throw_error(ERROR_TYPE, "%s() argument must be a string", funcname);
        return NULL;
    }
    return d->str;
}

static char get_delimiter(Data *delim)
{
    if (!delim || delim->type == TYPE_NONE)
        return ',';
    if (delim->type == TYPE_STR && delim->str[0])
        return delim->str[0];
    return ',';
}

//=========================================================
//  Row Parser — parses one CSV line into a list of strings
//=========================================================

static List *parse_row(const char *line, char delim)
{
    List *row = list_create(__size__);
    int len = strlen(line);
    char *field = malloc(len + 1);
    int fi = 0;
    int i = 0;
    int in_quotes = 0;

    while (i <= len)
    {
        char c = line[i];

        if (c == '"')
        {
            if (in_quotes && line[i + 1] == '"')
            {
                // Escaped quote inside quoted field
                field[fi++] = '"';
                i += 2;
                continue;
            }
            in_quotes = !in_quotes;
            i++;
            continue;
        }

        if ((c == delim || c == '\0' || c == '\n' || c == '\r') && !in_quotes)
        {
            field[fi] = '\0';
            list_append(createData(TYPE_STR, strdup(field)), row);
            fi = 0;

            if (c == '\0' || c == '\n')
                break;
            if (c == '\r' && line[i + 1] == '\n')
                i++; // handle \r\n
            i++;
            continue;
        }

        field[fi++] = c;
        i++;
    }

    free(field);
    return row;
}

//=========================================================
//  Row Stringifier — converts a list of strings to a CSV line
//=========================================================

static char *stringify_row(List *row, char delim)
{
    int cap = 256;
    char *result = malloc(cap);
    int len = 0;

    for (int i = 0; i < row->length; i++)
    {
        Data *item = (Data *)row->items[i];
        const char *field = item->type == TYPE_STR ? item->str : "";
        int flen = strlen(field);
        int needs_quotes = 0;

        // Quote if field contains delimiter, quote, newline
        for (int j = 0; j < flen; j++)
        {
            if (field[j] == delim || field[j] == '"' ||
                field[j] == '\n' || field[j] == '\r')
            {
                needs_quotes = 1;
                break;
            }
        }

        // Ensure capacity
        int needed = len + flen * 2 + 4; // *2 for escaped quotes
        if (needed > cap)
        {
            cap = needed * 2;
            result = realloc(result, cap);
        }

        if (i > 0)
            result[len++] = delim;

        if (needs_quotes)
        {
            result[len++] = '"';
            for (int j = 0; j < flen; j++)
            {
                if (field[j] == '"')
                    result[len++] = '"'; // escape quote
                result[len++] = field[j];
            }
            result[len++] = '"';
        }
        else
        {
            memcpy(result + len, field, flen);
            len += flen;
        }
    }

    result[len++] = '\n';
    result[len] = '\0';
    return result;
}

//=========================================================
//  Implementations
//=========================================================

// parse(text, delimiter=",") -> list
// Parses a CSV string into a list of rows, each row is a list of strings.
// Handles quoted fields and escaped quotes.
// Example: csv.parse("a,b,c\n1,2,3") -> [["a","b","c"],["1","2","3"]]
Data *__csv_parse__(Data *text, Data *delim)
{
    const char *str = get_str(text, "csv.parse");
    if (!str)
        return createData(TYPE_NONE, NULL);

    char d = get_delimiter(delim);
    List *result = list_create(__size__);
    char *copy = strdup(str);
    char *line = strtok(copy, "\n");

    while (line)
    {
        // Skip empty lines
        if (strlen(line) > 0 && !(strlen(line) == 1 && line[0] == '\r'))
        {
            List *row = parse_row(line, d);
            list_append(createData(TYPE_LIST, row), result);
        }
        line = strtok(NULL, "\n");
    }

    free(copy);
    return createData(TYPE_LIST, result);
}

// stringify(rows, delimiter=",") -> str
// Converts a list of rows (list of lists) into a CSV string.
// Automatically quotes fields that contain the delimiter or quotes.
// Example: csv.stringify([["a","b"],["1","2"]]) -> "a,b\n1,2\n"
Data *__csv_stringify__(Data *rows, Data *delim)
{
    if (!rows || rows->type != TYPE_LIST)
    {
        throw_error(ERROR_TYPE, "csv.stringify() argument must be a list of rows");
        return createData(TYPE_NONE, NULL);
    }

    char d = get_delimiter(delim);
    List *list = LIST_PTR(rows);
    int cap = 256;
    char *result = malloc(cap);
    int len = 0;

    for (int i = 0; i < list->length; i++)
    {
        Data *row_data = (Data *)list->items[i];
        if (!row_data || row_data->type != TYPE_LIST)
        {
            throw_error(ERROR_TYPE, "csv.stringify() each row must be a list");
            free(result);
            return createData(TYPE_NONE, NULL);
        }

        char *row_str = stringify_row(LIST_PTR(row_data), d);
        int rlen = strlen(row_str);

        if (len + rlen + 1 > cap)
        {
            cap = (len + rlen) * 2;
            result = realloc(result, cap);
        }

        memcpy(result + len, row_str, rlen);
        len += rlen;
        free(row_str);
    }

    result[len] = '\0';
    return createData(TYPE_STR, result);
}

// read(path, delimiter=",") -> list
// Reads a CSV file and returns a list of rows.
// First row is treated as data (not headers).
// Example: csv.read("data.csv") -> [["a","b"],["1","2"]]
Data *__csv_read__(Data *path, Data *delim)
{
    const char *p = get_str(path, "csv.read");
    if (!p)
        return createData(TYPE_NONE, NULL);

    FILE *f = fopen(p, "r");
    if (!f)
    {
        throw_error(ERROR_RUNTIME, "csv.read(): cannot open file '%s'", p);
        return createData(TYPE_NONE, NULL);
    }

    char d = get_delimiter(delim);
    List *result = list_create(__size__);
    char buf[65536];

    while (fgets(buf, sizeof(buf), f))
    {
        int blen = strlen(buf);
        // Strip trailing newline
        if (blen > 0 && buf[blen - 1] == '\n')
            buf[--blen] = '\0';
        if (blen > 0 && buf[blen - 1] == '\r')
            buf[--blen] = '\0';
        if (blen == 0)
            continue;

        List *row = parse_row(buf, d);
        list_append(createData(TYPE_LIST, row), result);
    }

    fclose(f);
    return createData(TYPE_LIST, result);
}

// write(path, rows, delimiter=",") -> none
// Writes a list of rows to a CSV file.
// Example: csv.write("out.csv", [["name","age"],["Alice","30"]])
Data *__csv_write__(Data *path, Data *rows, Data *delim)
{
    const char *p = get_str(path, "csv.write");
    if (!p)
        return createData(TYPE_NONE, NULL);

    if (!rows || rows->type != TYPE_LIST)
    {
        throw_error(ERROR_TYPE, "csv.write() rows must be a list");
        return createData(TYPE_NONE, NULL);
    }

    FILE *f = fopen(p, "w");
    if (!f)
    {
        throw_error(ERROR_RUNTIME, "csv.write(): cannot open file '%s'", p);
        return createData(TYPE_NONE, NULL);
    }

    char d = get_delimiter(delim);
    List *list = LIST_PTR(rows);

    for (int i = 0; i < list->length; i++)
    {
        Data *row_data = (Data *)list->items[i];
        if (!row_data || row_data->type != TYPE_LIST)
        {
            throw_error(ERROR_TYPE, "csv.write() each row must be a list");
            fclose(f);
            return createData(TYPE_NONE, NULL);
        }

        char *row_str = stringify_row(LIST_PTR(row_data), d);
        fputs(row_str, f);
        free(row_str);
    }

    fclose(f);
    return createData(TYPE_NONE, NULL);
}

//=========================================================
//  Module Initializer
//=========================================================

void initializeCsvModule(Runtime *mod_rt)
{
    for (int i = 0; i < CSV_MODULE_COUNT; i++)
    {
        Data *key = createData(TYPE_STR, (void *)CSV_MODULE[i].name);
        Data *val = createData(TYPE_MODULE, (void *)&CSV_MODULE[i]);
        env_save(key, val, mod_rt->env);
        data_free(key);
    }
}

//=========================================================
//  Dispatcher
//=========================================================

Data *dispatchCsvBuiltin(CsvType type, List *args, Dict *kwargs, Runtime *rt)
{
    UNUSED(kwargs);
    UNUSED(rt);

    switch (type)
    {
    case CSV_PARSE:
        return __csv_parse__(ARG(0), ARG(1));
    case CSV_STRINGIFY:
        return __csv_stringify__(ARG(0), ARG(1));
    case CSV_READ:
        return __csv_read__(ARG(0), ARG(1));
    case CSV_WRITE:
        return __csv_write__(ARG(0), ARG(1), ARG(2));
    default:
        throw_error(ERROR_SYNTAX, "unknown csv builtin type %d", type);
        return createData(TYPE_NONE, NULL);
    }
}