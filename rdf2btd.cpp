#include "rdf2btd.h"

#define TEMP_EXT ".parsed"
#define PYTHON "/usr/bin/python"
#define PY_N3_PARSER "parse_triples.py"

rdf2btd::rdf2btd(std::string i, std::string o)
{
	this->ifile = i;
	this->tfile = i + TEMP_EXT;
	this->ofile = o;
	this->n3parser = PY_N3_PARSER;
	this->dict = new Dict();
	std::cout << "Constructed rdf2btd object\n";
}

rdf2btd::~rdf2btd()
{
	std::cout << "Clearing dict\n";
	this->dict->clear();
	std::cout << "Deconstructing dict\n";
	delete this->dict;
	std::cout << "Deconstructing rdf2btd\n";
}


// goal:
//  run 'python parse_triples -i ifile -o ofile'
int rdf2btd::transform()
{
	pid_t pid;
	int status;
	char *cmd[7];
	
	asprintf(&cmd[0], PYTHON);
	asprintf(&cmd[1], PY_N3_PARSER);
	asprintf(&cmd[2], "-i");
	asprintf(&cmd[3], this->ifile.c_str());
	asprintf(&cmd[4], "-o");
	asprintf(&cmd[5], this->ofile.c_str());
	cmd[6] = NULL;
	std::cout << "Allocated memory\n";
	
	switch (pid = fork())
	{
		case -1:
			std::cerr << "Fork failure\n";
			return -1;
		case 0:
			std::cout << "Child proc attempting execv\n";
			execv(PYTHON, cmd);
			std::cout << "Child proc failure execv\n";
			exit(255);
			return 0;
		default:
			std::cout << "Parent proc waiting...\n";
			if (waitpid(pid, &status, 0) < 0)
			{
				std::cerr << "Child status failure\n";
				exit(254);
			}
			if (WIFEXITED(status))
			{
				std::cout << "Child " << pid << " returned " 
					<< WEXITSTATUS(status) << "\n";
				exit(WEXITSTATUS(status));
			}
			if (WIFSIGNALED(status))
			{
				std::cout << "Child " << pid << " killed: signal "
					<< WTERMSIG(status) << (WCOREDUMP(status) ?
					" - core dumped" : "");
				exit(1);
			}
			std::cout << "Child ran fine!\n";
			break;
	}
	
	
	std::cout << "Attempting to print\n";
	for (int i = 0; i < 6; ++i) {
		std::cout << cmd[i] << " ";
		free(cmd[i]);
	}
	std::cout << "\n";
	
	std::cout << "free'd\n";
	return 0;
}