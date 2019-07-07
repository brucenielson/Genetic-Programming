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
        { "_collections_abc", 6165528, 28905 },
        { "_compression", 6194433, 4087 },
        { "_weakrefset", 6198520, 7425 },
        { "abc", 6205945, 6414 },
        { "base64", 6212359, 16951 },
        { "bz2", 6229310, 11144 },
        { "codecs", 6240454, 33861 },
        { "collections", 6274315, -46573 },
        { "collections.abc", 6165528, 28905 },
        { "copyreg", 6320888, 4207 },
        { "dis", 6325095, 15168 },
        { "encodings", 6340263, -3910 },
        { "encodings.aliases", 6344173, 6259 },
        { "encodings.ascii", 6350432, 1847 },
        { "encodings.base64_codec", 6352279, 2386 },
        { "encodings.big5", 6354665, 1407 },
        { "encodings.big5hkscs", 6356072, 1417 },
        { "encodings.bz2_codec", 6357489, 3248 },
        { "encodings.charmap", 6360737, 2900 },
        { "encodings.cp037", 6363637, 2392 },
        { "encodings.cp1006", 6366029, 2468 },
        { "encodings.cp1026", 6368497, 2396 },
        { "encodings.cp1125", 6370893, 8089 },
        { "encodings.cp1140", 6378982, 2382 },
        { "encodings.cp1250", 6381364, 2419 },
        { "encodings.cp1251", 6383783, 2416 },
        { "encodings.cp1252", 6386199, 2419 },
        { "encodings.cp1253", 6388618, 2432 },
        { "encodings.cp1254", 6391050, 2421 },
        { "encodings.cp1255", 6393471, 2440 },
        { "encodings.cp1256", 6395911, 2418 },
        { "encodings.cp1257", 6398329, 2426 },
        { "encodings.cp1258", 6400755, 2424 },
        { "encodings.cp273", 6403179, 2378 },
        { "encodings.cp424", 6405557, 2422 },
        { "encodings.cp437", 6407979, 7806 },
        { "encodings.cp500", 6415785, 2392 },
        { "encodings.cp65001", 6418177, 1646 },
        { "encodings.cp720", 6419823, 2489 },
        { "encodings.cp737", 6422312, 8128 },
        { "encodings.cp775", 6430440, 7836 },
        { "encodings.cp850", 6438276, 7467 },
        { "encodings.cp852", 6445743, 7844 },
        { "encodings.cp855", 6453587, 8097 },
        { "encodings.cp856", 6461684, 2454 },
        { "encodings.cp857", 6464138, 7449 },
        { "encodings.cp858", 6471587, 7437 },
        { "encodings.cp860", 6479024, 7785 },
        { "encodings.cp861", 6486809, 7800 },
        { "encodings.cp862", 6494609, 7989 },
        { "encodings.cp863", 6502598, 7800 },
        { "encodings.cp864", 6510398, 7946 },
        { "encodings.cp865", 6518344, 7800 },
        { "encodings.cp866", 6526144, 8133 },
        { "encodings.cp869", 6534277, 7826 },
        { "encodings.cp874", 6542103, 2520 },
        { "encodings.cp875", 6544623, 2389 },
        { "encodings.cp932", 6547012, 1409 },
        { "encodings.cp949", 6548421, 1409 },
        { "encodings.cp950", 6549830, 1409 },
        { "encodings.euc_jis_2004", 6551239, 1423 },
        { "encodings.euc_jisx0213", 6552662, 1423 },
        { "encodings.euc_jp", 6554085, 1411 },
        { "encodings.euc_kr", 6555496, 1411 },
        { "encodings.gb18030", 6556907, 1413 },
        { "encodings.gb2312", 6558320, 1411 },
        { "encodings.gbk", 6559731, 1405 },
        { "encodings.hex_codec", 6561136, 2373 },
        { "encodings.hp_roman8", 6563509, 2593 },
        { "encodings.hz", 6566102, 1403 },
        { "encodings.idna", 6567505, 5687 },
        { "encodings.iso2022_jp", 6573192, 1424 },
        { "encodings.iso2022_jp_1", 6574616, 1428 },
        { "encodings.iso2022_jp_2", 6576044, 1428 },
        { "encodings.iso2022_jp_2004", 6577472, 1434 },
        { "encodings.iso2022_jp_3", 6578906, 1428 },
        { "encodings.iso2022_jp_ext", 6580334, 1432 },
        { "encodings.iso2022_kr", 6581766, 1424 },
        { "encodings.iso8859_1", 6583190, 2391 },
        { "encodings.iso8859_10", 6585581, 2396 },
        { "encodings.iso8859_11", 6587977, 2490 },
        { "encodings.iso8859_13", 6590467, 2399 },
        { "encodings.iso8859_14", 6592866, 2417 },
        { "encodings.iso8859_15", 6595283, 2396 },
        { "encodings.iso8859_16", 6597679, 2398 },
        { "encodings.iso8859_2", 6600077, 2391 },
        { "encodings.iso8859_3", 6602468, 2398 },
        { "encodings.iso8859_4", 6604866, 2391 },
        { "encodings.iso8859_5", 6607257, 2392 },
        { "encodings.iso8859_6", 6609649, 2436 },
        { "encodings.iso8859_7", 6612085, 2399 },
        { "encodings.iso8859_8", 6614484, 2430 },
        { "encodings.iso8859_9", 6616914, 2391 },
        { "encodings.johab", 6619305, 1409 },
        { "encodings.koi8_r", 6620714, 2443 },
        { "encodings.koi8_t", 6623157, 2354 },
        { "encodings.koi8_u", 6625511, 2429 },
        { "encodings.kz1048", 6627940, 2406 },
        { "encodings.latin_1", 6630346, 1859 },
        { "encodings.mac_arabic", 6632205, 7700 },
        { "encodings.mac_centeuro", 6639905, 2430 },
        { "encodings.mac_croatian", 6642335, 2438 },
        { "encodings.mac_cyrillic", 6644773, 2428 },
        { "encodings.mac_farsi", 6647201, 2372 },
        { "encodings.mac_greek", 6649573, 2412 },
        { "encodings.mac_iceland", 6651985, 2431 },
        { "encodings.mac_latin2", 6654416, 2572 },
        { "encodings.mac_roman", 6656988, 2429 },
        { "encodings.mac_romanian", 6659417, 2439 },
        { "encodings.mac_turkish", 6661856, 2432 },
        { "encodings.mbcs", 6664288, 1658 },
        { "encodings.oem", 6665946, 1471 },
        { "encodings.palmos", 6667417, 2419 },
        { "encodings.ptcp154", 6669836, 2513 },
        { "encodings.punycode", 6672349, 6380 },
        { "encodings.quopri_codec", 6678729, 2406 },
        { "encodings.raw_unicode_escape", 6681135, 1732 },
        { "encodings.rot_13", 6682867, 2992 },
        { "encodings.shift_jis", 6685859, 1417 },
        { "encodings.shift_jis_2004", 6687276, 1427 },
        { "encodings.shift_jisx0213", 6688703, 1427 },
        { "encodings.tis_620", 6690130, 2481 },
        { "encodings.undefined", 6692611, 2126 },
        { "encodings.unicode_escape", 6694737, 1712 },
        { "encodings.unicode_internal", 6696449, 1722 },
        { "encodings.utf_16", 6698171, 4796 },
        { "encodings.utf_16_be", 6702967, 1597 },
        { "encodings.utf_16_le", 6704564, 1597 },
        { "encodings.utf_32", 6706161, 4689 },
        { "encodings.utf_32_be", 6710850, 1490 },
        { "encodings.utf_32_le", 6712340, 1490 },
        { "encodings.utf_7", 6713830, 1518 },
        { "encodings.utf_8", 6715348, 1577 },
        { "encodings.utf_8_sig", 6716925, 4479 },
        { "encodings.uu_codec", 6721404, 3188 },
        { "encodings.zlib_codec", 6724592, 3086 },
        { "enum", 6727678, 24234 },
        { "functools", 6751912, 23918 },
        { "genericpath", 6775830, 3711 },
        { "heapq", 6779541, 14325 },
        { "importlib", 6793866, -3695 },
        { "importlib._bootstrap", 6797561, 29141 },
        { "importlib._bootstrap_external", 6826702, 41781 },
        { "importlib.machinery", 6868483, 935 },
        { "inspect", 6869418, 79995 },
        { "io", 6949413, 3372 },
        { "keyword", 6952785, 1772 },
        { "linecache", 6954557, 3752 },
        { "locale", 6958309, 34518 },
        { "ntpath", 6992827, 12967 },
        { "opcode", 7005794, 5341 },
        { "operator", 7011135, 13863 },
        { "os", 7024998, 29656 },
        { "quopri", 7054654, 5734 },
        { "re", 7060388, 13767 },
        { "reprlib", 7074155, 5313 },
        { "sre_compile", 7079468, 15166 },
        { "sre_constants", 7094634, 6254 },
        { "sre_parse", 7100888, 21320 },
        { "stat", 7122208, 3836 },
        { "stringprep", 7126044, 9995 },
        { "struct", 7136039, 297 },
        { "threading", 7136336, 37665 },
        { "token", 7174001, 3562 },
        { "tokenize", 7177563, 17794 },
        { "traceback", 7195357, 19586 },
        { "types", 7214943, 8939 },
        { "warnings", 7223882, 13903 },
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
