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

import urllib, os, sys, time


def main():
	links = [(l, int(get_size(l))) for l in open('data_links.txt', 'r')]
	links.sort(key=lambda x: x[1])
	for i, (l,s) in enumerate(links):
		print '\n\n{}\n\tthis data set has a size of {}\n\tWould you like to download it? [y/n] <- '.format(l, s)
		response = sys.stdin.readline()
		if 'y' in [c.lower() for c in response]:
			dest = get_test(i, (l,s))
			print '\n\tWould you like to run it? [y/n] <- '
			response = sys.stdin.readline()
			if 'y' in [c.lower() for c in response]:
				run_test(i, s, dest)
		
	
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
	
def run_test(i, s, nt):
	
	# Print a synopsis of the current test
	synopsis = '\n  # {:02d} : Size {:>12} KB : {:>40}\n'.format(i, s, nt)
	
	# Get names of test files
	btd_file = nt + '.btd'
	hdt_file = nt + '.hdt'
	
	# Generate the cmd lines for our program btd and hdt
	btd = './main -i {} -o {}'.format(nt, btd_file)
	hdt = '../hdt-lib-rc1-src/tools/rdf2hdt {} {}'.format(nt, hdt_file)
	
	# Show the prepared cmds
	sys.stderr.write('\n\nPreparing to launch:\n\t{}\n\t{}\n\n'.format(btd, hdt))
	
	# Run the commands
	t1 = time.time()
	os.system(btd)
	t2 = time.time()
	os.system(hdt)
	t3 = time.time()
	
	# Record run times
	btd_time = (t2 - t1)
	hdt_time = (t3 - t2)
	
	# get files sizes
	nt_size = os.path.getsize(nt)
	btd_size = os.path.getsize(btd_file)
	hdt_size = os.path.getsize(hdt_file)
	
	# calculate compression ratios
	btd_comp_r = (float(btd_size) / nt_size)
	hdt_comp_r = (float(hdt_size) / nt_size)

	# Format results into a nice string
	border = '--------------------------------------------------------------------------\n'
	fmt_str = '  BTD  : {:<5} : {:05.1f} : {:10d} : {:4.3f}\n'
	btd_res = fmt_str.format(btd_file, btd_time, btd_size, btd_comp_r)
	hdt_res = fmt_str.format(hdt_file, hdt_time, hdt_size, hdt_comp_r)
	
	# Record results
	print synopsis, border, btd_res, hdt_res
	

#
# Call this module as a main program
if __name__ == '__main__':
	# sys.stdout = open('results.out', 'w')
	main()