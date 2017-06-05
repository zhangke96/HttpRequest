#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include <utility>
#include "http_parser.h"
#include <map>

static std::string GETMethod("GET");
static std::string POSTMethod("POST");
static std::string HEADMehod("HEAD");

bool recFin = false;

enum HTTP_VERSION
{
    HTTP_1_0;
    HTTP_1_1;
};
enum MY_HTTP_METHOD
{
  MY_HTTP_ERROR, /* can not identify the method */
  MY_HTTP_GET,
  MY_HTTP_POST,
  MY_HTTP_HEAD,
};

/*
* judge the http method ignore the upper or lower case
*/
MY_HTTP_METHOD parser_method(std::string &method)
{
  std::transform(method.cbegin(), method.cend(), method.begin(), toupper);
  if (method == GETMethod)
    return MY_HTTP_GET;
  else if (method == POSTMethod)
    return MY_HTTP_POST;
  else if (method == HEADMehod)
    return MY_HTTP_HEAD;

  return MY_HTTP_ERROR;
}

/*
std::pair<std::string, std::string> parser_url(std::string &url)
{
  return std::pair<std::string, std::string>(std::string("zkhaha.cn"), std::string("/"));
}
*/

std::string generate_http(const MY_HTTP_METHOD &method, const std::string hostname, const std::string url)
{
    std::string http_request("GET ");
    http_request += url;
    http_request += " HTTP/1.1\r\nHost: ";
    http_request += hostname;
    http_request += "\r\nUser-Agent: zhangke\r\nAccept-Type: */*\r\nConnection: Keep-Alive\r\n\r\n";
    std::cout << http_request << std::endl;
    return http_request;
}

int call_message_begin_cb(http_parser *p)
{
    printf("\e[0;31mcall_message_begin\e[0m\n");
    return 0;
}
int call_header_field_cb(http_parser *p, const char *buf, size_t len)
{
    printf("\e[0;31mcall_header_field\e[0m %s %zu\n", buf, len);
    return 0;
}
int call_header_value_cb(http_parser *p, const char *buf, size_t len)
{
    printf("\e[0;31mcall_header_value \e[0m%s %zu\n", buf, len);
    return 0;
}
int call_request_url_cb(http_parser *p, const char *buf, size_t len)
{
    printf("\e[0;31mcall_request_url \e[0m%s %zu\n", buf, len);
    return 0;
}
int call_body_cb(http_parser *p, const char *buf, size_t len)
{
    printf("\e[0;31mcall_body \e[0m%s %zu\n", buf, len);
    return 0;
}
int call_headers_complete_cb(http_parser *p)
{
    printf("\e[0;31mcall_headers_complete\e[0m\n");
    return 0;
}
int call_message_complete_cb(http_parser *p)
{
    recFin = true;
    printf("\e[0;31mcall_message_complete\e[0m\n");
    printf("method: %d\n", p->method);
    return 0;
}
int call_response_status_cb(http_parser *p, const char *buf, size_t len)
{
    printf("\e[0;31mcall_reponse_status \e[0m%s %zu\n", buf, len);
    return 0;
}
int call_chunk_header_cb(http_parser *p)
{
    printf("\e[0;31mcall_chunk_header\e[0m\n");
    return 0;
}
int call_chunk_complete_cb(http_parser *p)
{
    printf("\e[0;31mcall_chunk_complete\e[0m\n");
    return 0;
}
