//
//  marc8set_macros.h
//  bibiconv
//

#ifndef BIBICONV_MARC8_CHARSET_MACROS_H
#define BIBICONV_MARC8_CHARSET_MACROS_H

/** Skip encoding of a character when found in a code table. */
#define IGNORE UCS4_IGNORED
/** Identifies an unencodable character in a code table. */
#define NONCHR UCS4_NONCHAR

/** Defines the unicode code point for a combining character. */
#define C(UCS) (union bib_iconv_code_info){ .code_point = { 1, 1, (UCS) } }
/** Defines the unicode code point for a non-combining character. */
#define N(UCS) (union bib_iconv_code_info){ .code_point = { 1, 0, (UCS) } }
/** Defines the index of the next trie span for a multi-byte set. */
#define I(IDX) (union bib_iconv_code_info){ .span_index = { 0, 1, (IDX) } }
/** Defines a value for use as an array terminator. */
#define T(NA) (union bib_iconv_code_info){ .span_index = { 0, 0, 0x0000 } }

#define R(SEQUENCE, _0, _1, _2, _3, _4, _5, _6, _7) \
    ROW_##SEQUENCE(_0, _1, _2, _3, _4, _5, _6, _7)

#pragma mark -

#define ROW_CCCCCCCC(_0, _1, _2, _3, _4, _5, _6, _7) \
    C(_0), C(_1), C(_2), C(_3), C(_4), C(_5), C(_6), C(_7)
#define ROW_CCCCCCCN(_0, _1, _2, _3, _4, _5, _6, _7) \
    C(_0), C(_1), C(_2), C(_3), C(_4), C(_5), C(_6), N(_7)
#define ROW_CCCCNCCC(_0, _1, _2, _3, _4, _5, _6, _7) \
    C(_0), C(_1), C(_2), C(_3), N(_4), C(_5), C(_6), C(_7)
#define ROW_CCCNNNCN(_0, _1, _2, _3, _4, _5, _6, _7) \
    C(_0), C(_1), C(_2), N(_3), N(_4), N(_5), C(_6), N(_7)
#define ROW_CCCNNNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    C(_0), C(_1), C(_2), N(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_NCCCCCCC(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), C(_1), C(_2), C(_3), C(_4), C(_5), C(_6), C(_7)
#define ROW_NNNCCCCC(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), C(_3), C(_4), C(_5), C(_6), C(_7)
#define ROW_NNNNNCCN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), N(_4), C(_5), C(_6), N(_7)
#define ROW_NNNNNNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), N(_4), N(_5), N(_6), N(_7)

#pragma mark -

