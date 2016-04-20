# Dictionary encoding 
#   by Adam Stallard and Aranya Ajith#
#
#  COP5725 Advanced Databases
#  Spring 2016
#
#  Simple utility to get links from a file and sort them by size
#  This is used so we can test files from small -> large
#  This will call our program too
#
##########################################################################

import urllib, os, sys



def main():
	links = [(l, int(get_size(l))) for l in open('data_links.txt', 'r')]
	links.sort(key=lambda x: x[1])
	for i, (l,s) in enumerate(links):
		run_test(i, s, get_test(i, (l,s)))
		break
		
	
def get_size(link):
	site = urllib.urlopen(link)
	meta = site.info()
	return meta.getheaders("Content-Length")[0]

def get_test(i, (l,s)):
	clean_l = '.'.join(((l.split('/')[-1]).split('.')[:-1])) + '.bz2'
	dest = 'tests/{}'.format(clean_l)
	urllib.URLopener().retrieve(l, dest)
	os.system('bzip2 -d ' + dest)
	return dest[:-4]
	
def run_test(i, s, t):
	print 'Test {:02d} : Size {}\n{}\n'.format(i, s, t)
	btd = './main -i {} -o {}.btd'.format(t, t[:-3])
	hdt = '../hdt-lib-rc1-src/tools/rdf2hdt -f n3 -i {} -o {}.hdt'.format(t, t[:-3])
	print 'Hit enter to run:\n\t{}\n\t{}\n'.format(btd, hdt)
	sys.stdin.readline()
	os.system(btd)
	os.system(hdt)
	
	

#
# Call this module as a main program
if __name__ == '__main__':
	# sys.stdout = open('results.txt', 'w')
	main()