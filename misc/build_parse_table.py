#!/usr/bin/python

import copy
from collections import namedtuple, OrderedDict

def read_rules(filename):
	rules = OrderedDict()
	terminals = set()
	nonterminals = set()
	with open(filename) as file:
		for production in file.read().split("\n"):
			lhs, rhs = production.split(":", 2) 
			rhs = rhs.split()
			if lhs in rules:
				rules[lhs].append(rhs)
			else:
				rules[lhs] = [rhs]
			if lhs.islower() and lhs not in nonterminals:
				nonterminals.add(lhs)
			elif not lhs.islower() and lhs not in terminals:
				terminals.add(lhs)
			for production in rhs:
				if production.islower() and production not in nonterminals:
					nonterminals.add(production)
				elif production != "EPS" and not production.islower() and production not in terminals:
					terminals.add(production)
	return rules, terminals, nonterminals 

def	compute_first(rules, terminals, nonterminals):
	first = {}
	for a in (terminals | {"eof"} | {"EPS"}):
		first[a] = {a}
	for a in nonterminals:
		first[a] = set()
	changed = True
	while (changed):
		changed = False
		for nt, production_set in rules.iteritems():
			for production in production_set:
				rhs = first[production[0]] - {"EPS"}
				i = 0
				k = len(production) - 1
				while (i <= k - 1 and "EPS" in first[production[i]]):
					rhs |= first[production[i + 1]] - {"EPS"}
					i += 1
				if (i == k) and "EPS" in first[production[k]]:
					rhs |= {"EPS"}
				if not first[nt] >= rhs:
					first[nt] |= rhs
					changed = True
	return first

def compute_follow(rules, terminals, nonterminals):
	follow = {nt: set() for nt in nonterminals}
	follow["s"] = {"eof"}

	changed = True
	while changed:
		changed = False
		for nt, production_set in rules.items():
			for production in production_set:
				trailer = follow[nt].copy()
				for symbol in reversed(production):
					if symbol in nonterminals:
						new_set = follow[symbol] | trailer
						if len(new_set) != len(follow[symbol]):
							changed = True
							follow[symbol] = new_set
						if "EPS" in first[symbol]:
							trailer |= first[symbol] - {"EPS"}
						else:
							trailer = first[symbol]
					else:
						trailer = first[symbol]
	return follow

Plus_set = namedtuple('Plus_set', ['num', 'predict'])

def compute_first_plus(rules, terminals, nonterminals, first, follow):
	first_plus = {}
	i = 0
	for lhs, production_set in rules.iteritems():
		for production in production_set:
			rule = lhs + ": " + " ".join(production)
			predict = copy.deepcopy(first[production[0]])
			if "EPS" in predict:
				predict |= follow[lhs]
				predict = predict - {"EPS"}
			first_plus[rule] = Plus_set(i, predict)
			i += 1
	return first_plus

def	build_lookup(symbols):
	lookup = {}
	for i, t in enumerate(symbols):
		lookup[t] = i
	return lookup

def build_parse_table(terminals, nonterminals, first, follow, first_plus):
	terminals.add("eof")
	table = [[-1 for i in terminals] for nt in nonterminals]
	terminal_lookup = build_lookup(terminals)
	nonterminal_lookup = build_lookup(nonterminals)

	for nt in nonterminals:
		for production in rules[nt]:
			rule = nt + ": " + " ".join(production)
			current = first_plus[rule]
			for w in current.predict:
				next_prod = terminal_lookup[w]
				next_lookup = nonterminal_lookup[nt]
				if table[next_lookup][next_prod] != -1:
					raise Exception("not ll(1)")
				table[next_lookup][next_prod] = current.num
	return table


if __name__ == "__main__":
	rules, terminals, nonterminals = read_rules("misc/simple_grammar.txt")
	first = compute_first(rules, terminals, nonterminals)
	follow = compute_follow(rules, terminals, nonterminals)
	first_plus = compute_first_plus(rules, terminals, nonterminals, first, follow)
	table = build_parse_table(terminals, nonterminals, first, follow, first_plus)
	for row in table:
		print(row)