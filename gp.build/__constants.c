
#include "nuitka/prelude.h"
#include "structseq.h"

// Sentinel PyObject to be used for all our call iterator endings. It will
// become a PyCObject pointing to NULL. It's address is unique, and that's
// enough for us to use it as sentinel value.
PyObject *_sentinel_value = NULL;

PyObject *Nuitka_dunder_compiled_value = NULL;

PyObject *const_int_0;
PyObject *const_str_dot;
PyObject *const_float_0_0;
PyObject *const_float_0_5;
PyObject *const_int_pos_1;
PyObject *const_int_pos_2;
PyObject *const_int_pos_3;
PyObject *const_str_empty;
PyObject *const_dict_empty;
PyObject *const_bytes_empty;
PyObject *const_str_plain_c;
PyObject *const_str_plain_n;
PyObject *const_str_plain_x;
PyObject *const_tuple_empty;
PyObject *const_str_plain_rb;
PyObject *const_str_plain_add;
PyObject *const_str_plain_end;
PyObject *const_str_plain_gtw;
PyObject *const_str_plain_ifw;
PyObject *const_str_plain_int;
PyObject *const_str_plain_len;
PyObject *const_str_plain_sum;
PyObject *const_str_plain_addw;
PyObject *const_str_plain_file;
PyObject *const_str_plain_iter;
PyObject *const_str_plain_name;
PyObject *const_str_plain_open;
PyObject *const_str_plain_read;
PyObject *const_str_plain_repr;
PyObject *const_str_plain_self;
PyObject *const_str_plain_send;
PyObject *const_str_plain_site;
PyObject *const_str_plain_subw;
PyObject *const_str_plain_type;
PyObject *const_float_minus_0_0;
PyObject *const_str_plain_bytes;
PyObject *const_str_plain_close;
PyObject *const_str_plain_flist;
PyObject *const_str_plain_level;
PyObject *const_str_plain_print;
PyObject *const_str_plain_range;
PyObject *const_str_plain_stats;
PyObject *const_str_plain_throw;
PyObject *const_str_plain_types;
PyObject *const_str_plain_format;
PyObject *const_str_plain_locals;
PyObject *const_str_plain_origin;
PyObject *const_str_plain_params;
PyObject *const_str_plain_stddev;
PyObject *const_str_plain___all__;
PyObject *const_str_plain___cmp__;
PyObject *const_str_plain___doc__;
PyObject *const_str_plain_compile;
PyObject *const_str_plain_globals;
PyObject *const_str_plain_inspect;
PyObject *const_str_plain___dict__;
PyObject *const_str_plain___exit__;
PyObject *const_str_plain___file__;
PyObject *const_str_plain___init__;
PyObject *const_str_plain___iter__;
PyObject *const_str_plain___main__;
PyObject *const_str_plain___name__;
PyObject *const_str_plain___path__;
PyObject *const_str_plain___spec__;
PyObject *const_str_plain_fromlist;
PyObject *const_str_plain_function;
PyObject *const_str_angle_metaclass;
PyObject *const_str_plain___class__;
PyObject *const_str_plain___debug__;
PyObject *const_str_plain___enter__;
PyObject *const_str_plain_bytearray;
PyObject *const_str_plain_metaclass;
PyObject *const_str_plain___cached__;
PyObject *const_str_plain___import__;
PyObject *const_str_plain___loader__;
PyObject *const_str_plain___module__;
PyObject *const_str_plain___getitem__;
PyObject *const_str_plain___package__;
PyObject *const_str_plain___prepare__;
PyObject *const_str_plain_classmethod;
PyObject *const_tuple_int_pos_1_tuple;
PyObject *const_str_plain___builtins__;
PyObject *const_str_plain___compiled__;
PyObject *const_str_plain___internal__;
PyObject *const_str_plain___qualname__;
PyObject *const_str_plain_has_location;
PyObject *const_str_plain_staticmethod;
PyObject *const_str_plain___metaclass__;
PyObject *const_str_plain__initializing;
PyObject *const_str_plain___mro_entries__;
PyObject *const_str_plain_functionwrapper;
PyObject *const_str_plain___class_getitem__;
PyObject *const_tuple_str_plain___class___tuple;
PyObject *const_str_plain_submodule_search_locations;
PyObject *const_str_digest_6de5855651d8835e19f0219a160bc8ba;
PyObject *const_str_digest_75fd71b1edada749c2ef7ac810062295;
PyObject *const_str_digest_9eba47f18b2f1b957086ae148e9796b9;

