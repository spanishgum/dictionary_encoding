# Dictionary encoding 
#   by Adam Stallard and Aranya Ajith#
#
#  COP5725 Advanced Databases
#  Spring 2016
#
#  Simple utility to get links from a file and sort them by size
#  This is used so we can test files from small -> large
#
##########################################################################

#
# standard libs for typical io / file handling
import os, sys, argparse

#
# the core library utilities which allow us to
#  actually parse the .nt files!
# Python was chosen necause of its simplicity here
# The C++ raptor library was challenging to build 
#  so we chose to create a middle-man process 
#  which creates 'intermediate' files during the 
#  compression process
import rdflib
from rdflib import URIRef, Graph, Namespace
from rdflib.plugins.parsers.notation3 import N3Parser

#
# Simple safety function to check if an argument
#  is an existing file. Prints an error msg if not
def file_err(arg):
	if not arg:
		return True
	if not os.path.isfile(arg):
		sys.stderr.write('Invalid path: {}\nUsing stdin instead\n'.format(arg))
		return True
	return False
	
#
# Simple utility that encodes a string
# This is needed before we write strings out to a file
#  otherwise AttributeErrors will be thrown from non
#  ascii characters
def enc(s):
	return s.encode('utf-8')

#
# Our command line argument controller
# This provides some flexibility in how our other programs
#  might call this script. It is also useful for quick debugging
def get_parser():
	p = argparse.ArgumentParser(description='N3Parser that splits triples with line breaks')
	p.add_argument('-i', '--ifile', help='specify input file')
	p.add_argument('-o', '--ofile', help='specify output file')
	return p

#
# Get the parsed cmds, decide if io should be file based
#  or redirected, parse the input into a rdflib graph
#  using the N3 (Ntriple) notation, and then,
#  iterate over the triples and print them onto individual
#  lines for our C++ programs
def main():

	parser = get_parser()
	args = parser.parse_args()

	if not file_err(args.ifile):
		ifile = open(args.ifile, 'r')
	else:
		ifile = sys.stdin

	if not args.ofile:
		ofile = sys.stdout
	else:
		ofile = open(args.ofile, 'w')

	g = Graph()
	result = g.parse(file=ifile, format='n3')

	for s, o, p in result:
		ofile.write('{}\n{}\n{}\n'.format(enc(s), enc(o), enc(p)))


#
# Call this module as a main program
if __name__ == '__main__':
	main()