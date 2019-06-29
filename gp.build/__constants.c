
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
PyObject *const_str_plain_c;
PyObject *const_str_plain_n;
PyObject *const_str_plain_x;
PyObject *const_tuple_empty;
PyObject *const_str_plain_rb;
PyObject *const_str_plain_add;
PyObject *const_str_plain_gtw;
PyObject *const_str_plain_ifw;
PyObject *const_str_plain_int;
PyObject *const_str_plain_len;
PyObject *const_str_plain_sum;
PyObject *const_str_plain_addw;
PyObject *const_str_plain_iter;
PyObject *const_str_plain_long;
PyObject *const_str_plain_name;
PyObject *const_str_plain_open;
PyObject *const_str_plain_read;
PyObject *const_str_plain_repr;
PyObject *const_str_plain_self;
PyObject *const_str_plain_site;
PyObject *const_str_plain_subw;
PyObject *const_str_plain_type;
PyObject *const_float_minus_0_0;
PyObject *const_str_plain_flist;
PyObject *const_str_plain_level;
PyObject *const_str_plain_range;
PyObject *const_str_plain_stats;
PyObject *const_str_plain_format;
PyObject *const_str_plain_locals;
PyObject *const_str_plain_params;
PyObject *const_str_plain_stddev;
PyObject *const_str_plain_xrange;
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
PyObject *const_str_plain_exc_type;
PyObject *const_str_plain_fromlist;
PyObject *const_str_plain_function;
PyObject *const_str_plain___class__;
PyObject *const_str_plain___debug__;
PyObject *const_str_plain___enter__;
PyObject *const_str_plain_bytearray;
PyObject *const_str_plain_exc_value;
PyObject *const_str_plain___import__;
PyObject *const_str_plain___module__;
PyObject *const_str_plain___delattr__;
PyObject *const_str_plain___getattr__;
PyObject *const_str_plain___package__;
PyObject *const_str_plain___setattr__;
PyObject *const_str_plain_classmethod;
PyObject *const_tuple_int_pos_1_tuple;
PyObject *const_str_plain___builtins__;
PyObject *const_str_plain___compiled__;
PyObject *const_str_plain_staticmethod;
PyObject *const_str_plain___metaclass__;
PyObject *const_str_plain_exc_traceback;
PyObject *const_str_plain_functionwrapper;
PyObject *const_str_digest_31eae35daa2ffa3077a1b67558489e1b;
PyObject *const_str_digest_7ed9dddfa3e9faf1cadc8ca7ffcfd48c;

