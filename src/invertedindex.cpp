#include "invertedindex.h"
#include "config.h"

template<typename T1, typename T2>
void func_merge(std::map<T1, std::vector<T2>> &map1, const std::map<T1, std::vector<T2>> &map2)
{
    for (const auto&[key, vec]:map2)
    {
        map1[key].insert(map1[key].cend(), vec.begin(), vec.end());
    }
}

std::string InvertedIndex::ReadDocument(const std::string &fileName)
{
    std::string currWord, currTextDocument;
    int counterWords = 0;
    int counterChar = 0;
    bool isTyping = false;

    std::fstream file;
    file.open(fileName, std::ios::in);
    if (!file.is_open()) {
        std::cout << "Unable to open file " << fileName << std::endl;
    } else {
        // получим размер файла
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

std::vector<std::string> InvertedIndex::SplitIntoWords(const std::string &text)
{
    std::vector<std::string> words;
    std::string word;
    for (const char c : text)
    {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
            }
            word.clear();
        } else {
            word.push_back(c);
        }
    }
    if (!word.empty()) words.push_back(word);
    return words;
}

void InvertedIndex::ToIndexDoc(const size_t &docId, const std::string &docFileName)
{
    std::map<std::string, std::vector<Entry>> freqDictionaryOneDoc;
    std::vector<std::string> words = SplitIntoWords(ReadDocument(docFileName));

    for (const std::string &word: words)
    {
        if (!freqDictionaryOneDoc.count(word))
        { // если такого слова в словаре ещё нет
            freqDictionaryOneDoc[word].push_back({docId, 1}); // создаём запись с таким словом
        }
        else
        {
            if (freqDictionaryOneDoc[word].back().doc_id == docId)
            {
                // если такое слово в словаре уже есть и относится к текущему документу
                freqDictionaryOneDoc[word].back().count++;  // увеличиваем количество вхождений на 1
            }
            else
            {
                // если это слово в текущем документе встретилось впервые, присваиваем 1.
                freqDictionaryOneDoc[word].push_back({docId, 1});
            }
        }
    }
    lockDictionary->lock();
    lengthInWordAllDocs += words.size();
    lengthInWordDoc[docId] = words.size();
    func_merge(freqDictionaryAllDocs, freqDictionaryOneDoc); // произведём слияние словарей
    lockDictionary->unlock();
}

void InvertedIndex::UpdateDocumentBase(const std::vector<std::string> &fileNames)
{
    freqDictionaryAllDocs.clear();
    lockDictionary = new std::mutex;
    if (fileNames.empty())
    {
        std::cerr << "WARNING: List of filenames is empty." << std::endl;
        return;
    }

    BS::thread_pool pool;

    size_t doc_id = 0;
    for (const auto &fileName:fileNames)
    {
        pool.push_task([this, doc_id, fileName]
                       {
                           ToIndexDoc(doc_id, fileName);
                       });

        doc_id++;
    }

    pool.wait_for_tasks();

    delete lockDictionary;
    lockDictionary = nullptr;
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word)
{
    if (freqDictionaryAllDocs.count(word))
    {
        return freqDictionaryAllDocs.at(word);
    }
    else
    {
        return std::vector<Entry>({});
    }
}















