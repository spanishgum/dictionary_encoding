/*
*
*    Dictionary Encoding and the RDF Storage Model
*       by Adam Stallard , Aranya Ajith
*          
*    COP5725 Advanced Databases
*       Spring 2016
*
*    "trie.h"
*
*     This source file
*
*
*
*
*
*
*
*/

#include "rdf2btd.h"

#define TEMP_EXT ".parsed"
#define PYTHON "/usr/bin/python"
#define PY_N3_PARSER "parse_triples.py"

// Initializes contextual information structures needed
//  to transform a .nt file into a .btd file
// If the provided input path does not exist,
//  then exit
rdf2btd::rdf2btd(std::string i, std::string o, std::string t)
	: ifile(i), ofile(o), tfile(t), 
		n3parser(PY_N3_PARSER), ctriples()
{
	this->dict = new Dict(this->ofile);
	if (!this->dict->fileExists(this->ifile))
	{
		std::cerr << "Fatal Error: Could not open input file.\n";
		exit(-1);
	}
}

// Makes a safety check that dict didnt die
//  but otherwise clears it and deletes it
rdf2btd::~rdf2btd()
{
	if (this->dict != nullptr)
	{
		this->dict->clear();
		delete this->dict;
	}
}

// This function simply calls the core functions of rdf2btd
//  It initiates the transformation, and compression, and 
//  the output
void rdf2btd::run()
{
	// begin by calling the subprocess transform
	//  which will create out tmp file for 
	//  character elimination and easier parsing
	std::cout << "Transforming input\n";
	this->transform(this->ifile, this->tfile);
	
	// With the tmp file created, initiate
	//  the compression stage
	std::cout << "Compressing transformation\n";
	this->compress(this->tfile);
	
	// After compression occurs and the dictionary is loaded
	//  we export to a binary file through serialization
	std::cout << "Writing compressed file\n";
	this->save(this->ofile);
}


// Simple utility which is called when a serious problem
//  occurs. The err message will be displayed, and the 
//  exit code will be set
void rdf2btd::fatal_err(std::string msg, int code)
{
	std::cerr << msg << std::endl;
	exit(code);
}


// Simple utility to verify the file stream is still good
// If it is not, then it wil print the argument err msg
void rdf2btd::verify_file(std::ifstream &ifs, std::string msg)
{
	if (!ifs)
	{
		fatal_err(msg, 2);
	}
}


// This function will transform an original '.nt' file
//  (in N3 format) into an intermediate file
// The intermediate file will essentially split S,P,O
//  onto separate lines. Also, the extraneous char marks
//  such as '<', '>' are removed for space efficiency
//  run 'python parse_triples -i ifile -o ofile'
void rdf2btd::transform(std::string _ifile, std::string _tfile)
{
	pid_t pid;
	int status;
	char *cmd[7];
	
	// allocate and print python command line info
	asprintf(&cmd[0], PYTHON);
	asprintf(&cmd[1], PY_N3_PARSER);
	asprintf(&cmd[2], "-i");
	asprintf(&cmd[3], _ifile.c_str());
	asprintf(&cmd[4], "-o");
	asprintf(&cmd[5], _tfile.c_str());
	cmd[6] = NULL;
	std::cout << "Allocated memory\n";
	
	// begin a child process to run the python script
	//  make sure it ran properly or exit
	switch (pid = fork())
	{
		case -1:
			std::cerr << "Fork failure\n";
			exit(1);
		case 0:
			execv(PYTHON, cmd);
			exit(255);
		default:
			if (waitpid(pid, &status, 0) < 0)
			{
				std::cerr << "Child status failure\n";
				exit(254);
			}
			if (WIFEXITED(status))
			{
				if (WEXITSTATUS(status) != 0) 
				{
					std::cerr << "Child " << pid << " returned " 
						<< WEXITSTATUS(status) << "\n";				
					exit(WEXITSTATUS(status));
				}
			}
			if (WIFSIGNALED(status))
			{
				std::cerr << "Child " << pid << " killed: signal "
					<< WTERMSIG(status) << (WCOREDUMP(status) ?
					" - core dumped" : "");
				exit(1);
			}
			break;
	}
	
	// free our command line buffers
	for (int i = 0; i < 6; ++i) {
		free(cmd[i]);
	}
}