static void _createGlobalConstants( void )
{
    NUITKA_MAY_BE_UNUSED PyObject *exception_type, *exception_value;
    NUITKA_MAY_BE_UNUSED PyTracebackObject *exception_tb;

#ifdef _MSC_VER
    // Prevent unused warnings in case of simple programs, the attribute
    // NUITKA_MAY_BE_UNUSED doesn't work for MSVC.
    (void *)exception_type; (void *)exception_value; (void *)exception_tb;
#endif

    const_int_0 = PyInt_FromLong( 0l );
    const_str_dot = UNSTREAM_CHAR( 46, 0 );
    const_float_0_0 = UNSTREAM_FLOAT( &constant_bin[ 2856 ] );
    const_float_0_5 = UNSTREAM_FLOAT( &constant_bin[ 2864 ] );
    const_int_pos_1 = PyInt_FromLong( 1l );
    const_int_pos_2 = PyInt_FromLong( 2l );
    const_int_pos_3 = PyInt_FromLong( 3l );
    const_str_empty = UNSTREAM_STRING( &constant_bin[ 0 ], 0, 0 );
    const_dict_empty = _PyDict_NewPresized( 0 );
    assert( PyDict_Size( const_dict_empty ) == 0 );
    const_str_plain_c = UNSTREAM_CHAR( 99, 1 );
    const_str_plain_n = UNSTREAM_CHAR( 110, 1 );
    const_str_plain_x = UNSTREAM_CHAR( 120, 1 );
    const_tuple_empty = PyTuple_New( 0 );
    const_str_plain_rb = UNSTREAM_STRING( &constant_bin[ 2872 ], 2, 1 );
    const_str_plain_add = UNSTREAM_STRING( &constant_bin[ 2874 ], 3, 1 );
    const_str_plain_gtw = UNSTREAM_STRING( &constant_bin[ 2877 ], 3, 1 );
    const_str_plain_ifw = UNSTREAM_STRING( &constant_bin[ 2880 ], 3, 1 );
    const_str_plain_int = UNSTREAM_STRING( &constant_bin[ 1207 ], 3, 1 );
    const_str_plain_len = UNSTREAM_STRING( &constant_bin[ 2883 ], 3, 1 );
    const_str_plain_sum = UNSTREAM_STRING( &constant_bin[ 2675 ], 3, 1 );
    const_str_plain_addw = UNSTREAM_STRING( &constant_bin[ 2886 ], 4, 1 );
    const_str_plain_iter = UNSTREAM_STRING( &constant_bin[ 2890 ], 4, 1 );
    const_str_plain_long = UNSTREAM_STRING( &constant_bin[ 2894 ], 4, 1 );
    const_str_plain_name = UNSTREAM_STRING( &constant_bin[ 2898 ], 4, 1 );
    const_str_plain_open = UNSTREAM_STRING( &constant_bin[ 2902 ], 4, 1 );
    const_str_plain_read = UNSTREAM_STRING( &constant_bin[ 2906 ], 4, 1 );
    const_str_plain_repr = UNSTREAM_STRING( &constant_bin[ 2910 ], 4, 1 );
    const_str_plain_self = UNSTREAM_STRING( &constant_bin[ 2914 ], 4, 1 );
    const_str_plain_site = UNSTREAM_STRING( &constant_bin[ 2331 ], 4, 1 );
    const_str_plain_subw = UNSTREAM_STRING( &constant_bin[ 2918 ], 4, 1 );
    const_str_plain_type = UNSTREAM_STRING( &constant_bin[ 2922 ], 4, 1 );
    const_float_minus_0_0 = UNSTREAM_FLOAT( &constant_bin[ 2926 ] );
    const_str_plain_flist = UNSTREAM_STRING( &constant_bin[ 2934 ], 5, 1 );
    const_str_plain_level = UNSTREAM_STRING( &constant_bin[ 2939 ], 5, 1 );
    const_str_plain_range = UNSTREAM_STRING( &constant_bin[ 2944 ], 5, 1 );
    const_str_plain_stats = UNSTREAM_STRING( &constant_bin[ 116 ], 5, 1 );
    const_str_plain_format = UNSTREAM_STRING( &constant_bin[ 2949 ], 6, 1 );
    const_str_plain_locals = UNSTREAM_STRING( &constant_bin[ 2955 ], 6, 1 );
    const_str_plain_params = UNSTREAM_STRING( &constant_bin[ 2961 ], 6, 1 );
    const_str_plain_stddev = UNSTREAM_STRING( &constant_bin[ 2967 ], 6, 1 );
    const_str_plain_xrange = UNSTREAM_STRING( &constant_bin[ 2973 ], 6, 1 );
    const_str_plain___all__ = UNSTREAM_STRING( &constant_bin[ 2979 ], 7, 1 );
    const_str_plain___cmp__ = UNSTREAM_STRING( &constant_bin[ 2986 ], 7, 1 );
    const_str_plain___doc__ = UNSTREAM_STRING( &constant_bin[ 2993 ], 7, 1 );
    const_str_plain_compile = UNSTREAM_STRING( &constant_bin[ 3000 ], 7, 1 );
    const_str_plain_globals = UNSTREAM_STRING( &constant_bin[ 3007 ], 7, 1 );
    const_str_plain_inspect = UNSTREAM_STRING( &constant_bin[ 3014 ], 7, 1 );
    const_str_plain___dict__ = UNSTREAM_STRING( &constant_bin[ 3021 ], 8, 1 );
    const_str_plain___exit__ = UNSTREAM_STRING( &constant_bin[ 3029 ], 8, 1 );
    const_str_plain___file__ = UNSTREAM_STRING( &constant_bin[ 3037 ], 8, 1 );
    const_str_plain___init__ = UNSTREAM_STRING( &constant_bin[ 3045 ], 8, 1 );
    const_str_plain___iter__ = UNSTREAM_STRING( &constant_bin[ 3053 ], 8, 1 );
    const_str_plain___main__ = UNSTREAM_STRING( &constant_bin[ 3061 ], 8, 1 );
    const_str_plain___name__ = UNSTREAM_STRING( &constant_bin[ 3069 ], 8, 1 );
    const_str_plain___path__ = UNSTREAM_STRING( &constant_bin[ 3077 ], 8, 1 );
    const_str_plain_exc_type = UNSTREAM_STRING( &constant_bin[ 3085 ], 8, 1 );
    const_str_plain_fromlist = UNSTREAM_STRING( &constant_bin[ 3093 ], 8, 1 );
    const_str_plain_function = UNSTREAM_STRING( &constant_bin[ 54 ], 8, 1 );
    const_str_plain___class__ = UNSTREAM_STRING( &constant_bin[ 3101 ], 9, 1 );
    const_str_plain___debug__ = UNSTREAM_STRING( &constant_bin[ 3110 ], 9, 1 );
    const_str_plain___enter__ = UNSTREAM_STRING( &constant_bin[ 3119 ], 9, 1 );
    const_str_plain_bytearray = UNSTREAM_STRING( &constant_bin[ 3128 ], 9, 1 );
    const_str_plain_exc_value = UNSTREAM_STRING( &constant_bin[ 3137 ], 9, 1 );
    const_str_plain___import__ = UNSTREAM_STRING( &constant_bin[ 3146 ], 10, 1 );
    const_str_plain___module__ = UNSTREAM_STRING( &constant_bin[ 3156 ], 10, 1 );
    const_str_plain___delattr__ = UNSTREAM_STRING( &constant_bin[ 3166 ], 11, 1 );
    const_str_plain___getattr__ = UNSTREAM_STRING( &constant_bin[ 3177 ], 11, 1 );
    const_str_plain___package__ = UNSTREAM_STRING( &constant_bin[ 3188 ], 11, 1 );
    const_str_plain___setattr__ = UNSTREAM_STRING( &constant_bin[ 3199 ], 11, 1 );
    const_str_plain_classmethod = UNSTREAM_STRING( &constant_bin[ 3210 ], 11, 1 );
    const_tuple_int_pos_1_tuple = PyTuple_New( 1 );
    PyTuple_SET_ITEM( const_tuple_int_pos_1_tuple, 0, const_int_pos_1 ); Py_INCREF( const_int_pos_1 );
    const_str_plain___builtins__ = UNSTREAM_STRING( &constant_bin[ 3221 ], 12, 1 );
    const_str_plain___compiled__ = UNSTREAM_STRING( &constant_bin[ 3233 ], 12, 1 );
    const_str_plain_staticmethod = UNSTREAM_STRING( &constant_bin[ 3245 ], 12, 1 );
    const_str_plain___metaclass__ = UNSTREAM_STRING( &constant_bin[ 3257 ], 13, 1 );
    const_str_plain_exc_traceback = UNSTREAM_STRING( &constant_bin[ 3270 ], 13, 1 );
    const_str_plain_functionwrapper = UNSTREAM_STRING( &constant_bin[ 2424 ], 15, 1 );
    const_str_digest_31eae35daa2ffa3077a1b67558489e1b = UNSTREAM_STRING( &constant_bin[ 3283 ], 22, 0 );
    const_str_digest_7ed9dddfa3e9faf1cadc8ca7ffcfd48c = UNSTREAM_STRING( &constant_bin[ 3283 ], 11, 0 );

#if _NUITKA_EXE
    /* Set the "sys.executable" path to the original CPython executable. */
    PySys_SetObject(
        (char *)"executable",
        const_str_digest_31eae35daa2ffa3077a1b67558489e1b
    );

#ifndef _NUITKA_STANDALONE
    /* Set the "sys.prefix" path to the original one. */
    PySys_SetObject(
        (char *)"prefix",
        const_str_digest_7ed9dddfa3e9faf1cadc8ca7ffcfd48c
    );

    /* Set the "sys.prefix" path to the original one. */
    PySys_SetObject(
        (char *)"exec_prefix",
        const_str_digest_7ed9dddfa3e9faf1cadc8ca7ffcfd48c
    );


#if PYTHON_VERSION >= 300
    /* Set the "sys.base_prefix" path to the original one. */
    PySys_SetObject(
        (char *)"base_prefix",
        None
    );

    /* Set the "sys.exec_base_prefix" path to the original one. */
    PySys_SetObject(
        (char *)"base_exec_prefix",
        None
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