static void _createGlobalConstants( void )
{
    NUITKA_MAY_BE_UNUSED PyObject *exception_type, *exception_value;
    NUITKA_MAY_BE_UNUSED PyTracebackObject *exception_tb;

#ifdef _MSC_VER
    // Prevent unused warnings in case of simple programs, the attribute
    // NUITKA_MAY_BE_UNUSED doesn't work for MSVC.
    (void *)exception_type; (void *)exception_value; (void *)exception_tb;
#endif

    const_int_0 = PyLong_FromUnsignedLong( 0ul );
    const_str_dot = UNSTREAM_STRING_ASCII( &constant_bin[ 28 ], 1, 0 );
    const_float_0_0 = UNSTREAM_FLOAT( &constant_bin[ 2006 ] );
    const_float_0_5 = UNSTREAM_FLOAT( &constant_bin[ 2014 ] );
    const_int_pos_1 = PyLong_FromUnsignedLong( 1ul );
    const_int_pos_2 = PyLong_FromUnsignedLong( 2ul );
    const_int_pos_3 = PyLong_FromUnsignedLong( 3ul );
    const_str_empty = UNSTREAM_STRING_ASCII( &constant_bin[ 0 ], 0, 0 );
    const_dict_empty = _PyDict_NewPresized( 0 );
    assert( PyDict_Size( const_dict_empty ) == 0 );
    const_bytes_empty = UNSTREAM_BYTES( &constant_bin[ 0 ], 0 );
    const_str_plain_c = UNSTREAM_STRING_ASCII( &constant_bin[ 64 ], 1, 1 );
    const_str_plain_n = UNSTREAM_STRING_ASCII( &constant_bin[ 9 ], 1, 1 );
    const_str_plain_x = UNSTREAM_STRING_ASCII( &constant_bin[ 70 ], 1, 1 );
    const_tuple_empty = PyTuple_New( 0 );
    const_str_plain_rb = UNSTREAM_STRING_ASCII( &constant_bin[ 2022 ], 2, 1 );
    const_str_plain_add = UNSTREAM_STRING_ASCII( &constant_bin[ 2024 ], 3, 1 );
    const_str_plain_end = UNSTREAM_STRING_ASCII( &constant_bin[ 748 ], 3, 1 );
    const_str_plain_gtw = UNSTREAM_STRING_ASCII( &constant_bin[ 2027 ], 3, 1 );
    const_str_plain_ifw = UNSTREAM_STRING_ASCII( &constant_bin[ 2030 ], 3, 1 );
    const_str_plain_int = UNSTREAM_STRING_ASCII( &constant_bin[ 1002 ], 3, 1 );
    const_str_plain_len = UNSTREAM_STRING_ASCII( &constant_bin[ 2033 ], 3, 1 );
    const_str_plain_sum = UNSTREAM_STRING_ASCII( &constant_bin[ 1964 ], 3, 1 );
    const_str_plain_addw = UNSTREAM_STRING_ASCII( &constant_bin[ 2036 ], 4, 1 );
    const_str_plain_file = UNSTREAM_STRING_ASCII( &constant_bin[ 2040 ], 4, 1 );
    const_str_plain_iter = UNSTREAM_STRING_ASCII( &constant_bin[ 2044 ], 4, 1 );
    const_str_plain_name = UNSTREAM_STRING_ASCII( &constant_bin[ 2048 ], 4, 1 );
    const_str_plain_open = UNSTREAM_STRING_ASCII( &constant_bin[ 2052 ], 4, 1 );
    const_str_plain_read = UNSTREAM_STRING_ASCII( &constant_bin[ 2056 ], 4, 1 );
    const_str_plain_repr = UNSTREAM_STRING_ASCII( &constant_bin[ 2060 ], 4, 1 );
    const_str_plain_self = UNSTREAM_STRING_ASCII( &constant_bin[ 2064 ], 4, 1 );
    const_str_plain_send = UNSTREAM_STRING_ASCII( &constant_bin[ 2068 ], 4, 1 );
    const_str_plain_site = UNSTREAM_STRING_ASCII( &constant_bin[ 2072 ], 4, 1 );
    const_str_plain_subw = UNSTREAM_STRING_ASCII( &constant_bin[ 2076 ], 4, 1 );
    const_str_plain_type = UNSTREAM_STRING_ASCII( &constant_bin[ 2080 ], 4, 1 );
    const_float_minus_0_0 = UNSTREAM_FLOAT( &constant_bin[ 2084 ] );
    const_str_plain_bytes = UNSTREAM_STRING_ASCII( &constant_bin[ 2092 ], 5, 1 );
    const_str_plain_close = UNSTREAM_STRING_ASCII( &constant_bin[ 2097 ], 5, 1 );
    const_str_plain_flist = UNSTREAM_STRING_ASCII( &constant_bin[ 2102 ], 5, 1 );
    const_str_plain_level = UNSTREAM_STRING_ASCII( &constant_bin[ 2107 ], 5, 1 );
    const_str_plain_print = UNSTREAM_STRING_ASCII( &constant_bin[ 2112 ], 5, 1 );
    const_str_plain_range = UNSTREAM_STRING_ASCII( &constant_bin[ 2117 ], 5, 1 );
    const_str_plain_stats = UNSTREAM_STRING_ASCII( &constant_bin[ 844 ], 5, 1 );
    const_str_plain_throw = UNSTREAM_STRING_ASCII( &constant_bin[ 2122 ], 5, 1 );
    const_str_plain_types = UNSTREAM_STRING_ASCII( &constant_bin[ 2127 ], 5, 1 );
    const_str_plain_format = UNSTREAM_STRING_ASCII( &constant_bin[ 2132 ], 6, 1 );
    const_str_plain_locals = UNSTREAM_STRING_ASCII( &constant_bin[ 674 ], 6, 1 );
    const_str_plain_origin = UNSTREAM_STRING_ASCII( &constant_bin[ 2138 ], 6, 1 );
    const_str_plain_params = UNSTREAM_STRING_ASCII( &constant_bin[ 2144 ], 6, 1 );
    const_str_plain_stddev = UNSTREAM_STRING_ASCII( &constant_bin[ 2150 ], 6, 1 );
    const_str_plain___all__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2156 ], 7, 1 );
    const_str_plain___cmp__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2163 ], 7, 1 );
    const_str_plain___doc__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2170 ], 7, 1 );
    const_str_plain_compile = UNSTREAM_STRING_ASCII( &constant_bin[ 2177 ], 7, 1 );
    const_str_plain_globals = UNSTREAM_STRING_ASCII( &constant_bin[ 2184 ], 7, 1 );
    const_str_plain_inspect = UNSTREAM_STRING_ASCII( &constant_bin[ 2191 ], 7, 1 );
    const_str_plain___dict__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2198 ], 8, 1 );
    const_str_plain___exit__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2206 ], 8, 1 );
    const_str_plain___file__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2214 ], 8, 1 );
    const_str_plain___init__ = UNSTREAM_STRING_ASCII( &constant_bin[ 29 ], 8, 1 );
    const_str_plain___iter__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2222 ], 8, 1 );
    const_str_plain___main__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2230 ], 8, 1 );
    const_str_plain___name__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2238 ], 8, 1 );
    const_str_plain___path__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2246 ], 8, 1 );
    const_str_plain___spec__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2254 ], 8, 1 );
    const_str_plain_fromlist = UNSTREAM_STRING_ASCII( &constant_bin[ 2262 ], 8, 1 );
    const_str_plain_function = UNSTREAM_STRING_ASCII( &constant_bin[ 92 ], 8, 1 );
    const_str_angle_metaclass = UNSTREAM_STRING_ASCII( &constant_bin[ 2270 ], 11, 0 );
    const_str_plain___class__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2281 ], 9, 1 );
    const_str_plain___debug__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2290 ], 9, 1 );
    const_str_plain___enter__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2299 ], 9, 1 );
    const_str_plain_bytearray = UNSTREAM_STRING_ASCII( &constant_bin[ 2308 ], 9, 1 );
    const_str_plain_metaclass = UNSTREAM_STRING_ASCII( &constant_bin[ 2271 ], 9, 1 );
    const_str_plain___cached__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2317 ], 10, 1 );
    const_str_plain___import__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2327 ], 10, 1 );
    const_str_plain___loader__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2337 ], 10, 1 );
    const_str_plain___module__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2347 ], 10, 1 );
    const_str_plain___getitem__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2357 ], 11, 1 );
    const_str_plain___package__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2368 ], 11, 1 );
    const_str_plain___prepare__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2379 ], 11, 1 );
    const_str_plain_classmethod = UNSTREAM_STRING_ASCII( &constant_bin[ 2390 ], 11, 1 );
    const_tuple_int_pos_1_tuple = PyTuple_New( 1 );
    PyTuple_SET_ITEM( const_tuple_int_pos_1_tuple, 0, const_int_pos_1 ); Py_INCREF( const_int_pos_1 );
    const_str_plain___builtins__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2401 ], 12, 1 );
    const_str_plain___compiled__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2413 ], 12, 1 );
    const_str_plain___internal__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2425 ], 12, 1 );
    const_str_plain___qualname__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2437 ], 12, 1 );
    const_str_plain_has_location = UNSTREAM_STRING_ASCII( &constant_bin[ 2449 ], 12, 1 );
    const_str_plain_staticmethod = UNSTREAM_STRING_ASCII( &constant_bin[ 2461 ], 12, 1 );
    const_str_plain___metaclass__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2473 ], 13, 1 );
    const_str_plain__initializing = UNSTREAM_STRING_ASCII( &constant_bin[ 2486 ], 13, 1 );
    const_str_plain___mro_entries__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2499 ], 15, 1 );
    const_str_plain_functionwrapper = UNSTREAM_STRING_ASCII( &constant_bin[ 1584 ], 15, 1 );
    const_str_plain___class_getitem__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2514 ], 17, 1 );
    const_tuple_str_plain___class___tuple = PyTuple_New( 1 );
    PyTuple_SET_ITEM( const_tuple_str_plain___class___tuple, 0, const_str_plain___class__ ); Py_INCREF( const_str_plain___class__ );
    const_str_plain_submodule_search_locations = UNSTREAM_STRING_ASCII( &constant_bin[ 2531 ], 26, 1 );
    const_str_digest_6de5855651d8835e19f0219a160bc8ba = UNSTREAM_STRING_ASCII( &constant_bin[ 2557 ], 71, 0 );
    const_str_digest_75fd71b1edada749c2ef7ac810062295 = UNSTREAM_STRING_ASCII( &constant_bin[ 2628 ], 46, 0 );
    const_str_digest_9eba47f18b2f1b957086ae148e9796b9 = UNSTREAM_STRING_ASCII( &constant_bin[ 2674 ], 11, 0 );

