#include "input_output_lists.hpp"
#include <string>
#include <iostream>

void namestnikov::inputLists(std::istream & in, ForwardList<pair_t> & dataList)
{
  std::string listParameters = "";
  in >> listParameters;
  while (in)
  {
    dataList.push_front({listParameters, ForwardList<unsigned long long>()});
    while (in >> listParameters && (std::isdigit(listParameters[0])))
    {
      dataList.front().second.push_front(std::stoull(listParameters));
    }
  }
}


void namestnikov::outputLists(std::ostream & out, ForwardList<pair_t> & dataList)
{
  if (dataList.empty())
  {
    out << "0";
  }
  else
  {
    dataList.reverse();
    ForwardIterator<pair_t> fwdIt = dataList.begin();
    for (; fwdIt != dataList.end(); ++fwdIt)
    {
      fwdIt != dataList.begin() ? out << " " << fwdIt->first : out << fwdIt->first;
    }
    //output numbers + output sums;
  }
}
