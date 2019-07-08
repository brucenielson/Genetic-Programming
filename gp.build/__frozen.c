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
        { "_collections_abc", 6181571, 28905 },
        { "_compression", 6210476, 4087 },
        { "_weakrefset", 6214563, 7425 },
        { "abc", 6221988, 6414 },
        { "base64", 6228402, 16951 },
        { "bz2", 6245353, 11144 },
        { "codecs", 6256497, 33861 },
        { "collections", 6290358, -46573 },
        { "collections.abc", 6181571, 28905 },
        { "copyreg", 6336931, 4207 },
        { "dis", 6341138, 15168 },
        { "encodings", 6356306, -3910 },
        { "encodings.aliases", 6360216, 6259 },
        { "encodings.ascii", 6366475, 1847 },
        { "encodings.base64_codec", 6368322, 2386 },
        { "encodings.big5", 6370708, 1407 },
        { "encodings.big5hkscs", 6372115, 1417 },
        { "encodings.bz2_codec", 6373532, 3248 },
        { "encodings.charmap", 6376780, 2900 },
        { "encodings.cp037", 6379680, 2392 },
        { "encodings.cp1006", 6382072, 2468 },
        { "encodings.cp1026", 6384540, 2396 },
        { "encodings.cp1125", 6386936, 8089 },
        { "encodings.cp1140", 6395025, 2382 },
        { "encodings.cp1250", 6397407, 2419 },
        { "encodings.cp1251", 6399826, 2416 },
        { "encodings.cp1252", 6402242, 2419 },
        { "encodings.cp1253", 6404661, 2432 },
        { "encodings.cp1254", 6407093, 2421 },
        { "encodings.cp1255", 6409514, 2440 },
        { "encodings.cp1256", 6411954, 2418 },
        { "encodings.cp1257", 6414372, 2426 },
        { "encodings.cp1258", 6416798, 2424 },
        { "encodings.cp273", 6419222, 2378 },
        { "encodings.cp424", 6421600, 2422 },
        { "encodings.cp437", 6424022, 7806 },
        { "encodings.cp500", 6431828, 2392 },
        { "encodings.cp65001", 6434220, 1646 },
        { "encodings.cp720", 6435866, 2489 },
        { "encodings.cp737", 6438355, 8128 },
        { "encodings.cp775", 6446483, 7836 },
        { "encodings.cp850", 6454319, 7467 },
        { "encodings.cp852", 6461786, 7844 },
        { "encodings.cp855", 6469630, 8097 },
        { "encodings.cp856", 6477727, 2454 },
        { "encodings.cp857", 6480181, 7449 },
        { "encodings.cp858", 6487630, 7437 },
        { "encodings.cp860", 6495067, 7785 },
        { "encodings.cp861", 6502852, 7800 },
        { "encodings.cp862", 6510652, 7989 },
        { "encodings.cp863", 6518641, 7800 },
        { "encodings.cp864", 6526441, 7946 },
        { "encodings.cp865", 6534387, 7800 },
        { "encodings.cp866", 6542187, 8133 },
        { "encodings.cp869", 6550320, 7826 },
        { "encodings.cp874", 6558146, 2520 },
        { "encodings.cp875", 6560666, 2389 },
        { "encodings.cp932", 6563055, 1409 },
        { "encodings.cp949", 6564464, 1409 },
        { "encodings.cp950", 6565873, 1409 },
        { "encodings.euc_jis_2004", 6567282, 1423 },
        { "encodings.euc_jisx0213", 6568705, 1423 },
        { "encodings.euc_jp", 6570128, 1411 },
        { "encodings.euc_kr", 6571539, 1411 },
        { "encodings.gb18030", 6572950, 1413 },
        { "encodings.gb2312", 6574363, 1411 },
        { "encodings.gbk", 6575774, 1405 },
        { "encodings.hex_codec", 6577179, 2373 },
        { "encodings.hp_roman8", 6579552, 2593 },
        { "encodings.hz", 6582145, 1403 },
        { "encodings.idna", 6583548, 5687 },
        { "encodings.iso2022_jp", 6589235, 1424 },
        { "encodings.iso2022_jp_1", 6590659, 1428 },
        { "encodings.iso2022_jp_2", 6592087, 1428 },
        { "encodings.iso2022_jp_2004", 6593515, 1434 },
        { "encodings.iso2022_jp_3", 6594949, 1428 },
        { "encodings.iso2022_jp_ext", 6596377, 1432 },
        { "encodings.iso2022_kr", 6597809, 1424 },
        { "encodings.iso8859_1", 6599233, 2391 },
        { "encodings.iso8859_10", 6601624, 2396 },
        { "encodings.iso8859_11", 6604020, 2490 },
        { "encodings.iso8859_13", 6606510, 2399 },
        { "encodings.iso8859_14", 6608909, 2417 },
        { "encodings.iso8859_15", 6611326, 2396 },
        { "encodings.iso8859_16", 6613722, 2398 },
        { "encodings.iso8859_2", 6616120, 2391 },
        { "encodings.iso8859_3", 6618511, 2398 },
        { "encodings.iso8859_4", 6620909, 2391 },
        { "encodings.iso8859_5", 6623300, 2392 },
        { "encodings.iso8859_6", 6625692, 2436 },
        { "encodings.iso8859_7", 6628128, 2399 },
        { "encodings.iso8859_8", 6630527, 2430 },
        { "encodings.iso8859_9", 6632957, 2391 },
        { "encodings.johab", 6635348, 1409 },
        { "encodings.koi8_r", 6636757, 2443 },
        { "encodings.koi8_t", 6639200, 2354 },
        { "encodings.koi8_u", 6641554, 2429 },
        { "encodings.kz1048", 6643983, 2406 },
        { "encodings.latin_1", 6646389, 1859 },
        { "encodings.mac_arabic", 6648248, 7700 },
        { "encodings.mac_centeuro", 6655948, 2430 },
        { "encodings.mac_croatian", 6658378, 2438 },
        { "encodings.mac_cyrillic", 6660816, 2428 },
        { "encodings.mac_farsi", 6663244, 2372 },
        { "encodings.mac_greek", 6665616, 2412 },
        { "encodings.mac_iceland", 6668028, 2431 },
        { "encodings.mac_latin2", 6670459, 2572 },
        { "encodings.mac_roman", 6673031, 2429 },
        { "encodings.mac_romanian", 6675460, 2439 },
        { "encodings.mac_turkish", 6677899, 2432 },
        { "encodings.mbcs", 6680331, 1658 },
        { "encodings.oem", 6681989, 1471 },
        { "encodings.palmos", 6683460, 2419 },
        { "encodings.ptcp154", 6685879, 2513 },
        { "encodings.punycode", 6688392, 6380 },
        { "encodings.quopri_codec", 6694772, 2406 },
        { "encodings.raw_unicode_escape", 6697178, 1732 },
        { "encodings.rot_13", 6698910, 2992 },
        { "encodings.shift_jis", 6701902, 1417 },
        { "encodings.shift_jis_2004", 6703319, 1427 },
        { "encodings.shift_jisx0213", 6704746, 1427 },
        { "encodings.tis_620", 6706173, 2481 },
        { "encodings.undefined", 6708654, 2126 },
        { "encodings.unicode_escape", 6710780, 1712 },
        { "encodings.unicode_internal", 6712492, 1722 },
        { "encodings.utf_16", 6714214, 4796 },
        { "encodings.utf_16_be", 6719010, 1597 },
        { "encodings.utf_16_le", 6720607, 1597 },
        { "encodings.utf_32", 6722204, 4689 },
        { "encodings.utf_32_be", 6726893, 1490 },
        { "encodings.utf_32_le", 6728383, 1490 },
        { "encodings.utf_7", 6729873, 1518 },
        { "encodings.utf_8", 6731391, 1577 },
        { "encodings.utf_8_sig", 6732968, 4479 },
        { "encodings.uu_codec", 6737447, 3188 },
        { "encodings.zlib_codec", 6740635, 3086 },
        { "enum", 6743721, 24234 },
        { "functools", 6767955, 23918 },
        { "genericpath", 6791873, 3711 },
        { "heapq", 6795584, 14325 },
        { "importlib", 6809909, -3695 },
        { "importlib._bootstrap", 6813604, 29141 },
        { "importlib._bootstrap_external", 6842745, 41781 },
        { "importlib.machinery", 6884526, 935 },
        { "inspect", 6885461, 79995 },
        { "io", 6965456, 3372 },
        { "keyword", 6968828, 1772 },
        { "linecache", 6970600, 3752 },
        { "locale", 6974352, 34518 },
        { "ntpath", 7008870, 12967 },
        { "opcode", 7021837, 5341 },
        { "operator", 66961, 13863 },
        { "os", 7027178, 29656 },
        { "quopri", 7056834, 5734 },
        { "re", 7062568, 13767 },
        { "reprlib", 7076335, 5313 },
        { "sre_compile", 7081648, 15166 },
        { "sre_constants", 7096814, 6254 },
        { "sre_parse", 7103068, 21320 },
        { "stat", 7124388, 3836 },
        { "stringprep", 7128224, 9995 },
        { "struct", 7138219, 297 },
        { "threading", 7138516, 37665 },
        { "token", 7176181, 3562 },
        { "tokenize", 7179743, 17794 },
        { "traceback", 7197537, 19586 },
        { "types", 7217123, 8939 },
        { "warnings", 7226062, 13903 },
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
