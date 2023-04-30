#ifndef CONFIG_H_IN
#define CONFIG_H_IN
/**
 *  *********************** ВНИМАНИЕ!!!!  ******************************
 *    данный файл генерируется автоматически, для внесения изменений в
 * конфигурацию программы - необходимо вносить изменения в файле config.in
 * *********************************************************************
 */
#include <string>

/** Версия файла config.json  с которой работает программа */
const std::string VERSION_APP = "0.1";

/** Каждый документ содержит не более 1000 слов */
const int MAX_WORDS_IN_FILE = 1000;

/** С максимальной длиной каждого в 100 символов. */
const int MAX_CHAR_IN_WORD = 100;

/** Поле списка запросов содержит не более 1000 запросов. */
const int MAX_REQUESTS = 1000;

/** Каждый запрос включает от одного до десяти слов */
const int MAX_WORDS_REQ = 10;

/** Пути к .json файлам по дефолту */
const std::string defaultConfigFileName = "config.json";
const std::string defaultRequestsFileName = "requests.json";
const std::string defaultAnswerFileName = "answer.json";

#endif // CONFIG_H_IN
