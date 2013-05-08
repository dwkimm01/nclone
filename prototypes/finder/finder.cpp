#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/range.hpp>
#include <boost/range/as_literal.hpp>
#include <boost/iterator.hpp>
using namespace std;
using namespace boost;

struct simple_finder
{
    simple_finder(const int skip) : len(skip), n(0) {}

    template<typename ForwardIteratorT>
    boost::iterator_range<ForwardIteratorT> operator()(
        ForwardIteratorT Begin,
        ForwardIteratorT End )
    {
        ForwardIteratorT nb = Begin + n;
        ForwardIteratorT ne = nb + len;
        if(nb > End) nb = End;
        if(ne > End) ne = End;
        n = ((ne - nb) - 1);
        return boost::iterator_range<ForwardIteratorT>(nb, ne);
    }

   int len;
   int n;
};


int main(int argc, char* argv[])
{
   string text = "This is some sample text";

   typedef find_iterator<string::iterator> string_find_iterator;
   for(string_find_iterator It = make_find_iterator(text, simple_finder(4));
      It != string_find_iterator();
      ++It)
   {
       cout << copy_range<std::string>(*It) << endl;
   }

   return 0;
}
