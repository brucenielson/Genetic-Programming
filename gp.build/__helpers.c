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

PyObject *CALL_FUNCTION_WITH_ARGS6( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 6; i++ )
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

        if ( function->m_args_simple && 6 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 6; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 6 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 6 * sizeof(PyObject *) );
            memcpy( python_pars + 6, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

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

            if ( parseArgumentsPos( function, python_pars, args, 6 ))
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

            if ( function->m_args_simple && 6 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 6; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 6 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 6 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 6, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

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

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 6 ) )
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
#if 6 == 0
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
                "%s() takes no arguments (6 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 6 == 1
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
                "%s() takes exactly one argument (6 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 6 );

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
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 6, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 6 );;
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
            6
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 6 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS8( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 8; i++ )
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

        if ( function->m_args_simple && 8 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 8; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 8 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 8 * sizeof(PyObject *) );
            memcpy( python_pars + 8, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

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

            if ( parseArgumentsPos( function, python_pars, args, 8 ))
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

            if ( function->m_args_simple && 8 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 8; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 8 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 8 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 8, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

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

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 8 ) )
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
#if 8 == 0
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
                "%s() takes no arguments (8 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_O )
        {
#if 8 == 1
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
                "%s() takes exactly one argument (8 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 8 );

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
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 8, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 8 );;
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
            8
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 8 );

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
    { "_asyncio", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_bz2", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_ctypes", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_decimal", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_elementtree", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_hashlib", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_lzma", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_msi", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_multiprocessing", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_overlapped", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_queue", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_socket", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_sqlite3", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_ssl", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_tkinter", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "copy", NULL, 2731, 7064, NUITKA_BYTECODE_FLAG },
    { "datetime", NULL, 9795, 57212, NUITKA_BYTECODE_FLAG },
    { "functionwrapper", MOD_INIT_NAME( functionwrapper ), 0, 0, NUITKA_COMPILED_MODULE },
    { "operator", NULL, 67007, 13863, NUITKA_BYTECODE_FLAG },
    { "pyexpat", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "random", NULL, 80870, 19371, NUITKA_BYTECODE_FLAG },
    { "select", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "site", NULL, 100241, 13219, NUITKA_BYTECODE_FLAG },
    { "stats", MOD_INIT_NAME( stats ), 0, 0, NUITKA_COMPILED_MODULE },
    { "unicodedata", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "__future__", NULL, 113460, 4095, NUITKA_BYTECODE_FLAG },
    { "_bootlocale", NULL, 117555, 1212, NUITKA_BYTECODE_FLAG },
    { "_compat_pickle", NULL, 118767, 5771, NUITKA_BYTECODE_FLAG },
    { "_dummy_thread", NULL, 124538, 4816, NUITKA_BYTECODE_FLAG },
    { "_markupbase", NULL, 129354, 7748, NUITKA_BYTECODE_FLAG },
    { "_osx_support", NULL, 137102, 9558, NUITKA_BYTECODE_FLAG },
    { "_py_abc", NULL, 146660, 4617, NUITKA_BYTECODE_FLAG },
    { "_pyio", NULL, 151277, 72781, NUITKA_BYTECODE_FLAG },
    { "_sitebuiltins", NULL, 224058, 3428, NUITKA_BYTECODE_FLAG },
    { "_strptime", NULL, 227486, 16067, NUITKA_BYTECODE_FLAG },
    { "_threading_local", NULL, 243553, 6375, NUITKA_BYTECODE_FLAG },
    { "aifc", NULL, 249928, 26106, NUITKA_BYTECODE_FLAG },
    { "argparse", NULL, 276034, 61876, NUITKA_BYTECODE_FLAG },
    { "ast", NULL, 337910, 12043, NUITKA_BYTECODE_FLAG },
    { "asynchat", NULL, 349953, 6797, NUITKA_BYTECODE_FLAG },
    { "asyncio", NULL, 356750, 653, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "asyncio.base_events", NULL, 357403, 48079, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_futures", NULL, 405482, 2065, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_subprocess", NULL, 407547, 9154, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_tasks", NULL, 416701, 1829, NUITKA_BYTECODE_FLAG },
    { "asyncio.constants", NULL, 418530, 554, NUITKA_BYTECODE_FLAG },
    { "asyncio.coroutines", NULL, 419084, 6341, NUITKA_BYTECODE_FLAG },
    { "asyncio.events", NULL, 425425, 27818, NUITKA_BYTECODE_FLAG },
    { "asyncio.format_helpers", NULL, 453243, 2280, NUITKA_BYTECODE_FLAG },
    { "asyncio.futures", NULL, 455523, 10708, NUITKA_BYTECODE_FLAG },
    { "asyncio.locks", NULL, 466231, 15876, NUITKA_BYTECODE_FLAG },
    { "asyncio.log", NULL, 482107, 203, NUITKA_BYTECODE_FLAG },
    { "asyncio.proactor_events", NULL, 482310, 20074, NUITKA_BYTECODE_FLAG },
    { "asyncio.protocols", NULL, 502384, 8691, NUITKA_BYTECODE_FLAG },
    { "asyncio.queues", NULL, 511075, 8136, NUITKA_BYTECODE_FLAG },
    { "asyncio.runners", NULL, 519211, 1877, NUITKA_BYTECODE_FLAG },
    { "asyncio.selector_events", NULL, 521088, 28086, NUITKA_BYTECODE_FLAG },
    { "asyncio.sslproto", NULL, 549174, 21174, NUITKA_BYTECODE_FLAG },
    { "asyncio.streams", NULL, 570348, 20251, NUITKA_BYTECODE_FLAG },
    { "asyncio.subprocess", NULL, 590599, 6715, NUITKA_BYTECODE_FLAG },
    { "asyncio.tasks", NULL, 597314, 21829, NUITKA_BYTECODE_FLAG },
    { "asyncio.transports", NULL, 619143, 12174, NUITKA_BYTECODE_FLAG },
    { "asyncio.windows_events", NULL, 631317, 23031, NUITKA_BYTECODE_FLAG },
    { "asyncio.windows_utils", NULL, 654348, 4364, NUITKA_BYTECODE_FLAG },
    { "asyncore", NULL, 658712, 15807, NUITKA_BYTECODE_FLAG },
    { "bdb", NULL, 674519, 24357, NUITKA_BYTECODE_FLAG },
    { "binhex", NULL, 698876, 12022, NUITKA_BYTECODE_FLAG },
    { "bisect", NULL, 710898, 2661, NUITKA_BYTECODE_FLAG },
    { "cProfile", NULL, 713559, 4514, NUITKA_BYTECODE_FLAG },
    { "calendar", NULL, 718073, 27387, NUITKA_BYTECODE_FLAG },
    { "cgi", NULL, 745460, 27154, NUITKA_BYTECODE_FLAG },
    { "cgitb", NULL, 772614, 10075, NUITKA_BYTECODE_FLAG },
    { "chunk", NULL, 782689, 4882, NUITKA_BYTECODE_FLAG },
    { "cmd", NULL, 787571, 12553, NUITKA_BYTECODE_FLAG },
    { "code", NULL, 800124, 9821, NUITKA_BYTECODE_FLAG },
    { "codeop", NULL, 809945, 6253, NUITKA_BYTECODE_FLAG },
    { "colorsys", NULL, 816198, 3260, NUITKA_BYTECODE_FLAG },
    { "compileall", NULL, 819458, 9069, NUITKA_BYTECODE_FLAG },
    { "concurrent", NULL, 828527, 109, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "concurrent.futures", NULL, 828636, 1049, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "concurrent.futures._base", NULL, 829685, 20846, NUITKA_BYTECODE_FLAG },
    { "concurrent.futures.process", NULL, 850531, 19524, NUITKA_BYTECODE_FLAG },
    { "concurrent.futures.thread", NULL, 870055, 5137, NUITKA_BYTECODE_FLAG },
    { "configparser", NULL, 875192, 45843, NUITKA_BYTECODE_FLAG },
    { "contextlib", NULL, 921035, 19863, NUITKA_BYTECODE_FLAG },
    { "contextvars", NULL, 940898, 220, NUITKA_BYTECODE_FLAG },
    { "copy", NULL, 2731, 7064, NUITKA_BYTECODE_FLAG },
    { "csv", NULL, 941118, 11795, NUITKA_BYTECODE_FLAG },
    { "ctypes", NULL, 952913, 16112, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes._aix", NULL, 969025, 9786, NUITKA_BYTECODE_FLAG },
    { "ctypes._endian", NULL, 978811, 1919, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib", NULL, 980730, 276, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes.macholib.dyld", NULL, 981006, 4309, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib.dylib", NULL, 985315, 1903, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib.framework", NULL, 987218, 2183, NUITKA_BYTECODE_FLAG },
    { "ctypes.util", NULL, 989401, 7712, NUITKA_BYTECODE_FLAG },
    { "ctypes.wintypes", NULL, 997113, 5080, NUITKA_BYTECODE_FLAG },
    { "dataclasses", NULL, 1002193, 22428, NUITKA_BYTECODE_FLAG },
    { "datetime", NULL, 9795, 57212, NUITKA_BYTECODE_FLAG },
    { "dbm", NULL, 1024621, 4129, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "dbm.dumb", NULL, 1028750, 8363, NUITKA_BYTECODE_FLAG },
    { "decimal", NULL, 1037113, 162135, NUITKA_BYTECODE_FLAG },
    { "difflib", NULL, 1199248, 59401, NUITKA_BYTECODE_FLAG },
    { "distutils", NULL, 1258649, 361, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils._msvccompiler", NULL, 1259010, 13581, NUITKA_BYTECODE_FLAG },
    { "distutils.archive_util", NULL, 1272591, 6495, NUITKA_BYTECODE_FLAG },
    { "distutils.bcppcompiler", NULL, 1279086, 6464, NUITKA_BYTECODE_FLAG },
    { "distutils.ccompiler", NULL, 1285550, 33179, NUITKA_BYTECODE_FLAG },
    { "distutils.cmd", NULL, 1318729, 13871, NUITKA_BYTECODE_FLAG },
    { "distutils.command", NULL, 1332600, 518, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.command.bdist", NULL, 1333118, 3617, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_dumb", NULL, 1336735, 3532, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_msi", NULL, 1340267, 19364, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_rpm", NULL, 1359631, 12457, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_wininst", NULL, 1372088, 7972, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build", NULL, 1380060, 3798, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_clib", NULL, 1383858, 4848, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_ext", NULL, 1388706, 15755, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_py", NULL, 1404461, 10354, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_scripts", NULL, 1414815, 4253, NUITKA_BYTECODE_FLAG },
    { "distutils.command.check", NULL, 1419068, 4773, NUITKA_BYTECODE_FLAG },
    { "distutils.command.clean", NULL, 1423841, 2055, NUITKA_BYTECODE_FLAG },
    { "distutils.command.config", NULL, 1425896, 10159, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install", NULL, 1436055, 13482, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_data", NULL, 1449537, 2248, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_egg_info", NULL, 1451785, 2946, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_headers", NULL, 1454731, 1663, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_lib", NULL, 1456394, 5041, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_scripts", NULL, 1461435, 2105, NUITKA_BYTECODE_FLAG },
    { "distutils.command.register", NULL, 1463540, 8440, NUITKA_BYTECODE_FLAG },
    { "distutils.command.sdist", NULL, 1471980, 14471, NUITKA_BYTECODE_FLAG },
    { "distutils.command.upload", NULL, 1486451, 5053, NUITKA_BYTECODE_FLAG },
    { "distutils.config", NULL, 1491504, 3448, NUITKA_BYTECODE_FLAG },
    { "distutils.core", NULL, 1494952, 6573, NUITKA_BYTECODE_FLAG },
    { "distutils.cygwinccompiler", NULL, 1501525, 8491, NUITKA_BYTECODE_FLAG },
    { "distutils.debug", NULL, 1510016, 171, NUITKA_BYTECODE_FLAG },
    { "distutils.dep_util", NULL, 1510187, 2687, NUITKA_BYTECODE_FLAG },
    { "distutils.dir_util", NULL, 1512874, 5781, NUITKA_BYTECODE_FLAG },
    { "distutils.dist", NULL, 1518655, 34402, NUITKA_BYTECODE_FLAG },
    { "distutils.errors", NULL, 1553057, 5457, NUITKA_BYTECODE_FLAG },
    { "distutils.extension", NULL, 1558514, 6868, NUITKA_BYTECODE_FLAG },
    { "distutils.fancy_getopt", NULL, 1565382, 10580, NUITKA_BYTECODE_FLAG },
    { "distutils.file_util", NULL, 1575962, 5866, NUITKA_BYTECODE_FLAG },
    { "distutils.filelist", NULL, 1581828, 9801, NUITKA_BYTECODE_FLAG },
    { "distutils.log", NULL, 1591629, 2282, NUITKA_BYTECODE_FLAG },
    { "distutils.msvc9compiler", NULL, 1593911, 17346, NUITKA_BYTECODE_FLAG },
    { "distutils.msvccompiler", NULL, 1611257, 14534, NUITKA_BYTECODE_FLAG },
    { "distutils.spawn", NULL, 1625791, 4972, NUITKA_BYTECODE_FLAG },
    { "distutils.sysconfig", NULL, 1630763, 11946, NUITKA_BYTECODE_FLAG },
    { "distutils.text_file", NULL, 1642709, 8408, NUITKA_BYTECODE_FLAG },
    { "distutils.unixccompiler", NULL, 1651117, 6503, NUITKA_BYTECODE_FLAG },
    { "distutils.util", NULL, 1657620, 15064, NUITKA_BYTECODE_FLAG },
    { "distutils.version", NULL, 1672684, 7319, NUITKA_BYTECODE_FLAG },
    { "distutils.versionpredicate", NULL, 1680003, 5066, NUITKA_BYTECODE_FLAG },
    { "doctest", NULL, 1685069, 75397, NUITKA_BYTECODE_FLAG },
    { "dummy_threading", NULL, 1760466, 1087, NUITKA_BYTECODE_FLAG },
    { "email", NULL, 1761553, 1654, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email._encoded_words", NULL, 1763207, 5571, NUITKA_BYTECODE_FLAG },
    { "email._header_value_parser", NULL, 1768778, 75593, NUITKA_BYTECODE_FLAG },
    { "email._parseaddr", NULL, 1844371, 12311, NUITKA_BYTECODE_FLAG },
    { "email._policybase", NULL, 1856682, 14813, NUITKA_BYTECODE_FLAG },
    { "email.base64mime", NULL, 1871495, 3198, NUITKA_BYTECODE_FLAG },
    { "email.charset", NULL, 1874693, 11492, NUITKA_BYTECODE_FLAG },
    { "email.contentmanager", NULL, 1886185, 7258, NUITKA_BYTECODE_FLAG },
    { "email.encoders", NULL, 1893443, 1627, NUITKA_BYTECODE_FLAG },
    { "email.errors", NULL, 1895070, 6154, NUITKA_BYTECODE_FLAG },
    { "email.feedparser", NULL, 1901224, 10588, NUITKA_BYTECODE_FLAG },
    { "email.generator", NULL, 1911812, 12463, NUITKA_BYTECODE_FLAG },
    { "email.header", NULL, 1924275, 16343, NUITKA_BYTECODE_FLAG },
    { "email.headerregistry", NULL, 1940618, 21260, NUITKA_BYTECODE_FLAG },
    { "email.iterators", NULL, 1961878, 1895, NUITKA_BYTECODE_FLAG },
    { "email.message", NULL, 1963773, 37903, NUITKA_BYTECODE_FLAG },
    { "email.mime", NULL, 2001676, 109, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email.mime.application", NULL, 2001785, 1420, NUITKA_BYTECODE_FLAG },
    { "email.mime.audio", NULL, 2003205, 2579, NUITKA_BYTECODE_FLAG },
    { "email.mime.base", NULL, 2005784, 1045, NUITKA_BYTECODE_FLAG },
    { "email.mime.image", NULL, 2006829, 1865, NUITKA_BYTECODE_FLAG },
    { "email.mime.message", NULL, 2008694, 1294, NUITKA_BYTECODE_FLAG },
    { "email.mime.multipart", NULL, 2009988, 1516, NUITKA_BYTECODE_FLAG },
    { "email.mime.nonmultipart", NULL, 2011504, 731, NUITKA_BYTECODE_FLAG },
    { "email.mime.text", NULL, 2012235, 1278, NUITKA_BYTECODE_FLAG },
    { "email.parser", NULL, 2013513, 5710, NUITKA_BYTECODE_FLAG },
    { "email.policy", NULL, 2019223, 9603, NUITKA_BYTECODE_FLAG },
    { "email.quoprimime", NULL, 2028826, 7627, NUITKA_BYTECODE_FLAG },
    { "email.utils", NULL, 2036453, 9430, NUITKA_BYTECODE_FLAG },
    { "filecmp", NULL, 2045883, 8270, NUITKA_BYTECODE_FLAG },
    { "fileinput", NULL, 2054153, 13161, NUITKA_BYTECODE_FLAG },
    { "fnmatch", NULL, 2067314, 3300, NUITKA_BYTECODE_FLAG },
    { "formatter", NULL, 2070614, 17516, NUITKA_BYTECODE_FLAG },
    { "fractions", NULL, 2088130, 18391, NUITKA_BYTECODE_FLAG },
    { "ftplib", NULL, 2106521, 28030, NUITKA_BYTECODE_FLAG },
    { "getopt", NULL, 2134551, 6202, NUITKA_BYTECODE_FLAG },
    { "getpass", NULL, 2140753, 4127, NUITKA_BYTECODE_FLAG },
    { "gettext", NULL, 2144880, 14131, NUITKA_BYTECODE_FLAG },
    { "glob", NULL, 2159011, 4222, NUITKA_BYTECODE_FLAG },
    { "gzip", NULL, 2163233, 17148, NUITKA_BYTECODE_FLAG },
    { "hashlib", NULL, 2180381, 6554, NUITKA_BYTECODE_FLAG },
    { "hmac", NULL, 2186935, 6076, NUITKA_BYTECODE_FLAG },
    { "html", NULL, 2193011, 3360, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "html.entities", NULL, 2196371, 50432, NUITKA_BYTECODE_FLAG },
    { "html.parser", NULL, 2246803, 11070, NUITKA_BYTECODE_FLAG },
    { "http", NULL, 2257873, 5885, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "http.client", NULL, 2263758, 34011, NUITKA_BYTECODE_FLAG },
    { "http.cookiejar", NULL, 2297769, 53548, NUITKA_BYTECODE_FLAG },
    { "http.cookies", NULL, 2351317, 15207, NUITKA_BYTECODE_FLAG },
    { "http.server", NULL, 2366524, 33330, NUITKA_BYTECODE_FLAG },
    { "imaplib", NULL, 2399854, 41393, NUITKA_BYTECODE_FLAG },
    { "imghdr", NULL, 2441247, 4105, NUITKA_BYTECODE_FLAG },
    { "imp", NULL, 2445352, 9716, NUITKA_BYTECODE_FLAG },
    { "importlib.abc", NULL, 2455068, 13450, NUITKA_BYTECODE_FLAG },
    { "importlib.resources", NULL, 2468518, 8302, NUITKA_BYTECODE_FLAG },
    { "importlib.util", NULL, 2476820, 9319, NUITKA_BYTECODE_FLAG },
    { "ipaddress", NULL, 2486139, 62957, NUITKA_BYTECODE_FLAG },
    { "json", NULL, 2549096, 12301, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "json.decoder", NULL, 2561397, 9787, NUITKA_BYTECODE_FLAG },
    { "json.encoder", NULL, 2571184, 11269, NUITKA_BYTECODE_FLAG },
    { "json.scanner", NULL, 2582453, 1959, NUITKA_BYTECODE_FLAG },
    { "json.tool", NULL, 2584412, 1440, NUITKA_BYTECODE_FLAG },
    { "lib2to3", NULL, 2585852, 106, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.btm_matcher", NULL, 2585958, 4856, NUITKA_BYTECODE_FLAG },
    { "lib2to3.btm_utils", NULL, 2590814, 6106, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_base", NULL, 2596920, 6198, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_util", NULL, 2603118, 12009, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes", NULL, 2615127, 112, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.fixes.fix_apply", NULL, 2615239, 1650, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_asserts", NULL, 2616889, 1235, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_basestring", NULL, 2618124, 625, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_buffer", NULL, 2618749, 770, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_dict", NULL, 2619519, 3278, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_except", NULL, 2622797, 2780, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exec", NULL, 2625577, 1111, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_execfile", NULL, 2626688, 1655, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exitfunc", NULL, 2628343, 2258, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_filter", NULL, 2630601, 2323, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_funcattrs", NULL, 2632924, 936, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_future", NULL, 2633860, 746, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_getcwdu", NULL, 2634606, 750, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_has_key", NULL, 2635356, 2880, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_idioms", NULL, 2638236, 3865, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_import", NULL, 2642101, 2747, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports", NULL, 2644848, 4311, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports2", NULL, 2649159, 510, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_input", NULL, 2649669, 912, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_intern", NULL, 2650581, 1119, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_isinstance", NULL, 2651700, 1517, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools", NULL, 2653217, 1506, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools_imports", NULL, 2654723, 1542, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_long", NULL, 2656265, 667, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_map", NULL, 2656932, 3055, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_metaclass", NULL, 2659987, 5308, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_methodattrs", NULL, 2665295, 898, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ne", NULL, 2666193, 769, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_next", NULL, 2666962, 3022, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_nonzero", NULL, 2669984, 885, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_numliterals", NULL, 2670869, 981, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_operator", NULL, 2671850, 4198, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_paren", NULL, 2676048, 1352, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_print", NULL, 2677400, 2287, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raise", NULL, 2679687, 2211, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raw_input", NULL, 2681898, 757, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_reduce", NULL, 2682655, 1092, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_reload", NULL, 2683747, 1131, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_renames", NULL, 2684878, 1955, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_repr", NULL, 2686833, 807, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_set_literal", NULL, 2687640, 1639, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_standarderror", NULL, 2689279, 682, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_sys_exc", NULL, 2689961, 1363, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_throw", NULL, 2691324, 1764, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_tuple_params", NULL, 2693088, 4558, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_types", NULL, 2697646, 1791, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_unicode", NULL, 2699437, 1503, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_urllib", NULL, 2700940, 5923, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ws_comma", NULL, 2706863, 1085, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xrange", NULL, 2707948, 2498, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xreadlines", NULL, 2710446, 1079, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_zip", NULL, 2711525, 1543, NUITKA_BYTECODE_FLAG },
    { "lib2to3.main", NULL, 2713068, 8499, NUITKA_BYTECODE_FLAG },
    { "lib2to3.patcomp", NULL, 2721567, 5574, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2", NULL, 2727141, 142, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.pgen2.driver", NULL, 2727283, 5103, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.grammar", NULL, 2732386, 6979, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.literals", NULL, 2739365, 1521, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.parse", NULL, 2740886, 6267, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.pgen", NULL, 2747153, 9743, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.token", NULL, 2756896, 1835, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.tokenize", NULL, 2758731, 15103, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pygram", NULL, 2773834, 1161, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pytree", NULL, 2774995, 24966, NUITKA_BYTECODE_FLAG },
    { "lib2to3.refactor", NULL, 2799961, 20367, NUITKA_BYTECODE_FLAG },
    { "logging", NULL, 2820328, 62521, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "logging.config", NULL, 2882849, 22978, NUITKA_BYTECODE_FLAG },
    { "logging.handlers", NULL, 2905827, 42946, NUITKA_BYTECODE_FLAG },
    { "lzma", NULL, 2948773, 11902, NUITKA_BYTECODE_FLAG },
    { "macpath", NULL, 2960675, 5770, NUITKA_BYTECODE_FLAG },
    { "mailbox", NULL, 2966445, 63611, NUITKA_BYTECODE_FLAG },
    { "mailcap", NULL, 3030056, 6447, NUITKA_BYTECODE_FLAG },
    { "mimetypes", NULL, 3036503, 15441, NUITKA_BYTECODE_FLAG },
    { "modulefinder", NULL, 3051944, 15315, NUITKA_BYTECODE_FLAG },
    { "msilib", NULL, 3067259, 15809, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "msilib.schema", NULL, 3083068, 56932, NUITKA_BYTECODE_FLAG },
    { "msilib.sequence", NULL, 3140000, 2587, NUITKA_BYTECODE_FLAG },
    { "msilib.text", NULL, 3142587, 8944, NUITKA_BYTECODE_FLAG },
    { "multiprocessing", NULL, 3151531, 490, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.connection", NULL, 3152021, 24895, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.context", NULL, 3176916, 13076, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.dummy", NULL, 3189992, 3768, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.dummy.connection", NULL, 3193760, 2476, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.forkserver", NULL, 3196236, 7727, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.heap", NULL, 3203963, 6387, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.managers", NULL, 3210350, 33967, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.pool", NULL, 3244317, 21199, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.popen_spawn_win32", NULL, 3265516, 3395, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.process", NULL, 3268911, 9389, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.queues", NULL, 3278300, 9400, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.reduction", NULL, 3287700, 7981, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.resource_sharer", NULL, 3295681, 5166, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.semaphore_tracker", NULL, 3300847, 3703, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.sharedctypes", NULL, 3304550, 6880, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.spawn", NULL, 3311430, 6431, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.synchronize", NULL, 3317861, 11146, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.util", NULL, 3329007, 9910, NUITKA_BYTECODE_FLAG },
    { "netrc", NULL, 3338917, 3726, NUITKA_BYTECODE_FLAG },
    { "nntplib", NULL, 3342643, 33714, NUITKA_BYTECODE_FLAG },
    { "nturl2path", NULL, 3376357, 1578, NUITKA_BYTECODE_FLAG },
    { "numbers", NULL, 3377935, 12155, NUITKA_BYTECODE_FLAG },
    { "optparse", NULL, 3390090, 47856, NUITKA_BYTECODE_FLAG },
    { "pathlib", NULL, 3437946, 41595, NUITKA_BYTECODE_FLAG },
    { "pdb", NULL, 3479541, 46839, NUITKA_BYTECODE_FLAG },
    { "pickle", NULL, 3526380, 42929, NUITKA_BYTECODE_FLAG },
    { "pickletools", NULL, 3569309, 65294, NUITKA_BYTECODE_FLAG },
    { "pipes", NULL, 3634603, 7766, NUITKA_BYTECODE_FLAG },
    { "pkgutil", NULL, 3642369, 16323, NUITKA_BYTECODE_FLAG },
    { "platform", NULL, 3658692, 28183, NUITKA_BYTECODE_FLAG },
    { "plistlib", NULL, 3686875, 25058, NUITKA_BYTECODE_FLAG },
    { "poplib", NULL, 3711933, 13299, NUITKA_BYTECODE_FLAG },
    { "posixpath", NULL, 3725232, 10393, NUITKA_BYTECODE_FLAG },
    { "pprint", NULL, 3735625, 15796, NUITKA_BYTECODE_FLAG },
    { "profile", NULL, 3751421, 13804, NUITKA_BYTECODE_FLAG },
    { "pstats", NULL, 3765225, 22257, NUITKA_BYTECODE_FLAG },
    { "py_compile", NULL, 3787482, 7154, NUITKA_BYTECODE_FLAG },
    { "pyclbr", NULL, 3794636, 10336, NUITKA_BYTECODE_FLAG },
    { "pydoc", NULL, 3804972, 84393, NUITKA_BYTECODE_FLAG },
    { "pydoc_data", NULL, 3889365, 109, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "pydoc_data.topics", NULL, 3889474, 411559, NUITKA_BYTECODE_FLAG },
    { "queue", NULL, 4301033, 11435, NUITKA_BYTECODE_FLAG },
    { "random", NULL, 80870, 19371, NUITKA_BYTECODE_FLAG },
    { "rlcompleter", NULL, 4312468, 5710, NUITKA_BYTECODE_FLAG },
    { "runpy", NULL, 4318178, 7908, NUITKA_BYTECODE_FLAG },
    { "sched", NULL, 4326086, 6484, NUITKA_BYTECODE_FLAG },
    { "secrets", NULL, 4332570, 2147, NUITKA_BYTECODE_FLAG },
    { "selectors", NULL, 4334717, 16911, NUITKA_BYTECODE_FLAG },
    { "shelve", NULL, 4351628, 9469, NUITKA_BYTECODE_FLAG },
    { "shlex", NULL, 4361097, 6953, NUITKA_BYTECODE_FLAG },
    { "shutil", NULL, 4368050, 30827, NUITKA_BYTECODE_FLAG },
    { "signal", NULL, 4398877, 2475, NUITKA_BYTECODE_FLAG },
    { "site", NULL, 100241, 13219, NUITKA_BYTECODE_FLAG },
    { "smtpd", NULL, 4401352, 26567, NUITKA_BYTECODE_FLAG },
    { "smtplib", NULL, 4427919, 35313, NUITKA_BYTECODE_FLAG },
    { "sndhdr", NULL, 4463232, 6866, NUITKA_BYTECODE_FLAG },
    { "socket", NULL, 4470098, 21984, NUITKA_BYTECODE_FLAG },
    { "socketserver", NULL, 4492082, 24162, NUITKA_BYTECODE_FLAG },
    { "sqlite3", NULL, 4516244, 137, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "sqlite3.dbapi2", NULL, 4516381, 2456, NUITKA_BYTECODE_FLAG },
    { "sqlite3.dump", NULL, 4518837, 1899, NUITKA_BYTECODE_FLAG },
    { "ssl", NULL, 4520736, 39734, NUITKA_BYTECODE_FLAG },
    { "statistics", NULL, 4560470, 18127, NUITKA_BYTECODE_FLAG },
    { "string", NULL, 4578597, 7798, NUITKA_BYTECODE_FLAG },
    { "subprocess", NULL, 4586395, 38728, NUITKA_BYTECODE_FLAG },
    { "sunau", NULL, 4625123, 17174, NUITKA_BYTECODE_FLAG },
    { "symbol", NULL, 4642297, 2528, NUITKA_BYTECODE_FLAG },
    { "symtable", NULL, 4644825, 10408, NUITKA_BYTECODE_FLAG },
    { "sysconfig", NULL, 4655233, 15407, NUITKA_BYTECODE_FLAG },
    { "tabnanny", NULL, 4670640, 6941, NUITKA_BYTECODE_FLAG },
    { "tarfile", NULL, 4677581, 61804, NUITKA_BYTECODE_FLAG },
    { "telnetlib", NULL, 4739385, 18065, NUITKA_BYTECODE_FLAG },
    { "tempfile", NULL, 4757450, 22108, NUITKA_BYTECODE_FLAG },
    { "textwrap", NULL, 4779558, 13575, NUITKA_BYTECODE_FLAG },
    { "this", NULL, 4793133, 1240, NUITKA_BYTECODE_FLAG },
    { "timeit", NULL, 4794373, 11610, NUITKA_BYTECODE_FLAG },
    { "tkinter", NULL, 4805983, 179138, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "tkinter.colorchooser", NULL, 4985121, 1098, NUITKA_BYTECODE_FLAG },
    { "tkinter.commondialog", NULL, 4986219, 1079, NUITKA_BYTECODE_FLAG },
    { "tkinter.constants", NULL, 4987298, 1631, NUITKA_BYTECODE_FLAG },
    { "tkinter.dialog", NULL, 4988929, 1407, NUITKA_BYTECODE_FLAG },
    { "tkinter.dnd", NULL, 4990336, 11149, NUITKA_BYTECODE_FLAG },
    { "tkinter.filedialog", NULL, 5001485, 12233, NUITKA_BYTECODE_FLAG },
    { "tkinter.font", NULL, 5013718, 6150, NUITKA_BYTECODE_FLAG },
    { "tkinter.messagebox", NULL, 5019868, 3015, NUITKA_BYTECODE_FLAG },
    { "tkinter.scrolledtext", NULL, 5022883, 2142, NUITKA_BYTECODE_FLAG },
    { "tkinter.simpledialog", NULL, 5025025, 10509, NUITKA_BYTECODE_FLAG },
    { "tkinter.tix", NULL, 5035534, 83629, NUITKA_BYTECODE_FLAG },
    { "tkinter.ttk", NULL, 5119163, 57813, NUITKA_BYTECODE_FLAG },
    { "trace", NULL, 5176976, 19110, NUITKA_BYTECODE_FLAG },
    { "tracemalloc", NULL, 5196086, 17239, NUITKA_BYTECODE_FLAG },
    { "turtle", NULL, 5213325, 130754, NUITKA_BYTECODE_FLAG },
    { "typing", NULL, 5344079, 49921, NUITKA_BYTECODE_FLAG },
    { "unittest", NULL, 5394000, 2974, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "unittest.case", NULL, 5396974, 48046, NUITKA_BYTECODE_FLAG },
    { "unittest.loader", NULL, 5445020, 14239, NUITKA_BYTECODE_FLAG },
    { "unittest.main", NULL, 5459259, 7401, NUITKA_BYTECODE_FLAG },
    { "unittest.mock", NULL, 5466660, 63169, NUITKA_BYTECODE_FLAG },
    { "unittest.result", NULL, 5529829, 7215, NUITKA_BYTECODE_FLAG },
    { "unittest.runner", NULL, 5537044, 6958, NUITKA_BYTECODE_FLAG },
    { "unittest.signals", NULL, 5544002, 2157, NUITKA_BYTECODE_FLAG },
    { "unittest.suite", NULL, 5546159, 9166, NUITKA_BYTECODE_FLAG },
    { "unittest.util", NULL, 5555325, 4484, NUITKA_BYTECODE_FLAG },
    { "urllib", NULL, 5559809, 105, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "urllib.error", NULL, 5559914, 2739, NUITKA_BYTECODE_FLAG },
    { "urllib.parse", NULL, 5562653, 30393, NUITKA_BYTECODE_FLAG },
    { "urllib.request", NULL, 5593046, 72267, NUITKA_BYTECODE_FLAG },
    { "urllib.response", NULL, 5665313, 3212, NUITKA_BYTECODE_FLAG },
    { "urllib.robotparser", NULL, 5668525, 7024, NUITKA_BYTECODE_FLAG },
    { "uu", NULL, 5675549, 3535, NUITKA_BYTECODE_FLAG },
    { "uuid", NULL, 5679084, 23162, NUITKA_BYTECODE_FLAG },
    { "venv", NULL, 5702246, 13800, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "wave", NULL, 5716046, 18251, NUITKA_BYTECODE_FLAG },
    { "weakref", NULL, 5734297, 19181, NUITKA_BYTECODE_FLAG },
    { "webbrowser", NULL, 5753478, 16337, NUITKA_BYTECODE_FLAG },
    { "wsgiref", NULL, 5769815, 701, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "wsgiref.handlers", NULL, 5770516, 16110, NUITKA_BYTECODE_FLAG },
    { "wsgiref.headers", NULL, 5786626, 7722, NUITKA_BYTECODE_FLAG },
    { "wsgiref.simple_server", NULL, 5794348, 5168, NUITKA_BYTECODE_FLAG },
    { "wsgiref.util", NULL, 5799516, 5143, NUITKA_BYTECODE_FLAG },
    { "wsgiref.validate", NULL, 5804659, 14639, NUITKA_BYTECODE_FLAG },
    { "xdrlib", NULL, 5819298, 8287, NUITKA_BYTECODE_FLAG },
    { "xml", NULL, 5827585, 669, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom", NULL, 5828254, 5421, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom.NodeFilter", NULL, 5833675, 936, NUITKA_BYTECODE_FLAG },
    { "xml.dom.domreg", NULL, 5834611, 2813, NUITKA_BYTECODE_FLAG },
    { "xml.dom.expatbuilder", NULL, 5837424, 26983, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minicompat", NULL, 5864407, 2782, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minidom", NULL, 5867189, 55582, NUITKA_BYTECODE_FLAG },
    { "xml.dom.pulldom", NULL, 5922771, 10455, NUITKA_BYTECODE_FLAG },
    { "xml.dom.xmlbuilder", NULL, 5933226, 12402, NUITKA_BYTECODE_FLAG },
    { "xml.etree", NULL, 5945628, 108, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.etree.ElementInclude", NULL, 5945736, 1544, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementPath", NULL, 5947280, 6312, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementTree", NULL, 5953592, 44776, NUITKA_BYTECODE_FLAG },
    { "xml.etree.cElementTree", NULL, 5998368, 150, NUITKA_BYTECODE_FLAG },
    { "xml.parsers", NULL, 5998518, 282, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.parsers.expat", NULL, 5998800, 311, NUITKA_BYTECODE_FLAG },
    { "xml.sax", NULL, 5999111, 3141, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.sax._exceptions", NULL, 6002252, 5450, NUITKA_BYTECODE_FLAG },
    { "xml.sax.expatreader", NULL, 6007702, 12353, NUITKA_BYTECODE_FLAG },
    { "xml.sax.handler", NULL, 6020055, 12326, NUITKA_BYTECODE_FLAG },
    { "xml.sax.saxutils", NULL, 6032381, 12779, NUITKA_BYTECODE_FLAG },
    { "xml.sax.xmlreader", NULL, 6045160, 16887, NUITKA_BYTECODE_FLAG },
    { "xmlrpc", NULL, 6062047, 105, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xmlrpc.client", NULL, 6062152, 34511, NUITKA_BYTECODE_FLAG },
    { "xmlrpc.server", NULL, 6096663, 29353, NUITKA_BYTECODE_FLAG },
    { "zipapp", NULL, 6126016, 5766, NUITKA_BYTECODE_FLAG },
    { "zipfile", NULL, 6131782, 49835, NUITKA_BYTECODE_FLAG },
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
