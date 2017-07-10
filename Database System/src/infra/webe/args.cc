#include "args.hh"

void construct_arg_desc(argdesc_vt& x) 
{
	typedef argdesc_t<Args, char>        carg_t;
	// typedef argdesc_t<Args, int>         iarg_t;
	typedef argdesc_t<Args, uint>        uarg_t;
	// typedef argdesc_t<Args, float>       farg_t;
	// typedef argdesc_t<Args, double>      darg_t;
	typedef argdesc_t<Args, bool>        barg_t;
	typedef argdesc_t<Args, std::string> sarg_t;

	x.push_back( new barg_t("--help", false, &Args::help, "print this message" ));
	x.push_back( new barg_t("--nsm", false, &Args::nsm, "use the n-ary storage model"));
	x.push_back( new barg_t("--pax", false, &Args::pax, "use the pax storage model"));
	x.push_back( new uarg_t("--core", 0, &Args::core, "core number to run the execution on"));
	x.push_back( new uarg_t("--pagesize", 16384, &Args::pageSize, "memory page size to use (e.g., 16384, 8192, 4096, ...)"));
	x.push_back( new uarg_t("--alignment", 4096, &Args::alignment, "memory page alignment to use. must be a multiple of sizeof(void*), that is also a power of two"));
	x.push_back( new uarg_t("--chunksize", 1000, &Args::chunkSize, "the number of memory pages in each chunk"));
	x.push_back( new barg_t("--tpch", false, &Args::tpch, "use the tpc-h dataset and benchmark *benchmark needs to be implemented*"));
	x.push_back( new barg_t("--intrelation", false, &Args::intRelation, "use the 'big integer relation' dataset and benchmark *benchmark needs to be implemented*"));
	x.push_back( new sarg_t("--sf", "1.0", &Args::sf, "used scale factor (sf) for the tpc-h dataset (currently only the following sf's are supported: 0.001 and 1.0)"));
	x.push_back( new barg_t("--all", false, &Args::all, "conduct a complete DBS test (bulk-load, update, queries)"));
	x.push_back( new barg_t("--bulkload", false, &Args::bl, "conduct a bulk-load test"));
	x.push_back( new barg_t("--measure", false, &Args::measure, "sets the flag to measure all conducted tests"));
	x.push_back( new barg_t("--print", false, &Args::print, "sets the flag to print all test results into a file"));
	x.push_back( new barg_t("--plot", false, &Args::plot, "sets the flag to start the plotting process"));
	x.push_back( new sarg_t("--path", "../data/", &Args::path, "path to the location where to store results of tests. relative path from folder 'src' or absolute path possible"));
	x.push_back( new uarg_t("--runs", 1, &Args::runs, "the number how often a test is repeated"));
	x.push_back( new uarg_t("--vectorized", 1, &Args::vectorized, "use vectorized processing and assigns a vector size"));
	x.push_back( new carg_t("--del", '\0', &Args::delimiter, "string delimiter used in the input file. char '\\0' (NUL) is the default delimiter and indicates that no delimiter is used"));
	x.push_back( new carg_t("--sep", '|', &Args::seperator, "attribute/column seperator used in the input file (e.g, '|', ';', ',',...)"));
	x.push_back( new uarg_t("--buffersize", 1000, &Args::bufferSize, "the number of tuples per chunk in the bulk load buffer"));
}

Args::Args() : 	
	_help(false), 
	_nsm(false), 
	_pax(false),
	_tpch(false),
	_int_relation(false),
	_all(false),
	_bulkload(false),
	_measure(false),
	_print(false),
	_plot(false),
	_core(0),
	_pageSize(16384),
	_alignment(4096),
	_chunkSize(1000),
	_bufferSize(1000),
	_runs(1),
	_vectorized(1),
	_delimiter(0),
	_seperator(124),
	_sf("1.0"),
	_path("../data/")
{}


