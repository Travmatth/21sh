#!/usr/bin/python

import copy
from collections import namedtuple
epsilon_set = {"EPS"}

ITEM = namedtuple("CC_ITEM", "production lookahead")

def read_rules(filename, rules, terminals, nonterminals):
	with open(filename) as file:
		productions = file.read().split("\n")
		for production in productions:
			sym, deriv_text = production.split(":", 2) 
			derivs = [[deriv for deriv in derivs_text.split()]
				for derivs_text in deriv_text.split(",")]
			rules[sym] = derivs
			if sym.islower() and sym not in nonterminals:
				nonterminals.add(sym)
			elif not sym.islower() and sym not in terminals:
				terminals.add(sym)
			for deriv in derivs:
				for symbol in deriv:
					if symbol.islower() and symbol not in nonterminals:
						nonterminals.add(symbol)
					elif not symbol.islower() and symbol not in terminals:
						terminals.add(symbol)

def build_first_sets(rules, first):
	for terminal in terminals:
		first[terminal] = set()
		first[terminal].add(terminal)
	for nonterminal in nonterminals:
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
	first["eof"] = {"eof"}

def construct_items(rule, derivs):
	items = []
	for i in len(derivs):
		item = []
		item.insert(0, rule)
		item.insert(1, derivs[i])
		item[1].insert(i, ".")
		items.append(item)
	return items

def	insert_lookahead(item, symbol):
	item.insert(2, symbol)
	return item

def closure(kernel, lookahead, rules, firsts, cc, nonterminals):
	changed = False
	# for production rule on symbol kernel
	for deriv in rules[kernel]:
		# create rule placeholder string, i.e. goal: . list
		next_sym = kernel + ":" + " . " + " ".join(deriv)
		item = ITEM(next_sym, lookahead)
		# add item with parent lookahead to canonical collection
		if item not in cc:
			cc.add(item)
			changed = True
		# add items with first terminals to canonical collection
		if kernel != "goal":
			for sym in firsts[deriv[0]]:
				item = ITEM(next_sym, sym)
				if item not in cc:
					cc.add(item)
					changed = True
		if changed and deriv[0] in nonterminals:
			closure(deriv[0], lookahead, rules, firsts, cc, nonterminals)


def construct_canonical_collection(rules, terminals, nonterminals, first_sets, canonical_collection):
	kernel = "goal"
	cc_0 = set()
	closure(kernel, "eof", rules, first_sets, cc_0, nonterminals)
	canonical_collection.insert(0, cc_0)

if __name__ == "__main__":
	rules = {}
	first_sets = {}
	terminals = set()
	nonterminals = set()
	read_rules("misc/paren_grammar.txt", rules, terminals, nonterminals)
	build_first_sets(rules, first_sets)
	canonical_collection = []
	construct_canonical_collection(rules, terminals, nonterminals, first_sets, canonical_collection)