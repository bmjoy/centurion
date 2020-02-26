#ifndef LMXUNICODE_H
#define LMXUNICODE_H

#include "lmxuser.h"

namespace lmx {

typedef unsigned char xmlChar;

/************************************************************************
 *                                                                      *
 * UNICODE version of the macros.                                       *
 *                                                                      *
 ************************************************************************/
/**
 * IS_BYTE_CHAR:
 * @c:  an byte value (int)
 *
 * Macro to check the following production in the XML spec:
 *
 * [2] Char ::= #x9 | #xA | #xD | [#x20...]
 * any byte character in the accepted range
 */
#define IS_BYTE_CHAR(c)  xmlIsChar_ch(c)

/**
 * IS_CHAR:
 * @c:  an UNICODE value (int)
 *
 * Macro to check the following production in the XML spec:
 *
 * [2] Char ::= #x9 | #xA | #xD | [#x20-#xD7FF] | [#xE000-#xFFFD]
 *                  | [#x10000-#x10FFFF]
 * any Unicode character, excluding the surrogate blocks, FFFE, and FFFF.
 */
#define IS_CHAR(c)   xmlIsCharQ(c)

/**
 * IS_CHAR_CH:
 * @c: an xmlChar (usually an unsigned char)
 *
 * Behaves like IS_CHAR on single-byte value
 */
#define IS_CHAR_CH(c)  xmlIsChar_ch(c)

/**
 * IS_BLANK:
 * @c:  an UNICODE value (int)
 *
 * Macro to check the following production in the XML spec:
 *
 * [3] S ::= (#x20 | #x9 | #xD | #xA)+
 */
#define IS_BLANK(c)  xmlIsBlankQ(c)

/**
 * IS_BLANK_CH:
 * @c:  an xmlChar value (normally unsigned char)
 *
 * Behaviour same as IS_BLANK
 */
#define IS_BLANK_CH(c)  xmlIsBlank_ch(c)

/**
 * IS_BASECHAR:
 * @c:  an UNICODE value (int)
 *
 * Macro to check the following production in the XML spec:
 *
 * [85] BaseChar ::= ... long list see REC ...
 */
#define IS_BASECHAR(c) xmlIsBaseCharQ(c)

/**
 * IS_DIGIT:
 * @c:  an UNICODE value (int)
 *
 * Macro to check the following production in the XML spec:
 *
 * [88] Digit ::= ... long list see REC ...
 */
#define IS_DIGIT(c) xmlIsDigitQ(c)

/**
 * IS_DIGIT_CH:
 * @c:  an xmlChar value (usually an unsigned char)
 *
 * Behaves like IS_DIGIT but with a single byte argument
 */
#define IS_DIGIT_CH(c)  xmlIsDigit_ch(c)

/**
 * IS_COMBINING:
 * @c:  an UNICODE value (int)
 *
 * Macro to check the following production in the XML spec:
 *
 * [87] CombiningChar ::= ... long list see REC ...
 */
#define IS_COMBINING(c) xmlIsCombiningQ(c)

/**
 * IS_COMBINING_CH:
 * @c:  an xmlChar (usually an unsigned char)
 *
 * Always false (all combining chars > 0xff)
 */
#define IS_COMBINING_CH(c) 0

/**
 * IS_EXTENDER:
 * @c:  an UNICODE value (int)
 *
 * Macro to check the following production in the XML spec:
 *
 *
 * [89] Extender ::= #x00B7 | #x02D0 | #x02D1 | #x0387 | #x0640 |
 *                   #x0E46 | #x0EC6 | #x3005 | [#x3031-#x3035] |
 *                   [#x309D-#x309E] | [#x30FC-#x30FE]
 */
#define IS_EXTENDER(c) xmlIsExtenderQ(c)

/**
 * IS_EXTENDER_CH:
 * @c:  an xmlChar value (usually an unsigned char)
 *
 * Behaves like IS_EXTENDER but with a single-byte argument
 */
#define IS_EXTENDER_CH(c)  xmlIsExtender_ch(c)

/**
 * IS_IDEOGRAPHIC:
 * @c:  an UNICODE value (int)
 *
 * Macro to check the following production in the XML spec:
 *
 *
 * [86] Ideographic ::= [#x4E00-#x9FA5] | #x3007 | [#x3021-#x3029]
 */
#define IS_IDEOGRAPHIC(c) xmlIsIdeographicQ(c)

/**
 * IS_LETTER:
 * @c:  an UNICODE value (int)
 *
 * Macro to check the following production in the XML spec:
 *
 *
 * [84] Letter ::= BaseChar | Ideographic
 */
#define IS_LETTER(c) (IS_BASECHAR(c) || IS_IDEOGRAPHIC(c))

/**
 * IS_LETTER_CH:
 * @c:  an xmlChar value (normally unsigned char)
 *
 * Macro behaves like IS_LETTER, but only check base chars
 *
 */
#define IS_LETTER_CH(c) xmlIsBaseChar_ch(c)

/**
 * IS_ASCII_LETTER:
 * @c: an xmlChar value
 *
 * Macro to check [a-zA-Z]
 *
 */
#define IS_ASCII_LETTER(c) (((0x41 <= (c)) && ((c) <= 0x5a)) || ((0x61 <= (c)) && ((c) <= 0x7a)))

/**
 * IS_ASCII_DIGIT:
 * @c: an xmlChar value
 *
 * Macro to check [0-9]
 *
 */
#define IS_ASCII_DIGIT(c)       ((0x30 <= (c)) && ((c) <= 0x39))

/**
 * IS_PUBIDCHAR:
 * @c:  an UNICODE value (int)
 *
 * Macro to check the following production in the XML spec:
 *
 *
 * [13] PubidChar ::= #x20 | #xD | #xA | [a-zA-Z0-9] | [-'()+,./:=?;!*#@$_%]
 */
#define IS_PUBIDCHAR(c) xmlIsPubidCharQ(c)

/**
 * IS_PUBIDCHAR_CH:
 * @c:  an xmlChar value (normally unsigned char)
 *
 * Same as IS_PUBIDCHAR but for single-byte value
 */
#define IS_PUBIDCHAR_CH(c) xmlIsPubidChar_ch(c)

/*
 * Define our typedefs and structures
 *
 */
typedef struct _xmlChSRange xmlChSRange;
typedef xmlChSRange *xmlChSRangePtr;
struct _xmlChSRange {
    unsigned short      low;
    unsigned short      high;
};

typedef struct _xmlChLRange xmlChLRange;
typedef xmlChLRange *xmlChLRangePtr;
struct _xmlChLRange {
    unsigned int        low;
    unsigned int        high;
};

typedef struct _xmlChRangeGroup xmlChRangeGroup;
typedef xmlChRangeGroup *xmlChRangeGroupPtr;
struct _xmlChRangeGroup {
    int                 nbShortRange;
    int                 nbLongRange;
    const xmlChSRange   *shortRange;    /* points to an array of ranges */
    const xmlChLRange   *longRange;
};

/**
 * Range checking routine
 */
LMX_RDECL int
                xmlCharInRange(unsigned int val, const xmlChRangeGroup *group);


/**
 * xmlIsBaseChar_ch:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 * (Removed end-of-line continuation markers to aid portability)
 */
#define xmlIsBaseChar_ch1(c)     ((0x41 <= (c)) && ((c) <= 0x5a)) || ((0x61 <= (c)) && ((c) <= 0x7a))
#define xmlIsBaseChar_ch2(c)     ((0xc0 <= (c)) && ((c) <= 0xd6)) || ((0xd8 <= (c)) && ((c) <= 0xf6)) || (0xf8 <= (c))

#define xmlIsBaseChar_ch(c)      (xmlIsBaseChar_ch1(c) || xmlIsBaseChar_ch2(c))

/**
 * xmlIsBaseCharQ:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 */
#define xmlIsBaseCharQ(c)       (((c) < 0x100) ? xmlIsBaseChar_ch((c)) : xmlCharInRange((c), &xmlIsBaseCharGroup))

extern LMX_RDECL const xmlChRangeGroup xmlIsBaseCharGroup;

/**
 * xmlIsBlank_ch:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 */
#define xmlIsBlank_ch(c)        (((c) == 0x20) || ((0x9 <= (c)) && ((c) <= 0xa)) || ((c) == 0xd))

/**
 * xmlIsBlankQ:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 */
#define xmlIsBlankQ(c)          (((c) < 0x100) ? xmlIsBlank_ch((c)) : 0)


/**
 * xmlIsChar_ch:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 */
#define xmlIsChar_ch(c)         (((0x9 <= (c)) && ((c) <= 0xa)) || ((c) == 0xd) || (0x20 <= (c)))

/**
 * xmlIsCharQ:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 * (Removed end-of-line continuation markers to aid portability)
 */
#define xmlIsCharQ1(c)          ((0x100 <= (c)) && ((c) <= 0xd7ff)) || ((0xe000 <= (c)) && ((c) <= 0xfffd))
#define xmlIsCharQ2(c)          ((0x10000 <= (c)) && ((c) <= 0x10ffff))

#define xmlIsCharQ(c)           (((c) < 0x100) ? xmlIsChar_ch((c)) : (xmlIsCharQ1(c) || xmlIsCharQ2(c)))

extern LMX_RDECL const xmlChRangeGroup xmlIsCharGroup;

/**
 * xmlIsCombiningQ:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 */
#define xmlIsCombiningQ(c)  (((c) < 0x100) ? 0 : xmlCharInRange((c), &xmlIsCombiningGroup))

extern LMX_RDECL const xmlChRangeGroup xmlIsCombiningGroup;

/**
 * xmlIsDigit_ch:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 */
#define xmlIsDigit_ch(c)    (((0x30 <= (c)) && ((c) <= 0x39)))

/**
 * xmlIsDigitQ:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 */
#define xmlIsDigitQ(c)      (((c) < 0x100) ? xmlIsDigit_ch((c)) : xmlCharInRange((c), &xmlIsDigitGroup))

extern LMX_RDECL const xmlChRangeGroup xmlIsDigitGroup;

/**
 * xmlIsExtender_ch:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 */
#define xmlIsExtender_ch(c) (((c) == 0xb7))

/**
 * xmlIsExtenderQ:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 */
#define xmlIsExtenderQ(c) (((c) < 0x100) ? xmlIsExtender_ch((c)) : xmlCharInRange((c), &xmlIsExtenderGroup))

extern LMX_RDECL const xmlChRangeGroup xmlIsExtenderGroup;

/**
 * xmlIsIdeographicQ:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 * (Removed end-of-line continuation markers to aid portability)
 */
#define xmlIsIdeographicQ1(c)    (((0x4e00 <= (c)) && ((c) <= 0x9fa5)) || ((c) == 0x3007) || ((0x3021 <= (c)) && ((c) <= 0x3029)))

#define xmlIsIdeographicQ(c)     (((c) < 0x100) ? 0 : xmlIsIdeographicQ1(c))

extern LMX_RDECL const xmlChRangeGroup xmlIsIdeographicGroup;
extern LMX_RDECL const unsigned char xmlIsPubidChar_tab[256];

/**
 * xmlIsPubidChar_ch:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 */
#define xmlIsPubidChar_ch(c)    (xmlIsPubidChar_tab[(c)])

/**
 * xmlIsPubidCharQ:
 * @c: char to validate
 *
 * Automatically generated by genChRanges.py
 */
#define xmlIsPubidCharQ(c)      (((c) < 0x100) ? xmlIsPubidChar_ch((c)) : 0)

LMX_RDECL int xmlIsBaseChar(unsigned int ch);
LMX_RDECL int xmlIsBlank(unsigned int ch);
LMX_RDECL int xmlIsChar(unsigned int ch);
LMX_RDECL int xmlIsCombining(unsigned int ch);
LMX_RDECL int xmlIsDigit(unsigned int ch);
LMX_RDECL int xmlIsExtender(unsigned int ch);
LMX_RDECL int xmlIsIdeographic(unsigned int ch);
LMX_RDECL int xmlIsPubidChar(unsigned int ch);

LMX_RDECL int xmlUCSIsAegeanNumbers     (int code);
LMX_RDECL int xmlUCSIsAlphabeticPresentationForms       (int code);
LMX_RDECL int xmlUCSIsArabic    (int code);
LMX_RDECL int xmlUCSIsArabicPresentationFormsA  (int code);
LMX_RDECL int xmlUCSIsArabicPresentationFormsB  (int code);
LMX_RDECL int xmlUCSIsArmenian  (int code);
LMX_RDECL int xmlUCSIsArrows    (int code);
LMX_RDECL int xmlUCSIsBasicLatin        (int code);
LMX_RDECL int xmlUCSIsBengali   (int code);
LMX_RDECL int xmlUCSIsBlockElements     (int code);
LMX_RDECL int xmlUCSIsBopomofo  (int code);
LMX_RDECL int xmlUCSIsBopomofoExtended  (int code);
LMX_RDECL int xmlUCSIsBoxDrawing        (int code);
LMX_RDECL int xmlUCSIsBraillePatterns   (int code);
LMX_RDECL int xmlUCSIsBuhid     (int code);
LMX_RDECL int xmlUCSIsByzantineMusicalSymbols   (int code);
LMX_RDECL int xmlUCSIsCJKCompatibility  (int code);
LMX_RDECL int xmlUCSIsCJKCompatibilityForms     (int code);
LMX_RDECL int xmlUCSIsCJKCompatibilityIdeographs        (int code);
LMX_RDECL int xmlUCSIsCJKCompatibilityIdeographsSupplement      (int code);
LMX_RDECL int xmlUCSIsCJKRadicalsSupplement     (int code);
LMX_RDECL int xmlUCSIsCJKSymbolsandPunctuation  (int code);
LMX_RDECL int xmlUCSIsCJKUnifiedIdeographs      (int code);
LMX_RDECL int xmlUCSIsCJKUnifiedIdeographsExtensionA    (int code);
LMX_RDECL int xmlUCSIsCJKUnifiedIdeographsExtensionB    (int code);
LMX_RDECL int xmlUCSIsCherokee  (int code);
LMX_RDECL int xmlUCSIsCombiningDiacriticalMarks (int code);
LMX_RDECL int xmlUCSIsCombiningDiacriticalMarksforSymbols       (int code);
LMX_RDECL int xmlUCSIsCombiningHalfMarks        (int code);
LMX_RDECL int xmlUCSIsCombiningMarksforSymbols  (int code);
LMX_RDECL int xmlUCSIsControlPictures   (int code);
LMX_RDECL int xmlUCSIsCurrencySymbols   (int code);
LMX_RDECL int xmlUCSIsCypriotSyllabary  (int code);
LMX_RDECL int xmlUCSIsCyrillic  (int code);
LMX_RDECL int xmlUCSIsCyrillicSupplement        (int code);
LMX_RDECL int xmlUCSIsDeseret   (int code);
LMX_RDECL int xmlUCSIsDevanagari        (int code);
LMX_RDECL int xmlUCSIsDingbats  (int code);
LMX_RDECL int xmlUCSIsEnclosedAlphanumerics     (int code);
LMX_RDECL int xmlUCSIsEnclosedCJKLettersandMonths       (int code);
LMX_RDECL int xmlUCSIsEthiopic  (int code);
LMX_RDECL int xmlUCSIsGeneralPunctuation        (int code);
LMX_RDECL int xmlUCSIsGeometricShapes   (int code);
LMX_RDECL int xmlUCSIsGeorgian  (int code);
LMX_RDECL int xmlUCSIsGothic    (int code);
LMX_RDECL int xmlUCSIsGreek     (int code);
LMX_RDECL int xmlUCSIsGreekExtended     (int code);
LMX_RDECL int xmlUCSIsGreekandCoptic    (int code);
LMX_RDECL int xmlUCSIsGujarati  (int code);
LMX_RDECL int xmlUCSIsGurmukhi  (int code);
LMX_RDECL int xmlUCSIsHalfwidthandFullwidthForms        (int code);
LMX_RDECL int xmlUCSIsHangulCompatibilityJamo   (int code);
LMX_RDECL int xmlUCSIsHangulJamo        (int code);
LMX_RDECL int xmlUCSIsHangulSyllables   (int code);
LMX_RDECL int xmlUCSIsHanunoo   (int code);
LMX_RDECL int xmlUCSIsHebrew    (int code);
LMX_RDECL int xmlUCSIsHighPrivateUseSurrogates  (int code);
LMX_RDECL int xmlUCSIsHighSurrogates    (int code);
LMX_RDECL int xmlUCSIsHiragana  (int code);
LMX_RDECL int xmlUCSIsIPAExtensions     (int code);
LMX_RDECL int xmlUCSIsIdeographicDescriptionCharacters  (int code);
LMX_RDECL int xmlUCSIsKanbun    (int code);
LMX_RDECL int xmlUCSIsKangxiRadicals    (int code);
LMX_RDECL int xmlUCSIsKannada   (int code);
LMX_RDECL int xmlUCSIsKatakana  (int code);
LMX_RDECL int xmlUCSIsKatakanaPhoneticExtensions        (int code);
LMX_RDECL int xmlUCSIsKhmer     (int code);
LMX_RDECL int xmlUCSIsKhmerSymbols      (int code);
LMX_RDECL int xmlUCSIsLao       (int code);
LMX_RDECL int xmlUCSIsLatin1Supplement  (int code);
LMX_RDECL int xmlUCSIsLatinExtendedA    (int code);
LMX_RDECL int xmlUCSIsLatinExtendedB    (int code);
LMX_RDECL int xmlUCSIsLatinExtendedAdditional   (int code);
LMX_RDECL int xmlUCSIsLetterlikeSymbols (int code);
LMX_RDECL int xmlUCSIsLimbu     (int code);
LMX_RDECL int xmlUCSIsLinearBIdeograms  (int code);
LMX_RDECL int xmlUCSIsLinearBSyllabary  (int code);
LMX_RDECL int xmlUCSIsLowSurrogates     (int code);
LMX_RDECL int xmlUCSIsMalayalam (int code);
LMX_RDECL int xmlUCSIsMathematicalAlphanumericSymbols   (int code);
LMX_RDECL int xmlUCSIsMathematicalOperators     (int code);
LMX_RDECL int xmlUCSIsMiscellaneousMathematicalSymbolsA (int code);
LMX_RDECL int xmlUCSIsMiscellaneousMathematicalSymbolsB (int code);
LMX_RDECL int xmlUCSIsMiscellaneousSymbols      (int code);
LMX_RDECL int xmlUCSIsMiscellaneousSymbolsandArrows     (int code);
LMX_RDECL int xmlUCSIsMiscellaneousTechnical    (int code);
LMX_RDECL int xmlUCSIsMongolian (int code);
LMX_RDECL int xmlUCSIsMusicalSymbols    (int code);
LMX_RDECL int xmlUCSIsMyanmar   (int code);
LMX_RDECL int xmlUCSIsNumberForms       (int code);
LMX_RDECL int xmlUCSIsOgham     (int code);
LMX_RDECL int xmlUCSIsOldItalic (int code);
LMX_RDECL int xmlUCSIsOpticalCharacterRecognition       (int code);
LMX_RDECL int xmlUCSIsOriya     (int code);
LMX_RDECL int xmlUCSIsOsmanya   (int code);
LMX_RDECL int xmlUCSIsPhoneticExtensions        (int code);
LMX_RDECL int xmlUCSIsPrivateUse        (int code);
LMX_RDECL int xmlUCSIsPrivateUseArea    (int code);
LMX_RDECL int xmlUCSIsRunic     (int code);
LMX_RDECL int xmlUCSIsShavian   (int code);
LMX_RDECL int xmlUCSIsSinhala   (int code);
LMX_RDECL int xmlUCSIsSmallFormVariants (int code);
LMX_RDECL int xmlUCSIsSpacingModifierLetters    (int code);
LMX_RDECL int xmlUCSIsSpecials  (int code);
LMX_RDECL int xmlUCSIsSuperscriptsandSubscripts (int code);
LMX_RDECL int xmlUCSIsSupplementalArrowsA       (int code);
LMX_RDECL int xmlUCSIsSupplementalArrowsB       (int code);
LMX_RDECL int xmlUCSIsSupplementalMathematicalOperators (int code);
LMX_RDECL int xmlUCSIsSupplementaryPrivateUseAreaA      (int code);
LMX_RDECL int xmlUCSIsSupplementaryPrivateUseAreaB      (int code);
LMX_RDECL int xmlUCSIsSyriac    (int code);
LMX_RDECL int xmlUCSIsTagalog   (int code);
LMX_RDECL int xmlUCSIsTagbanwa  (int code);
LMX_RDECL int xmlUCSIsTags      (int code);
LMX_RDECL int xmlUCSIsTaiLe     (int code);
LMX_RDECL int xmlUCSIsTaiXuanJingSymbols        (int code);
LMX_RDECL int xmlUCSIsTamil     (int code);
LMX_RDECL int xmlUCSIsTelugu    (int code);
LMX_RDECL int xmlUCSIsThaana    (int code);
LMX_RDECL int xmlUCSIsThai      (int code);
LMX_RDECL int xmlUCSIsTibetan   (int code);
LMX_RDECL int xmlUCSIsUgaritic  (int code);
LMX_RDECL int xmlUCSIsUnifiedCanadianAboriginalSyllabics        (int code);
LMX_RDECL int xmlUCSIsVariationSelectors        (int code);
LMX_RDECL int xmlUCSIsVariationSelectorsSupplement      (int code);
LMX_RDECL int xmlUCSIsYiRadicals        (int code);
LMX_RDECL int xmlUCSIsYiSyllables       (int code);
LMX_RDECL int xmlUCSIsYijingHexagramSymbols     (int code);

LMX_RDECL int xmlUCSIsBlock     (int code, const char *block);

LMX_RDECL int xmlUCSIsCatC      (int code);
LMX_RDECL int xmlUCSIsCatCc     (int code);
LMX_RDECL int xmlUCSIsCatCf     (int code);
LMX_RDECL int xmlUCSIsCatCo     (int code);
LMX_RDECL int xmlUCSIsCatCs     (int code);
LMX_RDECL int xmlUCSIsCatL      (int code);
LMX_RDECL int xmlUCSIsCatLl     (int code);
LMX_RDECL int xmlUCSIsCatLm     (int code);
LMX_RDECL int xmlUCSIsCatLo     (int code);
LMX_RDECL int xmlUCSIsCatLt     (int code);
LMX_RDECL int xmlUCSIsCatLu     (int code);
LMX_RDECL int xmlUCSIsCatM      (int code);
LMX_RDECL int xmlUCSIsCatMc     (int code);
LMX_RDECL int xmlUCSIsCatMe     (int code);
LMX_RDECL int xmlUCSIsCatMn     (int code);
LMX_RDECL int xmlUCSIsCatN      (int code);
LMX_RDECL int xmlUCSIsCatNd     (int code);
LMX_RDECL int xmlUCSIsCatNl     (int code);
LMX_RDECL int xmlUCSIsCatNo     (int code);
LMX_RDECL int xmlUCSIsCatP      (int code);
LMX_RDECL int xmlUCSIsCatPc     (int code);
LMX_RDECL int xmlUCSIsCatPd     (int code);
LMX_RDECL int xmlUCSIsCatPe     (int code);
LMX_RDECL int xmlUCSIsCatPf     (int code);
LMX_RDECL int xmlUCSIsCatPi     (int code);
LMX_RDECL int xmlUCSIsCatPo     (int code);
LMX_RDECL int xmlUCSIsCatPs     (int code);
LMX_RDECL int xmlUCSIsCatS      (int code);
LMX_RDECL int xmlUCSIsCatSc     (int code);
LMX_RDECL int xmlUCSIsCatSk     (int code);
LMX_RDECL int xmlUCSIsCatSm     (int code);
LMX_RDECL int xmlUCSIsCatSo     (int code);
LMX_RDECL int xmlUCSIsCatZ      (int code);
LMX_RDECL int xmlUCSIsCatZl     (int code);
LMX_RDECL int xmlUCSIsCatZp     (int code);
LMX_RDECL int xmlUCSIsCatZs     (int code);

LMX_RDECL int xmlUCSIsCat       (int code, const char *cat);

} /* End of namepsace lmx */

#endif /* LMXUNICODE_H */
