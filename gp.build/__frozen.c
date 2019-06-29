// This provides the frozen (compiled bytecode) files that are included if
// any.
#include <Python.h>

#include "nuitka/constants_blob.h"

// Blob from which modules are unstreamed.
#define stream_data constant_bin

// These modules should be loaded as bytecode. They may e.g. have to be loadable
// during "Py_Initialize" already, or for irrelevance, they are only included
// in this un-optimized form. These are not compiled by Nuitka, and therefore
// are not accelerated at all, merely bundled with the binary or module, so
// that CPython library can start out finding them.

struct frozen_desc {
    char const *name;
    ssize_t start;
    int size;
};

void copyFrozenModulesTo( struct _frozen *destination )
{
    struct frozen_desc frozen_modules[] = {
        { "base64", 3077824, 10962 },
        { "codecs", 3088786, 36364 },
        { "copy_reg", 3125150, 5050 },
        { "encodings", 3130200, -4350 },
        { "encodings.aliases", 3134550, 8757 },
        { "encodings.ascii", 3143307, 2223 },
        { "encodings.cp1252", 3145530, 2833 },
        { "encodings.cp437", 3148363, 8031 },
        { "encodings.idna", 3156394, 6326 },
        { "encodings.latin_1", 3162720, 2253 },
        { "encodings.mbcs", 3164973, 1995 },
        { "encodings.utf_16_be", 3166968, 1966 },
        { "encodings.utf_16_le", 3168934, 1966 },
        { "encodings.utf_32_be", 3170900, 1859 },
        { "encodings.utf_8", 3172759, 1918 },
        { "functools", 3174677, 5951 },
        { "locale", 3180628, 54920 },
        { "quopri", 3235548, 6511 },
        { "re", 3242059, 13300 },
        { "sre_compile", 3255359, 12476 },
        { "sre_constants", 3267835, 6162 },
        { "sre_parse", 3273997, 19829 },
        { "string", 3293826, 20196 },
        { "stringprep", 3314022, 14381 },
        { "struct", 3328403, 226 },
        { "types", 3328629, 2682 },
        { NULL, 0, 0 }
    };

    struct frozen_desc *current = frozen_modules;

    for(;;)
    {
        destination->name = (char *)current->name;
        destination->code = (unsigned char *)&constant_bin[ current->start ];
        destination->size = current->size;

        if (destination->name == NULL) break;

        current += 1;
        destination += 1;
    };
}
