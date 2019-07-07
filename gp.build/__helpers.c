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
    { "copy", NULL, 3339, 12056, NUITKA_BYTECODE_FLAG },
    { "functionwrapper", MOD_INIT_NAME( functionwrapper ), 0, 0, NUITKA_COMPILED_MODULE },
    { "pyexpat", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "random", NULL, 15395, 25349, NUITKA_BYTECODE_FLAG },
    { "select", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "stats", MOD_INIT_NAME( stats ), 0, 0, NUITKA_COMPILED_MODULE },
    { "unicodedata", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "BaseHTTPServer", NULL, 40744, 21589, NUITKA_BYTECODE_FLAG },
    { "Bastion", NULL, 62333, 6577, NUITKA_BYTECODE_FLAG },
    { "CGIHTTPServer", NULL, 68910, 10945, NUITKA_BYTECODE_FLAG },
    { "Canvas", NULL, 79855, 15156, NUITKA_BYTECODE_FLAG },
    { "ConfigParser", NULL, 95011, 24910, NUITKA_BYTECODE_FLAG },
    { "Cookie", NULL, 119921, 22459, NUITKA_BYTECODE_FLAG },
    { "Dialog", NULL, 142380, 1887, NUITKA_BYTECODE_FLAG },
    { "DocXMLRPCServer", NULL, 144267, 9737, NUITKA_BYTECODE_FLAG },
    { "FileDialog", NULL, 154004, 9606, NUITKA_BYTECODE_FLAG },
    { "FixTk", NULL, 163610, 2077, NUITKA_BYTECODE_FLAG },
    { "HTMLParser", NULL, 165687, 13566, NUITKA_BYTECODE_FLAG },
    { "MimeWriter", NULL, 179253, 7311, NUITKA_BYTECODE_FLAG },
    { "Queue", NULL, 186564, 9276, NUITKA_BYTECODE_FLAG },
    { "ScrolledText", NULL, 195840, 2631, NUITKA_BYTECODE_FLAG },
    { "SimpleDialog", NULL, 198471, 4276, NUITKA_BYTECODE_FLAG },
    { "SimpleHTTPServer", NULL, 202747, 8003, NUITKA_BYTECODE_FLAG },
    { "SimpleXMLRPCServer", NULL, 210750, 22674, NUITKA_BYTECODE_FLAG },
    { "SocketServer", NULL, 233424, 23765, NUITKA_BYTECODE_FLAG },
    { "StringIO", NULL, 257189, 11377, NUITKA_BYTECODE_FLAG },
    { "Tix", NULL, 268566, 94389, NUITKA_BYTECODE_FLAG },
    { "Tkconstants", NULL, 362955, 2233, NUITKA_BYTECODE_FLAG },
    { "Tkdnd", NULL, 365188, 12687, NUITKA_BYTECODE_FLAG },
    { "Tkinter", NULL, 377875, 197042, NUITKA_BYTECODE_FLAG },
    { "UserDict", NULL, 574917, 9478, NUITKA_BYTECODE_FLAG },
    { "UserList", NULL, 584395, 6399, NUITKA_BYTECODE_FLAG },
    { "UserString", NULL, 590794, 14516, NUITKA_BYTECODE_FLAG },
    { "_LWPCookieJar", NULL, 605310, 5494, NUITKA_BYTECODE_FLAG },
    { "_MozillaCookieJar", NULL, 610804, 4433, NUITKA_BYTECODE_FLAG },
    { "__future__", NULL, 615237, 4254, NUITKA_BYTECODE_FLAG },
    { "_abcoll", NULL, 619491, 25156, NUITKA_BYTECODE_FLAG },
    { "_osx_support", NULL, 644647, 11699, NUITKA_BYTECODE_FLAG },
    { "_pyio", NULL, 656346, 63811, NUITKA_BYTECODE_FLAG },
    { "_strptime", NULL, 720157, 14767, NUITKA_BYTECODE_FLAG },
    { "_threading_local", NULL, 734924, 6551, NUITKA_BYTECODE_FLAG },
    { "_weakrefset", NULL, 741475, 9430, NUITKA_BYTECODE_FLAG },
    { "abc", NULL, 750905, 6080, NUITKA_BYTECODE_FLAG },
    { "aifc", NULL, 756985, 30108, NUITKA_BYTECODE_FLAG },
    { "anydbm", NULL, 787093, 2777, NUITKA_BYTECODE_FLAG },
    { "argparse", NULL, 789870, 63568, NUITKA_BYTECODE_FLAG },
    { "ast", NULL, 853438, 12826, NUITKA_BYTECODE_FLAG },
    { "asynchat", NULL, 866264, 8661, NUITKA_BYTECODE_FLAG },
    { "asyncore", NULL, 874925, 18537, NUITKA_BYTECODE_FLAG },
    { "atexit", NULL, 893462, 2165, NUITKA_BYTECODE_FLAG },
    { "audiodev", NULL, 895627, 8326, NUITKA_BYTECODE_FLAG },
    { "bdb", NULL, 903953, 18788, NUITKA_BYTECODE_FLAG },
    { "binhex", NULL, 922741, 15196, NUITKA_BYTECODE_FLAG },
    { "bisect", NULL, 937937, 3038, NUITKA_BYTECODE_FLAG },
    { "bsddb", NULL, 940975, 12257, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "bsddb.db", NULL, 953232, 579, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbobj", NULL, 953811, 18280, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbrecio", NULL, 972091, 5233, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbshelve", NULL, 977324, 12754, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbtables", NULL, 990078, 24222, NUITKA_BYTECODE_FLAG },
    { "bsddb.dbutils", NULL, 1014300, 1609, NUITKA_BYTECODE_FLAG },
    { "cProfile", NULL, 1015909, 6230, NUITKA_BYTECODE_FLAG },
    { "calendar", NULL, 1022139, 27386, NUITKA_BYTECODE_FLAG },
    { "cgi", NULL, 1049525, 32210, NUITKA_BYTECODE_FLAG },
    { "cgitb", NULL, 1081735, 12086, NUITKA_BYTECODE_FLAG },
    { "chunk", NULL, 1093821, 5538, NUITKA_BYTECODE_FLAG },
    { "cmd", NULL, 1099359, 13926, NUITKA_BYTECODE_FLAG },
    { "code", NULL, 1113285, 10246, NUITKA_BYTECODE_FLAG },
    { "codeop", NULL, 1123531, 6539, NUITKA_BYTECODE_FLAG },
    { "collections", NULL, 1130070, 25353, NUITKA_BYTECODE_FLAG },
    { "colorsys", NULL, 1155423, 3943, NUITKA_BYTECODE_FLAG },
    { "commands", NULL, 1159366, 2431, NUITKA_BYTECODE_FLAG },
    { "compileall", NULL, 1161797, 6979, NUITKA_BYTECODE_FLAG },
    { "compiler", NULL, 1168776, 1284, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "compiler.ast", NULL, 1170060, 70172, NUITKA_BYTECODE_FLAG },
    { "compiler.consts", NULL, 1240232, 724, NUITKA_BYTECODE_FLAG },
    { "compiler.future", NULL, 1240956, 2988, NUITKA_BYTECODE_FLAG },
    { "compiler.misc", NULL, 1243944, 3630, NUITKA_BYTECODE_FLAG },
    { "compiler.pyassem", NULL, 1247574, 25591, NUITKA_BYTECODE_FLAG },
    { "compiler.pycodegen", NULL, 1273165, 55642, NUITKA_BYTECODE_FLAG },
    { "compiler.symbols", NULL, 1328807, 17389, NUITKA_BYTECODE_FLAG },
    { "compiler.syntax", NULL, 1346196, 1844, NUITKA_BYTECODE_FLAG },
    { "compiler.transformer", NULL, 1348040, 46997, NUITKA_BYTECODE_FLAG },
    { "compiler.visitor", NULL, 1395037, 4129, NUITKA_BYTECODE_FLAG },
    { "contextlib", NULL, 1399166, 4386, NUITKA_BYTECODE_FLAG },
    { "cookielib", NULL, 1403552, 54279, NUITKA_BYTECODE_FLAG },
    { "copy", NULL, 3339, 12056, NUITKA_BYTECODE_FLAG },
    { "csv", NULL, 1457831, 13367, NUITKA_BYTECODE_FLAG },
    { "ctypes", NULL, 1471198, 20015, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes._endian", NULL, 1491213, 2269, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib", NULL, 1493482, 303, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes.macholib.dyld", NULL, 1493785, 5699, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib.dylib", NULL, 1499484, 2287, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib.framework", NULL, 1501771, 2597, NUITKA_BYTECODE_FLAG },
    { "ctypes.util", NULL, 1504368, 7756, NUITKA_BYTECODE_FLAG },
    { "ctypes.wintypes", NULL, 1512124, 5882, NUITKA_BYTECODE_FLAG },
    { "curses", NULL, 1518006, 1530, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "dbhash", NULL, 1519536, 700, NUITKA_BYTECODE_FLAG },
    { "decimal", NULL, 1520236, 170085, NUITKA_BYTECODE_FLAG },
    { "difflib", NULL, 1690321, 61671, NUITKA_BYTECODE_FLAG },
    { "dircache", NULL, 1751992, 1548, NUITKA_BYTECODE_FLAG },
    { "dis", NULL, 1753540, 6180, NUITKA_BYTECODE_FLAG },
    { "distutils", NULL, 1759720, 402, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.archive_util", NULL, 1760122, 7408, NUITKA_BYTECODE_FLAG },
    { "distutils.bcppcompiler", NULL, 1767530, 7829, NUITKA_BYTECODE_FLAG },
    { "distutils.ccompiler", NULL, 1775359, 36547, NUITKA_BYTECODE_FLAG },
    { "distutils.cmd", NULL, 1811906, 16611, NUITKA_BYTECODE_FLAG },
    { "distutils.command", NULL, 1828517, 652, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.command.bdist", NULL, 1829169, 5181, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_dumb", NULL, 1834350, 5010, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_msi", NULL, 1839360, 23920, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_rpm", NULL, 1863280, 17692, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_wininst", NULL, 1880972, 10735, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build", NULL, 1891707, 5089, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_clib", NULL, 1896796, 6375, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_ext", NULL, 1903171, 19333, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_py", NULL, 1922504, 11420, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_scripts", NULL, 1933924, 4491, NUITKA_BYTECODE_FLAG },
    { "distutils.command.check", NULL, 1938415, 6200, NUITKA_BYTECODE_FLAG },
    { "distutils.command.clean", NULL, 1944615, 3143, NUITKA_BYTECODE_FLAG },
    { "distutils.command.config", NULL, 1947758, 12582, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install", NULL, 1960340, 16747, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_data", NULL, 1977087, 3121, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_egg_info", NULL, 1980208, 3713, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_headers", NULL, 1983921, 2253, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_lib", NULL, 1986174, 6716, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_scripts", NULL, 1992890, 2955, NUITKA_BYTECODE_FLAG },
    { "distutils.command.register", NULL, 1995845, 10149, NUITKA_BYTECODE_FLAG },
    { "distutils.command.sdist", NULL, 2005994, 16603, NUITKA_BYTECODE_FLAG },
    { "distutils.command.upload", NULL, 2022597, 6290, NUITKA_BYTECODE_FLAG },
    { "distutils.config", NULL, 2028887, 3527, NUITKA_BYTECODE_FLAG },
    { "distutils.core", NULL, 2032414, 7631, NUITKA_BYTECODE_FLAG },
    { "distutils.cygwinccompiler", NULL, 2040045, 9757, NUITKA_BYTECODE_FLAG },
    { "distutils.debug", NULL, 2049802, 241, NUITKA_BYTECODE_FLAG },
    { "distutils.dep_util", NULL, 2050043, 3152, NUITKA_BYTECODE_FLAG },
    { "distutils.dir_util", NULL, 2053195, 6743, NUITKA_BYTECODE_FLAG },
    { "distutils.dist", NULL, 2059938, 39228, NUITKA_BYTECODE_FLAG },
    { "distutils.emxccompiler", NULL, 2099166, 7414, NUITKA_BYTECODE_FLAG },
    { "distutils.errors", NULL, 2106580, 6177, NUITKA_BYTECODE_FLAG },
    { "distutils.extension", NULL, 2112757, 7384, NUITKA_BYTECODE_FLAG },
    { "distutils.fancy_getopt", NULL, 2120141, 11845, NUITKA_BYTECODE_FLAG },
    { "distutils.file_util", NULL, 2131986, 6717, NUITKA_BYTECODE_FLAG },
    { "distutils.filelist", NULL, 2138703, 10670, NUITKA_BYTECODE_FLAG },
    { "distutils.log", NULL, 2149373, 2718, NUITKA_BYTECODE_FLAG },
    { "distutils.msvc9compiler", NULL, 2152091, 21355, NUITKA_BYTECODE_FLAG },
    { "distutils.msvccompiler", NULL, 2173446, 17387, NUITKA_BYTECODE_FLAG },
    { "distutils.spawn", NULL, 2190833, 6356, NUITKA_BYTECODE_FLAG },
    { "distutils.sysconfig", NULL, 2197189, 13121, NUITKA_BYTECODE_FLAG },
    { "distutils.text_file", NULL, 2210310, 9196, NUITKA_BYTECODE_FLAG },
    { "distutils.unixccompiler", NULL, 2219506, 7535, NUITKA_BYTECODE_FLAG },
    { "distutils.util", NULL, 2227041, 14279, NUITKA_BYTECODE_FLAG },
    { "distutils.version", NULL, 2241320, 7125, NUITKA_BYTECODE_FLAG },
    { "distutils.versionpredicate", NULL, 2248445, 5499, NUITKA_BYTECODE_FLAG },
    { "doctest", NULL, 2253944, 83077, NUITKA_BYTECODE_FLAG },
    { "dumbdbm", NULL, 2337021, 6481, NUITKA_BYTECODE_FLAG },
    { "dummy_thread", NULL, 2343502, 5311, NUITKA_BYTECODE_FLAG },
    { "dummy_threading", NULL, 2348813, 1272, NUITKA_BYTECODE_FLAG },
    { "email", NULL, 2350085, 2834, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email._parseaddr", NULL, 2352919, 13682, NUITKA_BYTECODE_FLAG },
    { "email.base64mime", NULL, 2366601, 5290, NUITKA_BYTECODE_FLAG },
    { "email.charset", NULL, 2371891, 13448, NUITKA_BYTECODE_FLAG },
    { "email.encoders", NULL, 2385339, 2189, NUITKA_BYTECODE_FLAG },
    { "email.errors", NULL, 2387528, 3446, NUITKA_BYTECODE_FLAG },
    { "email.feedparser", NULL, 2390974, 11444, NUITKA_BYTECODE_FLAG },
    { "email.generator", NULL, 2402418, 10277, NUITKA_BYTECODE_FLAG },
    { "email.header", NULL, 2412695, 13574, NUITKA_BYTECODE_FLAG },
    { "email.iterators", NULL, 2426269, 2333, NUITKA_BYTECODE_FLAG },
    { "email.message", NULL, 2428602, 28435, NUITKA_BYTECODE_FLAG },
    { "email.mime", NULL, 2457037, 117, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email.mime.application", NULL, 2457154, 1561, NUITKA_BYTECODE_FLAG },
    { "email.mime.audio", NULL, 2458715, 2881, NUITKA_BYTECODE_FLAG },
    { "email.mime.base", NULL, 2461596, 1093, NUITKA_BYTECODE_FLAG },
    { "email.mime.image", NULL, 2462689, 2026, NUITKA_BYTECODE_FLAG },
    { "email.mime.message", NULL, 2464715, 1425, NUITKA_BYTECODE_FLAG },
    { "email.mime.multipart", NULL, 2466140, 1646, NUITKA_BYTECODE_FLAG },
    { "email.mime.nonmultipart", NULL, 2467786, 865, NUITKA_BYTECODE_FLAG },
    { "email.mime.text", NULL, 2468651, 1285, NUITKA_BYTECODE_FLAG },
    { "email.parser", NULL, 2469936, 3781, NUITKA_BYTECODE_FLAG },
    { "email.quoprimime", NULL, 2473717, 8777, NUITKA_BYTECODE_FLAG },
    { "email.utils", NULL, 2482494, 9034, NUITKA_BYTECODE_FLAG },
    { "filecmp", NULL, 2491528, 9514, NUITKA_BYTECODE_FLAG },
    { "fileinput", NULL, 2501042, 14727, NUITKA_BYTECODE_FLAG },
    { "fnmatch", NULL, 2515769, 3576, NUITKA_BYTECODE_FLAG },
    { "formatter", NULL, 2519345, 18785, NUITKA_BYTECODE_FLAG },
    { "fpformat", NULL, 2538130, 4624, NUITKA_BYTECODE_FLAG },
    { "fractions", NULL, 2542754, 19530, NUITKA_BYTECODE_FLAG },
    { "ftplib", NULL, 2562284, 34227, NUITKA_BYTECODE_FLAG },
    { "genericpath", NULL, 2596511, 3454, NUITKA_BYTECODE_FLAG },
    { "getopt", NULL, 2599965, 6596, NUITKA_BYTECODE_FLAG },
    { "getpass", NULL, 2606561, 4701, NUITKA_BYTECODE_FLAG },
    { "gettext", NULL, 2611262, 15364, NUITKA_BYTECODE_FLAG },
    { "glob", NULL, 2626626, 2895, NUITKA_BYTECODE_FLAG },
    { "gzip", NULL, 2629521, 15037, NUITKA_BYTECODE_FLAG },
    { "hashlib", NULL, 2644558, 7014, NUITKA_BYTECODE_FLAG },
    { "heapq", NULL, 2651572, 14467, NUITKA_BYTECODE_FLAG },
    { "hmac", NULL, 2666039, 4484, NUITKA_BYTECODE_FLAG },
    { "hotshot", NULL, 2670523, 3421, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "hotshot.log", NULL, 2673944, 5465, NUITKA_BYTECODE_FLAG },
    { "hotshot.stats", NULL, 2679409, 3205, NUITKA_BYTECODE_FLAG },
    { "hotshot.stones", NULL, 2682614, 1138, NUITKA_BYTECODE_FLAG },
    { "htmlentitydefs", NULL, 2683752, 6354, NUITKA_BYTECODE_FLAG },
    { "htmllib", NULL, 2690106, 19846, NUITKA_BYTECODE_FLAG },
    { "httplib", NULL, 2709952, 36520, NUITKA_BYTECODE_FLAG },
    { "ihooks", NULL, 2746472, 21019, NUITKA_BYTECODE_FLAG },
    { "imaplib", NULL, 2767491, 44858, NUITKA_BYTECODE_FLAG },
    { "imghdr", NULL, 2812349, 4750, NUITKA_BYTECODE_FLAG },
    { "importlib", NULL, 2817099, 1479, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "imputil", NULL, 2818578, 15425, NUITKA_BYTECODE_FLAG },
    { "inspect", NULL, 2834003, 39729, NUITKA_BYTECODE_FLAG },
    { "io", NULL, 2873732, 3555, NUITKA_BYTECODE_FLAG },
    { "json", NULL, 2877287, 13922, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "json.decoder", NULL, 2891209, 11892, NUITKA_BYTECODE_FLAG },
    { "json.encoder", NULL, 2903101, 13617, NUITKA_BYTECODE_FLAG },
    { "json.scanner", NULL, 2916718, 2206, NUITKA_BYTECODE_FLAG },
    { "json.tool", NULL, 2918924, 1276, NUITKA_BYTECODE_FLAG },
    { "keyword", NULL, 2920200, 2087, NUITKA_BYTECODE_FLAG },
    { "lib2to3", NULL, 2922287, 114, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.btm_matcher", NULL, 2922401, 5767, NUITKA_BYTECODE_FLAG },
    { "lib2to3.btm_utils", NULL, 2928168, 7490, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_base", NULL, 2935658, 7169, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_util", NULL, 2942827, 14505, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes", NULL, 2957332, 120, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.fixes.fix_apply", NULL, 2957452, 1860, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_asserts", NULL, 2959312, 1538, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_basestring", NULL, 2960850, 784, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_buffer", NULL, 2961634, 941, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_callable", NULL, 2962575, 1484, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_dict", NULL, 2964059, 3741, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_except", NULL, 2967800, 2981, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exec", NULL, 2970781, 1409, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_execfile", NULL, 2972190, 2050, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exitfunc", NULL, 2974240, 2724, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_filter", NULL, 2976964, 2247, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_funcattrs", NULL, 2979211, 1105, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_future", NULL, 2980316, 910, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_getcwdu", NULL, 2981226, 917, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_has_key", NULL, 2982143, 3175, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_idioms", NULL, 2985318, 4494, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_import", NULL, 2989812, 3215, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports", NULL, 2993027, 5322, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports2", NULL, 2998349, 616, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_input", NULL, 2998965, 1125, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_intern", NULL, 3000090, 1596, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_isinstance", NULL, 3001686, 1829, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools", NULL, 3003515, 1782, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools_imports", NULL, 3005297, 2007, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_long", NULL, 3007304, 832, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_map", NULL, 3008136, 3031, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_metaclass", NULL, 3011167, 6551, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_methodattrs", NULL, 3017718, 1129, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ne", NULL, 3018847, 973, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_next", NULL, 3019820, 3507, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_nonzero", NULL, 3023327, 1077, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_numliterals", NULL, 3024404, 1237, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_operator", NULL, 3025641, 5067, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_paren", NULL, 3030708, 1534, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_print", NULL, 3032242, 2715, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raise", NULL, 3034957, 2489, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raw_input", NULL, 3037446, 927, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_reduce", NULL, 3038373, 1253, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_renames", NULL, 3039626, 2428, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_repr", NULL, 3042054, 1007, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_set_literal", NULL, 3043061, 1976, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_standarderror", NULL, 3045037, 844, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_sys_exc", NULL, 3045881, 1693, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_throw", NULL, 3047574, 1987, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_tuple_params", NULL, 3049561, 5400, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_types", NULL, 3054961, 2187, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_unicode", NULL, 3057148, 1704, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_urllib", NULL, 3058852, 7107, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ws_comma", NULL, 3065959, 1373, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xrange", NULL, 3067332, 3039, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xreadlines", NULL, 3070371, 1143, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_zip", NULL, 3071514, 1337, NUITKA_BYTECODE_FLAG },
    { "lib2to3.main", NULL, 3072851, 9773, NUITKA_BYTECODE_FLAG },
    { "lib2to3.patcomp", NULL, 3082624, 6598, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2", NULL, 3089222, 161, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.pgen2.conv", NULL, 3089383, 8151, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.driver", NULL, 3097534, 5325, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.grammar", NULL, 3102859, 5983, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.literals", NULL, 3108842, 1990, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.parse", NULL, 3110832, 7168, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.pgen", NULL, 3118000, 12005, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.token", NULL, 3130005, 2275, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.tokenize", NULL, 3132280, 16862, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pygram", NULL, 3149142, 1391, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pytree", NULL, 3150533, 29915, NUITKA_BYTECODE_FLAG },
    { "lib2to3.refactor", NULL, 3180448, 23744, NUITKA_BYTECODE_FLAG },
    { "linecache", NULL, 3204192, 3234, NUITKA_BYTECODE_FLAG },
    { "logging", NULL, 3207426, 56792, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "logging.config", NULL, 3264218, 25678, NUITKA_BYTECODE_FLAG },
    { "logging.handlers", NULL, 3289896, 38961, NUITKA_BYTECODE_FLAG },
    { "macpath", NULL, 3328857, 7578, NUITKA_BYTECODE_FLAG },
    { "macurl2path", NULL, 3336435, 2216, NUITKA_BYTECODE_FLAG },
    { "mailbox", NULL, 3338651, 75623, NUITKA_BYTECODE_FLAG },
    { "mailcap", NULL, 3414274, 7006, NUITKA_BYTECODE_FLAG },
    { "markupbase", NULL, 3421280, 9224, NUITKA_BYTECODE_FLAG },
    { "md5", NULL, 3430504, 365, NUITKA_BYTECODE_FLAG },
    { "mhlib", NULL, 3430869, 33419, NUITKA_BYTECODE_FLAG },
    { "mimetools", NULL, 3464288, 8116, NUITKA_BYTECODE_FLAG },
    { "mimetypes", NULL, 3472404, 18196, NUITKA_BYTECODE_FLAG },
    { "mimify", NULL, 3490600, 11918, NUITKA_BYTECODE_FLAG },
    { "modulefinder", NULL, 3502518, 18583, NUITKA_BYTECODE_FLAG },
    { "msilib", NULL, 3521101, 19128, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "msilib.schema", NULL, 3540229, 59838, NUITKA_BYTECODE_FLAG },
    { "msilib.sequence", NULL, 3600067, 6264, NUITKA_BYTECODE_FLAG },
    { "msilib.text", NULL, 3606331, 18477, NUITKA_BYTECODE_FLAG },
    { "multifile", NULL, 3624808, 5337, NUITKA_BYTECODE_FLAG },
    { "multiprocessing", NULL, 3630145, 8327, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.connection", NULL, 3638472, 14249, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.dummy", NULL, 3652721, 5352, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.dummy.connection", NULL, 3658073, 2696, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.forking", NULL, 3660769, 14237, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.heap", NULL, 3675006, 6801, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.managers", NULL, 3681807, 37952, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.pool", NULL, 3719759, 22193, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.process", NULL, 3741952, 9357, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.queues", NULL, 3751309, 11361, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.reduction", NULL, 3762670, 5921, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.sharedctypes", NULL, 3768591, 8494, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.synchronize", NULL, 3777085, 10871, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.util", NULL, 3787956, 9982, NUITKA_BYTECODE_FLAG },
    { "mutex", NULL, 3797938, 2472, NUITKA_BYTECODE_FLAG },
    { "netrc", NULL, 3800410, 4619, NUITKA_BYTECODE_FLAG },
    { "new", NULL, 3805029, 849, NUITKA_BYTECODE_FLAG },
    { "nntplib", NULL, 3805878, 20816, NUITKA_BYTECODE_FLAG },
    { "ntpath", NULL, 3826694, 13012, NUITKA_BYTECODE_FLAG },
    { "nturl2path", NULL, 3839706, 1792, NUITKA_BYTECODE_FLAG },
    { "numbers", NULL, 3841498, 13694, NUITKA_BYTECODE_FLAG },
    { "opcode", NULL, 3855192, 6119, NUITKA_BYTECODE_FLAG },
    { "optparse", NULL, 3861311, 53423, NUITKA_BYTECODE_FLAG },
    { "os", NULL, 3914734, 25426, NUITKA_BYTECODE_FLAG },
    { "os2emxpath", NULL, 3940160, 4473, NUITKA_BYTECODE_FLAG },
    { "pdb", NULL, 3944633, 43091, NUITKA_BYTECODE_FLAG },
    { "pickle", NULL, 3987724, 38030, NUITKA_BYTECODE_FLAG },
    { "pickletools", NULL, 4025754, 56942, NUITKA_BYTECODE_FLAG },
    { "pipes", NULL, 4082696, 9220, NUITKA_BYTECODE_FLAG },
    { "pkgutil", NULL, 4091916, 18760, NUITKA_BYTECODE_FLAG },
    { "platform", NULL, 4110676, 36708, NUITKA_BYTECODE_FLAG },
    { "plistlib", NULL, 4147384, 18966, NUITKA_BYTECODE_FLAG },
    { "popen2", NULL, 4166350, 8937, NUITKA_BYTECODE_FLAG },
    { "poplib", NULL, 4175287, 13171, NUITKA_BYTECODE_FLAG },
    { "posixfile", NULL, 4188458, 7584, NUITKA_BYTECODE_FLAG },
    { "posixpath", NULL, 4196042, 11297, NUITKA_BYTECODE_FLAG },
    { "pprint", NULL, 4207339, 10091, NUITKA_BYTECODE_FLAG },
    { "profile", NULL, 4217430, 16255, NUITKA_BYTECODE_FLAG },
    { "pstats", NULL, 4233685, 24705, NUITKA_BYTECODE_FLAG },
    { "pty", NULL, 4258390, 4908, NUITKA_BYTECODE_FLAG },
    { "py_compile", NULL, 4263298, 6385, NUITKA_BYTECODE_FLAG },
    { "pyclbr", NULL, 4269683, 9578, NUITKA_BYTECODE_FLAG },
    { "pydoc", NULL, 4279261, 91444, NUITKA_BYTECODE_FLAG },
    { "pydoc_data", NULL, 4370705, 117, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "pydoc_data.topics", NULL, 4370822, 397628, NUITKA_BYTECODE_FLAG },
    { "random", NULL, 15395, 25349, NUITKA_BYTECODE_FLAG },
    { "repr", NULL, 4768450, 5292, NUITKA_BYTECODE_FLAG },
    { "rexec", NULL, 4773742, 23864, NUITKA_BYTECODE_FLAG },
    { "rfc822", NULL, 4797606, 31542, NUITKA_BYTECODE_FLAG },
    { "rlcompleter", NULL, 4829148, 5975, NUITKA_BYTECODE_FLAG },
    { "robotparser", NULL, 4835123, 7832, NUITKA_BYTECODE_FLAG },
    { "runpy", NULL, 4842955, 8300, NUITKA_BYTECODE_FLAG },
    { "sched", NULL, 4851255, 4941, NUITKA_BYTECODE_FLAG },
    { "sets", NULL, 4856196, 16607, NUITKA_BYTECODE_FLAG },
    { "sgmllib", NULL, 4872803, 15193, NUITKA_BYTECODE_FLAG },
    { "sha", NULL, 4887996, 408, NUITKA_BYTECODE_FLAG },
    { "shelve", NULL, 4888404, 10113, NUITKA_BYTECODE_FLAG },
    { "shlex", NULL, 4898517, 7470, NUITKA_BYTECODE_FLAG },
    { "shutil", NULL, 4905987, 18457, NUITKA_BYTECODE_FLAG },
    { "site", NULL, 4924444, 16407, NUITKA_BYTECODE_FLAG },
    { "smtpd", NULL, 4940851, 15733, NUITKA_BYTECODE_FLAG },
    { "smtplib", NULL, 4956584, 29982, NUITKA_BYTECODE_FLAG },
    { "sndhdr", NULL, 4986566, 7264, NUITKA_BYTECODE_FLAG },
    { "socket", NULL, 4993830, 15995, NUITKA_BYTECODE_FLAG },
    { "sqlite3", NULL, 5009825, 151, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "sqlite3.dbapi2", NULL, 5009976, 2655, NUITKA_BYTECODE_FLAG },
    { "sqlite3.dump", NULL, 5012631, 2048, NUITKA_BYTECODE_FLAG },
    { "sre", NULL, 5014679, 506, NUITKA_BYTECODE_FLAG },
    { "ssl", NULL, 5015185, 31291, NUITKA_BYTECODE_FLAG },
    { "stat", NULL, 5046476, 2693, NUITKA_BYTECODE_FLAG },
    { "statvfs", NULL, 5049169, 607, NUITKA_BYTECODE_FLAG },
    { "stringold", NULL, 5049776, 12406, NUITKA_BYTECODE_FLAG },
    { "subprocess", NULL, 5062182, 41664, NUITKA_BYTECODE_FLAG },
    { "sunau", NULL, 5103846, 18157, NUITKA_BYTECODE_FLAG },
    { "sunaudio", NULL, 5122003, 1954, NUITKA_BYTECODE_FLAG },
    { "symbol", NULL, 5123957, 3008, NUITKA_BYTECODE_FLAG },
    { "symtable", NULL, 5126965, 11528, NUITKA_BYTECODE_FLAG },
    { "sysconfig", NULL, 5138493, 17413, NUITKA_BYTECODE_FLAG },
    { "tabnanny", NULL, 5155906, 8135, NUITKA_BYTECODE_FLAG },
    { "tarfile", NULL, 5164041, 75023, NUITKA_BYTECODE_FLAG },
    { "telnetlib", NULL, 5239064, 22974, NUITKA_BYTECODE_FLAG },
    { "tempfile", NULL, 5262038, 20020, NUITKA_BYTECODE_FLAG },
    { "test", NULL, 5282058, 111, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "test.pystone", NULL, 5282169, 7921, NUITKA_BYTECODE_FLAG },
    { "textwrap", NULL, 5290090, 11954, NUITKA_BYTECODE_FLAG },
    { "this", NULL, 5302044, 1207, NUITKA_BYTECODE_FLAG },
    { "threading", NULL, 5303251, 42119, NUITKA_BYTECODE_FLAG },
    { "timeit", NULL, 5345370, 12110, NUITKA_BYTECODE_FLAG },
    { "tkColorChooser", NULL, 5357480, 1387, NUITKA_BYTECODE_FLAG },
    { "tkCommonDialog", NULL, 5358867, 1478, NUITKA_BYTECODE_FLAG },
    { "tkFileDialog", NULL, 5360345, 5073, NUITKA_BYTECODE_FLAG },
    { "tkFont", NULL, 5365418, 7016, NUITKA_BYTECODE_FLAG },
    { "tkMessageBox", NULL, 5372434, 3831, NUITKA_BYTECODE_FLAG },
    { "tkSimpleDialog", NULL, 5376265, 8975, NUITKA_BYTECODE_FLAG },
    { "toaiff", NULL, 5385240, 3078, NUITKA_BYTECODE_FLAG },
    { "token", NULL, 5388318, 3783, NUITKA_BYTECODE_FLAG },
    { "tokenize", NULL, 5392101, 14417, NUITKA_BYTECODE_FLAG },
    { "trace", NULL, 5406518, 22614, NUITKA_BYTECODE_FLAG },
    { "traceback", NULL, 5429132, 11571, NUITKA_BYTECODE_FLAG },
    { "ttk", NULL, 5440703, 61779, NUITKA_BYTECODE_FLAG },
    { "tty", NULL, 5502482, 1294, NUITKA_BYTECODE_FLAG },
    { "turtle", NULL, 5503776, 138312, NUITKA_BYTECODE_FLAG },
    { "unittest", NULL, 5642088, 2951, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "unittest.case", NULL, 5645039, 39878, NUITKA_BYTECODE_FLAG },
    { "unittest.loader", NULL, 5684917, 11262, NUITKA_BYTECODE_FLAG },
    { "unittest.main", NULL, 5696179, 7958, NUITKA_BYTECODE_FLAG },
    { "unittest.result", NULL, 5704137, 7796, NUITKA_BYTECODE_FLAG },
    { "unittest.runner", NULL, 5711933, 7536, NUITKA_BYTECODE_FLAG },
    { "unittest.signals", NULL, 5719469, 2708, NUITKA_BYTECODE_FLAG },
    { "unittest.suite", NULL, 5722177, 10355, NUITKA_BYTECODE_FLAG },
    { "unittest.util", NULL, 5732532, 4470, NUITKA_BYTECODE_FLAG },
    { "urllib", NULL, 5737002, 49965, NUITKA_BYTECODE_FLAG },
    { "urllib2", NULL, 5786967, 46607, NUITKA_BYTECODE_FLAG },
    { "urlparse", NULL, 5833574, 14379, NUITKA_BYTECODE_FLAG },
    { "user", NULL, 5847953, 1711, NUITKA_BYTECODE_FLAG },
    { "uu", NULL, 5849664, 4279, NUITKA_BYTECODE_FLAG },
    { "uuid", NULL, 5853943, 22344, NUITKA_BYTECODE_FLAG },
    { "warnings", NULL, 5876287, 13501, NUITKA_BYTECODE_FLAG },
    { "wave", NULL, 5889788, 19758, NUITKA_BYTECODE_FLAG },
    { "weakref", NULL, 5909546, 15574, NUITKA_BYTECODE_FLAG },
    { "webbrowser", NULL, 5925120, 19546, NUITKA_BYTECODE_FLAG },
    { "whichdb", NULL, 5944666, 2223, NUITKA_BYTECODE_FLAG },
    { "wsgiref", NULL, 5946889, 716, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "wsgiref.handlers", NULL, 5947605, 16061, NUITKA_BYTECODE_FLAG },
    { "wsgiref.headers", NULL, 5963666, 7375, NUITKA_BYTECODE_FLAG },
    { "wsgiref.simple_server", NULL, 5971041, 6157, NUITKA_BYTECODE_FLAG },
    { "wsgiref.util", NULL, 5977198, 5925, NUITKA_BYTECODE_FLAG },
    { "wsgiref.validate", NULL, 5983123, 16666, NUITKA_BYTECODE_FLAG },
    { "xdrlib", NULL, 5999789, 9684, NUITKA_BYTECODE_FLAG },
    { "xml", NULL, 6009473, 1065, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom", NULL, 6010538, 6361, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom.NodeFilter", NULL, 6016899, 1103, NUITKA_BYTECODE_FLAG },
    { "xml.dom.domreg", NULL, 6018002, 3278, NUITKA_BYTECODE_FLAG },
    { "xml.dom.expatbuilder", NULL, 6021280, 32736, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minicompat", NULL, 6054016, 3476, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minidom", NULL, 6057492, 64543, NUITKA_BYTECODE_FLAG },
    { "xml.dom.pulldom", NULL, 6122035, 12860, NUITKA_BYTECODE_FLAG },
    { "xml.dom.xmlbuilder", NULL, 6134895, 16231, NUITKA_BYTECODE_FLAG },
    { "xml.etree", NULL, 6151126, 116, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.etree.ElementInclude", NULL, 6151242, 1939, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementPath", NULL, 6153181, 7485, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementTree", NULL, 6160666, 34351, NUITKA_BYTECODE_FLAG },
    { "xml.etree.cElementTree", NULL, 6195017, 163, NUITKA_BYTECODE_FLAG },
    { "xml.parsers", NULL, 6195180, 301, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.parsers.expat", NULL, 6195481, 274, NUITKA_BYTECODE_FLAG },
    { "xml.sax", NULL, 6195755, 3661, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.sax._exceptions", NULL, 6199416, 6076, NUITKA_BYTECODE_FLAG },
    { "xml.sax.expatreader", NULL, 6205492, 14578, NUITKA_BYTECODE_FLAG },
    { "xml.sax.handler", NULL, 6220070, 12895, NUITKA_BYTECODE_FLAG },
    { "xml.sax.saxutils", NULL, 6232965, 14523, NUITKA_BYTECODE_FLAG },
    { "xml.sax.xmlreader", NULL, 6247488, 18874, NUITKA_BYTECODE_FLAG },
    { "xmllib", NULL, 6266362, 26570, NUITKA_BYTECODE_FLAG },
    { "xmlrpclib", NULL, 6292932, 43272, NUITKA_BYTECODE_FLAG },
    { "zipfile", NULL, 6336204, 41423, NUITKA_BYTECODE_FLAG },
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
