#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <utility>
#include "tool.h"
#include <netdb.h>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include "http_parser.h"
#include "ResponseParser.h"

using std::string;
using std::cout;
using std::endl;
using std::pair;

ResponseParser *aparser = NULL;

extern "C" int message_begin_cb(http_parser *p)
{
    return aparser->call_message_begin_cb(p);
}
extern "C" int header_field_cb(http_parser *p, const char *buf, size_t len)
{
    return aparser->call_header_field_cb(p, buf, len);
}
extern "C" int header_value_cb(http_parser *p, const char *buf, size_t len)
{
    return aparser->call_header_value_cb(p, buf, len);
}
extern "C" int body_cb(http_parser *p, const char *buf, size_t len)
{
    return aparser->call_body_cb(p, buf, len);
}
extern "C" int headers_complete_cb(http_parser *p)
{
    return aparser->call_headers_complete_cb(p);
}
extern "C" int message_complete_cb(http_parser *p)
{
    return aparser->call_message_complete_cb(p);
}
extern "C" int reponse_status_cb(http_parser *p, const char *buf, size_t len)
{
    return aparser->call_reponse_status_cb(p, buf, len);
}
extern "C" int chunk_header_cb(http_parser *p)
{
    return aparser->call_chunk_header_cb(p);
}
extern "C" int chunk_complete_cb(http_parser *p)
{
    return aparser->call_chunk_complete_cb(p);
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    cout << "Usage: ./request <http method> <hostname> <url> " << endl;
    return -1;
  }
  string method(argv[1]);
  string hostname(argv[2]);
  string url(argv[3]);
  MY_HTTP_METHOD method_result = parser_method(method);
  if (method_result == MY_HTTP_ERROR)
  {
    cout << "Can not identify the method: " << argv[1] << endl;
    return -1;
  }
//  pair<string, string> url_result = parser_url(url);

/*
  string hostname(std::move(url_result.first));
  url = std::move(url_result.second);
*/

  /* look up the DNS to get the ip address of the hostname */
  int err;
  struct addrinfo *ailist;
  if ((err = getaddrinfo(hostname.c_str(), "http", NULL, &ailist))!= 0)
  {
    cout << "getaddrinfo error:" << gai_strerror(err) << endl;
    return -1;
  }
  struct sockaddr_in *sinp = nullptr;
  if (ailist->ai_family == AF_INET)
  {
      sinp = (struct sockaddr_in *)ailist->ai_addr;
  }
  char abuf[100];
  cout << "will connect: " << inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN) << ":" << ntohs(sinp->sin_port) << endl;

  /* create socket */
  int sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

  if (connect(sfd, (sockaddr *)sinp, sizeof(*sinp)) != 0)
  {
    cout << "connect error: " << endl;
  }
  std::string http_request(std::move(generate_http(method_result, hostname, url)));
  cout << "sending : " << send(sfd, http_request.c_str(), http_request.length(), 0) << endl;
  extern bool recFin;
  int confirmFinMethod = 0;
  char recvbuf[80*1024];  /* recv buf */
  ssize_t recvSize;
  string temp;
  string offerLength("Content-Length: ");
  string chunked("Transfer-Encoding: chunked");
  char chunkedEnding[] = "/r/n0/r/n/r/n";
  size_t Length;
//  while (!recFin)
//  {
//    recvSize = recv(sfd, recvbuf, 4096, 0);
//    if (!confirmFinMethod)     /* have not confirm the finish the http body method */
//    {
//      std::stringstream ass(string(recvbuf));
//      while (ass >> temp)
//      {
//        if (temp == chunked)
//        {
//          confirmFinMethod = 1; /* chunked, read until get /r/n0/r/n/r/n */
//          break;
//        }
//        else if (temp.length() > offerLength.length())
//        {
//          if (string(temp.cbegin(), temp.cbegin()+offerLength.length()) == offerLength)
//          {
//            confirmFinMethod = 2; /* offer length, read until the offer length */
//            Length = atoi(string(temp.cbegin()+offerLength.length(), temp.cend()).c_str());
//          }
//        }
//      }
//    }
//  }
  http_parser *parser = (http_parser *)malloc(sizeof(http_parser));
  http_parser_init(parser, HTTP_RESPONSE);

  http_parser_settings settings;
  settings.on_message_begin = message_begin_cb;
  settings.on_header_field = header_field_cb;
  settings.on_header_value = header_value_cb;
  settings.on_url = call_request_url_cb;
  settings.on_status = reponse_status_cb;
  settings.on_body = body_cb;
  settings.on_headers_complete = headers_complete_cb;
  settings.on_message_complete = message_complete_cb;
  settings.on_chunk_header = chunk_header_cb;
  settings.on_chunk_complete = chunk_complete_cb;
    aparser = new ResponseParser(settings);
  while (true)
  {
    recvSize = recv(sfd, recvbuf, 80 * 1024, 0);
    if (recvSize < 0)
    {
      cout << "error when recive" << endl;
      close(sfd);
      return -1;
    }
      std::cout << recvbuf << std::endl;
   //   http_parser_execute(parser, &settings, recvbuf, recvSize);
    aparser->response_parser_execute(recvbuf, recvSize);
   printf("\e[0;31mrecFin: %d\e[0m\n", (bool)recFin);
    if (aparser->ifFinished())
      break;
  }
    cout << aparser->getStatusStr() << endl;
    cout << aparser->getBody() <<endl;
    std::map<std::string, std::string> headers = aparser->getHeaders();
    for (auto c : headers)
        cout << c.first << " : " << c.second << endl;
    delete aparser;
  return 0;

}
