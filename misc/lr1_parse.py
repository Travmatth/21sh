from bs4 import BeautifulSoup, NavigableString, Tag

nbsp = u'\xa0'

def parse_state(col, i, handles):
	arr = []
	for br in col.findAll('br'):
		next_s = br.nextSibling
		if not (next_s and isinstance(next_s, NavigableString)):
			continue
		next2_s = next_s.nextSibling
		if next2_s and isinstance(next2_s, Tag) and next2_s.name == 'br':
			text = str(next_s).strip()
			if (text):
				arr.append(text)
	handles.insert(i, arr)

def parse(soup, legend, parse_table, handles):
	for row in soup.table.thead.tr:
		legend.append(row.get_text())
	for i, row in enumerate(soup.table.tbody):
		parse_table.insert(i, [])
		for j, col in enumerate(row):
			if j == 0:
				parse_state(col, i, handles)
			elif col.text == nbsp:
				parse_table[i].insert(j - 1, '-')
			else:
				parse_table[i].insert(j - 1, col.contents[0].text)

def format_handles(raw_handles, handles):
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

def enumerate_dict(arr):
	return '"' + '", "'.join(arr) + '"'

def create_struct(handle):
	return '{ "'
			+ handle['lhs'] + '", '
			+ enumerate_dict(handle['rhs']) + ', '
			+ enumerate_dict(handle['lookaheads'])
			+ ' }'

def write_handlers(handles, rhs_size, lookahead_size):
	strings = [[create_struct(struct) for struct in state] for state in handles]


if __name__ == '__main__':
	legend = []
	handles = []
	raw_handles = []
	parse_table = []
	with open("misc/jison_table_lr1.htm") as f:
		soup = BeautifulSoup(f, features="html.parser")
	parse(soup, legend, parse_table, raw_handles)
	rhs_size, lookahead_size = format_handles(raw_handles, handles)
	# write_parse_table(legend, parse_table)
	write_handlers(handles, rhs_size, lookahead_size)