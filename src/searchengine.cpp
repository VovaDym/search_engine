#include <iostream>
#include <string>
#include "converterJSON.h"
#include "invertedindex.h"
#include "searchserver.h"
#include "config.h"

int main()
{
    ConverterJSON converterJson;
    InvertedIndex invertedIndex;


    if (!converterJson.IsValidVersion())
    {
        std::cerr << "Error!Incorrect application version!" << std::endl;
        return 0;
    }
    invertedIndex.UpdateDocumentBase(converterJson.GetFileNames());

    converterJson.GetInfoProject();
    SearchServer searchServer(invertedIndex);
    searchServer.SetMaxRequests(converterJson.GetResponsesLimit());
    auto queries = converterJson.GetRequests();
    auto rIndex = searchServer.Search(queries);
    converterJson.PutAnswers(rIndex);

    return 0;
}







