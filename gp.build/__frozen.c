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
        { "base64", 6377627, 10962 },
        { "codecs", 6388589, 36364 },
        { "copy_reg", 6424953, 5050 },
        { "encodings", 6430003, -4350 },
        { "encodings.aliases", 6434353, 8757 },
        { "encodings.ascii", 6443110, 2223 },
        { "encodings.base64_codec", 6445333, 3790 },
        { "encodings.big5", 6449123, 1727 },
        { "encodings.big5hkscs", 6450850, 1767 },
        { "encodings.bz2_codec", 6452617, 4670 },
        { "encodings.charmap", 6457287, 3420 },
        { "encodings.cp037", 6460707, 2796 },
        { "encodings.cp1006", 6463503, 2882 },
        { "encodings.cp1026", 6466385, 2810 },
        { "encodings.cp1140", 6469195, 2796 },
        { "encodings.cp1250", 6471991, 2833 },
        { "encodings.cp1251", 6474824, 2830 },
        { "encodings.cp1252", 6477654, 2833 },
        { "encodings.cp1253", 6480487, 2846 },
        { "encodings.cp1254", 6483333, 2835 },
        { "encodings.cp1255", 6486168, 2854 },
        { "encodings.cp1256", 6489022, 2832 },
        { "encodings.cp1257", 6491854, 2840 },
        { "encodings.cp1258", 6494694, 2838 },
        { "encodings.cp424", 6497532, 2826 },
        { "encodings.cp437", 6500358, 8031 },
        { "encodings.cp500", 6508389, 2796 },
        { "encodings.cp720", 6511185, 2893 },
        { "encodings.cp737", 6514078, 8259 },
        { "encodings.cp775", 6522337, 8045 },
        { "encodings.cp850", 6530382, 7778 },
        { "encodings.cp852", 6538160, 8047 },
        { "encodings.cp855", 6546207, 8228 },
        { "encodings.cp856", 6554435, 2858 },
        { "encodings.cp857", 6557293, 7768 },
        { "encodings.cp858", 6565061, 7748 },
        { "encodings.cp860", 6572809, 8014 },
        { "encodings.cp861", 6580823, 8025 },
        { "encodings.cp862", 6588848, 8160 },
        { "encodings.cp863", 6597008, 8025 },
        { "encodings.cp864", 6605033, 8156 },
        { "encodings.cp865", 6613189, 8025 },
        { "encodings.cp866", 6621214, 8260 },
        { "encodings.cp869", 6629474, 8072 },
        { "encodings.cp874", 6637546, 2924 },
        { "encodings.cp875", 6640470, 2793 },
        { "encodings.cp932", 6643263, 1735 },
        { "encodings.cp949", 6644998, 1735 },
        { "encodings.cp950", 6646733, 1735 },
        { "encodings.euc_jis_2004", 6648468, 1791 },
        { "encodings.euc_jisx0213", 6650259, 1791 },
        { "encodings.euc_jp", 6652050, 1743 },
        { "encodings.euc_kr", 6653793, 1743 },
        { "encodings.gb18030", 6655536, 1751 },
        { "encodings.gb2312", 6657287, 1743 },
        { "encodings.gbk", 6659030, 1719 },
        { "encodings.hex_codec", 6660749, 3742 },
        { "encodings.hp_roman8", 6664491, 4079 },
        { "encodings.hz", 6668570, 1711 },
        { "encodings.idna", 6670281, 6326 },
        { "encodings.iso2022_jp", 6676607, 1780 },
        { "encodings.iso2022_jp_1", 6678387, 1796 },
        { "encodings.iso2022_jp_2", 6680183, 1796 },
        { "encodings.iso2022_jp_2004", 6681979, 1820 },
        { "encodings.iso2022_jp_3", 6683799, 1796 },
        { "encodings.iso2022_jp_ext", 6685595, 1812 },
        { "encodings.iso2022_kr", 6687407, 1780 },
        { "encodings.iso8859_1", 6689187, 2835 },
        { "encodings.iso8859_10", 6692022, 2850 },
        { "encodings.iso8859_11", 6694872, 2944 },
        { "encodings.iso8859_13", 6697816, 2853 },
        { "encodings.iso8859_14", 6700669, 2871 },
        { "encodings.iso8859_15", 6703540, 2850 },
        { "encodings.iso8859_16", 6706390, 2852 },
        { "encodings.iso8859_2", 6709242, 2835 },
        { "encodings.iso8859_3", 6712077, 2842 },
        { "encodings.iso8859_4", 6714919, 2835 },
        { "encodings.iso8859_5", 6717754, 2836 },
        { "encodings.iso8859_6", 6720590, 2880 },
        { "encodings.iso8859_7", 6723470, 2843 },
        { "encodings.iso8859_8", 6726313, 2874 },
        { "encodings.iso8859_9", 6729187, 2835 },
        { "encodings.johab", 6732022, 1735 },
        { "encodings.koi8_r", 6733757, 2857 },
        { "encodings.koi8_u", 6736614, 2843 },
        { "encodings.latin_1", 6739457, 2253 },
        { "encodings.mac_arabic", 6741710, 7981 },
        { "encodings.mac_centeuro", 6749691, 2904 },
        { "encodings.mac_croatian", 6752595, 2912 },
        { "encodings.mac_cyrillic", 6755507, 2902 },
        { "encodings.mac_farsi", 6758409, 2816 },
        { "encodings.mac_greek", 6761225, 2856 },
        { "encodings.mac_iceland", 6764081, 2895 },
        { "encodings.mac_latin2", 6766976, 4874 },
        { "encodings.mac_roman", 6771850, 2873 },
        { "encodings.mac_romanian", 6774723, 2913 },
        { "encodings.mac_turkish", 6777636, 2896 },
        { "encodings.mbcs", 6780532, 1995 },
        { "encodings.palmos", 6782527, 3033 },
        { "encodings.ptcp154", 6785560, 4857 },
        { "encodings.punycode", 6790417, 7911 },
        { "encodings.quopri_codec", 6798328, 3608 },
        { "encodings.raw_unicode_escape", 6801936, 2175 },
        { "encodings.rot_13", 6804111, 3620 },
        { "encodings.shift_jis", 6807731, 1767 },
        { "encodings.shift_jis_2004", 6809498, 1807 },
        { "encodings.shift_jisx0213", 6811305, 1807 },
        { "encodings.string_escape", 6813112, 2034 },
        { "encodings.tis_620", 6815146, 2905 },
        { "encodings.undefined", 6818051, 2556 },
        { "encodings.unicode_escape", 6820607, 2123 },
        { "encodings.unicode_internal", 6822730, 2149 },
        { "encodings.utf_16", 6824879, 5100 },
        { "encodings.utf_16_be", 6829979, 1966 },
        { "encodings.utf_16_le", 6831945, 1966 },
        { "encodings.utf_32", 6833911, 5658 },
        { "encodings.utf_32_be", 6839569, 1859 },
        { "encodings.utf_32_le", 6841428, 1859 },
        { "encodings.utf_7", 6843287, 1859 },
        { "encodings.utf_8", 6845146, 1918 },
        { "encodings.utf_8_sig", 6847064, 4917 },
        { "encodings.uu_codec", 6851981, 4870 },
        { "encodings.zlib_codec", 6856851, 4590 },
        { "functools", 6861441, 5951 },
        { "locale", 6867392, 54920 },
        { "quopri", 6922312, 6511 },
        { "re", 6928823, 13300 },
        { "sre_compile", 6942123, 12476 },
        { "sre_constants", 6954599, 6162 },
        { "sre_parse", 6960761, 19829 },
        { "string", 6980590, 20196 },
        { "stringprep", 7000786, 14381 },
        { "struct", 7015167, 226 },
        { "types", 7015393, 2682 },
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
