#!/usr/bin/python

import copy
from collections import namedtuple

# https://github.com/search?l=Python&q=follow+symbol+reversed+trailer&type=Code
def read_rules(filename):
	rules = {}
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

def union(first, begins):
	n = len(first)
	first |= begins
	return len(first) != n

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
				# if not first[nt] >= rhs:
				if not first[nt] >= rhs:
					first[nt] |= rhs
					changed = True
	return first

def compute_follow(rules, terminals, nonterminals):
	follow = {}
	for A in nonterminals:
		follow[A] = set()
	follow["s"] = {"eof"}
	changed = True
	while (changed):
		changed = False
		for nt, production_set in rules.iteritems():
			for production in production_set:
				trailer = follow[nt]
				n = len(trailer)
				for symbol in reversed(production):
					if symbol in nonterminals:
						new_set = follow[symbol] | trailer
						if n != len(trailer):
							follow[symbol] = new_set
							print(symbol)
							print(follow[symbol])
							changed = True
						if "EPS" in first[symbol]:
							trailer |= first[symbol] - {"EPS"}
						else:
							trailer = first[symbol]
					else:
						trailer = first[symbol]
	return follow

def construct_canonical_collection(rules, terminals, nonterminals, first_sets, canonical_collection):
	pass

if __name__ == "__main__":
	rules, terminals, nonterminals = read_rules("misc/simple_grammar.txt")
	# print(rules)
	# first, follow, follow_plus = build_sets(rules, terminals, nonterminals)
	first = compute_first(rules, terminals, nonterminals)
	follow = compute_follow(rules, terminals, nonterminals)
	# print("first")
	# for k, v in first.iteritems():
	# 	print(k, v)
	print("follow")
	for k, v in follow.iteritems():
		print(k, v)
	# print("epsilon")
	# for k in follow_plus:
	# 	print(k)