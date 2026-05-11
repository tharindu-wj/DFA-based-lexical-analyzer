test_1.c: cover every well formed scenario with all keywords, identifiers, delimiters, numbers, back up boundaries,
            tabs, blank lines, and indentation.
test_2.c: cover panic mode scenarios and ending without a trailing newline to test EOF flush in ERROR state.
test_3a.c: identifier at the exact buffer limit (255 chars), catches the off by one bug in the IN_ID bounds check. 