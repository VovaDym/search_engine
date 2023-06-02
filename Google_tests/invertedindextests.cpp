#include "invertedindex.h"
#include <string>
#include "gtest/gtest.h"
#include <fstream>

std::string GenerateFileName(const std::string &fileName, int numberFile)
{
    return fileName + std::string(3 - std::to_string(numberFile).size(), '0') +
                      std::to_string(numberFile) + ".txt";
}

void TestInvertedIndexFunctionality( const std::vector<std::string> &docs,
                                     const std::vector<std::string> &requests,
                                     const std::vector<std::vector<Entry>> &expected)
{
    std::vector<std::vector<Entry>> result;
    std::vector<std::string> fileNames;
    InvertedIndex idx;

    std::fstream file;
// создадим файлы для теста
    int numberFile = 1;
    for (const auto &oneDoc:docs) {
        std::string fileName = GenerateFileName("testfile", numberFile);
        fileNames.push_back(fileName);
        file.open(fileName, std::ios::out);
        file << oneDoc;
        file.close();
        numberFile++;
    }

    idx.UpdateDocumentBase(fileNames);

    for (auto &request : requests)
    {
        std::vector<Entry> wordCount = idx.GetWordCount(request);

        std::sort(wordCount.begin(), wordCount.end(),
                  [](const auto &a, const auto &b)
                  {
                      return a.doc_id < b.doc_id;
                  });
        result.push_back(wordCount);
    }

    ASSERT_EQ(result, expected);
};

TEST(TestCaseInvertedIndex, TestBasic) {
    const std::vector<std::string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::vector<Entry>> expected = {
            {
                    {0, 1}
            }, {
                    {0, 1}, {1, 3}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {
    const std::vector<std::string> docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuccino"
    };
    const std::vector<std::string> requests = {"milk", "water", "cappuchino"};
    const std::vector<std::vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            }, {
                    {0, 3}, {1, 2}, {2, 5}
            },
            {}
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
