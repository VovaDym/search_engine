//
// Created by lebed on 17.04.2023.
//

#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#include <algorithm>
#include "invertedindex.h"

struct RelativeIndex {
    size_t doc_id;
    double rank;

    bool operator==(const RelativeIndex &other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }

    RelativeIndex() = default;

    RelativeIndex(size_t docId, double rank) {
        this->rank = rank;
        this->doc_id = docId;
    }
};

class SearchServer {
private:
    InvertedIndex mIndex;
    int mMaxRequests = 5;

public:

    /** Конструктор
    * @param idx в конструктор класса передаётся ссылка на класс
    * InvertedIndex, чтобы SearchServer мог узнать частоту слов
    * встречаемых в запросе
    */
    SearchServer(InvertedIndex &idx);

    /** Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла requests.json
    * @return возвращает отсортированный список релевантных ответов для
    * заданных запросов
    */
    std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string> &queriesInput);

    /** Метод задания максимального количества выдаваемых резуьтатов по зхапросу
    *
    * @param maxRequests
    */
    void SetMaxRequests(int maxRequests);

};
#endif //SEARCH_ENGINE_SEARCHSERVER_H
