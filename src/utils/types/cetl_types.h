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

typedef int8_t                cetl_int8_t;
typedef int16_t               cetl_int16_t;
typedef int32_t               cetl_int32_t;
typedef int64_t               cetl_int64_t;
typedef short                 cetl_short_t;
typedef int                   cetl_int_t;

// Size and index
typedef size_t                cetl_size_t;
typedef ptrdiff_t             cetl_ptrdiff_t;
typedef intptr_t              cetl_intptr_t;
typedef uintptr_t             cetl_uintptr_t;

// Bool types
typedef cetl_int_t            cetl_bool_t;
#define CETL_TRUE             1
#define CETL_FALSE            0

// Float point
typedef float                 cetl_float_t;
typedef double                cetl_double_t;

// Char
typedef char                  cetl_char_t;
typedef unsigned char         cetl_uchar_t;
typedef cetl_char_t*          cetl_str_t;
typedef const cetl_str_t      cetl_cstr_t;

// Pointers
typedef void                  cetl_void_t;
typedef cetl_void_t*          cetl_ptr_t;
typedef cetl_void_t const*    cetl_cptr_t;

// Result of operation
typedef int                   cetl_result_t;

// Iterators
typedef struct cetl_iterator         cetl_iterator;
typedef struct cetl_const_iterator   cetl_const_iterator;

// Containers forward declarations
struct cetl_element;
typedef struct cetl_llist     cetl_llist;
typedef struct cetl_stack     cetl_stack;
typedef struct cetl_vector    cetl_vector;
typedef struct cetl_string    cetl_string;
typedef struct cetl_queue     cetl_queue;
typedef struct cetl_dlist     cetl_dlist;
typedef struct cetl_cdlist    cetl_cdlist; 

#endif // CETL_PTYPES_H
