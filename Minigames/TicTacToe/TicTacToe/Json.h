#include "cpprest\http_client.h"
#include <iostream>
#include "cpprest\json.h"

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::client;

pplx::task<json::value> RequestJSONValueAsync(utility::string_t baseUrl);
void IterateJSONValue(utility::string_t baseUri);