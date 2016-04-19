import os, sys, argparse
import rdflib
from rdflib import URIRef, Graph, Namespace
from rdflib.plugins.parsers.notation3 import N3Parser

def file_err(arg):
	if not os.path.isfile(arg):
		sys.stderr.write('Invalid path: {}\nUsing stdin instead\n'.format(arg))
		return True
	return False
	
def enc():
	return s.encode('utf-8')

def get_parser():
	p = argparse.ArgumentParser(description='N3Parser that splits triples with line breaks')
	p.add_argument('-i', '--ifile', nargs=1, help='specify input file')
	p.add_argument('-o', '--ofile', nargs=1, help='specify output file')
	return p

def main():
	parser = get_parser()
	args = parser.parse_args()
	ifile = open(args.ifile, 'r') if not file_err(args.ifile) else sys.stdin
	ofile = open(args.ofile, 'w') if args.ofile else sys.stdout
	
	g = Graph()
	result = g.parse(file=ifile, format='n3')

	for s, o, p in result:
		ofile.write('{}\n{}\n{}\n'.format(enc(s), enc(o), enc(p)))


if __name__ == '__main__':
	main()