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
        { "_collections_abc", 6181617, 28905 },
        { "_compression", 6210522, 4087 },
        { "_weakrefset", 6214609, 7425 },
        { "abc", 6222034, 6414 },
        { "base64", 6228448, 16951 },
        { "bz2", 6245399, 11144 },
        { "codecs", 6256543, 33861 },
        { "collections", 6290404, -46573 },
        { "collections.abc", 6181617, 28905 },
        { "copyreg", 6336977, 4207 },
        { "dis", 6341184, 15168 },
        { "encodings", 6356352, -3910 },
        { "encodings.aliases", 6360262, 6259 },
        { "encodings.ascii", 6366521, 1847 },
        { "encodings.base64_codec", 6368368, 2386 },
        { "encodings.big5", 6370754, 1407 },
        { "encodings.big5hkscs", 6372161, 1417 },
        { "encodings.bz2_codec", 6373578, 3248 },
        { "encodings.charmap", 6376826, 2900 },
        { "encodings.cp037", 6379726, 2392 },
        { "encodings.cp1006", 6382118, 2468 },
        { "encodings.cp1026", 6384586, 2396 },
        { "encodings.cp1125", 6386982, 8089 },
        { "encodings.cp1140", 6395071, 2382 },
        { "encodings.cp1250", 6397453, 2419 },
        { "encodings.cp1251", 6399872, 2416 },
        { "encodings.cp1252", 6402288, 2419 },
        { "encodings.cp1253", 6404707, 2432 },
        { "encodings.cp1254", 6407139, 2421 },
        { "encodings.cp1255", 6409560, 2440 },
        { "encodings.cp1256", 6412000, 2418 },
        { "encodings.cp1257", 6414418, 2426 },
        { "encodings.cp1258", 6416844, 2424 },
        { "encodings.cp273", 6419268, 2378 },
        { "encodings.cp424", 6421646, 2422 },
        { "encodings.cp437", 6424068, 7806 },
        { "encodings.cp500", 6431874, 2392 },
        { "encodings.cp65001", 6434266, 1646 },
        { "encodings.cp720", 6435912, 2489 },
        { "encodings.cp737", 6438401, 8128 },
        { "encodings.cp775", 6446529, 7836 },
        { "encodings.cp850", 6454365, 7467 },
        { "encodings.cp852", 6461832, 7844 },
        { "encodings.cp855", 6469676, 8097 },
        { "encodings.cp856", 6477773, 2454 },
        { "encodings.cp857", 6480227, 7449 },
        { "encodings.cp858", 6487676, 7437 },
        { "encodings.cp860", 6495113, 7785 },
        { "encodings.cp861", 6502898, 7800 },
        { "encodings.cp862", 6510698, 7989 },
        { "encodings.cp863", 6518687, 7800 },
        { "encodings.cp864", 6526487, 7946 },
        { "encodings.cp865", 6534433, 7800 },
        { "encodings.cp866", 6542233, 8133 },
        { "encodings.cp869", 6550366, 7826 },
        { "encodings.cp874", 6558192, 2520 },
        { "encodings.cp875", 6560712, 2389 },
        { "encodings.cp932", 6563101, 1409 },
        { "encodings.cp949", 6564510, 1409 },
        { "encodings.cp950", 6565919, 1409 },
        { "encodings.euc_jis_2004", 6567328, 1423 },
        { "encodings.euc_jisx0213", 6568751, 1423 },
        { "encodings.euc_jp", 6570174, 1411 },
        { "encodings.euc_kr", 6571585, 1411 },
        { "encodings.gb18030", 6572996, 1413 },
        { "encodings.gb2312", 6574409, 1411 },
        { "encodings.gbk", 6575820, 1405 },
        { "encodings.hex_codec", 6577225, 2373 },
        { "encodings.hp_roman8", 6579598, 2593 },
        { "encodings.hz", 6582191, 1403 },
        { "encodings.idna", 6583594, 5687 },
        { "encodings.iso2022_jp", 6589281, 1424 },
        { "encodings.iso2022_jp_1", 6590705, 1428 },
        { "encodings.iso2022_jp_2", 6592133, 1428 },
        { "encodings.iso2022_jp_2004", 6593561, 1434 },
        { "encodings.iso2022_jp_3", 6594995, 1428 },
        { "encodings.iso2022_jp_ext", 6596423, 1432 },
        { "encodings.iso2022_kr", 6597855, 1424 },
        { "encodings.iso8859_1", 6599279, 2391 },
        { "encodings.iso8859_10", 6601670, 2396 },
        { "encodings.iso8859_11", 6604066, 2490 },
        { "encodings.iso8859_13", 6606556, 2399 },
        { "encodings.iso8859_14", 6608955, 2417 },
        { "encodings.iso8859_15", 6611372, 2396 },
        { "encodings.iso8859_16", 6613768, 2398 },
        { "encodings.iso8859_2", 6616166, 2391 },
        { "encodings.iso8859_3", 6618557, 2398 },
        { "encodings.iso8859_4", 6620955, 2391 },
        { "encodings.iso8859_5", 6623346, 2392 },
        { "encodings.iso8859_6", 6625738, 2436 },
        { "encodings.iso8859_7", 6628174, 2399 },
        { "encodings.iso8859_8", 6630573, 2430 },
        { "encodings.iso8859_9", 6633003, 2391 },
        { "encodings.johab", 6635394, 1409 },
        { "encodings.koi8_r", 6636803, 2443 },
        { "encodings.koi8_t", 6639246, 2354 },
        { "encodings.koi8_u", 6641600, 2429 },
        { "encodings.kz1048", 6644029, 2406 },
        { "encodings.latin_1", 6646435, 1859 },
        { "encodings.mac_arabic", 6648294, 7700 },
        { "encodings.mac_centeuro", 6655994, 2430 },
        { "encodings.mac_croatian", 6658424, 2438 },
        { "encodings.mac_cyrillic", 6660862, 2428 },
        { "encodings.mac_farsi", 6663290, 2372 },
        { "encodings.mac_greek", 6665662, 2412 },
        { "encodings.mac_iceland", 6668074, 2431 },
        { "encodings.mac_latin2", 6670505, 2572 },
        { "encodings.mac_roman", 6673077, 2429 },
        { "encodings.mac_romanian", 6675506, 2439 },
        { "encodings.mac_turkish", 6677945, 2432 },
        { "encodings.mbcs", 6680377, 1658 },
        { "encodings.oem", 6682035, 1471 },
        { "encodings.palmos", 6683506, 2419 },
        { "encodings.ptcp154", 6685925, 2513 },
        { "encodings.punycode", 6688438, 6380 },
        { "encodings.quopri_codec", 6694818, 2406 },
        { "encodings.raw_unicode_escape", 6697224, 1732 },
        { "encodings.rot_13", 6698956, 2992 },
        { "encodings.shift_jis", 6701948, 1417 },
        { "encodings.shift_jis_2004", 6703365, 1427 },
        { "encodings.shift_jisx0213", 6704792, 1427 },
        { "encodings.tis_620", 6706219, 2481 },
        { "encodings.undefined", 6708700, 2126 },
        { "encodings.unicode_escape", 6710826, 1712 },
        { "encodings.unicode_internal", 6712538, 1722 },
        { "encodings.utf_16", 6714260, 4796 },
        { "encodings.utf_16_be", 6719056, 1597 },
        { "encodings.utf_16_le", 6720653, 1597 },
        { "encodings.utf_32", 6722250, 4689 },
        { "encodings.utf_32_be", 6726939, 1490 },
        { "encodings.utf_32_le", 6728429, 1490 },
        { "encodings.utf_7", 6729919, 1518 },
        { "encodings.utf_8", 6731437, 1577 },
        { "encodings.utf_8_sig", 6733014, 4479 },
        { "encodings.uu_codec", 6737493, 3188 },
        { "encodings.zlib_codec", 6740681, 3086 },
        { "enum", 6743767, 24234 },
        { "functools", 6768001, 23918 },
        { "genericpath", 6791919, 3711 },
        { "heapq", 6795630, 14325 },
        { "importlib", 6809955, -3695 },
        { "importlib._bootstrap", 6813650, 29141 },
        { "importlib._bootstrap_external", 6842791, 41781 },
        { "importlib.machinery", 6884572, 935 },
        { "inspect", 6885507, 79995 },
        { "io", 6965502, 3372 },
        { "keyword", 6968874, 1772 },
        { "linecache", 6970646, 3752 },
        { "locale", 6974398, 34518 },
        { "ntpath", 7008916, 12967 },
        { "opcode", 7021883, 5341 },
        { "operator", 67007, 13863 },
        { "os", 7027224, 29656 },
        { "quopri", 7056880, 5734 },
        { "re", 7062614, 13767 },
        { "reprlib", 7076381, 5313 },
        { "sre_compile", 7081694, 15166 },
        { "sre_constants", 7096860, 6254 },
        { "sre_parse", 7103114, 21320 },
        { "stat", 7124434, 3836 },
        { "stringprep", 7128270, 9995 },
        { "struct", 7138265, 297 },
        { "threading", 7138562, 37665 },
        { "token", 7176227, 3562 },
        { "tokenize", 7179789, 17794 },
        { "traceback", 7197583, 19586 },
        { "types", 7217169, 8939 },
        { "warnings", 7226108, 13903 },
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
