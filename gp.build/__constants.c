
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
    const_float_0_0 = UNSTREAM_FLOAT( &constant_bin[ 2052 ] );
    const_float_0_5 = UNSTREAM_FLOAT( &constant_bin[ 2060 ] );
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
    const_str_plain_rb = UNSTREAM_STRING_ASCII( &constant_bin[ 2068 ], 2, 1 );
    const_str_plain_add = UNSTREAM_STRING_ASCII( &constant_bin[ 2070 ], 3, 1 );
    const_str_plain_end = UNSTREAM_STRING_ASCII( &constant_bin[ 802 ], 3, 1 );
    const_str_plain_gtw = UNSTREAM_STRING_ASCII( &constant_bin[ 2073 ], 3, 1 );
    const_str_plain_ifw = UNSTREAM_STRING_ASCII( &constant_bin[ 2076 ], 3, 1 );
    const_str_plain_int = UNSTREAM_STRING_ASCII( &constant_bin[ 1048 ], 3, 1 );
    const_str_plain_len = UNSTREAM_STRING_ASCII( &constant_bin[ 2079 ], 3, 1 );
    const_str_plain_sum = UNSTREAM_STRING_ASCII( &constant_bin[ 2010 ], 3, 1 );
    const_str_plain_addw = UNSTREAM_STRING_ASCII( &constant_bin[ 2082 ], 4, 1 );
    const_str_plain_file = UNSTREAM_STRING_ASCII( &constant_bin[ 2086 ], 4, 1 );
    const_str_plain_iter = UNSTREAM_STRING_ASCII( &constant_bin[ 2090 ], 4, 1 );
    const_str_plain_name = UNSTREAM_STRING_ASCII( &constant_bin[ 2094 ], 4, 1 );
    const_str_plain_open = UNSTREAM_STRING_ASCII( &constant_bin[ 2098 ], 4, 1 );
    const_str_plain_read = UNSTREAM_STRING_ASCII( &constant_bin[ 2102 ], 4, 1 );
    const_str_plain_repr = UNSTREAM_STRING_ASCII( &constant_bin[ 2106 ], 4, 1 );
    const_str_plain_self = UNSTREAM_STRING_ASCII( &constant_bin[ 2110 ], 4, 1 );
    const_str_plain_send = UNSTREAM_STRING_ASCII( &constant_bin[ 2114 ], 4, 1 );
    const_str_plain_site = UNSTREAM_STRING_ASCII( &constant_bin[ 2118 ], 4, 1 );
    const_str_plain_subw = UNSTREAM_STRING_ASCII( &constant_bin[ 2122 ], 4, 1 );
    const_str_plain_type = UNSTREAM_STRING_ASCII( &constant_bin[ 2126 ], 4, 1 );
    const_float_minus_0_0 = UNSTREAM_FLOAT( &constant_bin[ 2130 ] );
    const_str_plain_bytes = UNSTREAM_STRING_ASCII( &constant_bin[ 2138 ], 5, 1 );
    const_str_plain_close = UNSTREAM_STRING_ASCII( &constant_bin[ 2143 ], 5, 1 );
    const_str_plain_flist = UNSTREAM_STRING_ASCII( &constant_bin[ 2148 ], 5, 1 );
    const_str_plain_level = UNSTREAM_STRING_ASCII( &constant_bin[ 2153 ], 5, 1 );
    const_str_plain_print = UNSTREAM_STRING_ASCII( &constant_bin[ 2158 ], 5, 1 );
    const_str_plain_range = UNSTREAM_STRING_ASCII( &constant_bin[ 2163 ], 5, 1 );
    const_str_plain_stats = UNSTREAM_STRING_ASCII( &constant_bin[ 898 ], 5, 1 );
    const_str_plain_throw = UNSTREAM_STRING_ASCII( &constant_bin[ 2168 ], 5, 1 );
    const_str_plain_types = UNSTREAM_STRING_ASCII( &constant_bin[ 2173 ], 5, 1 );
    const_str_plain_format = UNSTREAM_STRING_ASCII( &constant_bin[ 2178 ], 6, 1 );
    const_str_plain_locals = UNSTREAM_STRING_ASCII( &constant_bin[ 728 ], 6, 1 );
    const_str_plain_origin = UNSTREAM_STRING_ASCII( &constant_bin[ 2184 ], 6, 1 );
    const_str_plain_params = UNSTREAM_STRING_ASCII( &constant_bin[ 2190 ], 6, 1 );
    const_str_plain_stddev = UNSTREAM_STRING_ASCII( &constant_bin[ 2196 ], 6, 1 );
    const_str_plain___all__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2202 ], 7, 1 );
    const_str_plain___cmp__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2209 ], 7, 1 );
    const_str_plain___doc__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2216 ], 7, 1 );
    const_str_plain_compile = UNSTREAM_STRING_ASCII( &constant_bin[ 2223 ], 7, 1 );
    const_str_plain_globals = UNSTREAM_STRING_ASCII( &constant_bin[ 2230 ], 7, 1 );
    const_str_plain_inspect = UNSTREAM_STRING_ASCII( &constant_bin[ 2237 ], 7, 1 );
    const_str_plain___dict__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2244 ], 8, 1 );
    const_str_plain___exit__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2252 ], 8, 1 );
    const_str_plain___file__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2260 ], 8, 1 );
    const_str_plain___init__ = UNSTREAM_STRING_ASCII( &constant_bin[ 29 ], 8, 1 );
    const_str_plain___iter__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2268 ], 8, 1 );
    const_str_plain___main__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2276 ], 8, 1 );
    const_str_plain___name__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2284 ], 8, 1 );
    const_str_plain___path__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2292 ], 8, 1 );
    const_str_plain___spec__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2300 ], 8, 1 );
    const_str_plain_fromlist = UNSTREAM_STRING_ASCII( &constant_bin[ 2308 ], 8, 1 );
    const_str_plain_function = UNSTREAM_STRING_ASCII( &constant_bin[ 92 ], 8, 1 );
    const_str_angle_metaclass = UNSTREAM_STRING_ASCII( &constant_bin[ 2316 ], 11, 0 );
    const_str_plain___class__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2327 ], 9, 1 );
    const_str_plain___debug__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2336 ], 9, 1 );
    const_str_plain___enter__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2345 ], 9, 1 );
    const_str_plain_bytearray = UNSTREAM_STRING_ASCII( &constant_bin[ 2354 ], 9, 1 );
    const_str_plain_metaclass = UNSTREAM_STRING_ASCII( &constant_bin[ 2317 ], 9, 1 );
    const_str_plain___cached__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2363 ], 10, 1 );
    const_str_plain___import__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2373 ], 10, 1 );
    const_str_plain___loader__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2383 ], 10, 1 );
    const_str_plain___module__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2393 ], 10, 1 );
    const_str_plain___getitem__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2403 ], 11, 1 );
    const_str_plain___package__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2414 ], 11, 1 );
    const_str_plain___prepare__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2425 ], 11, 1 );
    const_str_plain_classmethod = UNSTREAM_STRING_ASCII( &constant_bin[ 2436 ], 11, 1 );
    const_tuple_int_pos_1_tuple = PyTuple_New( 1 );
    PyTuple_SET_ITEM( const_tuple_int_pos_1_tuple, 0, const_int_pos_1 ); Py_INCREF( const_int_pos_1 );
    const_str_plain___builtins__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2447 ], 12, 1 );
    const_str_plain___compiled__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2459 ], 12, 1 );
    const_str_plain___internal__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2471 ], 12, 1 );
    const_str_plain___qualname__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2483 ], 12, 1 );
    const_str_plain_has_location = UNSTREAM_STRING_ASCII( &constant_bin[ 2495 ], 12, 1 );
    const_str_plain_staticmethod = UNSTREAM_STRING_ASCII( &constant_bin[ 2507 ], 12, 1 );
    const_str_plain___metaclass__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2519 ], 13, 1 );
    const_str_plain__initializing = UNSTREAM_STRING_ASCII( &constant_bin[ 2532 ], 13, 1 );
    const_str_plain___mro_entries__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2545 ], 15, 1 );
    const_str_plain_functionwrapper = UNSTREAM_STRING_ASCII( &constant_bin[ 1630 ], 15, 1 );
    const_str_plain___class_getitem__ = UNSTREAM_STRING_ASCII( &constant_bin[ 2560 ], 17, 1 );
    const_tuple_str_plain___class___tuple = PyTuple_New( 1 );
    PyTuple_SET_ITEM( const_tuple_str_plain___class___tuple, 0, const_str_plain___class__ ); Py_INCREF( const_str_plain___class__ );
    const_str_plain_submodule_search_locations = UNSTREAM_STRING_ASCII( &constant_bin[ 2577 ], 26, 1 );
    const_str_digest_6de5855651d8835e19f0219a160bc8ba = UNSTREAM_STRING_ASCII( &constant_bin[ 2603 ], 71, 0 );
    const_str_digest_75fd71b1edada749c2ef7ac810062295 = UNSTREAM_STRING_ASCII( &constant_bin[ 2674 ], 46, 0 );
    const_str_digest_9eba47f18b2f1b957086ae148e9796b9 = UNSTREAM_STRING_ASCII( &constant_bin[ 2720 ], 11, 0 );

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