#if _NUITKA_EXE
    /* Set the "sys.executable" path to the original CPython executable. */
    PySys_SetObject(
        (char *)"executable",
        const_str_digest_6de5855651d8835e19f0219a160bc8ba
    );

#ifndef _NUITKA_STANDALONE
    /* Set the "sys.prefix" path to the original one. */
    PySys_SetObject(
        (char *)"prefix",
        const_str_digest_9eba47f18b2f1b957086ae148e9796b9
    );

    /* Set the "sys.prefix" path to the original one. */
    PySys_SetObject(
        (char *)"exec_prefix",
        const_str_digest_9eba47f18b2f1b957086ae148e9796b9
    );


#if PYTHON_VERSION >= 300
    /* Set the "sys.base_prefix" path to the original one. */
    PySys_SetObject(
        (char *)"base_prefix",
        const_str_digest_9eba47f18b2f1b957086ae148e9796b9
    );

    /* Set the "sys.exec_base_prefix" path to the original one. */
    PySys_SetObject(
        (char *)"base_exec_prefix",
        const_str_digest_9eba47f18b2f1b957086ae148e9796b9
    );

#endif
#endif
#endif

    static PyTypeObject Nuitka_VersionInfoType;

    // Same fields as "sys.version_info" except no serial number.
    static PyStructSequence_Field Nuitka_VersionInfoFields[] = {
        {(char *)"major", (char *)"Major release number"},
        {(char *)"minor", (char *)"Minor release number"},
        {(char *)"micro", (char *)"Micro release number"},
        {(char *)"releaselevel", (char *)"'alpha', 'beta', 'candidate', or 'release'"},
        {0}
    };

    static PyStructSequence_Desc Nuitka_VersionInfoDesc = {
        (char *)"__nuitka_version__",                                    /* name */
        (char *)"__compiled__\n\nVersion information as a named tuple.", /* doc */
        Nuitka_VersionInfoFields,                                        /* fields */
        4
    };

    PyStructSequence_InitType(&Nuitka_VersionInfoType, &Nuitka_VersionInfoDesc);

    Nuitka_dunder_compiled_value = PyStructSequence_New(&Nuitka_VersionInfoType);
    assert(Nuitka_dunder_compiled_value != NULL);

    PyStructSequence_SET_ITEM(Nuitka_dunder_compiled_value, 0, PyInt_FromLong(0));
    PyStructSequence_SET_ITEM(Nuitka_dunder_compiled_value, 1, PyInt_FromLong(6));
    PyStructSequence_SET_ITEM(Nuitka_dunder_compiled_value, 2, PyInt_FromLong(4));

#if PYTHON_VERSION < 300
    PyStructSequence_SET_ITEM(Nuitka_dunder_compiled_value, 3, PyString_FromString("release"));
#else
    PyStructSequence_SET_ITEM(Nuitka_dunder_compiled_value, 3, PyUnicode_FromString("release"));
#endif
    // Prevent users from creating the Nuitka version type object.
    Nuitka_VersionInfoType.tp_init = NULL;
    Nuitka_VersionInfoType.tp_new = NULL;


}

// In debug mode we can check that the constants were not tampered with in any
// given moment. We typically do it at program exit, but we can add extra calls
// for sanity.
#ifndef __NUITKA_NO_ASSERT__
void checkGlobalConstants( void )
{

}
#endif

void createGlobalConstants( void )
{
    if ( _sentinel_value == NULL )
    {
#if PYTHON_VERSION < 300
        _sentinel_value = PyCObject_FromVoidPtr( NULL, NULL );
#else
        // The NULL value is not allowed for a capsule, so use something else.
        _sentinel_value = PyCapsule_New( (void *)27, "sentinel", NULL );
#endif
        assert( _sentinel_value );

        _createGlobalConstants();
    }
}
