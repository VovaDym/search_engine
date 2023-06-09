#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H


#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <thread>
#include <mutex>
#include <cmath>
#include <fstream>
#include "BS_thread_pool.hpp"



struct Entry
{
    size_t doc_id, count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const
    {
        return (doc_id == other.doc_id && count == other.count);
    }
    Entry() = default;
};
class InvertedIndex
{
public:

    InvertedIndex() = default;
    /**
    * Обновим или заполним базу документов, по которой будем совершать поиск
    * @param texts_input содержимое документов
    */
    void UpdateDocumentBase(const std::vector<std::string> &fileNames);
    /**
    * Метод определяет количество вхождений слова word в загруженной базе
     документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> GetWordCount(const std::string& word);

    /**
    * Метод делит элемент вектора fileNames на слова
    */
    std::vector<std::string> SplitIntoWords(const std::string &text);

private:
    std::mutex *lockDictionary = nullptr;
    
    std::map<std::string, std::vector<Entry>> freqDictionaryAllDocs;

    /**
    * Метод расчитывает колличество искомых слов в документе
    * @param docId - идентификатор документа
    * @param textDocument - строка-текст документа
    */
    void ToIndexDoc(const size_t &docId, const std::string &textDocument);

    /**
    * Чтение документа из файла
    * @param fileName - имя файла (полный путь к файлу)
    * @return std::string  - строка- соджержимое файла
    */
    std::string ReadDocument(const std::string &fileName);
};

#endif //SEARCH_ENGINE_INVERTEDINDEX_H
