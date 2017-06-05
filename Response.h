//
// Created by zhangke on 17-5-15.
//

#ifndef HTTPREQUEST_RESPONSE_H
#define HTTPREQUEST_RESPONSE_H

#include <string>
#include <map>
#include "http_parser.h"
#include "tool.h"

class Response {
public:
    Response(HTTP_VERSION v) : version(v) {}
    bool writeStatusCode(http_status s)
    {
        this->status = s;
        return true;
    }
    bool setHeadField(const std::string head, const std::string field)
    {
        headers[head] = field;
        return true;
    }
private:
    HTTP_VERSION version;    // initial when construct
    http_status status;
    std::map<std::string, std::string> headers;
};


#endif //HTTPREQUEST_RESPONSE_H
