"""
Step 1: Build FIRST sets needed to construct action/goto tables
	To build FIRST(X):
	1. if X is a terminal, FIRST(X) is {X}
	2. if X ::= <empty>, then <empty> in FIRST(X).
	3. if X ::= Y_1Y_2...Y_k then put FIRST(Y_1) in FIRST(X)
	4. if X is a non-terminal and X ::= Y_1Y_2...Y_k, then
		a in FIRST(X) if a in FIRST(Y_i) and <empty> in FIRST(Y_j ) for all
		1 < j < i.
	(If  <empty> not in FIRST(Y1), then FIRST(Y_i) is irrelevant, for 1 < i.)
https://parasol.tamu.edu/~rwerger/Courses/434/lec12-sum.pdf
"""