// This function will read in the intermediate file
//  that was transformed from the original through our
//  our python script
// It will finish by removing the intermediate file,
//  and storing our compressed triples in a container,
//  and writing 
void rdf2btd::compress(std::string _tfile)
{
	std::ifstream ifs;
	std::string line_triplet[3];
	CompressedTriple next_cTriple;
	TripleCompressor compressor(*this);
	
	// init the dictionary object
	this->dict = new Dict(this->ofile);
	
	// open and verify the intermediate file before streaming
	ifs.open(_tfile, std::ios::in);
	verify_file(ifs, "Fatal Error: Intermediate file lost.");
	while (ifs)
	{
		// loop over the line separated S,P,O values
		//  insert each one into the dictionary and get the mapped value
		for (int i = Subject; i != End; ++i)
		{
			// double check file and get line
			verify_file(ifs, "Fatal Error: Intermediate file corruption.");
			std::getline(ifs, line_triplet[i]);
			next_cTriple.get(static_cast<tri_idx>(i)) = compressor(line_triplet[i]);
		}
	}
	ifs.close();
	
	// show triple count as a statistic
	std::cout << "Found " << this->ctriples.size() << " triples in " 
		<< this->ifile << std::endl;
}

// Writes uint values to an output file stream in binary format
//  This supports compression
template <typename T>
void rdf2btd::writeData(std::ofstream& ofs, T data) 
{
	ofs.write(reinterpret_cast<const char *>(&data), sizeof(data));
}

// Reads uint values to an input file stream in binary format
template <typename T>
void rdf2btd::readData(std::ifstream& ifs, T *data) 
{
	ifs.read(reinterpret_cast<char *>(data), sizeof(*data));
}

// Begin writing the compressed triple set to the output file
// This will consist of writing the following:
//   triple count
//   triples
//   serialized dictionary
void rdf2btd::save(std::string _ofile)
{
	// open an output stream in binary mode
	std::ofstream ofs;
	ofs.open(_ofile, std::ios::out | std::ios::binary);
	this->dict->serialize(_ofile);
	ofs.close();
	
	return; ////////// FOR SOME REASON THIS BROKE :(
	
	// first write the number of triples in the header
	unsigned int num_triples = this->ctriples.size();
	this->writeData<unsigned int>(ofs, num_triples);
		
	// begin to iterate over triples
	std::vector<CompressedTriple>::iterator ctI;
	for (ctI = this->ctriples.begin(); ctI != this->ctriples.end(); ++ctI)
	{
		// iterate over S,P,O
		// write unsigned int value to binary output
		for (int i = Subject; i != End; ++i)
		{
			this->writeData(ofs, ctI->get(static_cast<tri_idx>(i)));
		}
	}
	
	// finally, serialize the dictionary
	this->dict->serialize(ofs);
	ofs.close();
}

// Take in a file argument and attempt to load it in
// This will consist of reading the following:
//   triple count
//   triples
//   serialized dictionary
void rdf2btd::load(std::string _ifile)
{
	// open an input stream in binary mode
	std::ifstream ifs;
	ifs.open(_ifile, std::ios::in | std::ios::binary);
	
	// first read the number of triples in the header
	unsigned int num_triples;
	this->readData<unsigned int>(ifs, &num_triples);
		
	// begin to iterate over triples
	for (unsigned int i = 0; i < num_triples; ++i)
	{
		CompressedTriple ctriple;
		
		// iterate over compressed S,P,O
		// read unsigned int values from binary input
		for (int i = 0; i < 3; ++i)
		{
			// Just make sure our input stream hasnt gone whacky
			//  before reading more bytes
			verify_file(ifs, "Fatal Error: Load file corruption.");
			this->readData(ifs, ctriple.getptr(static_cast<tri_idx>(i)));
		}
		
		// push the loaded triple to the triple vector
		this->ctriples.push_back(ctriple);
	}
	
	// finally, serialize the dictionary
	this->dict->deserialize(ifs);
	ifs.close();
}


	// dict -> .btd
	// statistics
	// compare(.nt, .btd+.sop)
	
