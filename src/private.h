#pragma once

#include <blackmagic/pair.h>  // PAIR_*
#include <blackmagic/token.h> // STRINGIZE
#include <o2s/string.h>       // string_append_format

#define append(S, ...) string_append_format(&S, __VA_ARGS__)
#define _append_field(STRING, MESSAGE, TYPE, FIELD) append(STRING, ", " STRINGIZE(FIELD) ": %" TYPE, MESSAGE->FIELD);
#define append_field(STRING, MESSAGE, TYPE_AND_FIELD) _append_field(STRING, MESSAGE, PAIR_FIRST TYPE_AND_FIELD, PAIR_LAST TYPE_AND_FIELD)

#define boolalpha(VALUE) (VALUE ? "true" : "false")