#define ROW_IIIIIIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), I(_3), I(_4), I(_5), I(_6), I(_7)
#define ROW_IIIIIIIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), I(_3), I(_4), I(_5), I(_6), N(_7)
#define ROW_IIIIIINI(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), I(_3), I(_4), I(_5), N(_6), I(_7)
#define ROW_IIIIINII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), I(_3), I(_4), N(_5), I(_6), I(_7)
#define ROW_IIIIININ(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), I(_3), I(_4), N(_5), I(_6), N(_7)
#define ROW_IIIIINNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), I(_3), I(_4), N(_5), N(_6), I(_7)
#define ROW_IIIINIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), I(_3), N(_4), I(_5), I(_6), I(_7)
#define ROW_IIIINIIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), I(_3), N(_4), I(_5), I(_6), N(_7)
#define ROW_IIIINNII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), I(_3), N(_4), N(_5), I(_6), I(_7)
#define ROW_IIIINNIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), I(_3), N(_4), N(_5), I(_6), N(_7)
#define ROW_IIIINNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), I(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_IIINIIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), N(_3), I(_4), I(_5), I(_6), I(_7)
#define ROW_IIINIIIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), N(_3), I(_4), I(_5), I(_6), N(_7)
#define ROW_IIININII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), N(_3), I(_4), N(_5), I(_6), I(_7)
#define ROW_IIININNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), N(_3), I(_4), N(_5), N(_6), N(_7)
#define ROW_IIINNIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), N(_3), N(_4), I(_5), I(_6), I(_7)
#define ROW_IIINNNIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), N(_3), N(_4), N(_5), I(_6), N(_7)
#define ROW_IIINNNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), I(_2), N(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_IINIIINN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), I(_3), I(_4), I(_5), N(_6), N(_7)
#define ROW_IINIINII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), I(_3), I(_4), N(_5), I(_6), I(_7)
#define ROW_IINIININ(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), I(_3), I(_4), N(_5), I(_6), N(_7)
#define ROW_IININIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), I(_3), N(_4), I(_5), I(_6), I(_7)
#define ROW_IINININI(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), I(_3), N(_4), I(_5), N(_6), I(_7)
#define ROW_IININNII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), I(_3), N(_4), N(_5), I(_6), I(_7)
#define ROW_IINNIIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), N(_3), I(_4), I(_5), I(_6), I(_7)
#define ROW_IINNNIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), N(_3), N(_4), I(_5), I(_6), I(_7)
#define ROW_IINNNINN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), N(_3), N(_4), I(_5), N(_6), N(_7)
#define ROW_IINNNNII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), N(_3), N(_4), N(_5), I(_6), I(_7)
#define ROW_IINNNNIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), N(_3), N(_4), N(_5), I(_6), N(_7)
#define ROW_IINNNNNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), N(_3), N(_4), N(_5), N(_6), I(_7)
#define ROW_IINNNNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), I(_1), N(_2), N(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_INIIIIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), I(_3), I(_4), I(_5), I(_6), I(_7)
#define ROW_INIIIIIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), I(_3), I(_4), I(_5), I(_6), N(_7)
#define ROW_INIIIINI(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), I(_3), I(_4), I(_5), N(_6), I(_7)
#define ROW_INIIIINN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), I(_3), I(_4), I(_5), N(_6), N(_7)
#define ROW_INIININI(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), I(_3), N(_4), I(_5), N(_6), I(_7)
#define ROW_INIININN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), I(_3), N(_4), I(_5), N(_6), N(_7)
#define ROW_INIINNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), I(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_ININIIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), N(_3), I(_4), I(_5), I(_6), I(_7)
#define ROW_INININNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), N(_3), I(_4), N(_5), N(_6), N(_7)
#define ROW_ININNINI(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), N(_3), N(_4), I(_5), N(_6), I(_7)
#define ROW_ININNNIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), N(_3), N(_4), N(_5), I(_6), N(_7)
#define ROW_ININNNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), I(_2), N(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_INNIIINI(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), I(_3), I(_4), I(_5), N(_6), I(_7)
#define ROW_INNIINII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), I(_3), I(_4), N(_5), I(_6), I(_7)
#define ROW_INNIINNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), I(_3), I(_4), N(_5), N(_6), I(_7)
#define ROW_INNIINNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), I(_3), I(_4), N(_5), N(_6), N(_7)
#define ROW_INNINNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), I(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_INNNIINI(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), N(_3), I(_4), I(_5), N(_6), I(_7)
#define ROW_INNNIINN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), N(_3), I(_4), I(_5), N(_6), N(_7)
#define ROW_INNNINNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), N(_3), I(_4), N(_5), N(_6), N(_7)
#define ROW_INNNNIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), N(_3), N(_4), I(_5), I(_6), I(_7)
#define ROW_INNNNINN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), N(_3), N(_4), I(_5), N(_6), N(_7)
#define ROW_INNNNNNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), N(_3), N(_4), N(_5), N(_6), I(_7)
#define ROW_INNNNNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    I(_0), N(_1), N(_2), N(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_NIIIIIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), I(_2), I(_3), I(_4), I(_5), I(_6), I(_7)
#define ROW_NIIIIINI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), I(_2), I(_3), I(_4), I(_5), N(_6), I(_7)
#define ROW_NIIINNNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), I(_2), I(_3), N(_4), N(_5), N(_6), I(_7)
#define ROW_NIIINNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), I(_2), I(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_NIINIIIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), I(_2), N(_3), I(_4), I(_5), I(_6), N(_7)
#define ROW_NIININNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), I(_2), N(_3), I(_4), N(_5), N(_6), N(_7)
#define ROW_NIINNNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), I(_2), N(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_NINIIIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), I(_3), I(_4), I(_5), I(_6), I(_7)
#define ROW_NINIIIIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), I(_3), I(_4), I(_5), I(_6), N(_7)
#define ROW_NINIINNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), I(_3), I(_4), N(_5), N(_6), I(_7)
#define ROW_NINININI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), I(_3), N(_4), I(_5), N(_6), I(_7)
#define ROW_NINININN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), I(_3), N(_4), I(_5), N(_6), N(_7)
#define ROW_NININNIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), I(_3), N(_4), N(_5), I(_6), N(_7)
#define ROW_NINNIINI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), N(_3), I(_4), I(_5), N(_6), I(_7)
#define ROW_NINNIINN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), N(_3), I(_4), I(_5), N(_6), N(_7)
#define ROW_NINNINNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), N(_3), I(_4), N(_5), N(_6), I(_7)
#define ROW_NINNINNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), N(_3), I(_4), N(_5), N(_6), N(_7)
#define ROW_NINNNIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), N(_3), N(_4), I(_5), I(_6), I(_7)
#define ROW_NINNNIIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), N(_3), N(_4), I(_5), I(_6), N(_7)
#define ROW_NINNNNII(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), N(_3), N(_4), N(_5), I(_6), I(_7)
#define ROW_NINNNNIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), N(_3), N(_4), N(_5), I(_6), N(_7)
#define ROW_NINNNNNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), N(_3), N(_4), N(_5), N(_6), I(_7)
#define ROW_NINNNNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), I(_1), N(_2), N(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_NNIIIIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), I(_2), I(_3), I(_4), I(_5), I(_6), I(_7)
#define ROW_NNIIIIIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), I(_2), I(_3), I(_4), I(_5), I(_6), N(_7)
#define ROW_NNIIIINI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), I(_2), I(_3), I(_4), I(_5), N(_6), I(_7)
#define ROW_NNIINNNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), I(_2), I(_3), N(_4), N(_5), N(_6), I(_7)
#define ROW_NNINNINI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), I(_2), N(_3), N(_4), I(_5), N(_6), I(_7)
#define ROW_NNINNNIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), I(_2), N(_3), N(_4), N(_5), I(_6), N(_7)
#define ROW_NNINNNNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), I(_2), N(_3), N(_4), N(_5), N(_6), I(_7)
#define ROW_NNINNNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), I(_2), N(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_NNNIIIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), I(_3), I(_4), I(_5), I(_6), I(_7)
#define ROW_NNNIIIIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), I(_3), I(_4), I(_5), I(_6), N(_7)
#define ROW_NNNIIINN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), I(_3), I(_4), I(_5), N(_6), N(_7)
#define ROW_NNNIINII(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), I(_3), I(_4), N(_5), I(_6), I(_7)
#define ROW_NNNIINNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), I(_3), I(_4), N(_5), N(_6), N(_7)
#define ROW_NNNININN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), I(_3), N(_4), I(_5), N(_6), N(_7)
#define ROW_NNNINNNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), I(_3), N(_4), N(_5), N(_6), I(_7)
#define ROW_NNNINNNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), I(_3), N(_4), N(_5), N(_6), N(_7)
#define ROW_NNNNIIIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), I(_4), I(_5), I(_6), N(_7)
#define ROW_NNNNIINI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), I(_4), I(_5), N(_6), I(_7)
#define ROW_NNNNIINN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), I(_4), I(_5), N(_6), N(_7)
#define ROW_NNNNINNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), I(_4), N(_5), N(_6), I(_7)
#define ROW_NNNNINNN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), I(_4), N(_5), N(_6), N(_7)
#define ROW_NNNNNIII(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), N(_4), I(_5), I(_6), I(_7)
#define ROW_NNNNNIIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), N(_4), I(_5), I(_6), N(_7)
#define ROW_NNNNNINI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), N(_4), I(_5), N(_6), I(_7)
#define ROW_NNNNNINN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), N(_4), I(_5), N(_6), N(_7)
#define ROW_NNNNNNII(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), N(_4), N(_5), I(_6), I(_7)
#define ROW_NNNNNNIN(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), N(_4), N(_5), I(_6), N(_7)
#define ROW_NNNNNNNI(_0, _1, _2, _3, _4, _5, _6, _7) \
    N(_0), N(_1), N(_2), N(_3), N(_4), N(_5), N(_6), I(_7)

#endif /* BIBICONV_MARC8_CHARSET_MACROS_H */
