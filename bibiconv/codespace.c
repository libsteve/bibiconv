//
//  codespace.c
//  bibiconv
//

#include <stdlib.h>
#include "codespace.h"
#include "cstderrno.h"

/**
 * Compares the given key to the given code page to see if the page with the
 * given is ordered before or after the given page.
 *
 * - parameter k: The ``bib_iconv_code_page/high_bits`` value for the code page
 *   being searched for.
 * - parameter p: The current code page being looked at in a sorted array.
 * - returns:
 *   - `-1` if the page being searched for is somewhere before the given page.
 *   - `1` if the page being searched for is somewhere after the given page.
 *   - `0` if the given page is the one being searched for.
 */
static int compare_key_to_code_page(void const *k, void const *p) {
    uint32_t key = *(uint32_t const *)k;
    bib_iconv_code_page_t const *page = p;
    if (key < page->high_bits) {
        return -1;
    } else if (key > page->high_bits) {
        return 1;
    } else {
        return 0;
    }
}

/**
 * Compares the given code point to the given code range to see if the range
 * with the given code point is ordered before or after the given range.
 *
 * - parameter c: The Unicode code point that lies within the desired range.
 * - parameter r: The current code range being looked at in a sorted array.
 * - returns:
 *   - `-1` if the range being searched for is somewhere before the given range.
 *   - `1` if the page being searched for is somewhere after the given range.
 *   - `0` if the given range contains the given unicode code point.
 */
static int compare_code_point_to_code_range(void const *c, void const *r) {
    ucs4_t code_point = *(ucs4_t const *)c;
    bib_iconv_code_range_t const *range = r;
    if (code_point < range->first) {
        return -1;
    } else if (code_point >= (range->first + range->count)) {
        return 1;
    } else {
        return 0;
    }
}

struct bib_iconv_code_entry const *
bib_iconv_code_space_lookup(struct bib_iconv_code_space const *space,
                            ucs4_t unicode) {
    /* Search for the code page containing the given Unicode code point. */
    bib_iconv_code_page_t const *page;
    uint32_t const page_key = unicode >> 8;
    page = bsearch(&page_key, space->pages, space->page_count,
                   sizeof(bib_iconv_code_page_t), compare_key_to_code_page);
    if (page == nullptr) {
        /* No code page was found, so the code point cannot be converted. */
        return nullptr;
    }

    /* Search for the code range that includes the given Unicode code point. */
    bib_iconv_code_range_t const *range;
    range = bsearch(&unicode, page->ranges, page->range_count,
                    sizeof(bib_iconv_code_range_t),
                    compare_code_point_to_code_range);
    if (range == nullptr) {
        /* No code range was found, so the code point cannot be converted. */
        return nullptr;
    }

    /* Get the entry for the given Unicode code point. */
    size_t index = unicode - range->first;
    bib_iconv_code_entry_t const *entry = &(range->entries[index]);
    if (entry->candidates == nullptr) {
        /* The found entry doesn't have any candidates. */
        /* This indicates that the code point cannot be converted. */
        return nullptr;
    }

    /* The entry has candidates and can be converted to the target encoding. */
    return entry;
}
