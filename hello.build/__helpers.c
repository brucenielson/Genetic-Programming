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
    { "site", NULL, 565, 13219, NUITKA_BYTECODE_FLAG },
    { "unicodedata", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "__future__", NULL, 13784, 4095, NUITKA_BYTECODE_FLAG },
    { "_bootlocale", NULL, 17879, 1212, NUITKA_BYTECODE_FLAG },
    { "_compat_pickle", NULL, 19091, 5771, NUITKA_BYTECODE_FLAG },
    { "_dummy_thread", NULL, 24862, 4816, NUITKA_BYTECODE_FLAG },
    { "_markupbase", NULL, 29678, 7748, NUITKA_BYTECODE_FLAG },
    { "_osx_support", NULL, 37426, 9558, NUITKA_BYTECODE_FLAG },
    { "_py_abc", NULL, 46984, 4617, NUITKA_BYTECODE_FLAG },
    { "_pyio", NULL, 51601, 72781, NUITKA_BYTECODE_FLAG },
    { "_sitebuiltins", NULL, 124382, 3428, NUITKA_BYTECODE_FLAG },
    { "_strptime", NULL, 127810, 16067, NUITKA_BYTECODE_FLAG },
    { "_threading_local", NULL, 143877, 6375, NUITKA_BYTECODE_FLAG },
    { "aifc", NULL, 150252, 26106, NUITKA_BYTECODE_FLAG },
    { "argparse", NULL, 176358, 61876, NUITKA_BYTECODE_FLAG },
    { "ast", NULL, 238234, 12043, NUITKA_BYTECODE_FLAG },
    { "asynchat", NULL, 250277, 6797, NUITKA_BYTECODE_FLAG },
    { "asyncio", NULL, 257074, 653, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "asyncio.base_events", NULL, 257727, 48079, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_futures", NULL, 305806, 2065, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_subprocess", NULL, 307871, 9154, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_tasks", NULL, 317025, 1829, NUITKA_BYTECODE_FLAG },
    { "asyncio.constants", NULL, 318854, 554, NUITKA_BYTECODE_FLAG },
    { "asyncio.coroutines", NULL, 319408, 6341, NUITKA_BYTECODE_FLAG },
    { "asyncio.events", NULL, 325749, 27818, NUITKA_BYTECODE_FLAG },
    { "asyncio.format_helpers", NULL, 353567, 2280, NUITKA_BYTECODE_FLAG },
    { "asyncio.futures", NULL, 355847, 10708, NUITKA_BYTECODE_FLAG },
    { "asyncio.locks", NULL, 366555, 15876, NUITKA_BYTECODE_FLAG },
    { "asyncio.log", NULL, 382431, 203, NUITKA_BYTECODE_FLAG },
    { "asyncio.proactor_events", NULL, 382634, 20074, NUITKA_BYTECODE_FLAG },
    { "asyncio.protocols", NULL, 402708, 8691, NUITKA_BYTECODE_FLAG },
    { "asyncio.queues", NULL, 411399, 8136, NUITKA_BYTECODE_FLAG },
    { "asyncio.runners", NULL, 419535, 1877, NUITKA_BYTECODE_FLAG },
    { "asyncio.selector_events", NULL, 421412, 28086, NUITKA_BYTECODE_FLAG },
    { "asyncio.sslproto", NULL, 449498, 21174, NUITKA_BYTECODE_FLAG },
    { "asyncio.streams", NULL, 470672, 20251, NUITKA_BYTECODE_FLAG },
    { "asyncio.subprocess", NULL, 490923, 6715, NUITKA_BYTECODE_FLAG },
    { "asyncio.tasks", NULL, 497638, 21829, NUITKA_BYTECODE_FLAG },
    { "asyncio.transports", NULL, 519467, 12174, NUITKA_BYTECODE_FLAG },
    { "asyncio.windows_events", NULL, 531641, 23031, NUITKA_BYTECODE_FLAG },
    { "asyncio.windows_utils", NULL, 554672, 4364, NUITKA_BYTECODE_FLAG },
    { "asyncore", NULL, 559036, 15807, NUITKA_BYTECODE_FLAG },
    { "bdb", NULL, 574843, 24357, NUITKA_BYTECODE_FLAG },
    { "binhex", NULL, 599200, 12022, NUITKA_BYTECODE_FLAG },
    { "bisect", NULL, 611222, 2661, NUITKA_BYTECODE_FLAG },
    { "cProfile", NULL, 613883, 4514, NUITKA_BYTECODE_FLAG },
    { "calendar", NULL, 618397, 27387, NUITKA_BYTECODE_FLAG },
    { "cgi", NULL, 645784, 27154, NUITKA_BYTECODE_FLAG },
    { "cgitb", NULL, 672938, 10075, NUITKA_BYTECODE_FLAG },
    { "chunk", NULL, 683013, 4882, NUITKA_BYTECODE_FLAG },
    { "cmd", NULL, 687895, 12553, NUITKA_BYTECODE_FLAG },
    { "code", NULL, 700448, 9821, NUITKA_BYTECODE_FLAG },
    { "codeop", NULL, 710269, 6253, NUITKA_BYTECODE_FLAG },
    { "colorsys", NULL, 716522, 3260, NUITKA_BYTECODE_FLAG },
    { "compileall", NULL, 719782, 9069, NUITKA_BYTECODE_FLAG },
    { "concurrent", NULL, 728851, 109, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "concurrent.futures", NULL, 728960, 1049, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "concurrent.futures._base", NULL, 730009, 20846, NUITKA_BYTECODE_FLAG },
    { "concurrent.futures.process", NULL, 750855, 19524, NUITKA_BYTECODE_FLAG },
    { "concurrent.futures.thread", NULL, 770379, 5137, NUITKA_BYTECODE_FLAG },
    { "configparser", NULL, 775516, 45843, NUITKA_BYTECODE_FLAG },
    { "contextlib", NULL, 821359, 19863, NUITKA_BYTECODE_FLAG },
    { "contextvars", NULL, 841222, 220, NUITKA_BYTECODE_FLAG },
    { "copy", NULL, 841442, 7064, NUITKA_BYTECODE_FLAG },
    { "csv", NULL, 848506, 11795, NUITKA_BYTECODE_FLAG },
    { "ctypes", NULL, 860301, 16112, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes._aix", NULL, 876413, 9786, NUITKA_BYTECODE_FLAG },
    { "ctypes._endian", NULL, 886199, 1919, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib", NULL, 888118, 276, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes.macholib.dyld", NULL, 888394, 4309, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib.dylib", NULL, 892703, 1903, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib.framework", NULL, 894606, 2183, NUITKA_BYTECODE_FLAG },
    { "ctypes.util", NULL, 896789, 7712, NUITKA_BYTECODE_FLAG },
    { "ctypes.wintypes", NULL, 904501, 5080, NUITKA_BYTECODE_FLAG },
    { "dataclasses", NULL, 909581, 22428, NUITKA_BYTECODE_FLAG },
    { "datetime", NULL, 932009, 57212, NUITKA_BYTECODE_FLAG },
    { "dbm", NULL, 989221, 4129, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "dbm.dumb", NULL, 993350, 8363, NUITKA_BYTECODE_FLAG },
    { "decimal", NULL, 1001713, 162135, NUITKA_BYTECODE_FLAG },
    { "difflib", NULL, 1163848, 59401, NUITKA_BYTECODE_FLAG },
    { "distutils", NULL, 1223249, 361, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils._msvccompiler", NULL, 1223610, 13581, NUITKA_BYTECODE_FLAG },
    { "distutils.archive_util", NULL, 1237191, 6495, NUITKA_BYTECODE_FLAG },
    { "distutils.bcppcompiler", NULL, 1243686, 6464, NUITKA_BYTECODE_FLAG },
    { "distutils.ccompiler", NULL, 1250150, 33179, NUITKA_BYTECODE_FLAG },
    { "distutils.cmd", NULL, 1283329, 13871, NUITKA_BYTECODE_FLAG },
    { "distutils.command", NULL, 1297200, 518, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.command.bdist", NULL, 1297718, 3617, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_dumb", NULL, 1301335, 3532, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_msi", NULL, 1304867, 19364, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_rpm", NULL, 1324231, 12457, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_wininst", NULL, 1336688, 7972, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build", NULL, 1344660, 3798, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_clib", NULL, 1348458, 4848, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_ext", NULL, 1353306, 15755, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_py", NULL, 1369061, 10354, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_scripts", NULL, 1379415, 4253, NUITKA_BYTECODE_FLAG },
    { "distutils.command.check", NULL, 1383668, 4773, NUITKA_BYTECODE_FLAG },
    { "distutils.command.clean", NULL, 1388441, 2055, NUITKA_BYTECODE_FLAG },
    { "distutils.command.config", NULL, 1390496, 10159, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install", NULL, 1400655, 13482, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_data", NULL, 1414137, 2248, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_egg_info", NULL, 1416385, 2946, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_headers", NULL, 1419331, 1663, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_lib", NULL, 1420994, 5041, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_scripts", NULL, 1426035, 2105, NUITKA_BYTECODE_FLAG },
    { "distutils.command.register", NULL, 1428140, 8440, NUITKA_BYTECODE_FLAG },
    { "distutils.command.sdist", NULL, 1436580, 14471, NUITKA_BYTECODE_FLAG },
    { "distutils.command.upload", NULL, 1451051, 5053, NUITKA_BYTECODE_FLAG },
    { "distutils.config", NULL, 1456104, 3448, NUITKA_BYTECODE_FLAG },
    { "distutils.core", NULL, 1459552, 6573, NUITKA_BYTECODE_FLAG },
    { "distutils.cygwinccompiler", NULL, 1466125, 8491, NUITKA_BYTECODE_FLAG },
    { "distutils.debug", NULL, 1474616, 171, NUITKA_BYTECODE_FLAG },
    { "distutils.dep_util", NULL, 1474787, 2687, NUITKA_BYTECODE_FLAG },
    { "distutils.dir_util", NULL, 1477474, 5781, NUITKA_BYTECODE_FLAG },
    { "distutils.dist", NULL, 1483255, 34402, NUITKA_BYTECODE_FLAG },
    { "distutils.errors", NULL, 1517657, 5457, NUITKA_BYTECODE_FLAG },
    { "distutils.extension", NULL, 1523114, 6868, NUITKA_BYTECODE_FLAG },
    { "distutils.fancy_getopt", NULL, 1529982, 10580, NUITKA_BYTECODE_FLAG },
    { "distutils.file_util", NULL, 1540562, 5866, NUITKA_BYTECODE_FLAG },
    { "distutils.filelist", NULL, 1546428, 9801, NUITKA_BYTECODE_FLAG },
    { "distutils.log", NULL, 1556229, 2282, NUITKA_BYTECODE_FLAG },
    { "distutils.msvc9compiler", NULL, 1558511, 17346, NUITKA_BYTECODE_FLAG },
    { "distutils.msvccompiler", NULL, 1575857, 14534, NUITKA_BYTECODE_FLAG },
    { "distutils.spawn", NULL, 1590391, 4972, NUITKA_BYTECODE_FLAG },
    { "distutils.sysconfig", NULL, 1595363, 11946, NUITKA_BYTECODE_FLAG },
    { "distutils.text_file", NULL, 1607309, 8408, NUITKA_BYTECODE_FLAG },
    { "distutils.unixccompiler", NULL, 1615717, 6503, NUITKA_BYTECODE_FLAG },
    { "distutils.util", NULL, 1622220, 15064, NUITKA_BYTECODE_FLAG },
    { "distutils.version", NULL, 1637284, 7319, NUITKA_BYTECODE_FLAG },
    { "distutils.versionpredicate", NULL, 1644603, 5066, NUITKA_BYTECODE_FLAG },
    { "doctest", NULL, 1649669, 75397, NUITKA_BYTECODE_FLAG },
    { "dummy_threading", NULL, 1725066, 1087, NUITKA_BYTECODE_FLAG },
    { "email", NULL, 1726153, 1654, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email._encoded_words", NULL, 1727807, 5571, NUITKA_BYTECODE_FLAG },
    { "email._header_value_parser", NULL, 1733378, 75593, NUITKA_BYTECODE_FLAG },
    { "email._parseaddr", NULL, 1808971, 12311, NUITKA_BYTECODE_FLAG },
    { "email._policybase", NULL, 1821282, 14813, NUITKA_BYTECODE_FLAG },
    { "email.base64mime", NULL, 1836095, 3198, NUITKA_BYTECODE_FLAG },
    { "email.charset", NULL, 1839293, 11492, NUITKA_BYTECODE_FLAG },
    { "email.contentmanager", NULL, 1850785, 7258, NUITKA_BYTECODE_FLAG },
    { "email.encoders", NULL, 1858043, 1627, NUITKA_BYTECODE_FLAG },
    { "email.errors", NULL, 1859670, 6154, NUITKA_BYTECODE_FLAG },
    { "email.feedparser", NULL, 1865824, 10588, NUITKA_BYTECODE_FLAG },
    { "email.generator", NULL, 1876412, 12463, NUITKA_BYTECODE_FLAG },
    { "email.header", NULL, 1888875, 16343, NUITKA_BYTECODE_FLAG },
    { "email.headerregistry", NULL, 1905218, 21260, NUITKA_BYTECODE_FLAG },
    { "email.iterators", NULL, 1926478, 1895, NUITKA_BYTECODE_FLAG },
    { "email.message", NULL, 1928373, 37903, NUITKA_BYTECODE_FLAG },
    { "email.mime", NULL, 1966276, 109, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email.mime.application", NULL, 1966385, 1420, NUITKA_BYTECODE_FLAG },
    { "email.mime.audio", NULL, 1967805, 2579, NUITKA_BYTECODE_FLAG },
    { "email.mime.base", NULL, 1970384, 1045, NUITKA_BYTECODE_FLAG },
    { "email.mime.image", NULL, 1971429, 1865, NUITKA_BYTECODE_FLAG },
    { "email.mime.message", NULL, 1973294, 1294, NUITKA_BYTECODE_FLAG },
    { "email.mime.multipart", NULL, 1974588, 1516, NUITKA_BYTECODE_FLAG },
    { "email.mime.nonmultipart", NULL, 1976104, 731, NUITKA_BYTECODE_FLAG },
    { "email.mime.text", NULL, 1976835, 1278, NUITKA_BYTECODE_FLAG },
    { "email.parser", NULL, 1978113, 5710, NUITKA_BYTECODE_FLAG },
    { "email.policy", NULL, 1983823, 9603, NUITKA_BYTECODE_FLAG },
    { "email.quoprimime", NULL, 1993426, 7627, NUITKA_BYTECODE_FLAG },
    { "email.utils", NULL, 2001053, 9430, NUITKA_BYTECODE_FLAG },
    { "filecmp", NULL, 2010483, 8270, NUITKA_BYTECODE_FLAG },
    { "fileinput", NULL, 2018753, 13161, NUITKA_BYTECODE_FLAG },
    { "fnmatch", NULL, 2031914, 3300, NUITKA_BYTECODE_FLAG },
    { "formatter", NULL, 2035214, 17516, NUITKA_BYTECODE_FLAG },
    { "fractions", NULL, 2052730, 18391, NUITKA_BYTECODE_FLAG },
    { "ftplib", NULL, 2071121, 28030, NUITKA_BYTECODE_FLAG },
    { "getopt", NULL, 2099151, 6202, NUITKA_BYTECODE_FLAG },
    { "getpass", NULL, 2105353, 4127, NUITKA_BYTECODE_FLAG },
    { "gettext", NULL, 2109480, 14131, NUITKA_BYTECODE_FLAG },
    { "glob", NULL, 2123611, 4222, NUITKA_BYTECODE_FLAG },
    { "gzip", NULL, 2127833, 17148, NUITKA_BYTECODE_FLAG },
    { "hashlib", NULL, 2144981, 6554, NUITKA_BYTECODE_FLAG },
    { "hmac", NULL, 2151535, 6076, NUITKA_BYTECODE_FLAG },
    { "html", NULL, 2157611, 3360, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "html.entities", NULL, 2160971, 50432, NUITKA_BYTECODE_FLAG },
    { "html.parser", NULL, 2211403, 11070, NUITKA_BYTECODE_FLAG },
    { "http", NULL, 2222473, 5885, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "http.client", NULL, 2228358, 34011, NUITKA_BYTECODE_FLAG },
    { "http.cookiejar", NULL, 2262369, 53548, NUITKA_BYTECODE_FLAG },
    { "http.cookies", NULL, 2315917, 15207, NUITKA_BYTECODE_FLAG },
    { "http.server", NULL, 2331124, 33330, NUITKA_BYTECODE_FLAG },
    { "imaplib", NULL, 2364454, 41393, NUITKA_BYTECODE_FLAG },
    { "imghdr", NULL, 2405847, 4105, NUITKA_BYTECODE_FLAG },
    { "imp", NULL, 2409952, 9716, NUITKA_BYTECODE_FLAG },
    { "importlib.abc", NULL, 2419668, 13450, NUITKA_BYTECODE_FLAG },
    { "importlib.resources", NULL, 2433118, 8302, NUITKA_BYTECODE_FLAG },
    { "importlib.util", NULL, 2441420, 9319, NUITKA_BYTECODE_FLAG },
    { "ipaddress", NULL, 2450739, 62957, NUITKA_BYTECODE_FLAG },
    { "json", NULL, 2513696, 12301, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "json.decoder", NULL, 2525997, 9787, NUITKA_BYTECODE_FLAG },
    { "json.encoder", NULL, 2535784, 11269, NUITKA_BYTECODE_FLAG },
    { "json.scanner", NULL, 2547053, 1959, NUITKA_BYTECODE_FLAG },
    { "json.tool", NULL, 2549012, 1440, NUITKA_BYTECODE_FLAG },
    { "lib2to3", NULL, 2550452, 106, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.btm_matcher", NULL, 2550558, 4856, NUITKA_BYTECODE_FLAG },
    { "lib2to3.btm_utils", NULL, 2555414, 6106, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_base", NULL, 2561520, 6198, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_util", NULL, 2567718, 12009, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes", NULL, 2579727, 112, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.fixes.fix_apply", NULL, 2579839, 1650, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_asserts", NULL, 2581489, 1235, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_basestring", NULL, 2582724, 625, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_buffer", NULL, 2583349, 770, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_dict", NULL, 2584119, 3278, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_except", NULL, 2587397, 2780, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exec", NULL, 2590177, 1111, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_execfile", NULL, 2591288, 1655, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exitfunc", NULL, 2592943, 2258, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_filter", NULL, 2595201, 2323, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_funcattrs", NULL, 2597524, 936, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_future", NULL, 2598460, 746, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_getcwdu", NULL, 2599206, 750, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_has_key", NULL, 2599956, 2880, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_idioms", NULL, 2602836, 3865, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_import", NULL, 2606701, 2747, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports", NULL, 2609448, 4311, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports2", NULL, 2613759, 510, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_input", NULL, 2614269, 912, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_intern", NULL, 2615181, 1119, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_isinstance", NULL, 2616300, 1517, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools", NULL, 2617817, 1506, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools_imports", NULL, 2619323, 1542, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_long", NULL, 2620865, 667, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_map", NULL, 2621532, 3055, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_metaclass", NULL, 2624587, 5308, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_methodattrs", NULL, 2629895, 898, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ne", NULL, 2630793, 769, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_next", NULL, 2631562, 3022, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_nonzero", NULL, 2634584, 885, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_numliterals", NULL, 2635469, 981, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_operator", NULL, 2636450, 4198, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_paren", NULL, 2640648, 1352, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_print", NULL, 2642000, 2287, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raise", NULL, 2644287, 2211, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raw_input", NULL, 2646498, 757, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_reduce", NULL, 2647255, 1092, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_reload", NULL, 2648347, 1131, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_renames", NULL, 2649478, 1955, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_repr", NULL, 2651433, 807, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_set_literal", NULL, 2652240, 1639, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_standarderror", NULL, 2653879, 682, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_sys_exc", NULL, 2654561, 1363, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_throw", NULL, 2655924, 1764, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_tuple_params", NULL, 2657688, 4558, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_types", NULL, 2662246, 1791, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_unicode", NULL, 2664037, 1503, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_urllib", NULL, 2665540, 5923, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ws_comma", NULL, 2671463, 1085, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xrange", NULL, 2672548, 2498, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xreadlines", NULL, 2675046, 1079, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_zip", NULL, 2676125, 1543, NUITKA_BYTECODE_FLAG },
    { "lib2to3.main", NULL, 2677668, 8499, NUITKA_BYTECODE_FLAG },
    { "lib2to3.patcomp", NULL, 2686167, 5574, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2", NULL, 2691741, 142, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.pgen2.driver", NULL, 2691883, 5103, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.grammar", NULL, 2696986, 6979, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.literals", NULL, 2703965, 1521, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.parse", NULL, 2705486, 6267, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.pgen", NULL, 2711753, 9743, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.token", NULL, 2721496, 1835, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.tokenize", NULL, 2723331, 15103, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pygram", NULL, 2738434, 1161, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pytree", NULL, 2739595, 24966, NUITKA_BYTECODE_FLAG },
    { "lib2to3.refactor", NULL, 2764561, 20367, NUITKA_BYTECODE_FLAG },
    { "logging", NULL, 2784928, 62521, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "logging.config", NULL, 2847449, 22978, NUITKA_BYTECODE_FLAG },
    { "logging.handlers", NULL, 2870427, 42946, NUITKA_BYTECODE_FLAG },
    { "lzma", NULL, 2913373, 11902, NUITKA_BYTECODE_FLAG },
    { "macpath", NULL, 2925275, 5770, NUITKA_BYTECODE_FLAG },
    { "mailbox", NULL, 2931045, 63611, NUITKA_BYTECODE_FLAG },
    { "mailcap", NULL, 2994656, 6447, NUITKA_BYTECODE_FLAG },
    { "mimetypes", NULL, 3001103, 15441, NUITKA_BYTECODE_FLAG },
    { "modulefinder", NULL, 3016544, 15315, NUITKA_BYTECODE_FLAG },
    { "msilib", NULL, 3031859, 15809, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "msilib.schema", NULL, 3047668, 56932, NUITKA_BYTECODE_FLAG },
    { "msilib.sequence", NULL, 3104600, 2587, NUITKA_BYTECODE_FLAG },
    { "msilib.text", NULL, 3107187, 8944, NUITKA_BYTECODE_FLAG },
    { "multiprocessing", NULL, 3116131, 490, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.connection", NULL, 3116621, 24895, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.context", NULL, 3141516, 13076, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.dummy", NULL, 3154592, 3768, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.dummy.connection", NULL, 3158360, 2476, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.forkserver", NULL, 3160836, 7727, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.heap", NULL, 3168563, 6387, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.managers", NULL, 3174950, 33967, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.pool", NULL, 3208917, 21199, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.popen_spawn_win32", NULL, 3230116, 3395, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.process", NULL, 3233511, 9389, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.queues", NULL, 3242900, 9400, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.reduction", NULL, 3252300, 7981, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.resource_sharer", NULL, 3260281, 5166, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.semaphore_tracker", NULL, 3265447, 3703, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.sharedctypes", NULL, 3269150, 6880, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.spawn", NULL, 3276030, 6431, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.synchronize", NULL, 3282461, 11146, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.util", NULL, 3293607, 9910, NUITKA_BYTECODE_FLAG },
    { "netrc", NULL, 3303517, 3726, NUITKA_BYTECODE_FLAG },
    { "nntplib", NULL, 3307243, 33714, NUITKA_BYTECODE_FLAG },
    { "nturl2path", NULL, 3340957, 1578, NUITKA_BYTECODE_FLAG },
    { "numbers", NULL, 3342535, 12155, NUITKA_BYTECODE_FLAG },
    { "optparse", NULL, 3354690, 47856, NUITKA_BYTECODE_FLAG },
    { "pathlib", NULL, 3402546, 41595, NUITKA_BYTECODE_FLAG },
    { "pdb", NULL, 3444141, 46839, NUITKA_BYTECODE_FLAG },
    { "pickle", NULL, 3490980, 42929, NUITKA_BYTECODE_FLAG },
    { "pickletools", NULL, 3533909, 65294, NUITKA_BYTECODE_FLAG },
    { "pipes", NULL, 3599203, 7766, NUITKA_BYTECODE_FLAG },
    { "pkgutil", NULL, 3606969, 16323, NUITKA_BYTECODE_FLAG },
    { "platform", NULL, 3623292, 28183, NUITKA_BYTECODE_FLAG },
    { "plistlib", NULL, 3651475, 25058, NUITKA_BYTECODE_FLAG },
    { "poplib", NULL, 3676533, 13299, NUITKA_BYTECODE_FLAG },
    { "posixpath", NULL, 3689832, 10393, NUITKA_BYTECODE_FLAG },
    { "pprint", NULL, 3700225, 15796, NUITKA_BYTECODE_FLAG },
    { "profile", NULL, 3716021, 13804, NUITKA_BYTECODE_FLAG },
    { "pstats", NULL, 3729825, 22257, NUITKA_BYTECODE_FLAG },
    { "py_compile", NULL, 3752082, 7154, NUITKA_BYTECODE_FLAG },
    { "pyclbr", NULL, 3759236, 10336, NUITKA_BYTECODE_FLAG },
    { "pydoc", NULL, 3769572, 84393, NUITKA_BYTECODE_FLAG },
    { "pydoc_data", NULL, 3853965, 109, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "pydoc_data.topics", NULL, 3854074, 411559, NUITKA_BYTECODE_FLAG },
    { "queue", NULL, 4265633, 11435, NUITKA_BYTECODE_FLAG },
    { "random", NULL, 4277068, 19371, NUITKA_BYTECODE_FLAG },
    { "rlcompleter", NULL, 4296439, 5710, NUITKA_BYTECODE_FLAG },
    { "runpy", NULL, 4302149, 7908, NUITKA_BYTECODE_FLAG },
    { "sched", NULL, 4310057, 6484, NUITKA_BYTECODE_FLAG },
    { "secrets", NULL, 4316541, 2147, NUITKA_BYTECODE_FLAG },
    { "selectors", NULL, 4318688, 16911, NUITKA_BYTECODE_FLAG },
    { "shelve", NULL, 4335599, 9469, NUITKA_BYTECODE_FLAG },
    { "shlex", NULL, 4345068, 6953, NUITKA_BYTECODE_FLAG },
    { "shutil", NULL, 4352021, 30827, NUITKA_BYTECODE_FLAG },
    { "signal", NULL, 4382848, 2475, NUITKA_BYTECODE_FLAG },
    { "site", NULL, 565, 13219, NUITKA_BYTECODE_FLAG },
    { "smtpd", NULL, 4385323, 26567, NUITKA_BYTECODE_FLAG },
    { "smtplib", NULL, 4411890, 35313, NUITKA_BYTECODE_FLAG },
    { "sndhdr", NULL, 4447203, 6866, NUITKA_BYTECODE_FLAG },
    { "socket", NULL, 4454069, 21984, NUITKA_BYTECODE_FLAG },
    { "socketserver", NULL, 4476053, 24162, NUITKA_BYTECODE_FLAG },
    { "sqlite3", NULL, 4500215, 137, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "sqlite3.dbapi2", NULL, 4500352, 2456, NUITKA_BYTECODE_FLAG },
    { "sqlite3.dump", NULL, 4502808, 1899, NUITKA_BYTECODE_FLAG },
    { "ssl", NULL, 4504707, 39734, NUITKA_BYTECODE_FLAG },
    { "statistics", NULL, 4544441, 18127, NUITKA_BYTECODE_FLAG },
    { "string", NULL, 4562568, 7798, NUITKA_BYTECODE_FLAG },
    { "subprocess", NULL, 4570366, 38728, NUITKA_BYTECODE_FLAG },
    { "sunau", NULL, 4609094, 17174, NUITKA_BYTECODE_FLAG },
    { "symbol", NULL, 4626268, 2528, NUITKA_BYTECODE_FLAG },
    { "symtable", NULL, 4628796, 10408, NUITKA_BYTECODE_FLAG },
    { "sysconfig", NULL, 4639204, 15407, NUITKA_BYTECODE_FLAG },
    { "tabnanny", NULL, 4654611, 6941, NUITKA_BYTECODE_FLAG },
    { "tarfile", NULL, 4661552, 61804, NUITKA_BYTECODE_FLAG },
    { "telnetlib", NULL, 4723356, 18065, NUITKA_BYTECODE_FLAG },
    { "tempfile", NULL, 4741421, 22108, NUITKA_BYTECODE_FLAG },
    { "textwrap", NULL, 4763529, 13575, NUITKA_BYTECODE_FLAG },
    { "this", NULL, 4777104, 1240, NUITKA_BYTECODE_FLAG },
    { "timeit", NULL, 4778344, 11610, NUITKA_BYTECODE_FLAG },
    { "tkinter", NULL, 4789954, 179138, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "tkinter.colorchooser", NULL, 4969092, 1098, NUITKA_BYTECODE_FLAG },
    { "tkinter.commondialog", NULL, 4970190, 1079, NUITKA_BYTECODE_FLAG },
    { "tkinter.constants", NULL, 4971269, 1631, NUITKA_BYTECODE_FLAG },
    { "tkinter.dialog", NULL, 4972900, 1407, NUITKA_BYTECODE_FLAG },
    { "tkinter.dnd", NULL, 4974307, 11149, NUITKA_BYTECODE_FLAG },
    { "tkinter.filedialog", NULL, 4985456, 12233, NUITKA_BYTECODE_FLAG },
    { "tkinter.font", NULL, 4997689, 6150, NUITKA_BYTECODE_FLAG },
    { "tkinter.messagebox", NULL, 5003839, 3015, NUITKA_BYTECODE_FLAG },
    { "tkinter.scrolledtext", NULL, 5006854, 2142, NUITKA_BYTECODE_FLAG },
    { "tkinter.simpledialog", NULL, 5008996, 10509, NUITKA_BYTECODE_FLAG },
    { "tkinter.tix", NULL, 5019505, 83629, NUITKA_BYTECODE_FLAG },
    { "tkinter.ttk", NULL, 5103134, 57813, NUITKA_BYTECODE_FLAG },
    { "trace", NULL, 5160947, 19110, NUITKA_BYTECODE_FLAG },
    { "tracemalloc", NULL, 5180057, 17239, NUITKA_BYTECODE_FLAG },
    { "turtle", NULL, 5197296, 130754, NUITKA_BYTECODE_FLAG },
    { "typing", NULL, 5328050, 49921, NUITKA_BYTECODE_FLAG },
    { "unittest", NULL, 5377971, 2974, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "unittest.case", NULL, 5380945, 48046, NUITKA_BYTECODE_FLAG },
    { "unittest.loader", NULL, 5428991, 14239, NUITKA_BYTECODE_FLAG },
    { "unittest.main", NULL, 5443230, 7401, NUITKA_BYTECODE_FLAG },
    { "unittest.mock", NULL, 5450631, 63169, NUITKA_BYTECODE_FLAG },
    { "unittest.result", NULL, 5513800, 7215, NUITKA_BYTECODE_FLAG },
    { "unittest.runner", NULL, 5521015, 6958, NUITKA_BYTECODE_FLAG },
    { "unittest.signals", NULL, 5527973, 2157, NUITKA_BYTECODE_FLAG },
    { "unittest.suite", NULL, 5530130, 9166, NUITKA_BYTECODE_FLAG },
    { "unittest.util", NULL, 5539296, 4484, NUITKA_BYTECODE_FLAG },
    { "urllib", NULL, 5543780, 105, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "urllib.error", NULL, 5543885, 2739, NUITKA_BYTECODE_FLAG },
    { "urllib.parse", NULL, 5546624, 30393, NUITKA_BYTECODE_FLAG },
    { "urllib.request", NULL, 5577017, 72267, NUITKA_BYTECODE_FLAG },
    { "urllib.response", NULL, 5649284, 3212, NUITKA_BYTECODE_FLAG },
    { "urllib.robotparser", NULL, 5652496, 7024, NUITKA_BYTECODE_FLAG },
    { "uu", NULL, 5659520, 3535, NUITKA_BYTECODE_FLAG },
    { "uuid", NULL, 5663055, 23162, NUITKA_BYTECODE_FLAG },
    { "venv", NULL, 5686217, 13800, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "wave", NULL, 5700017, 18251, NUITKA_BYTECODE_FLAG },
    { "weakref", NULL, 5718268, 19181, NUITKA_BYTECODE_FLAG },
    { "webbrowser", NULL, 5737449, 16337, NUITKA_BYTECODE_FLAG },
    { "wsgiref", NULL, 5753786, 701, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "wsgiref.handlers", NULL, 5754487, 16110, NUITKA_BYTECODE_FLAG },
    { "wsgiref.headers", NULL, 5770597, 7722, NUITKA_BYTECODE_FLAG },
    { "wsgiref.simple_server", NULL, 5778319, 5168, NUITKA_BYTECODE_FLAG },
    { "wsgiref.util", NULL, 5783487, 5143, NUITKA_BYTECODE_FLAG },
    { "wsgiref.validate", NULL, 5788630, 14639, NUITKA_BYTECODE_FLAG },
    { "xdrlib", NULL, 5803269, 8287, NUITKA_BYTECODE_FLAG },
    { "xml", NULL, 5811556, 669, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom", NULL, 5812225, 5421, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom.NodeFilter", NULL, 5817646, 936, NUITKA_BYTECODE_FLAG },
    { "xml.dom.domreg", NULL, 5818582, 2813, NUITKA_BYTECODE_FLAG },
    { "xml.dom.expatbuilder", NULL, 5821395, 26983, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minicompat", NULL, 5848378, 2782, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minidom", NULL, 5851160, 55582, NUITKA_BYTECODE_FLAG },
    { "xml.dom.pulldom", NULL, 5906742, 10455, NUITKA_BYTECODE_FLAG },
    { "xml.dom.xmlbuilder", NULL, 5917197, 12402, NUITKA_BYTECODE_FLAG },
    { "xml.etree", NULL, 5929599, 108, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.etree.ElementInclude", NULL, 5929707, 1544, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementPath", NULL, 5931251, 6312, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementTree", NULL, 5937563, 44776, NUITKA_BYTECODE_FLAG },
    { "xml.etree.cElementTree", NULL, 5982339, 150, NUITKA_BYTECODE_FLAG },
    { "xml.parsers", NULL, 5982489, 282, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.parsers.expat", NULL, 5982771, 311, NUITKA_BYTECODE_FLAG },
    { "xml.sax", NULL, 5983082, 3141, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.sax._exceptions", NULL, 5986223, 5450, NUITKA_BYTECODE_FLAG },
    { "xml.sax.expatreader", NULL, 5991673, 12353, NUITKA_BYTECODE_FLAG },
    { "xml.sax.handler", NULL, 6004026, 12326, NUITKA_BYTECODE_FLAG },
    { "xml.sax.saxutils", NULL, 6016352, 12779, NUITKA_BYTECODE_FLAG },
    { "xml.sax.xmlreader", NULL, 6029131, 16887, NUITKA_BYTECODE_FLAG },
    { "xmlrpc", NULL, 6046018, 105, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xmlrpc.client", NULL, 6046123, 34511, NUITKA_BYTECODE_FLAG },
    { "xmlrpc.server", NULL, 6080634, 29353, NUITKA_BYTECODE_FLAG },
    { "zipapp", NULL, 6109987, 5766, NUITKA_BYTECODE_FLAG },
    { "zipfile", NULL, 6115753, 49835, NUITKA_BYTECODE_FLAG },
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
