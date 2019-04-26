import re
from bs4 import BeautifulSoup, NavigableString, Tag

nbsp = u'\xa0'

def parse_state(div):
	arr = []
	div = str(div)
	div = re.sub(r'<\/?div[ ]?[a-z]*?=?"?[a-z|A-Z|0-9|-]*?"?>', '', div)
	div = filter(None, div.split('<br/>'))
	for handle in div:
		handle_dict = {}
		handle_str = handle.split(' -&gt; ')
		handle_dict['lhs'] = handle_str[0]
		handle_str = handle_str[1].split(' #lookaheads= ')
		handle_dict['rhs'] = handle_str[0].split()
		handle_dict['lookaheads'] = handle_str[1].split()
		arr.append(handle_dict)
	return arr

def parse(soup, legend, parse_table, handles):
	for row in soup.table.thead.tr:
		legend.append(row.get_text())
	for i, row in enumerate(soup.table.tbody):
		parse_table.insert(i, [])
		for j, col in enumerate(row):
			if j == 0:
				handles.insert(i, parse_state(col.div))
			elif col.text == nbsp:
				parse_table[i].insert(j - 1, '-')
			else:
				parse_table[i].insert(j - 1, col.contents[0].text)

def format_prods(raw_handles, handles):
	max_rhs = 0
	max_lookaheads = 0
	for i, state in enumerate(raw_handles):
		handles.insert(i, [])
		for j, handle in enumerate(state):
			prod = {}
			string = handle.split(' -> ')
			prod["lhs"] = string[0]
			string = string[1]
			string = string.split(' #lookaheads= ')
			rhs = string[0].split(' ')
			lookaheads = string[1].split(' ')
			if len(rhs) > max_rhs:
				max_rhs = len(rhs)
			if len(lookaheads) > max_lookaheads:
				max_lookaheads = len(lookaheads)
			prod["rhs"] = rhs
			prod["lookaheads"] = lookaheads
			handles[i].insert(j, prod)
	return max_rhs, max_lookaheads

def write_parse_table(legend, parse_table):
	x = len(parse_table)
	y = len(legend)
	table = 'char **parse_table[' + str(x) + '][' + str(y) + '] = {'
	for i, state in enumerate(parse_table):
		size = 0
		table += '\n\t{\n'
		for j, sym in enumerate(state):
			next = '\t'
			next = '"' + sym + '"'
			if j != y - 2:
				next += ','
			next_len = len(next)
			if size + next_len > 60:
				table += '\n\t\t'
				size = 0
			else:
				next += ' '
			table += next
			size += next_len
		table += '}'
		if i != x - 1:
			table += ', '
	table += '\n};'
	with open('src/parser/parse_table.c', 'w+') as f:
		f.write(table)

def create_struct(handle):
	return  handle['lhs'] \
		+ '#' + ','.join(handle['rhs']) \
		+ '#' + ','.join(handle['lookaheads']) + '\n'

def write_prods(handles):
	out = str(len(handles)) + '\n'
	for i, state in enumerate(handles):
		out += '@' + str(i) + ',' + str(len(state)) + '\n'
		out += ''.join([create_struct(struct) for struct in state])
	with open('misc/parse_prods.txt', 'w+') as f:
		f.write(out)


if __name__ == '__main__':
	legend = []
	handles = []
	parse_table = []
	with open("misc/jison_table_lr1.htm") as f:
		soup = BeautifulSoup(f, features="html.parser")
	parse(soup, legend, parse_table, handles)
	# write_parse_table(legend, parse_table)
	# write_prods(handles)