//
// Created by zhangke on 17-3-29.
//
/*
 * ResponseParser is used to parse http response,
 *
 */

#ifndef HTTPREQUEST_RESPONSEPARSER_H
#define HTTPREQUEST_RESPONSEPARSER_H

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include "http_parser.h"



class ResponseParser {
public:
    int getStatusCode() const;
    std::string getStatusStr() const
    {
        return status;
    }
    std::map<std::string, std::string> getHeaders() const;
    const std::string *getValueByField(const std::string &) const;
    bool ifExistField(const std::string &) const;
    void response_parser_execute(char *recvbuf, size_t recvSize);
    bool ifFinished() const
    {
        return finish;
    }
    ResponseParser(const http_parser_settings &userSettings) : statusCode(0), start(false), finish(false), headers_completed(false), isChunk(false), status("unknow"), settings(userSettings)
    {
        parser = (http_parser *)malloc(sizeof(http_parser));
        http_parser_init(parser, HTTP_RESPONSE);
//        settings.on_message_begin = &(this->call_message_begin_cb);
   //     settings.on_message_begin = message_begin_cb(this);
       // settings.on_header_field = this->call_header_field_cb;
       // settings.on_header_value = this->call_header_value_cb;
       // settings.on_body = this->call_body_cb;
       // settings.on_headers_complete = this->call_headers_complete_cb;
       // settings.on_message_complete = this->call_message_complete_cb;
       // settings.on_status = this->call_reponse_status_cb;
       // settings.on_chunk_header = this->call_chunk_header_cb;
       // settings.on_chunk_complete = this->call_chunk_complete_cb;
    }
    int call_message_begin_cb(http_parser *p);
    int call_header_field_cb(http_parser *p, const char *buf, size_t len);
    int call_header_value_cb(http_parser *p, const char *buf, size_t len);
    int call_body_cb(http_parser *p, const char *buf, size_t len);
    int call_headers_complete_cb(http_parser *p);
    int call_message_complete_cb(http_parser *p);
    int call_reponse_status_cb(http_parser *p, const char *buf, size_t len);
    int call_chunk_header_cb(http_parser *p);
    int call_chunk_complete_cb(http_parser *p);
    std::string getBody() const
    {
        return body;
    }

private:
    int statusCode;
    std::map<std::string, std::string> headers;
    http_parser *parser;
    bool start;
    bool finish;
    bool headers_completed;
    bool isChunk;
    std::string temp_field;
    std::string temp_value;
    std::string body;
    std::string status;
    http_parser_settings settings;


};

#endif //HTTPREQUEST_RESPONSEPARSER_H
