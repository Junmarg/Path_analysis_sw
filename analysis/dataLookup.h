#ifndef DATALOOKUP_H
#define DATALOOKUP_H

#include <iostream>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/uri.h>
#include <cpprest/json.h>

using namespace utility;
using namespace web;
using namespace web::http;
using namespace web::http::client;
using namespace concurrency::streams;

class dataLookup
{
public:
    dataLookup();

    //auto datafilestream();
};

#endif // DATALOOKUP_H
