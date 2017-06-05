//
// Created by zhangke on 17-3-29.
//

#include "ResponseParser.h"

int ResponseParser::getStatusCode() const {
    return statusCode;
}

std::map<std::string, std::string> ResponseParser::getHeaders() const {
    return headers;
}

const std::string * ResponseParser::getValueByField(const std::string &field) const {
    auto index = headers.find(field);
    if (index == headers.cend())
        return NULL;
    else
        return &(*index).first;
}

bool ResponseParser::ifExistField(const std::string &field) const {
    auto index = headers.find(field);
    if (index == headers.cend())
        return false;
    else
        return true;
}

void ResponseParser::response_parser_execute(char *recvbuf, size_t recvSize) {
    http_parser_execute(parser, &settings, recvbuf, recvSize);
}

int ResponseParser::call_message_begin_cb(http_parser *p) {
    printf("\e[0;31mcall_message_begin\e[0m\n");
    start = true;
    return 0;
}

int ResponseParser::call_message_complete_cb(http_parser *p) {
    printf("\e[0;31mcall_message_complete\e[0m\n");
    finish = true;
    return 0;
}
int ResponseParser::call_header_field_cb(http_parser *p, const char *buf, size_t len) {
    printf("\e[0;31mcall_header_field_\e[0m\n");
    temp_field = std::string(buf, len);
    return 0;
}
int ResponseParser::call_header_value_cb(http_parser *p, const char *buf, size_t len) {
    printf("\e[0;31mcall_header_value\e[0m\n");
    temp_value = std::string(buf, len);
    headers.insert(make_pair(temp_field, temp_value));
    return 0;
}
int ResponseParser::call_headers_complete_cb(http_parser *p) {
    printf("\e[0;31mcall_headers_complete\e[0m\n");
    headers_completed = true;
    return 0;
}
int ResponseParser::call_body_cb(http_parser *p, const char *buf, size_t len) {
    printf("\e[0;31mcall_body\e[0m\n");
    body += std::string(buf, len);
    return 0;
}
int ResponseParser::call_reponse_status_cb(http_parser *p, const char *buf, size_t len) {
    printf("\e[0;31mcall_response_status\e[0m\n");
    status = std::string(buf, len);
    return 0;
}
int ResponseParser::call_chunk_header_cb(http_parser *p) {
    printf("\e[0;31mcall_chunk_header\e[0m\n");
    isChunk = true;
    return 0;
}
int ResponseParser::call_chunk_complete_cb(http_parser *p) {
    printf("\e[0;31mcall_chunk_complete_cb\e[0m\n");
    isChunk = true;
    return 0;
}
