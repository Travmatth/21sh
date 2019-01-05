#!/usr/bin/python

import copy
from collections import namedtuple

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
			for deriv in rhs:
				if deriv.islower() and deriv not in nonterminals:
					nonterminals.add(deriv)
				elif deriv != "EPS" and not deriv.islower() and deriv not in terminals:
					terminals.add(deriv)
	return rules, terminals, nonterminals 

def union(first, begins):
	n = len(first)
	first |= begins
	return len(first) != n

def build_sets(rules, terminals, nonterminals):
	first = {}
	follow = {}
	follow_plus = {}
	epsilon = set()

	for nt in nonterminals:
		first[nt] = set()
		follow[nt] = set()
		follow_plus[nt] = set()
	changed = True
	first.update((i, {i}) for i in terminals)
	first["eof"] = {"eof"}
	first["EPS"] = {"EPS"}
	follow["s"] = {"eof"}
	while (changed):
		changed = False
		for nt, derivs in rules.iteritems():
			# print(nt, derivs)
			for deriv in derivs:
				# build first set
				for symbol in deriv:
					# print(symbol)
					changed |= union(first[nt], first[symbol])
					if symbol not in epsilon:
						break
					else:
						changed |= union(epsilon, {nt})
				# build follow set
				trailer = follow[nt]
				for symbol in reversed(deriv):
					if symbol in follow:
						changed |= union(follow[symbol], trailer)
					if symbol in epsilon:
						trailer = trailer.union(first[symbol])
					else:
						trailer = first[symbol]
		if not changed:
			return first, follow, epsilon

def	compute_first(rules, terminals, nonterminals):
	for a in (terminals | {"eof"} | {"EPS"})
		first[a] = {a}
	for a in nonterminals:
		first[a] = set()
	while (True)
		for nt, derivs in rules:
			for deriv in derivs:
				rhs = first[deriv[0]] - {"EPS"}
				i = 0
				while ("EPS" in first[deriv[i]] and i <= len(deriv) - 2):
					rhs |= first[deriv[i + 1]] - {"eps"}
					i += 1
<<<<<<< HEAD
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

# need closure to accept a set of items, rather than a kernel
def closure(items, rules, firsts, nonterminals):
	changed = True
	tmp = set()
	while (changed):
		for item in items:
			changed = False
			# for production rule on symbol kernel
			kernel = item.production.split(":")[0]
			for production in rules[kernel]:
				for deriv in rules[production[0]]:
					# create rule placeholder string, i.e. goal: . list
					next_sym = production[0] + ":" + " . " + " ".join(deriv)
					next_item = ITEM(next_sym, item.lookahead)
					# add next_item with parent lookahead to canonical collection
					if next_item not in items and next_item not in tmp:
						tmp.add(next_item)
						changed = True
					# add items with first terminals to canonical collection
					for sym in firsts[deriv[0]]:
						next_item = ITEM(next_sym, sym)
						if next_item not in items and next_item not in tmp:
							tmp.add(next_item)
							changed = True
					if changed and deriv[0] in nonterminals:
						closure(items | tmp, rules, firsts, nonterminals)
		items = items | tmp
	return items

def goto(cc, terminal, goto_set):
	moved = set()
	for item in cc:
		pass
=======
			if (i == k) and "eps" in first[deriv[k]]:
				rhs |= {"eps"}
			first[nt] |= rhs
>>>>>>> rewriting first/follow computations, again

def compute_follow(rules, terminals, nonterminals):
	follow = {}
	for A in nonterminals:
		follow[A] = set()
	follow["s"] = {"eof"}
	while (true):
		for nt, derivs in rules.iteritems():
			for deriv in derivs:
				trailer = copy.deepcopy(follow[nt])
				for symbol in reversed(deriv):
					if symbol in nonterminals:
						follow[symbol] |= trailer
						if "EPS" in first[symbol]:
							trailer |= first[symbol] - {"EPS"}
						else:
							trailer |= first[symbol]
					else:
						trailer |= first[symbol]

def construct_canonical_collection(rules, terminals, nonterminals, first_sets, canonical_collection):
<<<<<<< HEAD
	cc_0 = set([ITEM("goal: . symbol", "eof")])
	cc_0 = cc_0 | closure(cc_0, rules, first_sets, nonterminals)
	print(len(cc_0))
	print(cc_0)
	# goto_set = set()
	# goto(cc_0, "(", goto_set)
	# canonical_collection.insert(0, cc_0)
=======
	pass
>>>>>>> rewriting first/follow computations, again

if __name__ == "__main__":
	rules, terminals, nonterminals = read_rules("misc/simple_grammar.txt")
	# print(rules)
	first, follow, follow_plus = build_sets(rules, terminals, nonterminals)
	print("first")
	for k, v in first.iteritems():
		print(k, v)
	print("follow")
	for k, v in follow.iteritems():
		print(k, v)
	print("epsilon")
	for k in follow_plus:
		print(k)