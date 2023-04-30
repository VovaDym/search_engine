<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/VovaDym/searchengine">
    <img src="Resource/logo.png" alt="Logo" width="80" height="80">
  </a>

<h2 align="center">"SkillboxSearchEngine"</h2>

  <p align="center">
    Приложение локального поискового движка по файлам

  </p>
</div>


### <a name="Использованные-библиотеки">Использованные библиотеки</a>
При разработке данной программы были использованы следующие библиотеки
- [JSON for MODERN C++](https://github.com/nlohmann/json)
- [A C++17 Thread Pool for High-Performance Scientific Computing](https://github.com/bshoshany/thread-pool)
- [GoogleTest](https://github.com/google/googletest)

### <a name="Сборка">Сборка приложения</a>
Сборка и проверка функционирования приложения производилась под управлением операционной системе Wndows10.
Проверена сборка на:

* Microsoft Visual Studion 17 2022
* minGW64 v6.0

Запуск приложения из командной строки

  	        mkdir build
  	        cd build
  	        cmake ..
  	        cmake --build . --config Release --target searchengine

Для работы обязательно нужно что бы .json файлы находились в одной директории с исполняемым файлом,
поэтому скопируйте исполняемый файл в корневую дерикторию проекта:

            cd ..\\Release
		copy serchengine.exe ..
		
После чего можно запустить serchengine.exe

		cd ..
		.\ serchengine.exe
		
Для запуска Unit-testов  необходимо в папке build, ранее созданной выполнить следующее:

	        cmake --build . --config Release --target Google_Tests_All_Test
	        cd ..\\Release
	        copy Google_Tests_All_Test.exe ..
	        cd ..
	        .\ Google_Tests_All_Test.exe

### <a name="Конфигурирование">Конфигурирование приложения</a>
Основные настройки/конфигурация программы содержится в файле config.json.
Файл должен располагаться в корневой директории приложения.

Пример файла конфигурации приведён ниже:

                {
                    "config": {
	                "name": "SkillboxSearchEngine",
	                "version": "0.1"
                        "max_responses": 5,         
                        
                    },
                    "files": [
                        "Resource\\file001.txt",
                        "Resource\\file002.txt",
                        "Resource\\file003.txt"
                    ]
                }
            
**name** - отображаемое имя приложения при запуске.
**version** - версия приложения/JSON конфигурационного файла, (должна совпадать с текущей версией поисковой программы).
**max_responses** - количество отображаемых ответов в файле answers.json  
**files:** - перечень файлов среди которых производиться поиск   
"Resource\\file.txt" - относительный путь к файлу.   

Файлы для поиска должны быть текстовыми, содержать латинские символы. Спец символы, знаки препинания и т.п. при индексировании и поиске игнорируются.

Для задания запросов поиска необходимо сформировать файл requests.json, Формат и пример файла requests.json приведён ниже.

                 {
                     "requests":[
                           "milk water", 
                           "sugar", 
                           "milk water sugar"
                      ]
                 }
  
**requests** - индетификатор запросов (обязательное поле);  
"***milk water***", "***sugar***", "***milk water sugar***" - поисковые запросы.

К файлу *requests.json* и поисковым запросам предъявляются следующие требования/ограничения:
- в файле может быть не более 1000 запросов(остальные не будут учитываться);
- запрос выполняется прописными латинскими буквами;
- в слове запроса допускается не более 100 символов

### <a name="Запуск">Запуск приложения</a>
После подготовки файла конфигурации и файла поисковых запросов, приложение готово к запуску.
Для запуска приложения необходимо выполнить следующие условия:
- Файлы config.json и request.json должны находиться в одной папке с файлом searchengine.exe;
- Относительные пути к файлам-документам должны строиться от папки в которой запускается searchengine.exe;

Запуск приложения выполняется запуском файла searchengine.exe на исполнение. После запуска приложения в консоли будет отображена следующая информация:

                   Start SkillboxSearchEngine
                   Version: 0.1
                   Max request limit: 5

В случае возникновения нештатных ситуаций будет выведены дополнительные сообщения, и возможно досрочно прекращена выполнение программы.
После удачного завершения работы программы ( Process finished with exit code 0), в директории запуска программы будет создан файл answers.json.
Формат результирующего файла описан [ниже](#Результат).

### <a name="Результат">Результат работы приложения</a>
Результатом работы приложения является созданный файл answer.json
Пример файла ниже:

                   {
                       "answers": {
                           "request001": {
                               "result": false
                           },
                           "request002": {
                               "relevance": [
                                   {
                                       "docID": 4,
                                       "rank": 1.0
                                   }
                               ],
                               "result": true
                           }
                   }
               
* **answers** — базовое поле в этом файле, которое содержит ответы на запросы.
* **request001 … 002** — идентификатор запроса, по которому сформирован ответ.
* **result** – результат поиска запроса. Если он принимает значение ***true***, значит по
  данному запросу найден хотя бы один документ. Если результат имеет значение
  ***false***, значит ни одного документа не найдено.
* **relevance** включается в файл ***answers.json***, если на этот запрос удалось найти
  более одного документа.
    * <Идентификатор документа>("docID") — идентификатор документа, в котором найден ответ на запрос. Например, если в поле config.json поле files содержит:                       
      "files": [
      "file001.txt", для данного файла docid будет равен 0
      "file002.txt"  для данного файла docid будет равен 1
      ]
    * **<ранг ответа>(“rank”)** — ранг или релевантность. Это число показывает,
      насколько документ подходит для заданного запроса. В ответе id документов
      располагаются в порядке уменьшения релевантности.


