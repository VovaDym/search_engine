#include "searchserver.h"

SearchServer::SearchServer(InvertedIndex &idx) : mIndex(idx) {};

std::vector<std::vector<RelativeIndex>> SearchServer::Search(const std::vector<std::string> &queriesInput)
{
    std::vector<std::vector<RelativeIndex>> resultAllRequests;
    for (const auto &query: queriesInput)
    {
        std::map<size_t, size_t> docIdFreqWord; //docId, count words query
        std::vector<RelativeIndex> resultOneRequest;
        for (const auto &queryWord: mIndex.SplitIntoWords(query))
        {
            for (auto &entry:mIndex.GetWordCount(queryWord))
            {
                docIdFreqWord[entry.doc_id] += entry.count;
            }
        }
        if (docIdFreqWord.empty())
        {
            resultOneRequest = std::vector<RelativeIndex>();
            resultAllRequests.push_back(resultOneRequest);
        }
        else
        {
            std::multimap<size_t, size_t, std::greater<size_t>> sortResult;
            for (const auto &[docID, rank]:docIdFreqWord)
            {
                sortResult.insert({rank, docID});
                if (sortResult.size() > mMaxRequests)
                {
                    sortResult.erase(std::prev(sortResult.end()));
                }
            }
            size_t maxRank = sortResult.begin()->first;
            for (const auto &[rank, docId]:sortResult)
            {
                resultOneRequest.push_back(RelativeIndex(docId, std::round(static_cast<double>(rank) / maxRank * 1000) / 1000));
            }
            resultAllRequests.push_back(resultOneRequest);
        }

    }
    return resultAllRequests;
};

void SearchServer::SetMaxRequests(int maxRequests)
{
    if (maxRequests <= 0) {
        std::cerr << "WARNING! Max request must be greater 0" << std::endl;
        std::cout << "Max request set 5" << std::endl;
        SearchServer::mMaxRequests = 5;
    } else {
        SearchServer::mMaxRequests = maxRequests;
    }
}



