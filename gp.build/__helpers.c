// This file contains helper functions that are automatically created from
// templates.

#include "nuitka/prelude.h"

extern PyObject *callPythonFunction( PyObject *func, PyObject **args, int count );


PyObject *CALL_FUNCTION_WITH_ARGS1( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 1; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 1 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 1; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 1 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 1 * sizeof(PyObject *) );
            memcpy( python_pars + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 1 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 1 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 1; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 1 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 1 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 1 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 1 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (1 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 1 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (1 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 1 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 1, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 1 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            1
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 1 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS2( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 2; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 2 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 2; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 2 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 2 * sizeof(PyObject *) );
            memcpy( python_pars + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 2 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 2 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 2; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 2 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 2 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 2 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 2 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (2 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 2 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (2 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 2 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 2, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 2 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            2
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 2 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS3( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 3; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 3 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 3; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 3 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 3 * sizeof(PyObject *) );
            memcpy( python_pars + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 3 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 3 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 3; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 3 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 3 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 3 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 3 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (3 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 3 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (3 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 3 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 3, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 3 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            3
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 3 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS4( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 4; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 4 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 4; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 4 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 4 * sizeof(PyObject *) );
            memcpy( python_pars + 4, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 4 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 4 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 4; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 4 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 4 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 4, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 4 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 4 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (4 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 4 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (4 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 4 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 4, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 4 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            4
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 4 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS5( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 5; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 5 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 5; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 5 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 5 * sizeof(PyObject *) );
            memcpy( python_pars + 5, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 5 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 5 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 5; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 5 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 5 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 5, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 5 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 5 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (5 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 5 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (5 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 5 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 5, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 5 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            5
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 5 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_METHOD_WITH_ARGS1( PyObject *source, PyObject *attr_name, PyObject **args )
{
    CHECK_OBJECT( source );
    CHECK_OBJECT( attr_name );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 1; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    PyTypeObject *type = Py_TYPE( source );

    if ( type->tp_getattro == PyObject_GenericGetAttr )
    {
        // Unfortunately this is required, although of cause rarely necessary.
        if (unlikely( type->tp_dict == NULL ))
        {
            if (unlikely( PyType_Ready( type ) < 0 ))
            {
                return NULL;
            }
        }

        PyObject *descr = _PyType_Lookup( type, attr_name );
        descrgetfunc func = NULL;

        if ( descr != NULL )
        {
            Py_INCREF( descr );

#if PYTHON_VERSION < 300
            if ( PyType_HasFeature( Py_TYPE( descr ), Py_TPFLAGS_HAVE_CLASS ) )
            {
#endif
                func = Py_TYPE( descr )->tp_descr_get;

                if ( func != NULL && PyDescr_IsData( descr ) )
                {
                    PyObject *called_object = func( descr, source, (PyObject *)type );
                    Py_DECREF( descr );

                    PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                        called_object,
                        args
                    );
                    Py_DECREF( called_object );
                    return result;
                }
#if PYTHON_VERSION < 300
            }
#endif
        }

        Py_ssize_t dictoffset = type->tp_dictoffset;
        PyObject *dict = NULL;

        if ( dictoffset != 0 )
        {
            // Negative dictionary offsets have special meaning.
            if ( dictoffset < 0 )
            {
                Py_ssize_t tsize;
                size_t size;

                tsize = ((PyVarObject *)source)->ob_size;
                if (tsize < 0)
                    tsize = -tsize;
                size = _PyObject_VAR_SIZE( type, tsize );

                dictoffset += (long)size;
            }

            PyObject **dictptr = (PyObject **) ((char *)source + dictoffset);
            dict = *dictptr;
        }

        if ( dict != NULL )
        {
            CHECK_OBJECT( dict );

            Py_INCREF( dict );

            PyObject *called_object = PyDict_GetItem( dict, attr_name );

            if ( called_object != NULL )
            {
                Py_INCREF( called_object );
                Py_XDECREF( descr );
                Py_DECREF( dict );

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF( called_object );
                return result;
            }

            Py_DECREF( dict );
        }

        if ( func != NULL )
        {
            if ( func == Nuitka_Function_Type.tp_descr_get )
            {
                PyObject *result = Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)descr,
                    source,
                    args,
                    1
                );

                Py_DECREF( descr );

                return result;
            }
            else
            {
                PyObject *called_object = func( descr, source, (PyObject *)type );
                CHECK_OBJECT( called_object );

                Py_DECREF( descr );

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF( called_object );

                return result;
            }
        }

        if ( descr != NULL )
        {
            CHECK_OBJECT( descr );

            PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                descr,
                args
            );
            Py_DECREF( descr );

            return result;
        }

#if PYTHON_VERSION < 300
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            PyString_AS_STRING( attr_name )
        );
#else
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%U'",
            type->tp_name,
            attr_name
        );
#endif
        return NULL;
    }
#if PYTHON_VERSION < 300
    else if ( type == &PyInstance_Type )
    {
        PyInstanceObject *source_instance = (PyInstanceObject *)source;

        // The special cases have their own variant on the code generation level
        // as we are called with constants only.
        assert( attr_name != const_str_plain___dict__ );
        assert( attr_name != const_str_plain___class__ );

        // Try the instance dict first.
        PyObject *called_object = GET_STRING_DICT_VALUE(
            (PyDictObject *)source_instance->in_dict,
            (PyStringObject *)attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            return CALL_FUNCTION_WITH_ARGS1( called_object, args );
        }

        // Then check the class dictionaries.
        called_object = FIND_ATTRIBUTE_IN_CLASS(
            source_instance->in_class,
            attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            descrgetfunc descr_get = Py_TYPE( called_object )->tp_descr_get;

            if ( descr_get == Nuitka_Function_Type.tp_descr_get )
            {
                return Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)called_object,
                    source,
                    args,
                    1
                );
            }
            else if ( descr_get != NULL )
            {
                PyObject *method = descr_get(
                    called_object,
                    source,
                    (PyObject *)source_instance->in_class
                );

                if (unlikely( method == NULL ))
                {
                    return NULL;
                }

                PyObject *result = CALL_FUNCTION_WITH_ARGS1( method, args );
                Py_DECREF( method );
                return result;
            }
            else
            {
                return CALL_FUNCTION_WITH_ARGS1( called_object, args );
            }

        }
        else if (unlikely( source_instance->in_class->cl_getattr == NULL ))
        {
            PyErr_Format(
                PyExc_AttributeError,
                "%s instance has no attribute '%s'",
                PyString_AS_STRING( source_instance->in_class->cl_name ),
                PyString_AS_STRING( attr_name )
            );

            return NULL;
        }
        else
        {
            // Finally allow the "__getattr__" override to provide it or else
            // it's an error.

            PyObject *args2[] = {
                source,
                attr_name
            };

            called_object = CALL_FUNCTION_WITH_ARGS2(
                source_instance->in_class->cl_getattr,
                args2
            );

            if (unlikely( called_object == NULL ))
            {
                return NULL;
            }

            PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                called_object,
                args
            );
            Py_DECREF( called_object );
            return result;
        }
    }
#endif
    else if ( type->tp_getattro != NULL )
    {
        PyObject *called_object = (*type->tp_getattro)(
            source,
            attr_name
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else if ( type->tp_getattr != NULL )
    {
        PyObject *called_object = (*type->tp_getattr)(
            source,
            (char *)Nuitka_String_AsString_Unchecked( attr_name )
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else
    {
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            Nuitka_String_AsString_Unchecked( attr_name )
        );

        return NULL;
    }
}

PyObject *CALL_METHOD_WITH_ARGS2( PyObject *source, PyObject *attr_name, PyObject **args )
{
    CHECK_OBJECT( source );
    CHECK_OBJECT( attr_name );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 2; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    PyTypeObject *type = Py_TYPE( source );

    if ( type->tp_getattro == PyObject_GenericGetAttr )
    {
        // Unfortunately this is required, although of cause rarely necessary.
        if (unlikely( type->tp_dict == NULL ))
        {
            if (unlikely( PyType_Ready( type ) < 0 ))
            {
                return NULL;
            }
        }

        PyObject *descr = _PyType_Lookup( type, attr_name );
        descrgetfunc func = NULL;

        if ( descr != NULL )
        {
            Py_INCREF( descr );

#if PYTHON_VERSION < 300
            if ( PyType_HasFeature( Py_TYPE( descr ), Py_TPFLAGS_HAVE_CLASS ) )
            {
#endif
                func = Py_TYPE( descr )->tp_descr_get;

                if ( func != NULL && PyDescr_IsData( descr ) )
                {
                    PyObject *called_object = func( descr, source, (PyObject *)type );
                    Py_DECREF( descr );

                    PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                        called_object,
                        args
                    );
                    Py_DECREF( called_object );
                    return result;
                }
#if PYTHON_VERSION < 300
            }
#endif
        }

        Py_ssize_t dictoffset = type->tp_dictoffset;
        PyObject *dict = NULL;

        if ( dictoffset != 0 )
        {
            // Negative dictionary offsets have special meaning.
            if ( dictoffset < 0 )
            {
                Py_ssize_t tsize;
                size_t size;

                tsize = ((PyVarObject *)source)->ob_size;
                if (tsize < 0)
                    tsize = -tsize;
                size = _PyObject_VAR_SIZE( type, tsize );

                dictoffset += (long)size;
            }

            PyObject **dictptr = (PyObject **) ((char *)source + dictoffset);
            dict = *dictptr;
        }

        if ( dict != NULL )
        {
            CHECK_OBJECT( dict );

            Py_INCREF( dict );

            PyObject *called_object = PyDict_GetItem( dict, attr_name );

            if ( called_object != NULL )
            {
                Py_INCREF( called_object );
                Py_XDECREF( descr );
                Py_DECREF( dict );

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                    called_object,
                    args
                );
                Py_DECREF( called_object );
                return result;
            }

            Py_DECREF( dict );
        }

        if ( func != NULL )
        {
            if ( func == Nuitka_Function_Type.tp_descr_get )
            {
                PyObject *result = Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)descr,
                    source,
                    args,
                    2
                );

                Py_DECREF( descr );

                return result;
            }
            else
            {
                PyObject *called_object = func( descr, source, (PyObject *)type );
                CHECK_OBJECT( called_object );

                Py_DECREF( descr );

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                    called_object,
                    args
                );
                Py_DECREF( called_object );

                return result;
            }
        }

        if ( descr != NULL )
        {
            CHECK_OBJECT( descr );

            PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                descr,
                args
            );
            Py_DECREF( descr );

            return result;
        }

#if PYTHON_VERSION < 300
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            PyString_AS_STRING( attr_name )
        );
#else
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%U'",
            type->tp_name,
            attr_name
        );
#endif
        return NULL;
    }
#if PYTHON_VERSION < 300
    else if ( type == &PyInstance_Type )
    {
        PyInstanceObject *source_instance = (PyInstanceObject *)source;

        // The special cases have their own variant on the code generation level
        // as we are called with constants only.
        assert( attr_name != const_str_plain___dict__ );
        assert( attr_name != const_str_plain___class__ );

        // Try the instance dict first.
        PyObject *called_object = GET_STRING_DICT_VALUE(
            (PyDictObject *)source_instance->in_dict,
            (PyStringObject *)attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            return CALL_FUNCTION_WITH_ARGS2( called_object, args );
        }

        // Then check the class dictionaries.
        called_object = FIND_ATTRIBUTE_IN_CLASS(
            source_instance->in_class,
            attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            descrgetfunc descr_get = Py_TYPE( called_object )->tp_descr_get;

            if ( descr_get == Nuitka_Function_Type.tp_descr_get )
            {
                return Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)called_object,
                    source,
                    args,
                    2
                );
            }
            else if ( descr_get != NULL )
            {
                PyObject *method = descr_get(
                    called_object,
                    source,
                    (PyObject *)source_instance->in_class
                );

                if (unlikely( method == NULL ))
                {
                    return NULL;
                }

                PyObject *result = CALL_FUNCTION_WITH_ARGS2( method, args );
                Py_DECREF( method );
                return result;
            }
            else
            {
                return CALL_FUNCTION_WITH_ARGS2( called_object, args );
            }

        }
        else if (unlikely( source_instance->in_class->cl_getattr == NULL ))
        {
            PyErr_Format(
                PyExc_AttributeError,
                "%s instance has no attribute '%s'",
                PyString_AS_STRING( source_instance->in_class->cl_name ),
                PyString_AS_STRING( attr_name )
            );

            return NULL;
        }
        else
        {
            // Finally allow the "__getattr__" override to provide it or else
            // it's an error.

            PyObject *args2[] = {
                source,
                attr_name
            };

            called_object = CALL_FUNCTION_WITH_ARGS2(
                source_instance->in_class->cl_getattr,
                args2
            );

            if (unlikely( called_object == NULL ))
            {
                return NULL;
            }

            PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                called_object,
                args
            );
            Py_DECREF( called_object );
            return result;
        }
    }
