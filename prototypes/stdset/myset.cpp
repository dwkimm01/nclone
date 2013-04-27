#include <iostream>
#include <map>
#include <string>
#include <set>
#include <vector>
using namespace std;

// From: http://en.wikibooks.org/wiki/Algorithm_Implementation/Strings/Levenshtein_distance#C.2B.2B 
template <class T> unsigned int edit_distance(const T& s1, const T& s2)
{
    const size_t len1 = s1.size(), len2 = s2.size();
    vector<vector<unsigned int> > d(len1 + 1, vector<unsigned int>(len2 + 1));
 
    d[0][0] = 0;
    for(unsigned int i = 1; i <= len1; ++i) d[i][0] = i;
    for(unsigned int i = 1; i <= len2; ++i) d[0][i] = i;
 
    for(unsigned int i = 1; i <= len1; ++i)
        for(unsigned int j = 1; j <= len2; ++j)
                      d[i][j] = std::min( std::min(d[i - 1][j] + 1,d[i][j - 1] + 1),
                          d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1) );
    return d[len1][len2];
}


unsigned int operator-(const std::string &s1, const std::string &s2)
{
   return edit_distance(s1, s2);
}

/*struct STimestampedData
{
    std::string m_timestamp;
    std::string m_data;

    bool operator<(const STimestampedData& rhs) const
    {
        return m_timestamp < rhs.m_timestamp;
    }
};*/


/*struct TimestampCompare
{
    bool operator()(const STimestampedData & left, const STimestampedData & right) const
    {
        return left.m_timestamp < right.m_timestamp;
    }
};*/

// typedef std::set<STimestampedData,TimestampCompare> TimestampedDataSet;

//TimestampedDataSet::iterator FindClosest(TimestampedDataSet & data, STimestampedData & searchkey)
template <typename T, typename U>
typename T::iterator FindClosest(T &data, U &searchkey) 
{
    auto upper = data.lower_bound(searchkey);
    if (upper == data.begin() || (*upper) == searchkey)
        return upper;
    auto lower = upper;
    --lower;
    if (upper == data.end() ||
        (searchkey - (*lower)) < ((*upper) - searchkey))
        return lower;
    return upper;
}


// Needs clang++ for emplace
int main(int argc, char* argv[])
{
    cout << "Closest..." << endl;

    set<string> s;
    s.emplace("jump");
    s.emplace("history");
    s.emplace("list");
    s.emplace("newconn");


    auto itr = FindClosest(s, "ne");
//    set<STimestampedData> s;
//    TimestampedDataSet s;
//    STimestampedData d;

/*
    d.m_timestamp = "10";
    d.m_data = "Ten";
    s.emplace(d);

    d.m_timestamp = "20";
    d.m_data = "Twenty";
    s.emplace(d);

    d.m_timestamp = "20";
    d.m_data = "Eighteen";
    auto itr = FindClosest(s, d);
*/

/*    d.m_timestamp = "jump"; d.m_data = 1; s.emplace(d);
    d.m_timestamp = "history"; d.m_data = 2; s.emplace(d);
    d.m_timestamp = "save"; d.m_data = 3; s.emplace(d);

    d.m_timestamp = "hi"; d.m_data = 4;
    auto itr = FindClosest(s, d);
*/

    cout << "s.size() = " << s.size() << endl;
    if(s.end() != itr)
    {
       cout << "Closest " << (*itr) << endl;
    }

    return 0;
}


