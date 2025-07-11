#ifndef CETL_PTYPES_H
#define CETL_PTYPES_H

#include <stddef.h>
#include <stdint.h>

// Base types
typedef uint8_t               cetl_byte_t;
typedef uint16_t              cetl_uint16_t;
typedef uint32_t              cetl_uint32_t;
typedef uint64_t              cetl_uint64_t;
typedef unsigned int          cetl_uint_t;
typedef unsigned short        cetl_ushort_t;

typedef int8_t                cetl_sint8_t;
typedef int16_t               cetl_sint16_t;
typedef int32_t               cetl_sint32_t;
typedef int64_t               cetl_sint64_t;
typedef short                 cetl_short_t;
typedef int                   cetl_int_t;

// Size and index
typedef size_t                cetl_size_t;
typedef ptrdiff_t             cetl_ptrdiff_t;
typedef intptr_t              cetl_intptr_t;
typedef uintptr_t             cetl_uintptr_t;

// Bool types
typedef                       cetl_int_t cetl_bool_t;
#define CETL_TRUE             true
#define CETL_FALSE            false

// Char
typedef char                  cetl_char_t;
typedef unsigned char         cetl_uchar_t;

// Pointers
typedef void                  cetl_void_t;
typedef cetl_void_t*          cetl_ptr_t;
typedef cetl_ptr_t * const    cetl_cptr_t;

// Result of operation
typedef int                   cetl_result_t;

#endif // CETL_PTYPES_H
