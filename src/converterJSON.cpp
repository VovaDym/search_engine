#include "converterJSON.h"
#include <fstream>

void ConverterJSON::ReadJsonFile(const std::string &fileName)
{
    std::fstream mFile;

    mFile.open(fileName, std::ios::in);
    if (!mFile.is_open())
    {
        throw ExceptionError("Unable to read file " + fileName);
    }
    else
    {
        try {
            if(fileName == mRequestsFileName)
            {
                mFile >> requestJSON;
                mFile.close();
            }
            else if (fileName == mConfigFileName)
            {
                mFile >> configJSON;
                mFile.close();
            }
        }
        catch (...)
        {
            throw ExceptionError("File " + fileName + " corrupted");
        }
    }
}

std::vector<std::string> ConverterJSON::GetRequests()
{
    ReadJsonFile(mRequestsFileName);
    try {
        if (requestJSON["requests"].empty() || requestJSON.empty())
        {
            std::cout << "WARNING!!: Request file empty" << std::endl;
            return std::vector<std::string>();
        }
        auto endRequestsList = requestJSON["requests"].end();
        if (requestJSON["requests"].size() > MAX_REQUESTS)
        {
            std::cout << "WARNING!!: To many requests" << std::endl;
            endRequestsList = requestJSON["requests"].begin() + MAX_REQUESTS;
        }
        return std::vector<std::string>(requestJSON["requests"].begin(), endRequestsList);
    }
    catch (...)
    {
        throw ExceptionError("Error config in " + mConfigFileName + " file.");
    }
};

std::string ConverterJSON::ReadDocument(const std::string &fileName) {
    std::string currWord, currTextDocument;
    int counterWords = 0;
    int counterChar = 0;
    bool isTyping = false;
    std::fstream file;
    file.open(fileName, std::ios::in);
    if (!file.is_open()) {
        std::cout << "Unable to open file " << fileName << std::endl;
    } else {

        file.seekg(0, std::ios::end);
        std::streampos lengthFile = file.tellg();
        file.seekg(0, std::ios::beg);

        std::vector<char> fileBuff(lengthFile);
        file.read(&fileBuff[0], lengthFile);
        file.close();

        for (auto ptrc = fileBuff.begin(); ptrc != fileBuff.end(); ++ptrc) {
            if ((*ptrc >= 'A' && *ptrc <= 'Z') || (*ptrc >= 'a' && *ptrc <= 'z')) {
                if (++counterChar <= MAX_CHAR_IN_WORD) {
                    currTextDocument += tolower(*ptrc);
                } else if (!isTyping) {
                    std::cerr << "WARNING: count of character  in word " << fileName << " greater then "
                              << MAX_CHAR_IN_WORD << std::endl;
                    isTyping = true;
                }
            }

            if (counterWords > MAX_WORDS_IN_FILE) {
                std::cerr << "WARNING: count of word in document " << fileName << " greater then "
                          << MAX_WORDS_IN_FILE << std::endl;
                break;
            }

            if (*ptrc == ' ') {
                if ((ptrc + 1) != fileBuff.end() && *(ptrc + 1) != ' ') {
                    currTextDocument += ' ';
                    counterWords++;
                    counterChar = 0;
                    isTyping = false;
                }
            }
        }
    }
    return currTextDocument;
}

//std::vector<std::string> ConverterJSON:: GetTextDocuments()
//{
//
//    std::vector <std::string> fileNamesDoc;
//    std::vector <std::string> textDocuments;
//    ReadJsonFile(mConfigFileName);
//
//    if(configJSON["config"]["version"] != VERSION_APP)
//    {
//        configJSON.clear();
//    }
//
//    if (configJSON["files"].empty() || configJSON.empty())
//    {
//        std::cout << "WARNING!!: Config file empty" << std::endl;
//        return textDocuments;
//    }
//
//
//    fileNamesDoc = std::vector<std::string>(configJSON["files"].begin(), configJSON["files"].end());
//    for (int i = 0;i <  fileNamesDoc.size(); i++)
//    {
//
//        textDocuments.push_back(ReadDocument(fileNamesDoc[i]));
//
//    }
//
//    return textDocuments;
//}

std::vector<std::string> ConverterJSON::GetFileNames() {
    if(configJSON["config"]["version"] != VERSION_APP)
    {
       configJSON.clear();
    }

    if (configJSON.empty()) {
        std::cout << "WARNING!!: List of file names is empty" << std::endl;
        return std::vector<std::string>();
    }
    try {
        return std::vector<std::string>(configJSON["files"].begin(), configJSON["files"].end());
    } catch (...)
    {
        throw ExceptionError("Error config in " + mConfigFileName + " file.");
    }
}

int ConverterJSON::GetResponsesLimit()
{
    if (!configJSON.empty())
    {
        try
        {
            return configJSON["config"]["max_responses"];
        }
        catch (...)
        {
            throw ExceptionError("Error config in " + mConfigFileName + " file.");
        }
    }
    else
    {
        return 0;
    }
}

void ConverterJSON::WriteJsonFile(const std::string &fileName, nlohmann::json &jsonToWrite)
{
    std::fstream mFile;
    const int SPACES_SEPARATOR = 4;
    mFile.open(mAnswerFileName, std::ios::out);
    if (!mFile.is_open())
    {
        throw ExceptionError("Unable to create file answers.json");
    }
    else
    {
        try
        {
            mFile << jsonToWrite.dump(SPACES_SEPARATOR);
            mFile.close();
        }
        catch (...)
        {
            throw ExceptionError("Unable to write file answers.json");
        }
    }
}

nlohmann::json ConverterJSON::CreatAnswerJson(const std::vector<std::vector<RelativeIndex>> &answers)
{
    int maxResponses = GetResponsesLimit();
    int numberRequest = 1;
    for (auto &requestAnswer:answers)
    {
        int response = 0;
        nlohmann::json relevance = nlohmann::json::object_t();
        std::string strNumberRequest = "request" + std::string(3 - std::to_string(numberRequest).size(), '0')
                                                 + std::to_string(numberRequest);
        if (requestAnswer.empty())
        {
            answerJSON["answers"][strNumberRequest]["result"] = false;
        }
        else if (!requestAnswer.empty())
        {
            for (auto item2 = requestAnswer.begin();item2 != requestAnswer.end() && response < maxResponses; ++item2, ++response)
            {
                if (item2 == requestAnswer.begin())
                {
                    answerJSON["answers"][strNumberRequest]["result"] = true;
                    answerJSON["answers"][strNumberRequest]["relevance"].push_back({{"docID", item2->doc_id},{"rank",  item2->rank}});
                }
                else
                {
                    answerJSON["answers"][strNumberRequest]["relevance"].push_back({{"docID", item2->doc_id},{"rank",  item2->rank}});
                }
            }
        }
        numberRequest++;
    }
    return answerJSON;
}

void ConverterJSON::PutAnswers(const std::vector<std::vector<RelativeIndex>> &answers)
{
    auto answersJson = CreatAnswerJson(answers);
    WriteJsonFile(mAnswerFileName, answersJson);
}

bool ConverterJSON::IsValidVersion()
{
    ReadJsonFile(mConfigFileName);
    return configJSON["config"]["version"] == VERSION_APP;
}

void ConverterJSON::GetInfoProject()
{
    std::cout << "Start " << configJSON["config"]["name"] << std::endl;
    std::cout << "Version: " << configJSON["config"]["version"] << std::endl;
    std::cout << "Max request limit: " << configJSON["config"]["max_responses"] << std::endl;
}

