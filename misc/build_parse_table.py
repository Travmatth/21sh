#!/usr/bin/python

epsilon_set = {"EPS"}
terminal_set = {"(", ")", "NUM", "NAME", "+", "-", "X", "/", "EOF", "EPS"}
nonterminal_set = {"s", "expr", "expr'", "term", "term'", "factor"}

def read_rules(filename, rules):
	with open(filename) as file:
		rules_text = file.read().split("\n")
		for rule in rules_text:
			sym, deriv_text = rule.split(":", 2) 
			derivs = [[deriv for deriv in derivs_text.split()]
				for derivs_text in deriv_text.split(",")]
			rules[sym] = derivs

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

if __name__ == "__main__":
	rules = {}
	first_sets = {}
	read_rules("misc/simple_grammar.txt", rules)
	build_first_sets(rules, first_sets)
	print(first_sets)