#endif
    else if ( type->tp_getattro != NULL )
    {
        PyObject *called_object = (*type->tp_getattro)(
            source,
            attr_name
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS2(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else if ( type->tp_getattr != NULL )
    {
        PyObject *called_object = (*type->tp_getattr)(
            source,
            (char *)Nuitka_String_AsString_Unchecked( attr_name )
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS2(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else
    {
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            Nuitka_String_AsString_Unchecked( attr_name )
        );

        return NULL;
    }
}
/* Code to register embedded modules for meta path based loading if any. */

#include "nuitka/unfreezing.h"

/* Table for lookup to find compiled or bytecode modules included in this
 * binary or module, or put along this binary as extension modules. We do
 * our own loading for each of these.
 */
MOD_INIT_DECL( __main__ );
MOD_INIT_DECL( functionwrapper );
MOD_INIT_DECL( stats );
static struct Nuitka_MetaPathBasedLoaderEntry meta_path_loader_entries[] =
{
    { "__main__", MOD_INIT_NAME( __main__ ), 0, 0, NUITKA_COMPILED_MODULE },
    { "_bsddb", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_ctypes", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_elementtree", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_hashlib", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_msi", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_multiprocessing", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_socket", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_sqlite3", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_ssl", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_tkinter", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "bz2", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "copy", NULL, 3305, 12056, NUITKA_BYTECODE_FLAG },
    { "functionwrapper", MOD_INIT_NAME( functionwrapper ), 0, 0, NUITKA_COMPILED_MODULE },
    { "pyexpat", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "random", NULL, 15361, 25349, NUITKA_BYTECODE_FLAG },
    { "select", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "stats", MOD_INIT_NAME( stats ), 0, 0, NUITKA_COMPILED_MODULE },
    { "unicodedata", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "BaseHTTPServer", NULL, 40710, 21589, NUITKA_BYTECODE_FLAG },
    { "ConfigParser", NULL, 62299, 24910, NUITKA_BYTECODE_FLAG },
    { "Cookie", NULL, 87209, 22459, NUITKA_BYTECODE_FLAG },
    { "HTMLParser", NULL, 109668, 13566, NUITKA_BYTECODE_FLAG },
    { "Queue", NULL, 123234, 9276, NUITKA_BYTECODE_FLAG },
    { "SimpleHTTPServer", NULL, 132510, 8003, NUITKA_BYTECODE_FLAG },
    { "SimpleXMLRPCServer", NULL, 140513, 22674, NUITKA_BYTECODE_FLAG },
    { "SocketServer", NULL, 163187, 23765, NUITKA_BYTECODE_FLAG },
    { "StringIO", NULL, 186952, 11377, NUITKA_BYTECODE_FLAG },
    { "UserDict", NULL, 198329, 9478, NUITKA_BYTECODE_FLAG },
    { "_LWPCookieJar", NULL, 207807, 5494, NUITKA_BYTECODE_FLAG },
    { "_MozillaCookieJar", NULL, 213301, 4433, NUITKA_BYTECODE_FLAG },
    { "__future__", NULL, 217734, 4254, NUITKA_BYTECODE_FLAG },
    { "_abcoll", NULL, 221988, 25156, NUITKA_BYTECODE_FLAG },
    { "_strptime", NULL, 247144, 14767, NUITKA_BYTECODE_FLAG },
    { "_threading_local", NULL, 261911, 6551, NUITKA_BYTECODE_FLAG },
    { "_weakrefset", NULL, 268462, 9430, NUITKA_BYTECODE_FLAG },
    { "abc", NULL, 277892, 6080, NUITKA_BYTECODE_FLAG },
    { "argparse", NULL, 283972, 63568, NUITKA_BYTECODE_FLAG },
    { "ast", NULL, 347540, 12826, NUITKA_BYTECODE_FLAG },
    { "atexit", NULL, 360366, 2165, NUITKA_BYTECODE_FLAG },
    { "bisect", NULL, 362531, 3038, NUITKA_BYTECODE_FLAG },
    { "calendar", NULL, 365569, 27386, NUITKA_BYTECODE_FLAG },
    { "cgi", NULL, 392955, 32210, NUITKA_BYTECODE_FLAG },
    { "code", NULL, 425165, 10246, NUITKA_BYTECODE_FLAG },
    { "codeop", NULL, 435411, 6539, NUITKA_BYTECODE_FLAG },
    { "collections", NULL, 441950, 25353, NUITKA_BYTECODE_FLAG },
    { "commands", NULL, 467303, 2431, NUITKA_BYTECODE_FLAG },
    { "compileall", NULL, 469734, 6979, NUITKA_BYTECODE_FLAG },
    { "contextlib", NULL, 476713, 4386, NUITKA_BYTECODE_FLAG },
    { "cookielib", NULL, 481099, 54279, NUITKA_BYTECODE_FLAG },
    { "copy", NULL, 3305, 12056, NUITKA_BYTECODE_FLAG },
    { "csv", NULL, 535378, 13367, NUITKA_BYTECODE_FLAG },
    { "ctypes", NULL, 548745, 20015, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes._endian", NULL, 568760, 2269, NUITKA_BYTECODE_FLAG },
    { "ctypes.util", NULL, 571029, 7756, NUITKA_BYTECODE_FLAG },
    { "ctypes.wintypes", NULL, 578785, 5882, NUITKA_BYTECODE_FLAG },
    { "difflib", NULL, 584667, 61671, NUITKA_BYTECODE_FLAG },
    { "dis", NULL, 646338, 6180, NUITKA_BYTECODE_FLAG },
    { "distutils", NULL, 652518, 402, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.archive_util", NULL, 652920, 7408, NUITKA_BYTECODE_FLAG },
    { "distutils.ccompiler", NULL, 660328, 36547, NUITKA_BYTECODE_FLAG },
    { "distutils.cmd", NULL, 696875, 16611, NUITKA_BYTECODE_FLAG },
    { "distutils.command", NULL, 713486, 652, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.command.bdist", NULL, 714138, 5181, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_dumb", NULL, 719319, 5010, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_rpm", NULL, 724329, 17692, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_wininst", NULL, 742021, 10735, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build", NULL, 752756, 5089, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_clib", NULL, 757845, 6375, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_ext", NULL, 764220, 19333, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_py", NULL, 783553, 11420, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_scripts", NULL, 794973, 4491, NUITKA_BYTECODE_FLAG },
    { "distutils.command.clean", NULL, 799464, 3143, NUITKA_BYTECODE_FLAG },
    { "distutils.command.config", NULL, 802607, 12582, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install", NULL, 815189, 16747, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_data", NULL, 831936, 3121, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_headers", NULL, 835057, 2253, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_lib", NULL, 837310, 6716, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_scripts", NULL, 844026, 2955, NUITKA_BYTECODE_FLAG },
    { "distutils.command.sdist", NULL, 846981, 16603, NUITKA_BYTECODE_FLAG },
    { "distutils.config", NULL, 863584, 3527, NUITKA_BYTECODE_FLAG },
    { "distutils.core", NULL, 867111, 7631, NUITKA_BYTECODE_FLAG },
    { "distutils.cygwinccompiler", NULL, 874742, 9757, NUITKA_BYTECODE_FLAG },
    { "distutils.debug", NULL, 884499, 241, NUITKA_BYTECODE_FLAG },
    { "distutils.dep_util", NULL, 884740, 3152, NUITKA_BYTECODE_FLAG },
    { "distutils.dir_util", NULL, 887892, 6743, NUITKA_BYTECODE_FLAG },
    { "distutils.dist", NULL, 894635, 39228, NUITKA_BYTECODE_FLAG },
    { "distutils.errors", NULL, 933863, 6177, NUITKA_BYTECODE_FLAG },
    { "distutils.extension", NULL, 940040, 7384, NUITKA_BYTECODE_FLAG },
    { "distutils.fancy_getopt", NULL, 947424, 11845, NUITKA_BYTECODE_FLAG },
    { "distutils.file_util", NULL, 959269, 6717, NUITKA_BYTECODE_FLAG },
    { "distutils.filelist", NULL, 965986, 10670, NUITKA_BYTECODE_FLAG },
    { "distutils.log", NULL, 976656, 2718, NUITKA_BYTECODE_FLAG },
    { "distutils.msvc9compiler", NULL, 979374, 21355, NUITKA_BYTECODE_FLAG },
    { "distutils.msvccompiler", NULL, 1000729, 17387, NUITKA_BYTECODE_FLAG },
    { "distutils.spawn", NULL, 1018116, 6356, NUITKA_BYTECODE_FLAG },
    { "distutils.sysconfig", NULL, 1024472, 13121, NUITKA_BYTECODE_FLAG },
    { "distutils.text_file", NULL, 1037593, 9196, NUITKA_BYTECODE_FLAG },
    { "distutils.unixccompiler", NULL, 1046789, 7535, NUITKA_BYTECODE_FLAG },
    { "distutils.util", NULL, 1054324, 14279, NUITKA_BYTECODE_FLAG },
    { "distutils.version", NULL, 1068603, 7125, NUITKA_BYTECODE_FLAG },
    { "email", NULL, 1075728, 2834, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email._parseaddr", NULL, 1078562, 13682, NUITKA_BYTECODE_FLAG },
    { "email.base64mime", NULL, 1092244, 5290, NUITKA_BYTECODE_FLAG },
    { "email.charset", NULL, 1097534, 13448, NUITKA_BYTECODE_FLAG },
    { "email.encoders", NULL, 1110982, 2189, NUITKA_BYTECODE_FLAG },
    { "email.errors", NULL, 1113171, 3446, NUITKA_BYTECODE_FLAG },
    { "email.feedparser", NULL, 1116617, 11444, NUITKA_BYTECODE_FLAG },
    { "email.iterators", NULL, 1128061, 2333, NUITKA_BYTECODE_FLAG },
    { "email.message", NULL, 1130394, 28435, NUITKA_BYTECODE_FLAG },
    { "email.mime", NULL, 1158829, 117, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email.parser", NULL, 1158946, 3781, NUITKA_BYTECODE_FLAG },
    { "email.quoprimime", NULL, 1162727, 8777, NUITKA_BYTECODE_FLAG },
    { "email.utils", NULL, 1171504, 9034, NUITKA_BYTECODE_FLAG },
    { "encodings.base64_codec", NULL, 1180538, 3790, NUITKA_BYTECODE_FLAG },
    { "encodings.big5", NULL, 1184328, 1727, NUITKA_BYTECODE_FLAG },
    { "encodings.big5hkscs", NULL, 1186055, 1767, NUITKA_BYTECODE_FLAG },
    { "encodings.bz2_codec", NULL, 1187822, 4670, NUITKA_BYTECODE_FLAG },
    { "encodings.charmap", NULL, 1192492, 3420, NUITKA_BYTECODE_FLAG },
    { "encodings.cp037", NULL, 1195912, 2796, NUITKA_BYTECODE_FLAG },
    { "encodings.cp1006", NULL, 1198708, 2882, NUITKA_BYTECODE_FLAG },
    { "encodings.cp1026", NULL, 1201590, 2810, NUITKA_BYTECODE_FLAG },
    { "encodings.cp1140", NULL, 1204400, 2796, NUITKA_BYTECODE_FLAG },
    { "encodings.cp1250", NULL, 1207196, 2833, NUITKA_BYTECODE_FLAG },
    { "encodings.cp1251", NULL, 1210029, 2830, NUITKA_BYTECODE_FLAG },
    { "encodings.cp1253", NULL, 1212859, 2846, NUITKA_BYTECODE_FLAG },
    { "encodings.cp1254", NULL, 1215705, 2835, NUITKA_BYTECODE_FLAG },
    { "encodings.cp1255", NULL, 1218540, 2854, NUITKA_BYTECODE_FLAG },
    { "encodings.cp1256", NULL, 1221394, 2832, NUITKA_BYTECODE_FLAG },
    { "encodings.cp1257", NULL, 1224226, 2840, NUITKA_BYTECODE_FLAG },
    { "encodings.cp1258", NULL, 1227066, 2838, NUITKA_BYTECODE_FLAG },
    { "encodings.cp424", NULL, 1229904, 2826, NUITKA_BYTECODE_FLAG },
    { "encodings.cp500", NULL, 1232730, 2796, NUITKA_BYTECODE_FLAG },
    { "encodings.cp720", NULL, 1235526, 2893, NUITKA_BYTECODE_FLAG },
    { "encodings.cp737", NULL, 1238419, 8259, NUITKA_BYTECODE_FLAG },
    { "encodings.cp775", NULL, 1246678, 8045, NUITKA_BYTECODE_FLAG },
    { "encodings.cp850", NULL, 1254723, 7778, NUITKA_BYTECODE_FLAG },
    { "encodings.cp852", NULL, 1262501, 8047, NUITKA_BYTECODE_FLAG },
    { "encodings.cp855", NULL, 1270548, 8228, NUITKA_BYTECODE_FLAG },
    { "encodings.cp856", NULL, 1278776, 2858, NUITKA_BYTECODE_FLAG },
    { "encodings.cp857", NULL, 1281634, 7768, NUITKA_BYTECODE_FLAG },
    { "encodings.cp858", NULL, 1289402, 7748, NUITKA_BYTECODE_FLAG },
    { "encodings.cp860", NULL, 1297150, 8014, NUITKA_BYTECODE_FLAG },
    { "encodings.cp861", NULL, 1305164, 8025, NUITKA_BYTECODE_FLAG },
    { "encodings.cp862", NULL, 1313189, 8160, NUITKA_BYTECODE_FLAG },
    { "encodings.cp863", NULL, 1321349, 8025, NUITKA_BYTECODE_FLAG },
    { "encodings.cp864", NULL, 1329374, 8156, NUITKA_BYTECODE_FLAG },
    { "encodings.cp865", NULL, 1337530, 8025, NUITKA_BYTECODE_FLAG },
    { "encodings.cp866", NULL, 1345555, 8260, NUITKA_BYTECODE_FLAG },
    { "encodings.cp869", NULL, 1353815, 8072, NUITKA_BYTECODE_FLAG },
    { "encodings.cp874", NULL, 1361887, 2924, NUITKA_BYTECODE_FLAG },
    { "encodings.cp875", NULL, 1364811, 2793, NUITKA_BYTECODE_FLAG },
    { "encodings.cp932", NULL, 1367604, 1735, NUITKA_BYTECODE_FLAG },
    { "encodings.cp949", NULL, 1369339, 1735, NUITKA_BYTECODE_FLAG },
    { "encodings.cp950", NULL, 1371074, 1735, NUITKA_BYTECODE_FLAG },
    { "encodings.euc_jis_2004", NULL, 1372809, 1791, NUITKA_BYTECODE_FLAG },
    { "encodings.euc_jisx0213", NULL, 1374600, 1791, NUITKA_BYTECODE_FLAG },
    { "encodings.euc_jp", NULL, 1376391, 1743, NUITKA_BYTECODE_FLAG },
    { "encodings.euc_kr", NULL, 1378134, 1743, NUITKA_BYTECODE_FLAG },
    { "encodings.gb18030", NULL, 1379877, 1751, NUITKA_BYTECODE_FLAG },
    { "encodings.gb2312", NULL, 1381628, 1743, NUITKA_BYTECODE_FLAG },
    { "encodings.gbk", NULL, 1383371, 1719, NUITKA_BYTECODE_FLAG },
    { "encodings.hex_codec", NULL, 1385090, 3742, NUITKA_BYTECODE_FLAG },
    { "encodings.hp_roman8", NULL, 1388832, 4079, NUITKA_BYTECODE_FLAG },
    { "encodings.hz", NULL, 1392911, 1711, NUITKA_BYTECODE_FLAG },
    { "encodings.iso2022_jp", NULL, 1394622, 1780, NUITKA_BYTECODE_FLAG },
    { "encodings.iso2022_jp_1", NULL, 1396402, 1796, NUITKA_BYTECODE_FLAG },
    { "encodings.iso2022_jp_2", NULL, 1398198, 1796, NUITKA_BYTECODE_FLAG },
    { "encodings.iso2022_jp_2004", NULL, 1399994, 1820, NUITKA_BYTECODE_FLAG },
    { "encodings.iso2022_jp_3", NULL, 1401814, 1796, NUITKA_BYTECODE_FLAG },
    { "encodings.iso2022_jp_ext", NULL, 1403610, 1812, NUITKA_BYTECODE_FLAG },
    { "encodings.iso2022_kr", NULL, 1405422, 1780, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_1", NULL, 1407202, 2835, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_10", NULL, 1410037, 2850, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_11", NULL, 1412887, 2944, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_13", NULL, 1415831, 2853, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_14", NULL, 1418684, 2871, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_15", NULL, 1421555, 2850, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_16", NULL, 1424405, 2852, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_2", NULL, 1427257, 2835, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_3", NULL, 1430092, 2842, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_4", NULL, 1432934, 2835, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_5", NULL, 1435769, 2836, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_6", NULL, 1438605, 2880, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_7", NULL, 1441485, 2843, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_8", NULL, 1444328, 2874, NUITKA_BYTECODE_FLAG },
    { "encodings.iso8859_9", NULL, 1447202, 2835, NUITKA_BYTECODE_FLAG },
    { "encodings.johab", NULL, 1450037, 1735, NUITKA_BYTECODE_FLAG },
    { "encodings.koi8_r", NULL, 1451772, 2857, NUITKA_BYTECODE_FLAG },
    { "encodings.koi8_u", NULL, 1454629, 2843, NUITKA_BYTECODE_FLAG },
    { "encodings.mac_arabic", NULL, 1457472, 7981, NUITKA_BYTECODE_FLAG },
    { "encodings.mac_centeuro", NULL, 1465453, 2904, NUITKA_BYTECODE_FLAG },
    { "encodings.mac_croatian", NULL, 1468357, 2912, NUITKA_BYTECODE_FLAG },
    { "encodings.mac_cyrillic", NULL, 1471269, 2902, NUITKA_BYTECODE_FLAG },
    { "encodings.mac_farsi", NULL, 1474171, 2816, NUITKA_BYTECODE_FLAG },
    { "encodings.mac_greek", NULL, 1476987, 2856, NUITKA_BYTECODE_FLAG },
    { "encodings.mac_iceland", NULL, 1479843, 2895, NUITKA_BYTECODE_FLAG },
    { "encodings.mac_latin2", NULL, 1482738, 4874, NUITKA_BYTECODE_FLAG },
    { "encodings.mac_roman", NULL, 1487612, 2873, NUITKA_BYTECODE_FLAG },
    { "encodings.mac_romanian", NULL, 1490485, 2913, NUITKA_BYTECODE_FLAG },
    { "encodings.mac_turkish", NULL, 1493398, 2896, NUITKA_BYTECODE_FLAG },
    { "encodings.palmos", NULL, 1496294, 3033, NUITKA_BYTECODE_FLAG },
    { "encodings.ptcp154", NULL, 1499327, 4857, NUITKA_BYTECODE_FLAG },
    { "encodings.punycode", NULL, 1504184, 7911, NUITKA_BYTECODE_FLAG },
    { "encodings.quopri_codec", NULL, 1512095, 3608, NUITKA_BYTECODE_FLAG },
    { "encodings.raw_unicode_escape", NULL, 1515703, 2175, NUITKA_BYTECODE_FLAG },
    { "encodings.rot_13", NULL, 1517878, 3620, NUITKA_BYTECODE_FLAG },
    { "encodings.shift_jis", NULL, 1521498, 1767, NUITKA_BYTECODE_FLAG },
    { "encodings.shift_jis_2004", NULL, 1523265, 1807, NUITKA_BYTECODE_FLAG },
    { "encodings.shift_jisx0213", NULL, 1525072, 1807, NUITKA_BYTECODE_FLAG },
    { "encodings.string_escape", NULL, 1526879, 2034, NUITKA_BYTECODE_FLAG },
    { "encodings.tis_620", NULL, 1528913, 2905, NUITKA_BYTECODE_FLAG },
    { "encodings.undefined", NULL, 1531818, 2556, NUITKA_BYTECODE_FLAG },
    { "encodings.unicode_escape", NULL, 1534374, 2123, NUITKA_BYTECODE_FLAG },
    { "encodings.unicode_internal", NULL, 1536497, 2149, NUITKA_BYTECODE_FLAG },
    { "encodings.utf_16", NULL, 1538646, 5100, NUITKA_BYTECODE_FLAG },
    { "encodings.utf_32", NULL, 1543746, 5658, NUITKA_BYTECODE_FLAG },
    { "encodings.utf_32_le", NULL, 1549404, 1859, NUITKA_BYTECODE_FLAG },
    { "encodings.utf_7", NULL, 1551263, 1859, NUITKA_BYTECODE_FLAG },
    { "encodings.utf_8_sig", NULL, 1553122, 4917, NUITKA_BYTECODE_FLAG },
    { "encodings.uu_codec", NULL, 1558039, 4870, NUITKA_BYTECODE_FLAG },
    { "encodings.zlib_codec", NULL, 1562909, 4590, NUITKA_BYTECODE_FLAG },
    { "fnmatch", NULL, 1567499, 3576, NUITKA_BYTECODE_FLAG },
    { "genericpath", NULL, 1571075, 3454, NUITKA_BYTECODE_FLAG },
    { "getopt", NULL, 1574529, 6596, NUITKA_BYTECODE_FLAG },
    { "getpass", NULL, 1581125, 4701, NUITKA_BYTECODE_FLAG },
    { "gettext", NULL, 1585826, 15364, NUITKA_BYTECODE_FLAG },
    { "glob", NULL, 1601190, 2895, NUITKA_BYTECODE_FLAG },
    { "gzip", NULL, 1604085, 15037, NUITKA_BYTECODE_FLAG },
    { "hashlib", NULL, 1619122, 7014, NUITKA_BYTECODE_FLAG },
    { "heapq", NULL, 1626136, 14467, NUITKA_BYTECODE_FLAG },
    { "hmac", NULL, 1640603, 4484, NUITKA_BYTECODE_FLAG },
    { "htmlentitydefs", NULL, 1645087, 6354, NUITKA_BYTECODE_FLAG },
    { "httplib", NULL, 1651441, 36520, NUITKA_BYTECODE_FLAG },
    { "importlib", NULL, 1687961, 1479, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "inspect", NULL, 1689440, 39729, NUITKA_BYTECODE_FLAG },
    { "io", NULL, 1729169, 3555, NUITKA_BYTECODE_FLAG },
    { "json", NULL, 1732724, 13922, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "json.decoder", NULL, 1746646, 11892, NUITKA_BYTECODE_FLAG },
    { "json.encoder", NULL, 1758538, 13617, NUITKA_BYTECODE_FLAG },
    { "json.scanner", NULL, 1772155, 2206, NUITKA_BYTECODE_FLAG },
    { "keyword", NULL, 1774361, 2087, NUITKA_BYTECODE_FLAG },
    { "linecache", NULL, 1776448, 3234, NUITKA_BYTECODE_FLAG },
    { "logging", NULL, 1779682, 56792, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "logging.config", NULL, 1836474, 25678, NUITKA_BYTECODE_FLAG },
    { "logging.handlers", NULL, 1862152, 38961, NUITKA_BYTECODE_FLAG },
    { "markupbase", NULL, 1901113, 9224, NUITKA_BYTECODE_FLAG },
    { "mimetools", NULL, 1910337, 8116, NUITKA_BYTECODE_FLAG },
    { "mimetypes", NULL, 1918453, 18196, NUITKA_BYTECODE_FLAG },
    { "multiprocessing", NULL, 1936649, 8327, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.process", NULL, 1944976, 9357, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.util", NULL, 1954333, 9982, NUITKA_BYTECODE_FLAG },
    { "netrc", NULL, 1964315, 4619, NUITKA_BYTECODE_FLAG },
    { "ntpath", NULL, 1968934, 13012, NUITKA_BYTECODE_FLAG },
    { "nturl2path", NULL, 1981946, 1792, NUITKA_BYTECODE_FLAG },
    { "numbers", NULL, 1983738, 13694, NUITKA_BYTECODE_FLAG },
    { "opcode", NULL, 1997432, 6119, NUITKA_BYTECODE_FLAG },
    { "optparse", NULL, 2003551, 53423, NUITKA_BYTECODE_FLAG },
    { "os", NULL, 2056974, 25426, NUITKA_BYTECODE_FLAG },
    { "pickle", NULL, 2082400, 38030, NUITKA_BYTECODE_FLAG },
    { "pkgutil", NULL, 2120430, 18760, NUITKA_BYTECODE_FLAG },
    { "platform", NULL, 2139190, 36708, NUITKA_BYTECODE_FLAG },
    { "plistlib", NULL, 2175898, 18966, NUITKA_BYTECODE_FLAG },
    { "posixpath", NULL, 2194864, 11297, NUITKA_BYTECODE_FLAG },
    { "pprint", NULL, 2206161, 10091, NUITKA_BYTECODE_FLAG },
    { "py_compile", NULL, 2216252, 6385, NUITKA_BYTECODE_FLAG },
    { "random", NULL, 15361, 25349, NUITKA_BYTECODE_FLAG },
    { "rfc822", NULL, 2222637, 31542, NUITKA_BYTECODE_FLAG },
    { "runpy", NULL, 2254179, 8300, NUITKA_BYTECODE_FLAG },
    { "shlex", NULL, 2262479, 7470, NUITKA_BYTECODE_FLAG },
    { "shutil", NULL, 2269949, 18457, NUITKA_BYTECODE_FLAG },
    { "site", NULL, 2288406, 16407, NUITKA_BYTECODE_FLAG },
    { "socket", NULL, 2304813, 15995, NUITKA_BYTECODE_FLAG },
    { "ssl", NULL, 2320808, 31291, NUITKA_BYTECODE_FLAG },
    { "stat", NULL, 2352099, 2693, NUITKA_BYTECODE_FLAG },
    { "subprocess", NULL, 2354792, 41664, NUITKA_BYTECODE_FLAG },
    { "symbol", NULL, 2396456, 3008, NUITKA_BYTECODE_FLAG },
    { "sysconfig", NULL, 2399464, 17413, NUITKA_BYTECODE_FLAG },
    { "tarfile", NULL, 2416877, 75023, NUITKA_BYTECODE_FLAG },
    { "tempfile", NULL, 2491900, 20020, NUITKA_BYTECODE_FLAG },
    { "textwrap", NULL, 2511920, 11954, NUITKA_BYTECODE_FLAG },
    { "threading", NULL, 2523874, 42119, NUITKA_BYTECODE_FLAG },
    { "timeit", NULL, 2565993, 12110, NUITKA_BYTECODE_FLAG },
    { "token", NULL, 2578103, 3783, NUITKA_BYTECODE_FLAG },
    { "tokenize", NULL, 2581886, 14417, NUITKA_BYTECODE_FLAG },
    { "trace", NULL, 2596303, 22614, NUITKA_BYTECODE_FLAG },
    { "traceback", NULL, 2618917, 11571, NUITKA_BYTECODE_FLAG },
    { "tty", NULL, 2630488, 1294, NUITKA_BYTECODE_FLAG },
    { "unittest", NULL, 2631782, 2951, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "unittest.case", NULL, 2634733, 39878, NUITKA_BYTECODE_FLAG },
    { "unittest.loader", NULL, 2674611, 11262, NUITKA_BYTECODE_FLAG },
    { "unittest.main", NULL, 2685873, 7958, NUITKA_BYTECODE_FLAG },
    { "unittest.result", NULL, 2693831, 7796, NUITKA_BYTECODE_FLAG },
    { "unittest.runner", NULL, 2701627, 7536, NUITKA_BYTECODE_FLAG },
    { "unittest.signals", NULL, 2709163, 2708, NUITKA_BYTECODE_FLAG },
    { "unittest.suite", NULL, 2711871, 10355, NUITKA_BYTECODE_FLAG },
    { "unittest.util", NULL, 2722226, 4470, NUITKA_BYTECODE_FLAG },
    { "urllib", NULL, 2726696, 49965, NUITKA_BYTECODE_FLAG },
    { "urllib2", NULL, 2776661, 46607, NUITKA_BYTECODE_FLAG },
    { "urlparse", NULL, 2823268, 14379, NUITKA_BYTECODE_FLAG },
    { "uu", NULL, 2837647, 4279, NUITKA_BYTECODE_FLAG },
    { "uuid", NULL, 2841926, 22344, NUITKA_BYTECODE_FLAG },
    { "warnings", NULL, 2864270, 13501, NUITKA_BYTECODE_FLAG },
    { "weakref", NULL, 2877771, 15574, NUITKA_BYTECODE_FLAG },
    { "xml", NULL, 2893345, 1065, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.etree", NULL, 2894410, 116, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.etree.ElementPath", NULL, 2894526, 7485, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementTree", NULL, 2902011, 34351, NUITKA_BYTECODE_FLAG },
    { "xml.etree.cElementTree", NULL, 2936362, 163, NUITKA_BYTECODE_FLAG },
    { "xml.parsers", NULL, 2936525, 301, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.parsers.expat", NULL, 2936826, 274, NUITKA_BYTECODE_FLAG },
    { "xml.sax", NULL, 2937100, 3661, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.sax._exceptions", NULL, 2940761, 6076, NUITKA_BYTECODE_FLAG },
    { "xml.sax.handler", NULL, 2946837, 12895, NUITKA_BYTECODE_FLAG },
    { "xml.sax.saxutils", NULL, 2959732, 14523, NUITKA_BYTECODE_FLAG },
    { "xml.sax.xmlreader", NULL, 2974255, 18874, NUITKA_BYTECODE_FLAG },
    { "xmlrpclib", NULL, 2993129, 43272, NUITKA_BYTECODE_FLAG },
    { "zipfile", NULL, 3036401, 41423, NUITKA_BYTECODE_FLAG },
    { NULL, NULL, 0, 0, 0 }
};

void setupMetaPathBasedLoader( void )
{
    static bool init_done = false;

    if ( init_done == false )
    {
        registerMetaPathBasedUnfreezer( meta_path_loader_entries );
        init_done = true;
    }
}
