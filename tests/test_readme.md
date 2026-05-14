test_1.c: cover every well formed scenario with all keywords, identifiers, delimiters, numbers, back up boundaries,
            tabs, blank lines, and indentation.
test_2.c: cover panic mode scenarios and ending without a trailing newline to test EOF flush in ERROR state.
test_3a.c: identifier at the exact buffer limit (255 chars), catches the off by one bug in the IDENTIFIER bounds check.
test_3b.c: identifier over the buffer limit, proves IDENTIFIER transitions to ERROR safely without overflow.
test_3c.c: digits over the buffer limit, proves NUMBER transitions to ERROR safely without overflow.
test_3d.c: 10,000 nested braces to test the stability under stress.
test_5a.c: Empty input which expects exit only.
test_5b.c: Whitespace only input which expects exit only.
test_5c.c: Single token with no trailing newline which expects EOF flush of pending lexeme works