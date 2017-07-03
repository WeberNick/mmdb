#ifndef INFRA_MOER_SIMPLE_DICTIONARY_HH
#define INFRA_MOER_SIMPLE_DICTIONARY_HH


#include <map>
#include <vector>

/*
 *   SimpleDictionary
 *
 *     maps
 *  T --> uint
 *      und
 *  uint --> T
 */

template<class T>
class SimpleDictionary {
  public:
    typedef unsigned int uint;
    typedef std::vector<T>    idx2t_t;
    typedef std::map<T, uint> t2idx_t;
  public:
    SimpleDictionary();
  public:
    void     insert(const T& x); 
    uint     get_idx(const T& x); // if necessary: INSERTS !!!
    int      lookup(const T& x) const; // returns < 0 if not found;
    bool     contains(const T& x) const;
    const T& get_content(uint x) const { return _idx2t[x]; }
  public:
    uint size() const { return _t2idx.size(); } // egal
  public:
    std::ostream& print(std::ostream& os) const;
  private:
    idx2t_t _idx2t;
    t2idx_t _t2idx;
};


template<class T>
SimpleDictionary<T>::SimpleDictionary() : _idx2t(), _t2idx() {
}


template<class T>
typename SimpleDictionary<T>::uint
SimpleDictionary<T>::get_idx(const T& x) {
  typename t2idx_t::const_iterator iter = _t2idx.find(x);
  if(_t2idx.end() == iter) {
    const uint lNewIdx = _idx2t.size();
    _idx2t.push_back(x);
    _t2idx.insert(std::pair<T,uint>(x,lNewIdx));
    return lNewIdx;
  }
  return (*iter).second;
}

template<class T>
void
SimpleDictionary<T>::insert(const T& x) {
  typename t2idx_t::const_iterator iter = _t2idx.find(x);
  if(_t2idx.end() == iter) {
    const uint lNewIdx = _idx2t.size();
    _idx2t.push_back(x);
    _t2idx.insert(std::pair<T,uint>(x,lNewIdx));
  }
}

template<class T>
int
SimpleDictionary<T>::lookup(const T& x) const {
  typename t2idx_t::const_iterator iter = _t2idx.find(x);
  if(_t2idx.end() == iter) {
    return -1;
  }
  return (*iter).second;
}

template<class T>
bool
SimpleDictionary<T>::contains(const T& x) const {
  typename t2idx_t::const_iterator iter = _t2idx.find(x);
  return (_t2idx.end() != iter);
}

template<class T>
std::ostream& 
SimpleDictionary<T>::print(std::ostream& os) const {
  for(uint i = 0; i < _idx2t.size(); ++i) {
    os << i << ' '
       << _idx2t[i] << std::endl;
  }
  return os;
}

#endif
