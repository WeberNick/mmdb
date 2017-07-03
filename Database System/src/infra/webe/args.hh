#ifndef INFRA_WEBE_ARGS_HH
#define INFRA_WEBE_ARGS_HH

#include "infra/moer/argbase.hh"
#include <string>

class Args
{
	public:
		typedef unsigned int uint;
	public:
		Args();

	public:
		inline const bool help(){ return _help; }
		inline void help(const bool& x) { _help = x; }

		inline const bool nsm(){ return _nsm; }
		inline void nsm(const bool& x) { _nsm = x; }

		inline const bool pax(){ return _pax;}
		inline void pax(const bool& x){ _pax = x;}

		inline const bool vectorized(){ return _vectorized;}
		inline void vectorized(const bool& x){ _vectorized = x;}

		inline const uint core(){ return _core;}
		inline void core(const uint& x){ _core = x;}

		inline const uint pageSize(){ return _pageSize;}
		inline void pageSize(const uint& x){ _pageSize = x;}

		inline const uint alignment(){ return _alignment;}
		inline void alignment(const uint& x){ _alignment = x;}

		inline const uint chunkSize(){ return _chunkSize;}
		inline void chunkSize(const uint& x){ _chunkSize = x;}

		inline const bool tpch(){ return _tpch;}
		inline void tpch(const bool& x){ _tpch = x;}

		inline const bool intRelation(){ return _int_relation;}
		inline void intRelation(const bool& x){ _int_relation = x;}

		inline const std::string sf(){ return _sf;}
		inline void sf(const std::string& x){ _sf = x;}

		inline const bool all(){ return _all;}
		inline void all(const bool& x){ _all = x;}

		inline const bool bl(){ return _bulkload;}
		inline void bl(const bool& x){ _bulkload = x;}

		inline const bool measure(){ return _measure;}
		inline void measure(const bool& x){ _measure = x;}

		inline const bool print(){ return _print;}
		inline void print(const bool& x){ _print = x;}

		inline const bool plot(){ return _plot;}
		inline void plot(const bool& x){ _plot = x;}

		inline const std::string path(){ return _path;}
		inline void path(const std::string& x){ _path = x;}

		inline const uint runs(){ return _runs;}
		inline void runs(const uint& x){ _runs = x;}

		inline const uint vectorizedSize(){ return _vectorizedSize;}
		inline void vectorizedSize(const uint& x){ _vectorizedSize = x; vectorized(true);}

		inline const char delimiter(){ return _delimiter; }
		inline void delimiter(const char& x) { _delimiter = x; }

		inline const char seperator(){ return _seperator; }
		inline void seperator(const char& x) { _seperator = x; }

		inline const uint bufferSize(){ return _bufferSize;}
		inline void bufferSize(const uint& x){ _bufferSize = x;}

	private:
		bool _help;
		bool _nsm;
		bool _pax;
		bool _vectorized;
		bool _tpch;
		bool _int_relation;
		bool _all;
		bool _bulkload;
		bool _measure;
		bool _print;
		bool _plot;

		uint _core;
		uint _pageSize;
		uint _alignment;
		uint _chunkSize;		//memory chunk
		uint _bufferSize;	//bulk load buffer chunk
		uint _runs;
		uint _vectorizedSize;

		char _delimiter;
		char _seperator;

		std::string _sf;
		std::string _path;
};

typedef std::vector<argdescbase_t<Args>*> argdesc_vt;

void construct_arg_desc(argdesc_vt& aArgDesc);

#endif