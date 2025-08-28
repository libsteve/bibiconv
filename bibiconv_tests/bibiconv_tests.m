//
//  bibiconv_tests.m
//  bibiconv-tests
//
//  Created by Steve Brunwasser on 3/6/25.
//

#import <XCTest/XCTest.h>
#import <bibiconv/bibiconv.h>
#import <bibiconv/codetable.h>
#import <bibiconv/marc8_tables.h>
#import "marc8_table_macros.h"

@interface bibiconv_tests : XCTestCase

@end

@implementation bibiconv_tests

- (void)testBasicLatinSuccess {
    errno = 0;
    size_t location = 0;
    ucs4_t unicode = 0x00;
    bool is_combining = false;
    char const *const input = "Hello world!";
    bool success = bib_iconv_charset_lookup(&bib_iconv_marc8_basic_latin,
                                            strlen(input), input,
                                            &location, &unicode, &is_combining);
    XCTAssertEqual(errno, 0);
    XCTAssertTrue(success);
    XCTAssertEqual(location, 1);
    XCTAssertEqual(unicode, 'H');
    XCTAssertFalse(is_combining);
}

- (void)testBasicLatinFailure {
    errno = 0;
    size_t location = 0;
    ucs4_t unicode = 0x00;
    bool is_combining = false;
    char const input[2] = { 0x80, 0x00 };
    bool success = bib_iconv_charset_lookup(&bib_iconv_marc8_basic_latin,
                                            strlen(input), input,
                                            &location, &unicode, &is_combining);
    XCTAssertEqual(errno, EILSEQ);
    XCTAssertFalse(success);
    XCTAssertEqual(location, 0);
    XCTAssertEqual(unicode, 0x00);
    XCTAssertFalse(is_combining);
}

- (void)testExtendedLatinCombiningSuccess {
    errno = 0;
    size_t location = 0;
    ucs4_t unicode = 0x00;
    bool is_combining = false;
    char const input[2] = { 0xE0, 0x00 };
    bool success = bib_iconv_charset_lookup(&bib_iconv_marc8_extended_latin,
                                            strlen(input), input,
                                            &location, &unicode, &is_combining);
    XCTAssertEqual(errno, 0);
    XCTAssertTrue(success);
    XCTAssertEqual(location, 1);
    XCTAssertEqual(unicode, 0x0309);
    XCTAssertTrue(is_combining);
}

- (void)testExtendedLatinFailure {
    errno = 0;
    size_t location = 0;
    ucs4_t unicode = 0x00;
    bool is_combining = false;
    char const input[2] = { 0x50, 0x00 };
    bool success = bib_iconv_charset_lookup(&bib_iconv_marc8_extended_latin,
                                            strlen(input), input,
                                            &location, &unicode, &is_combining);
    XCTAssertEqual(errno, EILSEQ);
    XCTAssertFalse(success);
    XCTAssertEqual(location, 0);
    XCTAssertEqual(unicode, 0x00);
    XCTAssertFalse(is_combining);
}

- (void)testG0EastAsianCharacterSetMultibyteSequenceSuccess {
    errno = 0;
    size_t location = 0;
    ucs4_t unicode = 0x00;
    bool is_combining = false;
    char const input[] = { 0x21, 0x2F, 0x30, 0x00 };
    bool success = bib_iconv_charset_lookup(&bib_iconv_marc8_eacc,
                                            strlen(input), input,
                                            &location, &unicode, &is_combining);
    XCTAssertEqual(errno, 0);
    XCTAssertTrue(success);
    XCTAssertEqual(location, 3);
    XCTAssertEqual(unicode, 0x3007);
    XCTAssertFalse(is_combining);
}

- (void)testG1EastAsianCharacterSetMultibyteSequenceSuccess {
    errno = 0;
    size_t location = 0;
    ucs4_t unicode = 0x00;
    bool is_combining = false;
    char const input[] = { 0xA1, 0x2F, 0x30, 0x00 };
    bool success = bib_iconv_charset_lookup(&bib_iconv_marc8_eacc,
                                            strlen(input), input,
                                            &location, &unicode, &is_combining);
    XCTAssertEqual(errno, 0);
    XCTAssertTrue(success);
    XCTAssertEqual(location, 3);
    XCTAssertEqual(unicode, 0x3007);
    XCTAssertFalse(is_combining);
}

- (void)testEastAsianCharacterSetSingleByteSequenceFailure {
    errno = 0;
    size_t location = 0;
    ucs4_t unicode = 0x00;
    bool is_combining = false;
    char const input[2] = { 0x30, 0x00 };
    bool success = bib_iconv_charset_lookup(&bib_iconv_marc8_eacc,
                                            strlen(input), input,
                                            &location, &unicode, &is_combining);
    XCTAssertEqual(errno, EILSEQ);
    XCTAssertFalse(success);
    XCTAssertEqual(location, 0);
    XCTAssertEqual(unicode, 0x00);
    XCTAssertFalse(is_combining);
}

