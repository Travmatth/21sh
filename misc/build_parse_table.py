#!/usr/bin/python

epsilon_set = {"EPS"}

def read_rules(filename, rules, terminal_set, nonterminal_set):
	with open(filename) as file:
		productions = file.read().split("\n")
		for production in productions:
			sym, deriv_text = production.split(":", 2) 
			derivs = [[deriv for deriv in derivs_text.split()]
				for derivs_text in deriv_text.split(",")]
			rules[sym] = derivs
			if sym.islower() and sym not in nonterminal_set:
				nonterminal_set.add(sym)
			elif not sym.islower() and sym not in terminal_set:
				terminal_set.add(sym)
			for deriv in derivs:
				for symbol in deriv:
					if symbol.islower() and symbol not in nonterminal_set:
						nonterminal_set.add(symbol)
					elif not symbol.islower() and symbol not in terminal_set:
						terminal_set.add(symbol)

def build_first_sets(rules, first):
	for terminal in terminal_set:
		first[terminal] = set()
		first[terminal].add(terminal)
	for nonterminal in nonterminal_set:
		first[nonterminal] = set()
	changed = True
	while (changed):
		changed = False
		for rule, derivs in rules.iteritems():
			for deriv in derivs:
				rhs = set()
				k = len(deriv) - 1
				rhs = first[deriv[0]] - epsilon_set
				i = 0
				while (i <= k - 1 and "EPS" in first[deriv[i]]):
					rhs = rhs | (first[deriv[i + 1]] - epsilon_set)
					i += 1
				if i == k and "EPS" in first[deriv[k]]:
					rhs = rhs | epsilon_set
				if not first[rule] >= rhs:
					first[rule] = first[rule] | rhs
					changed = True

def construct_cc_0(cc_0)
	cc_0.insert(0, "s")
	cc_0.insert(1, rules["s"])
	cc_0[1].insert(0, ".")
	cc_0.insert(2, "eof")

def construct_canonical_collection(rules, terminal_set, nonterminal_set, canonical_collection)
	cc_0 = []
	construct_cc_0(cc_0)

if __name__ == "__main__":
	rules = {}
	first_sets = {}
	terminal_set = set()
	nonterminal_set = set()
	read_rules("misc/paren_grammar.txt", rules, terminal_set, nonterminal_set)
	build_first_sets(rules, first_sets)
	canonical_collection = []
	construct_canonical_collection(rules, terminal_set, nonterminal_set, canonical_collection)
	print(first_sets)