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
/* Code to register embedded modules for meta path based loading if any. */

#include "nuitka/unfreezing.h"

/* Table for lookup to find compiled or bytecode modules included in this
 * binary or module, or put along this binary as extension modules. We do
 * our own loading for each of these.
 */
MOD_INIT_DECL( __main__ );
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
    { "pyexpat", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "select", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "site", NULL, 505, 13219, NUITKA_BYTECODE_FLAG },
    { "unicodedata", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "__future__", NULL, 13724, 4095, NUITKA_BYTECODE_FLAG },
    { "_bootlocale", NULL, 17819, 1212, NUITKA_BYTECODE_FLAG },
    { "_compat_pickle", NULL, 19031, 5771, NUITKA_BYTECODE_FLAG },
    { "_dummy_thread", NULL, 24802, 4816, NUITKA_BYTECODE_FLAG },
    { "_markupbase", NULL, 29618, 7748, NUITKA_BYTECODE_FLAG },
    { "_osx_support", NULL, 37366, 9558, NUITKA_BYTECODE_FLAG },
    { "_py_abc", NULL, 46924, 4617, NUITKA_BYTECODE_FLAG },
    { "_pyio", NULL, 51541, 72781, NUITKA_BYTECODE_FLAG },
    { "_sitebuiltins", NULL, 124322, 3428, NUITKA_BYTECODE_FLAG },
    { "_strptime", NULL, 127750, 16067, NUITKA_BYTECODE_FLAG },
    { "_threading_local", NULL, 143817, 6375, NUITKA_BYTECODE_FLAG },
    { "aifc", NULL, 150192, 26106, NUITKA_BYTECODE_FLAG },
    { "argparse", NULL, 176298, 61876, NUITKA_BYTECODE_FLAG },
    { "ast", NULL, 238174, 12043, NUITKA_BYTECODE_FLAG },
    { "asynchat", NULL, 250217, 6797, NUITKA_BYTECODE_FLAG },
    { "asyncio", NULL, 257014, 653, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "asyncio.base_events", NULL, 257667, 48079, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_futures", NULL, 305746, 2065, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_subprocess", NULL, 307811, 9154, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_tasks", NULL, 316965, 1829, NUITKA_BYTECODE_FLAG },
    { "asyncio.constants", NULL, 318794, 554, NUITKA_BYTECODE_FLAG },
    { "asyncio.coroutines", NULL, 319348, 6341, NUITKA_BYTECODE_FLAG },
    { "asyncio.events", NULL, 325689, 27818, NUITKA_BYTECODE_FLAG },
    { "asyncio.format_helpers", NULL, 353507, 2280, NUITKA_BYTECODE_FLAG },
    { "asyncio.futures", NULL, 355787, 10708, NUITKA_BYTECODE_FLAG },
    { "asyncio.locks", NULL, 366495, 15876, NUITKA_BYTECODE_FLAG },
    { "asyncio.log", NULL, 382371, 203, NUITKA_BYTECODE_FLAG },
    { "asyncio.proactor_events", NULL, 382574, 20074, NUITKA_BYTECODE_FLAG },
    { "asyncio.protocols", NULL, 402648, 8691, NUITKA_BYTECODE_FLAG },
    { "asyncio.queues", NULL, 411339, 8136, NUITKA_BYTECODE_FLAG },
    { "asyncio.runners", NULL, 419475, 1877, NUITKA_BYTECODE_FLAG },
    { "asyncio.selector_events", NULL, 421352, 28086, NUITKA_BYTECODE_FLAG },
    { "asyncio.sslproto", NULL, 449438, 21174, NUITKA_BYTECODE_FLAG },
    { "asyncio.streams", NULL, 470612, 20251, NUITKA_BYTECODE_FLAG },
    { "asyncio.subprocess", NULL, 490863, 6715, NUITKA_BYTECODE_FLAG },
    { "asyncio.tasks", NULL, 497578, 21829, NUITKA_BYTECODE_FLAG },
    { "asyncio.transports", NULL, 519407, 12174, NUITKA_BYTECODE_FLAG },
    { "asyncio.windows_events", NULL, 531581, 23031, NUITKA_BYTECODE_FLAG },
    { "asyncio.windows_utils", NULL, 554612, 4364, NUITKA_BYTECODE_FLAG },
    { "asyncore", NULL, 558976, 15807, NUITKA_BYTECODE_FLAG },
    { "bdb", NULL, 574783, 24357, NUITKA_BYTECODE_FLAG },
    { "binhex", NULL, 599140, 12022, NUITKA_BYTECODE_FLAG },
    { "bisect", NULL, 611162, 2661, NUITKA_BYTECODE_FLAG },
    { "cProfile", NULL, 613823, 4514, NUITKA_BYTECODE_FLAG },
    { "calendar", NULL, 618337, 27387, NUITKA_BYTECODE_FLAG },
    { "cgi", NULL, 645724, 27154, NUITKA_BYTECODE_FLAG },
    { "cgitb", NULL, 672878, 10075, NUITKA_BYTECODE_FLAG },
    { "chunk", NULL, 682953, 4882, NUITKA_BYTECODE_FLAG },
    { "cmd", NULL, 687835, 12553, NUITKA_BYTECODE_FLAG },
    { "code", NULL, 700388, 9821, NUITKA_BYTECODE_FLAG },
    { "codeop", NULL, 710209, 6253, NUITKA_BYTECODE_FLAG },
    { "colorsys", NULL, 716462, 3260, NUITKA_BYTECODE_FLAG },
    { "compileall", NULL, 719722, 9069, NUITKA_BYTECODE_FLAG },
    { "concurrent", NULL, 728791, 109, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "concurrent.futures", NULL, 728900, 1049, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "concurrent.futures._base", NULL, 729949, 20846, NUITKA_BYTECODE_FLAG },
    { "concurrent.futures.process", NULL, 750795, 19524, NUITKA_BYTECODE_FLAG },
    { "concurrent.futures.thread", NULL, 770319, 5137, NUITKA_BYTECODE_FLAG },
    { "configparser", NULL, 775456, 45843, NUITKA_BYTECODE_FLAG },
    { "contextlib", NULL, 821299, 19863, NUITKA_BYTECODE_FLAG },
    { "contextvars", NULL, 841162, 220, NUITKA_BYTECODE_FLAG },
    { "copy", NULL, 841382, 7064, NUITKA_BYTECODE_FLAG },
    { "csv", NULL, 848446, 11795, NUITKA_BYTECODE_FLAG },
    { "ctypes", NULL, 860241, 16112, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes._aix", NULL, 876353, 9786, NUITKA_BYTECODE_FLAG },
    { "ctypes._endian", NULL, 886139, 1919, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib", NULL, 888058, 276, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes.macholib.dyld", NULL, 888334, 4309, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib.dylib", NULL, 892643, 1903, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib.framework", NULL, 894546, 2183, NUITKA_BYTECODE_FLAG },
    { "ctypes.util", NULL, 896729, 7712, NUITKA_BYTECODE_FLAG },
    { "ctypes.wintypes", NULL, 904441, 5080, NUITKA_BYTECODE_FLAG },
    { "dataclasses", NULL, 909521, 22428, NUITKA_BYTECODE_FLAG },
    { "datetime", NULL, 931949, 57212, NUITKA_BYTECODE_FLAG },
    { "dbm", NULL, 989161, 4129, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "dbm.dumb", NULL, 993290, 8363, NUITKA_BYTECODE_FLAG },
    { "decimal", NULL, 1001653, 162135, NUITKA_BYTECODE_FLAG },
    { "difflib", NULL, 1163788, 59401, NUITKA_BYTECODE_FLAG },
    { "distutils", NULL, 1223189, 361, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils._msvccompiler", NULL, 1223550, 13581, NUITKA_BYTECODE_FLAG },
    { "distutils.archive_util", NULL, 1237131, 6495, NUITKA_BYTECODE_FLAG },
    { "distutils.bcppcompiler", NULL, 1243626, 6464, NUITKA_BYTECODE_FLAG },
    { "distutils.ccompiler", NULL, 1250090, 33179, NUITKA_BYTECODE_FLAG },
    { "distutils.cmd", NULL, 1283269, 13871, NUITKA_BYTECODE_FLAG },
    { "distutils.command", NULL, 1297140, 518, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.command.bdist", NULL, 1297658, 3617, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_dumb", NULL, 1301275, 3532, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_msi", NULL, 1304807, 19364, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_rpm", NULL, 1324171, 12457, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_wininst", NULL, 1336628, 7972, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build", NULL, 1344600, 3798, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_clib", NULL, 1348398, 4848, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_ext", NULL, 1353246, 15755, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_py", NULL, 1369001, 10354, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_scripts", NULL, 1379355, 4253, NUITKA_BYTECODE_FLAG },
    { "distutils.command.check", NULL, 1383608, 4773, NUITKA_BYTECODE_FLAG },
    { "distutils.command.clean", NULL, 1388381, 2055, NUITKA_BYTECODE_FLAG },
    { "distutils.command.config", NULL, 1390436, 10159, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install", NULL, 1400595, 13482, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_data", NULL, 1414077, 2248, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_egg_info", NULL, 1416325, 2946, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_headers", NULL, 1419271, 1663, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_lib", NULL, 1420934, 5041, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_scripts", NULL, 1425975, 2105, NUITKA_BYTECODE_FLAG },
    { "distutils.command.register", NULL, 1428080, 8440, NUITKA_BYTECODE_FLAG },
    { "distutils.command.sdist", NULL, 1436520, 14471, NUITKA_BYTECODE_FLAG },
    { "distutils.command.upload", NULL, 1450991, 5053, NUITKA_BYTECODE_FLAG },
    { "distutils.config", NULL, 1456044, 3448, NUITKA_BYTECODE_FLAG },
    { "distutils.core", NULL, 1459492, 6573, NUITKA_BYTECODE_FLAG },
    { "distutils.cygwinccompiler", NULL, 1466065, 8491, NUITKA_BYTECODE_FLAG },
    { "distutils.debug", NULL, 1474556, 171, NUITKA_BYTECODE_FLAG },
    { "distutils.dep_util", NULL, 1474727, 2687, NUITKA_BYTECODE_FLAG },
    { "distutils.dir_util", NULL, 1477414, 5781, NUITKA_BYTECODE_FLAG },
    { "distutils.dist", NULL, 1483195, 34402, NUITKA_BYTECODE_FLAG },
    { "distutils.errors", NULL, 1517597, 5457, NUITKA_BYTECODE_FLAG },
    { "distutils.extension", NULL, 1523054, 6868, NUITKA_BYTECODE_FLAG },
    { "distutils.fancy_getopt", NULL, 1529922, 10580, NUITKA_BYTECODE_FLAG },
    { "distutils.file_util", NULL, 1540502, 5866, NUITKA_BYTECODE_FLAG },
    { "distutils.filelist", NULL, 1546368, 9801, NUITKA_BYTECODE_FLAG },
    { "distutils.log", NULL, 1556169, 2282, NUITKA_BYTECODE_FLAG },
    { "distutils.msvc9compiler", NULL, 1558451, 17346, NUITKA_BYTECODE_FLAG },
    { "distutils.msvccompiler", NULL, 1575797, 14534, NUITKA_BYTECODE_FLAG },
    { "distutils.spawn", NULL, 1590331, 4972, NUITKA_BYTECODE_FLAG },
    { "distutils.sysconfig", NULL, 1595303, 11946, NUITKA_BYTECODE_FLAG },
    { "distutils.text_file", NULL, 1607249, 8408, NUITKA_BYTECODE_FLAG },
    { "distutils.unixccompiler", NULL, 1615657, 6503, NUITKA_BYTECODE_FLAG },
    { "distutils.util", NULL, 1622160, 15064, NUITKA_BYTECODE_FLAG },
    { "distutils.version", NULL, 1637224, 7319, NUITKA_BYTECODE_FLAG },
    { "distutils.versionpredicate", NULL, 1644543, 5066, NUITKA_BYTECODE_FLAG },
    { "doctest", NULL, 1649609, 75397, NUITKA_BYTECODE_FLAG },
    { "dummy_threading", NULL, 1725006, 1087, NUITKA_BYTECODE_FLAG },
    { "email", NULL, 1726093, 1654, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email._encoded_words", NULL, 1727747, 5571, NUITKA_BYTECODE_FLAG },
    { "email._header_value_parser", NULL, 1733318, 75593, NUITKA_BYTECODE_FLAG },
    { "email._parseaddr", NULL, 1808911, 12311, NUITKA_BYTECODE_FLAG },
    { "email._policybase", NULL, 1821222, 14813, NUITKA_BYTECODE_FLAG },
    { "email.base64mime", NULL, 1836035, 3198, NUITKA_BYTECODE_FLAG },
    { "email.charset", NULL, 1839233, 11492, NUITKA_BYTECODE_FLAG },
    { "email.contentmanager", NULL, 1850725, 7258, NUITKA_BYTECODE_FLAG },
    { "email.encoders", NULL, 1857983, 1627, NUITKA_BYTECODE_FLAG },
    { "email.errors", NULL, 1859610, 6154, NUITKA_BYTECODE_FLAG },
    { "email.feedparser", NULL, 1865764, 10588, NUITKA_BYTECODE_FLAG },
    { "email.generator", NULL, 1876352, 12463, NUITKA_BYTECODE_FLAG },
    { "email.header", NULL, 1888815, 16343, NUITKA_BYTECODE_FLAG },
    { "email.headerregistry", NULL, 1905158, 21260, NUITKA_BYTECODE_FLAG },
    { "email.iterators", NULL, 1926418, 1895, NUITKA_BYTECODE_FLAG },
    { "email.message", NULL, 1928313, 37903, NUITKA_BYTECODE_FLAG },
    { "email.mime", NULL, 1966216, 109, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email.mime.application", NULL, 1966325, 1420, NUITKA_BYTECODE_FLAG },
    { "email.mime.audio", NULL, 1967745, 2579, NUITKA_BYTECODE_FLAG },
    { "email.mime.base", NULL, 1970324, 1045, NUITKA_BYTECODE_FLAG },
    { "email.mime.image", NULL, 1971369, 1865, NUITKA_BYTECODE_FLAG },
    { "email.mime.message", NULL, 1973234, 1294, NUITKA_BYTECODE_FLAG },
    { "email.mime.multipart", NULL, 1974528, 1516, NUITKA_BYTECODE_FLAG },
    { "email.mime.nonmultipart", NULL, 1976044, 731, NUITKA_BYTECODE_FLAG },
    { "email.mime.text", NULL, 1976775, 1278, NUITKA_BYTECODE_FLAG },
    { "email.parser", NULL, 1978053, 5710, NUITKA_BYTECODE_FLAG },
    { "email.policy", NULL, 1983763, 9603, NUITKA_BYTECODE_FLAG },
    { "email.quoprimime", NULL, 1993366, 7627, NUITKA_BYTECODE_FLAG },
    { "email.utils", NULL, 2000993, 9430, NUITKA_BYTECODE_FLAG },
    { "filecmp", NULL, 2010423, 8270, NUITKA_BYTECODE_FLAG },
    { "fileinput", NULL, 2018693, 13161, NUITKA_BYTECODE_FLAG },
    { "fnmatch", NULL, 2031854, 3300, NUITKA_BYTECODE_FLAG },
    { "formatter", NULL, 2035154, 17516, NUITKA_BYTECODE_FLAG },
    { "fractions", NULL, 2052670, 18391, NUITKA_BYTECODE_FLAG },
    { "ftplib", NULL, 2071061, 28030, NUITKA_BYTECODE_FLAG },
    { "getopt", NULL, 2099091, 6202, NUITKA_BYTECODE_FLAG },
    { "getpass", NULL, 2105293, 4127, NUITKA_BYTECODE_FLAG },
    { "gettext", NULL, 2109420, 14131, NUITKA_BYTECODE_FLAG },
    { "glob", NULL, 2123551, 4222, NUITKA_BYTECODE_FLAG },
    { "gzip", NULL, 2127773, 17148, NUITKA_BYTECODE_FLAG },
    { "hashlib", NULL, 2144921, 6554, NUITKA_BYTECODE_FLAG },
    { "hmac", NULL, 2151475, 6076, NUITKA_BYTECODE_FLAG },
    { "html", NULL, 2157551, 3360, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "html.entities", NULL, 2160911, 50432, NUITKA_BYTECODE_FLAG },
    { "html.parser", NULL, 2211343, 11070, NUITKA_BYTECODE_FLAG },
    { "http", NULL, 2222413, 5885, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "http.client", NULL, 2228298, 34011, NUITKA_BYTECODE_FLAG },
    { "http.cookiejar", NULL, 2262309, 53548, NUITKA_BYTECODE_FLAG },
    { "http.cookies", NULL, 2315857, 15207, NUITKA_BYTECODE_FLAG },
    { "http.server", NULL, 2331064, 33330, NUITKA_BYTECODE_FLAG },
    { "imaplib", NULL, 2364394, 41393, NUITKA_BYTECODE_FLAG },
    { "imghdr", NULL, 2405787, 4105, NUITKA_BYTECODE_FLAG },
    { "imp", NULL, 2409892, 9716, NUITKA_BYTECODE_FLAG },
    { "importlib.abc", NULL, 2419608, 13450, NUITKA_BYTECODE_FLAG },
    { "importlib.resources", NULL, 2433058, 8302, NUITKA_BYTECODE_FLAG },
    { "importlib.util", NULL, 2441360, 9319, NUITKA_BYTECODE_FLAG },
    { "ipaddress", NULL, 2450679, 62957, NUITKA_BYTECODE_FLAG },
    { "json", NULL, 2513636, 12301, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "json.decoder", NULL, 2525937, 9787, NUITKA_BYTECODE_FLAG },
    { "json.encoder", NULL, 2535724, 11269, NUITKA_BYTECODE_FLAG },
    { "json.scanner", NULL, 2546993, 1959, NUITKA_BYTECODE_FLAG },
    { "json.tool", NULL, 2548952, 1440, NUITKA_BYTECODE_FLAG },
    { "lib2to3", NULL, 2550392, 106, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.btm_matcher", NULL, 2550498, 4856, NUITKA_BYTECODE_FLAG },
    { "lib2to3.btm_utils", NULL, 2555354, 6106, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_base", NULL, 2561460, 6198, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_util", NULL, 2567658, 12009, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes", NULL, 2579667, 112, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.fixes.fix_apply", NULL, 2579779, 1650, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_asserts", NULL, 2581429, 1235, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_basestring", NULL, 2582664, 625, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_buffer", NULL, 2583289, 770, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_dict", NULL, 2584059, 3278, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_except", NULL, 2587337, 2780, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exec", NULL, 2590117, 1111, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_execfile", NULL, 2591228, 1655, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exitfunc", NULL, 2592883, 2258, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_filter", NULL, 2595141, 2323, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_funcattrs", NULL, 2597464, 936, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_future", NULL, 2598400, 746, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_getcwdu", NULL, 2599146, 750, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_has_key", NULL, 2599896, 2880, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_idioms", NULL, 2602776, 3865, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_import", NULL, 2606641, 2747, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports", NULL, 2609388, 4311, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports2", NULL, 2613699, 510, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_input", NULL, 2614209, 912, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_intern", NULL, 2615121, 1119, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_isinstance", NULL, 2616240, 1517, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools", NULL, 2617757, 1506, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools_imports", NULL, 2619263, 1542, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_long", NULL, 2620805, 667, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_map", NULL, 2621472, 3055, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_metaclass", NULL, 2624527, 5308, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_methodattrs", NULL, 2629835, 898, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ne", NULL, 2630733, 769, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_next", NULL, 2631502, 3022, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_nonzero", NULL, 2634524, 885, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_numliterals", NULL, 2635409, 981, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_operator", NULL, 2636390, 4198, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_paren", NULL, 2640588, 1352, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_print", NULL, 2641940, 2287, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raise", NULL, 2644227, 2211, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raw_input", NULL, 2646438, 757, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_reduce", NULL, 2647195, 1092, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_reload", NULL, 2648287, 1131, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_renames", NULL, 2649418, 1955, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_repr", NULL, 2651373, 807, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_set_literal", NULL, 2652180, 1639, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_standarderror", NULL, 2653819, 682, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_sys_exc", NULL, 2654501, 1363, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_throw", NULL, 2655864, 1764, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_tuple_params", NULL, 2657628, 4558, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_types", NULL, 2662186, 1791, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_unicode", NULL, 2663977, 1503, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_urllib", NULL, 2665480, 5923, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ws_comma", NULL, 2671403, 1085, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xrange", NULL, 2672488, 2498, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xreadlines", NULL, 2674986, 1079, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_zip", NULL, 2676065, 1543, NUITKA_BYTECODE_FLAG },
    { "lib2to3.main", NULL, 2677608, 8499, NUITKA_BYTECODE_FLAG },
    { "lib2to3.patcomp", NULL, 2686107, 5574, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2", NULL, 2691681, 142, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.pgen2.driver", NULL, 2691823, 5103, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.grammar", NULL, 2696926, 6979, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.literals", NULL, 2703905, 1521, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.parse", NULL, 2705426, 6267, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.pgen", NULL, 2711693, 9743, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.token", NULL, 2721436, 1835, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.tokenize", NULL, 2723271, 15103, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pygram", NULL, 2738374, 1161, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pytree", NULL, 2739535, 24966, NUITKA_BYTECODE_FLAG },
    { "lib2to3.refactor", NULL, 2764501, 20367, NUITKA_BYTECODE_FLAG },
    { "logging", NULL, 2784868, 62521, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "logging.config", NULL, 2847389, 22978, NUITKA_BYTECODE_FLAG },
    { "logging.handlers", NULL, 2870367, 42946, NUITKA_BYTECODE_FLAG },
    { "lzma", NULL, 2913313, 11902, NUITKA_BYTECODE_FLAG },
    { "macpath", NULL, 2925215, 5770, NUITKA_BYTECODE_FLAG },
    { "mailbox", NULL, 2930985, 63611, NUITKA_BYTECODE_FLAG },
    { "mailcap", NULL, 2994596, 6447, NUITKA_BYTECODE_FLAG },
    { "mimetypes", NULL, 3001043, 15441, NUITKA_BYTECODE_FLAG },
    { "modulefinder", NULL, 3016484, 15315, NUITKA_BYTECODE_FLAG },
    { "msilib", NULL, 3031799, 15809, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "msilib.schema", NULL, 3047608, 56932, NUITKA_BYTECODE_FLAG },
    { "msilib.sequence", NULL, 3104540, 2587, NUITKA_BYTECODE_FLAG },
    { "msilib.text", NULL, 3107127, 8944, NUITKA_BYTECODE_FLAG },
    { "multiprocessing", NULL, 3116071, 490, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.connection", NULL, 3116561, 24895, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.context", NULL, 3141456, 13076, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.dummy", NULL, 3154532, 3768, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.dummy.connection", NULL, 3158300, 2476, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.forkserver", NULL, 3160776, 7727, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.heap", NULL, 3168503, 6387, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.managers", NULL, 3174890, 33967, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.pool", NULL, 3208857, 21199, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.popen_spawn_win32", NULL, 3230056, 3395, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.process", NULL, 3233451, 9389, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.queues", NULL, 3242840, 9400, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.reduction", NULL, 3252240, 7981, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.resource_sharer", NULL, 3260221, 5166, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.semaphore_tracker", NULL, 3265387, 3703, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.sharedctypes", NULL, 3269090, 6880, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.spawn", NULL, 3275970, 6431, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.synchronize", NULL, 3282401, 11146, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.util", NULL, 3293547, 9910, NUITKA_BYTECODE_FLAG },
    { "netrc", NULL, 3303457, 3726, NUITKA_BYTECODE_FLAG },
    { "nntplib", NULL, 3307183, 33714, NUITKA_BYTECODE_FLAG },
    { "nturl2path", NULL, 3340897, 1578, NUITKA_BYTECODE_FLAG },
    { "numbers", NULL, 3342475, 12155, NUITKA_BYTECODE_FLAG },
    { "optparse", NULL, 3354630, 47856, NUITKA_BYTECODE_FLAG },
    { "pathlib", NULL, 3402486, 41595, NUITKA_BYTECODE_FLAG },
    { "pdb", NULL, 3444081, 46839, NUITKA_BYTECODE_FLAG },
    { "pickle", NULL, 3490920, 42929, NUITKA_BYTECODE_FLAG },
    { "pickletools", NULL, 3533849, 65294, NUITKA_BYTECODE_FLAG },
    { "pipes", NULL, 3599143, 7766, NUITKA_BYTECODE_FLAG },
    { "pkgutil", NULL, 3606909, 16323, NUITKA_BYTECODE_FLAG },
    { "platform", NULL, 3623232, 28183, NUITKA_BYTECODE_FLAG },
    { "plistlib", NULL, 3651415, 25058, NUITKA_BYTECODE_FLAG },
    { "poplib", NULL, 3676473, 13299, NUITKA_BYTECODE_FLAG },
    { "posixpath", NULL, 3689772, 10393, NUITKA_BYTECODE_FLAG },
    { "pprint", NULL, 3700165, 15796, NUITKA_BYTECODE_FLAG },
    { "profile", NULL, 3715961, 13804, NUITKA_BYTECODE_FLAG },
    { "pstats", NULL, 3729765, 22257, NUITKA_BYTECODE_FLAG },
    { "py_compile", NULL, 3752022, 7154, NUITKA_BYTECODE_FLAG },
    { "pyclbr", NULL, 3759176, 10336, NUITKA_BYTECODE_FLAG },
    { "pydoc", NULL, 3769512, 84393, NUITKA_BYTECODE_FLAG },
    { "pydoc_data", NULL, 3853905, 109, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "pydoc_data.topics", NULL, 3854014, 411559, NUITKA_BYTECODE_FLAG },
    { "queue", NULL, 4265573, 11435, NUITKA_BYTECODE_FLAG },
    { "random", NULL, 4277008, 19371, NUITKA_BYTECODE_FLAG },
    { "rlcompleter", NULL, 4296379, 5710, NUITKA_BYTECODE_FLAG },
    { "runpy", NULL, 4302089, 7908, NUITKA_BYTECODE_FLAG },
    { "sched", NULL, 4309997, 6484, NUITKA_BYTECODE_FLAG },
    { "secrets", NULL, 4316481, 2147, NUITKA_BYTECODE_FLAG },
    { "selectors", NULL, 4318628, 16911, NUITKA_BYTECODE_FLAG },
    { "shelve", NULL, 4335539, 9469, NUITKA_BYTECODE_FLAG },
    { "shlex", NULL, 4345008, 6953, NUITKA_BYTECODE_FLAG },
    { "shutil", NULL, 4351961, 30827, NUITKA_BYTECODE_FLAG },
    { "signal", NULL, 4382788, 2475, NUITKA_BYTECODE_FLAG },
    { "site", NULL, 505, 13219, NUITKA_BYTECODE_FLAG },
    { "smtpd", NULL, 4385263, 26567, NUITKA_BYTECODE_FLAG },
    { "smtplib", NULL, 4411830, 35313, NUITKA_BYTECODE_FLAG },
    { "sndhdr", NULL, 4447143, 6866, NUITKA_BYTECODE_FLAG },
    { "socket", NULL, 4454009, 21984, NUITKA_BYTECODE_FLAG },
    { "socketserver", NULL, 4475993, 24162, NUITKA_BYTECODE_FLAG },
    { "sqlite3", NULL, 4500155, 137, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "sqlite3.dbapi2", NULL, 4500292, 2456, NUITKA_BYTECODE_FLAG },
    { "sqlite3.dump", NULL, 4502748, 1899, NUITKA_BYTECODE_FLAG },
    { "ssl", NULL, 4504647, 39734, NUITKA_BYTECODE_FLAG },
    { "statistics", NULL, 4544381, 18127, NUITKA_BYTECODE_FLAG },
    { "string", NULL, 4562508, 7798, NUITKA_BYTECODE_FLAG },
    { "subprocess", NULL, 4570306, 38728, NUITKA_BYTECODE_FLAG },
    { "sunau", NULL, 4609034, 17174, NUITKA_BYTECODE_FLAG },
    { "symbol", NULL, 4626208, 2528, NUITKA_BYTECODE_FLAG },
    { "symtable", NULL, 4628736, 10408, NUITKA_BYTECODE_FLAG },
    { "sysconfig", NULL, 4639144, 15407, NUITKA_BYTECODE_FLAG },
    { "tabnanny", NULL, 4654551, 6941, NUITKA_BYTECODE_FLAG },
    { "tarfile", NULL, 4661492, 61804, NUITKA_BYTECODE_FLAG },
    { "telnetlib", NULL, 4723296, 18065, NUITKA_BYTECODE_FLAG },
    { "tempfile", NULL, 4741361, 22108, NUITKA_BYTECODE_FLAG },
    { "textwrap", NULL, 4763469, 13575, NUITKA_BYTECODE_FLAG },
    { "this", NULL, 4777044, 1240, NUITKA_BYTECODE_FLAG },
    { "timeit", NULL, 4778284, 11610, NUITKA_BYTECODE_FLAG },
    { "tkinter", NULL, 4789894, 179138, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "tkinter.colorchooser", NULL, 4969032, 1098, NUITKA_BYTECODE_FLAG },
    { "tkinter.commondialog", NULL, 4970130, 1079, NUITKA_BYTECODE_FLAG },
    { "tkinter.constants", NULL, 4971209, 1631, NUITKA_BYTECODE_FLAG },
    { "tkinter.dialog", NULL, 4972840, 1407, NUITKA_BYTECODE_FLAG },
    { "tkinter.dnd", NULL, 4974247, 11149, NUITKA_BYTECODE_FLAG },
    { "tkinter.filedialog", NULL, 4985396, 12233, NUITKA_BYTECODE_FLAG },
    { "tkinter.font", NULL, 4997629, 6150, NUITKA_BYTECODE_FLAG },
    { "tkinter.messagebox", NULL, 5003779, 3015, NUITKA_BYTECODE_FLAG },
    { "tkinter.scrolledtext", NULL, 5006794, 2142, NUITKA_BYTECODE_FLAG },
    { "tkinter.simpledialog", NULL, 5008936, 10509, NUITKA_BYTECODE_FLAG },
    { "tkinter.tix", NULL, 5019445, 83629, NUITKA_BYTECODE_FLAG },
    { "tkinter.ttk", NULL, 5103074, 57813, NUITKA_BYTECODE_FLAG },
    { "trace", NULL, 5160887, 19110, NUITKA_BYTECODE_FLAG },
    { "tracemalloc", NULL, 5179997, 17239, NUITKA_BYTECODE_FLAG },
    { "turtle", NULL, 5197236, 130754, NUITKA_BYTECODE_FLAG },
    { "typing", NULL, 5327990, 49921, NUITKA_BYTECODE_FLAG },
    { "unittest", NULL, 5377911, 2974, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "unittest.case", NULL, 5380885, 48046, NUITKA_BYTECODE_FLAG },
    { "unittest.loader", NULL, 5428931, 14239, NUITKA_BYTECODE_FLAG },
    { "unittest.main", NULL, 5443170, 7401, NUITKA_BYTECODE_FLAG },
    { "unittest.mock", NULL, 5450571, 63169, NUITKA_BYTECODE_FLAG },
    { "unittest.result", NULL, 5513740, 7215, NUITKA_BYTECODE_FLAG },
    { "unittest.runner", NULL, 5520955, 6958, NUITKA_BYTECODE_FLAG },
    { "unittest.signals", NULL, 5527913, 2157, NUITKA_BYTECODE_FLAG },
    { "unittest.suite", NULL, 5530070, 9166, NUITKA_BYTECODE_FLAG },
    { "unittest.util", NULL, 5539236, 4484, NUITKA_BYTECODE_FLAG },
    { "urllib", NULL, 5543720, 105, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "urllib.error", NULL, 5543825, 2739, NUITKA_BYTECODE_FLAG },
    { "urllib.parse", NULL, 5546564, 30393, NUITKA_BYTECODE_FLAG },
    { "urllib.request", NULL, 5576957, 72267, NUITKA_BYTECODE_FLAG },
    { "urllib.response", NULL, 5649224, 3212, NUITKA_BYTECODE_FLAG },
    { "urllib.robotparser", NULL, 5652436, 7024, NUITKA_BYTECODE_FLAG },
    { "uu", NULL, 5659460, 3535, NUITKA_BYTECODE_FLAG },
    { "uuid", NULL, 5662995, 23162, NUITKA_BYTECODE_FLAG },
    { "venv", NULL, 5686157, 13800, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "wave", NULL, 5699957, 18251, NUITKA_BYTECODE_FLAG },
    { "weakref", NULL, 5718208, 19181, NUITKA_BYTECODE_FLAG },
    { "webbrowser", NULL, 5737389, 16337, NUITKA_BYTECODE_FLAG },
    { "wsgiref", NULL, 5753726, 701, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "wsgiref.handlers", NULL, 5754427, 16110, NUITKA_BYTECODE_FLAG },
    { "wsgiref.headers", NULL, 5770537, 7722, NUITKA_BYTECODE_FLAG },
    { "wsgiref.simple_server", NULL, 5778259, 5168, NUITKA_BYTECODE_FLAG },
    { "wsgiref.util", NULL, 5783427, 5143, NUITKA_BYTECODE_FLAG },
    { "wsgiref.validate", NULL, 5788570, 14639, NUITKA_BYTECODE_FLAG },
    { "xdrlib", NULL, 5803209, 8287, NUITKA_BYTECODE_FLAG },
    { "xml", NULL, 5811496, 669, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom", NULL, 5812165, 5421, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom.NodeFilter", NULL, 5817586, 936, NUITKA_BYTECODE_FLAG },
    { "xml.dom.domreg", NULL, 5818522, 2813, NUITKA_BYTECODE_FLAG },
    { "xml.dom.expatbuilder", NULL, 5821335, 26983, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minicompat", NULL, 5848318, 2782, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minidom", NULL, 5851100, 55582, NUITKA_BYTECODE_FLAG },
    { "xml.dom.pulldom", NULL, 5906682, 10455, NUITKA_BYTECODE_FLAG },
    { "xml.dom.xmlbuilder", NULL, 5917137, 12402, NUITKA_BYTECODE_FLAG },
    { "xml.etree", NULL, 5929539, 108, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.etree.ElementInclude", NULL, 5929647, 1544, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementPath", NULL, 5931191, 6312, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementTree", NULL, 5937503, 44776, NUITKA_BYTECODE_FLAG },
    { "xml.etree.cElementTree", NULL, 5982279, 150, NUITKA_BYTECODE_FLAG },
    { "xml.parsers", NULL, 5982429, 282, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.parsers.expat", NULL, 5982711, 311, NUITKA_BYTECODE_FLAG },
    { "xml.sax", NULL, 5983022, 3141, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.sax._exceptions", NULL, 5986163, 5450, NUITKA_BYTECODE_FLAG },
    { "xml.sax.expatreader", NULL, 5991613, 12353, NUITKA_BYTECODE_FLAG },
    { "xml.sax.handler", NULL, 6003966, 12326, NUITKA_BYTECODE_FLAG },
    { "xml.sax.saxutils", NULL, 6016292, 12779, NUITKA_BYTECODE_FLAG },
    { "xml.sax.xmlreader", NULL, 6029071, 16887, NUITKA_BYTECODE_FLAG },
    { "xmlrpc", NULL, 6045958, 105, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xmlrpc.client", NULL, 6046063, 34511, NUITKA_BYTECODE_FLAG },
    { "xmlrpc.server", NULL, 6080574, 29353, NUITKA_BYTECODE_FLAG },
    { "zipapp", NULL, 6109927, 5766, NUITKA_BYTECODE_FLAG },
    { "zipfile", NULL, 6115693, 49835, NUITKA_BYTECODE_FLAG },
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