- (void)testEastAsianCharacterSetMultibyteSequenceFailure {
    errno = 0;
    size_t location = 0;
    ucs4_t unicode = 0x00;
    bool is_combining = false;
    char const input[4] = { 0x21, 0x2F, 0xFF, 0x00 };
    bool success = bib_iconv_charset_lookup(&bib_iconv_marc8_eacc,
                                            strlen(input), input,
                                            &location, &unicode, &is_combining);
    XCTAssertEqual(errno, EILSEQ);
    XCTAssertFalse(success);
    XCTAssertEqual(location, 0);
    XCTAssertEqual(unicode, 0x00);
    XCTAssertFalse(is_combining);
}

- (void)testIconvBasicLatinSuccess {
    bib_iconv_t cd = bib_iconv_open("UTF-8", "MARC-8");
    XCTAssertNotEqual(cd, (bib_iconv_t)-1);
    char const *const from = "ABCDEFG";
    char const *src = from;
    size_t srclen = sizeof(from);
    char destination[32] = { 0 };
    char *dst = destination;
    size_t dstlen = sizeof(destination);
    size_t result = bib_iconv(cd, &src, &srclen, &dst, &dstlen);
    XCTAssertEqual(result, 0);
    XCTAssertEqual(strcmp("ABCDEFG", destination), 0);
    bib_iconv_close(cd);
}

- (void)testIconvG0EastAsianCharactersSuccess {
    bib_iconv_t cd = bib_iconv_open("UTF-8", "MARC-8");
    XCTAssertNotEqual(cd, (bib_iconv_t)-1);
    char const from[] = "\e$(1\x21\x2F\x30";
    char const *src = from;
    size_t srclen = sizeof(from);
    char destination[32] = { 0 };
    char *dst = destination;
    size_t dstlen = sizeof(destination);
    size_t result = bib_iconv(cd, &src, &srclen, &dst, &dstlen);
    XCTAssertEqual(result, 0);
    XCTAssertEqual(strcmp("\u3007", destination), 0);
    bib_iconv_close(cd);
}

- (void)testIconvG1EastAsianCharactersSuccess {
    bib_iconv_t cd = bib_iconv_open("UTF-8", "MARC-8");
    XCTAssertNotEqual(cd, (bib_iconv_t)-1);
    char const from[] = "\e$)1\xA1\x2F\x30";
    char const *src = from;
    size_t srclen = sizeof(from);
    char destination[32] = { 0 };
    char *dst = destination;
    size_t dstlen = sizeof(destination);
    size_t result = bib_iconv(cd, &src, &srclen, &dst, &dstlen);
    XCTAssertEqual(result, 0);
    XCTAssertEqual(strcmp("\u3007", destination), 0);
    bib_iconv_close(cd);
}

- (void)testIconvUTF8ToUTF8 {
    bib_iconv_t cd = bib_iconv_open("UTF-8", "UTF-8");
    XCTAssertNotEqual(cd, (bib_iconv_t)-1);
    char const from[] = "Hello world! \U0001f600";
    char const *src = from;
    size_t srclen = sizeof(from);
    char destination[32] = { 0 };
    char *dst = destination;
    size_t dstlen = sizeof(destination);
    size_t result = bib_iconv(cd, &src, &srclen, &dst, &dstlen);
    XCTAssertEqual(result, 0);
    XCTAssertEqual(strcmp("Hello world! \U0001f600", destination), 0);
    bib_iconv_close(cd);
}

- (void)testIconvMARC8ToUTF8 {
    bib_iconv_t cd = bib_iconv_open("utf8", "marc8");
    XCTAssertNotEqual(cd, (bib_iconv_t)-1);
    char const from[] = "Hello \e$1\x21\x30\x2D\e(B!";
    char const *src = from;
    size_t srclen = sizeof(from);
    char destination[32] = { 0 };
    char *dst = destination;
    size_t dstlen = sizeof(destination);
    size_t result = bib_iconv(cd, &src, &srclen, &dst, &dstlen);
    XCTAssertEqual(result, 0);
    XCTAssertEqual(strcmp("Hello \u4E16!", destination), 0);
    bib_iconv_close(cd);
}

- (void)testIconvOpenUTF8ToMARC8Failure {
    /* Conversion to MARC-8 is not yet supported. */
    bib_iconv_t cd = bib_iconv_open("marc8", "utf8");
    XCTAssertEqual(errno, EINVAL);
    XCTAssertEqual(cd, (bib_iconv_t)-1);
    if (cd != (bib_iconv_t)-1) {
        bib_iconv_close(cd);
    }
}

@end
