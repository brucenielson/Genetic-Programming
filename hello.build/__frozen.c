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
        { "_collections_abc", 6165588, 28905 },
        { "_compression", 6194493, 4087 },
        { "_weakrefset", 6198580, 7425 },
        { "abc", 6206005, 6414 },
        { "base64", 6212419, 16951 },
        { "bz2", 6229370, 11144 },
        { "codecs", 6240514, 33861 },
        { "collections", 6274375, -46573 },
        { "collections.abc", 6165588, 28905 },
        { "copyreg", 6320948, 4207 },
        { "dis", 6325155, 15168 },
        { "encodings", 6340323, -3910 },
        { "encodings.aliases", 6344233, 6259 },
        { "encodings.ascii", 6350492, 1847 },
        { "encodings.base64_codec", 6352339, 2386 },
        { "encodings.big5", 6354725, 1407 },
        { "encodings.big5hkscs", 6356132, 1417 },
        { "encodings.bz2_codec", 6357549, 3248 },
        { "encodings.charmap", 6360797, 2900 },
        { "encodings.cp037", 6363697, 2392 },
        { "encodings.cp1006", 6366089, 2468 },
        { "encodings.cp1026", 6368557, 2396 },
        { "encodings.cp1125", 6370953, 8089 },
        { "encodings.cp1140", 6379042, 2382 },
        { "encodings.cp1250", 6381424, 2419 },
        { "encodings.cp1251", 6383843, 2416 },
        { "encodings.cp1252", 6386259, 2419 },
        { "encodings.cp1253", 6388678, 2432 },
        { "encodings.cp1254", 6391110, 2421 },
        { "encodings.cp1255", 6393531, 2440 },
        { "encodings.cp1256", 6395971, 2418 },
        { "encodings.cp1257", 6398389, 2426 },
        { "encodings.cp1258", 6400815, 2424 },
        { "encodings.cp273", 6403239, 2378 },
        { "encodings.cp424", 6405617, 2422 },
        { "encodings.cp437", 6408039, 7806 },
        { "encodings.cp500", 6415845, 2392 },
        { "encodings.cp65001", 6418237, 1646 },
        { "encodings.cp720", 6419883, 2489 },
        { "encodings.cp737", 6422372, 8128 },
        { "encodings.cp775", 6430500, 7836 },
        { "encodings.cp850", 6438336, 7467 },
        { "encodings.cp852", 6445803, 7844 },
        { "encodings.cp855", 6453647, 8097 },
        { "encodings.cp856", 6461744, 2454 },
        { "encodings.cp857", 6464198, 7449 },
        { "encodings.cp858", 6471647, 7437 },
        { "encodings.cp860", 6479084, 7785 },
        { "encodings.cp861", 6486869, 7800 },
        { "encodings.cp862", 6494669, 7989 },
        { "encodings.cp863", 6502658, 7800 },
        { "encodings.cp864", 6510458, 7946 },
        { "encodings.cp865", 6518404, 7800 },
        { "encodings.cp866", 6526204, 8133 },
        { "encodings.cp869", 6534337, 7826 },
        { "encodings.cp874", 6542163, 2520 },
        { "encodings.cp875", 6544683, 2389 },
        { "encodings.cp932", 6547072, 1409 },
        { "encodings.cp949", 6548481, 1409 },
        { "encodings.cp950", 6549890, 1409 },
        { "encodings.euc_jis_2004", 6551299, 1423 },
        { "encodings.euc_jisx0213", 6552722, 1423 },
        { "encodings.euc_jp", 6554145, 1411 },
        { "encodings.euc_kr", 6555556, 1411 },
        { "encodings.gb18030", 6556967, 1413 },
        { "encodings.gb2312", 6558380, 1411 },
        { "encodings.gbk", 6559791, 1405 },
        { "encodings.hex_codec", 6561196, 2373 },
        { "encodings.hp_roman8", 6563569, 2593 },
        { "encodings.hz", 6566162, 1403 },
        { "encodings.idna", 6567565, 5687 },
        { "encodings.iso2022_jp", 6573252, 1424 },
        { "encodings.iso2022_jp_1", 6574676, 1428 },
        { "encodings.iso2022_jp_2", 6576104, 1428 },
        { "encodings.iso2022_jp_2004", 6577532, 1434 },
        { "encodings.iso2022_jp_3", 6578966, 1428 },
        { "encodings.iso2022_jp_ext", 6580394, 1432 },
        { "encodings.iso2022_kr", 6581826, 1424 },
        { "encodings.iso8859_1", 6583250, 2391 },
        { "encodings.iso8859_10", 6585641, 2396 },
        { "encodings.iso8859_11", 6588037, 2490 },
        { "encodings.iso8859_13", 6590527, 2399 },
        { "encodings.iso8859_14", 6592926, 2417 },
        { "encodings.iso8859_15", 6595343, 2396 },
        { "encodings.iso8859_16", 6597739, 2398 },
        { "encodings.iso8859_2", 6600137, 2391 },
        { "encodings.iso8859_3", 6602528, 2398 },
        { "encodings.iso8859_4", 6604926, 2391 },
        { "encodings.iso8859_5", 6607317, 2392 },
        { "encodings.iso8859_6", 6609709, 2436 },
        { "encodings.iso8859_7", 6612145, 2399 },
        { "encodings.iso8859_8", 6614544, 2430 },
        { "encodings.iso8859_9", 6616974, 2391 },
        { "encodings.johab", 6619365, 1409 },
        { "encodings.koi8_r", 6620774, 2443 },
        { "encodings.koi8_t", 6623217, 2354 },
        { "encodings.koi8_u", 6625571, 2429 },
        { "encodings.kz1048", 6628000, 2406 },
        { "encodings.latin_1", 6630406, 1859 },
        { "encodings.mac_arabic", 6632265, 7700 },
        { "encodings.mac_centeuro", 6639965, 2430 },
        { "encodings.mac_croatian", 6642395, 2438 },
        { "encodings.mac_cyrillic", 6644833, 2428 },
        { "encodings.mac_farsi", 6647261, 2372 },
        { "encodings.mac_greek", 6649633, 2412 },
        { "encodings.mac_iceland", 6652045, 2431 },
        { "encodings.mac_latin2", 6654476, 2572 },
        { "encodings.mac_roman", 6657048, 2429 },
        { "encodings.mac_romanian", 6659477, 2439 },
        { "encodings.mac_turkish", 6661916, 2432 },
        { "encodings.mbcs", 6664348, 1658 },
        { "encodings.oem", 6666006, 1471 },
        { "encodings.palmos", 6667477, 2419 },
        { "encodings.ptcp154", 6669896, 2513 },
        { "encodings.punycode", 6672409, 6380 },
        { "encodings.quopri_codec", 6678789, 2406 },
        { "encodings.raw_unicode_escape", 6681195, 1732 },
        { "encodings.rot_13", 6682927, 2992 },
        { "encodings.shift_jis", 6685919, 1417 },
        { "encodings.shift_jis_2004", 6687336, 1427 },
        { "encodings.shift_jisx0213", 6688763, 1427 },
        { "encodings.tis_620", 6690190, 2481 },
        { "encodings.undefined", 6692671, 2126 },
        { "encodings.unicode_escape", 6694797, 1712 },
        { "encodings.unicode_internal", 6696509, 1722 },
        { "encodings.utf_16", 6698231, 4796 },
        { "encodings.utf_16_be", 6703027, 1597 },
        { "encodings.utf_16_le", 6704624, 1597 },
        { "encodings.utf_32", 6706221, 4689 },
        { "encodings.utf_32_be", 6710910, 1490 },
        { "encodings.utf_32_le", 6712400, 1490 },
        { "encodings.utf_7", 6713890, 1518 },
        { "encodings.utf_8", 6715408, 1577 },
        { "encodings.utf_8_sig", 6716985, 4479 },
        { "encodings.uu_codec", 6721464, 3188 },
        { "encodings.zlib_codec", 6724652, 3086 },
        { "enum", 6727738, 24234 },
        { "functools", 6751972, 23918 },
        { "genericpath", 6775890, 3711 },
        { "heapq", 6779601, 14325 },
        { "importlib", 6793926, -3695 },
        { "importlib._bootstrap", 6797621, 29141 },
        { "importlib._bootstrap_external", 6826762, 41781 },
        { "importlib.machinery", 6868543, 935 },
        { "inspect", 6869478, 79995 },
        { "io", 6949473, 3372 },
        { "keyword", 6952845, 1772 },
        { "linecache", 6954617, 3752 },
        { "locale", 6958369, 34518 },
        { "ntpath", 6992887, 12967 },
        { "opcode", 7005854, 5341 },
        { "operator", 7011195, 13863 },
        { "os", 7025058, 29656 },
        { "quopri", 7054714, 5734 },
        { "re", 7060448, 13767 },
        { "reprlib", 7074215, 5313 },
        { "sre_compile", 7079528, 15166 },
        { "sre_constants", 7094694, 6254 },
        { "sre_parse", 7100948, 21320 },
        { "stat", 7122268, 3836 },
        { "stringprep", 7126104, 9995 },
        { "struct", 7136099, 297 },
        { "threading", 7136396, 37665 },
        { "token", 7174061, 3562 },
        { "tokenize", 7177623, 17794 },
        { "traceback", 7195417, 19586 },
        { "types", 7215003, 8939 },
        { "warnings", 7223942, 13903 },
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
