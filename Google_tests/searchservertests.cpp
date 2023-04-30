#include "invertedindex.h"
#include <string>
#include "gtest/gtest.h"
#include "searchserver.h"


std::string GenerateFileName_(const std::string &fileName, int numberFile)
{
    return fileName + std::string(3 - std::to_string(numberFile).size(), '0') +
                      std::to_string(numberFile) + ".txt";
}

void TestSearchServerFunctionality(const std::vector<std::string> &docs,
                                    const std::vector<std::string> &requests,
                                    const std::vector<std::vector<RelativeIndex>> &expected,
                                    int maxRequests)
{
    std::vector<std::vector<RelativeIndex>> result;
    std::vector<std::string> fileNames({});
    InvertedIndex idx;

    std::fstream file;
    // создадим файлы для проверки
    int numberFile = 1;
    for (const auto &oneDoc:docs)
    {
        std::string fileName = GenerateFileName_("testfile", numberFile);
        fileNames.push_back(fileName);
        file.open(fileName, std::ios::out);
        file << oneDoc;
        file.close();
        numberFile++;
    }
    idx.UpdateDocumentBase(fileNames);

    SearchServer srv(idx);
    srv.SetMaxRequests(maxRequests);
    result = srv.Search(requests);

    ASSERT_EQ(result, expected);
};
TEST(TestCaseSearchServer, TestSimple) {
const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
};
const std::vector<std::string> request = {"milk water", "sugar"};
const std::vector<std::vector<RelativeIndex>> expected = {
        {
                {2, 1},
                {0, 0.7},
                {1, 0.3}
        },
        {
        }
};
    TestSearchServerFunctionality(docs, request, expected, docs.size());
}

TEST(TestCaseSearchServer, TestTop5) {
const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland"
};

const std::vector<std::string> request = {"moscow is the capital of russia"};
const std::vector<std::vector<RelativeIndex>> expected = {
        {
                {7, 1},
                {14, 1},
                {0, 0.667},
                {1, 0.667},
                {2, 0.667}

        }
};
    TestSearchServerFunctionality(docs, request, expected, 5);
}
