#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "config.h"
#include "nlohmann/json.hpp"
#include "searchserver.h"

/**
* Класс для работы с JSON-файлами
*/
class ConverterJSON {
    nlohmann::json configJSON;
    nlohmann::json requestJSON;
    nlohmann::json answerJSON;
    std::fstream mFile;
    /**
    * Пути к файлам .json
    */
    std::string mAnswerFileName = defaultAnswerFileName;
    std::string mConfigFileName = defaultConfigFileName;
    std::string mRequestsFileName = defaultRequestsFileName;
public:
    ConverterJSON() = default;
    /**
    * Метот чтения .json файла
    */
    void ReadJsonFile(const std::string &fileName);
    /**
    * Метот заполнения вектора текстами всех документов базы данных
    */

    /** Метод возвращает вектор имён файлов для индексации
    * @return
    */
    std::vector<std::string> GetFileNames();
    /**
    * Метод считывает поле max_responses для определения предельного
    * количества ответов на один запрос
    * @return
    */
    int GetResponsesLimit();
    /**
    * Метод получения запросов из файла requests.json
    * @return возвращает список запросов из файла requests.json
    */
    std::vector<std::string> GetRequests();
    /**
    * Запись в файл answers.json результаты поисковых запросов
    */

    std::string ReadDocument(const std::string &fileName);
    void PutAnswers(const std::vector<std::vector<RelativeIndex>> &answers);
    /**
    * Метот создания answerJSON файла
    */
    nlohmann::json CreatAnswerJson(const std::vector<std::vector<RelativeIndex>> &answers);

    /** Метод проверяет соответсвыие версии ПО и версии JSON файла config
     * @return true если версия файла config.json свпадает с версией программы
     */
    bool IsValidVersion();
    /**
    * Метот выводит в консоль информацию о проекте
    */
    void GetInfoProject();
    /**
    * Метот записи .json файла
    */
    void WriteJsonFile(const std::string &fileName, nlohmann::json &jsonToWrite);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
