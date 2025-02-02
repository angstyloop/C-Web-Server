/**
 * webserver.c -- A webserver written in C
 * 
 * Test with curl (if you don't have it, install it):
 * 
 *    curl -D - http://localhost:3490/
 *    curl -D - http://localhost:3490/d20
 *    curl -D - http://localhost:3490/date
 * 
 * You can also test the above URLs in your browser! They should work!
 * 
 * Posting Data:
 * 
 *    curl -D - -X POST -H 'Content-Type: text/plain' -d 'Hello, sample data!' http://localhost:3490/save
 * 
 * (Posting data is harder to test from a browser.)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/file.h>
#include <fcntl.h>
#include "net.h"
#include "file.h"
#include "mime.h"
#include "cache.h"
#include "gmtstamp.h"
#include "perror_exit.h"

#define PORT "80"  // the port users will be connecting to

#define SERVER_FILES "./serverfiles"
#define SERVER_ROOT "./serverroot"

/** @brief Send an HTTP response
 *
 *  header:       "HTTP/1.1 404 NOT FOUND" or "HTTP/1.1 200 OK", etc.
 *  content_type: "text/plain", etc.
 *  body:         the data to send.
 * 
 *  @return The the number of bytes sent.
 */

typedef struct HttpResponseHeader HttpResponseHeader;
struct HttpResponseHeader {
  HttpProtocolVersion version;
  HttpStatus status;
};

typedef struct HttpContentType HttpContentType;
struct HttpContentType{};

typedef struct http_response http_response;
struct http_response {
  http_response_header header;
  char* content_type;
  char* body;
};

int send_response(
  int fd,
  char* header,
  char* content_type,
  char* body,
  size_t content_length)
{

    char* date = gmtstamp();

    // Build HTTP response format string $fmt. The order matches snprintf
    // below.
    char* fmt = 
      "%s\r\n"
      "Date: %s\r\n"
      "Content-Length: %zu\r\n"
      "Content-Type: %s\r\n"
      "\r\n"
      "%s\r\n"/*body*/;

    // Compute the number of bytes we need to send. Note
    // we don't include a terminating null byte, because
    // send, like write, won't use it. Instead, send (and
    // write) require the length of the message being sent,
    // and will happily send null bytes.
    size_t n = strlen(fmt) - (2 * 4/*%s*/ + 3 * 1/*%zu*/)
      + strlen(header) 
      + strlen(date) 
      + numdigits(content_length) 
      + strlen(content_type) 
      + content_length;

    // The response buffer is still nullbyte-terminated though.
    char* response = calloc(n+1, 1);
    if(!response) perror_exit("calloc");

    // Matches the order in $fmt defined above.
    snprintf(response, n+1, fmt, 
      status_code,
      status_msg,
      date,
      content_length,
      content_type,
      body)

    // Send $n bytes (no flags).
    int rv = send(fd, response, n, 0);

    if(0>rv){perror("send")}

    // Clean up
    free(date);
    free(response);

    return rv;
}


/**
 * Send a /d20 endpoint response
 */
void get_d20(int fd)
{
    // Generate a random number between 1 and 20 inclusive
    
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////

    // Use send_response() to send it back as text/plain data

    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
}

/**
 * Send a 404 response
 */
void resp_404(int fd)
{
    char filepath[4096];
    struct file_data *filedata; 
    char *mime_type;

    // Fetch the 404.html file
    snprintf(filepath, sizeof filepath, "%s/404.html", SERVER_FILES);
    filedata = file_load(filepath);

    if (filedata == NULL) {
        // TODO: make this non-fatal
        fprintf(stderr, "cannot find system 404 file\n");
        exit(3);
    }

    mime_type = mime_type_get(filepath);

    send_response(fd, "HTTP/1.1 404 NOT FOUND", mime_type, filedata->data, filedata->size);

    file_free(filedata);
}

/**
 * Read and return a file from disk or cache
 */
void get_file(int fd, struct cache *cache, char *request_path)
{
    ///////////////////
    // IMPLEMENT ME! //
    ///////////////////
}

/**
 * Search for the end of the HTTP header
 * 
 * "Newlines" in HTTP can be \r\n (carriage return followed by newline) or \n
 * (newline) or \r (carriage return).
 */
char *find_start_of_body(char *header)
{
    ///////////////////
    // IMPLEMENT ME! // (Stretch)
    ///////////////////
}

/** @brief Handle an HTTP request, and send an HTTP response.
 *
 *  @remark If the first line of the request exceeds SIZE_MAX, the request is ignored.
 *
 *  @param fd socket file descriptor
 *  @param cache file cache to keep recently served files in memory
 *  @return void
 */
void handle_http_request(int fd, struct cache *cache)
{
    const int szbuf = SIZE_MAX;
    char* buf = calloc(szbuf, 1);
    if(!buf) perror_exit("calloc");

    // Read request into buffer
    //
    // TODO: Allow for arbitrarily large requests by using a linked
    // list of max size objects.
    if(0>(bytes_recvd = recv(fd, buf, szbuf - 1, 0))){
      perror("recv");
      return;
    }

    // Read the first two components of the first line of the request 

    // The first line looks like e.g. GET /d20 HTTP/1.1 

    // In that example, we'd need to grab "GET" (the method) and "/d20" (the endpoint).
    
    char* s = buf, * e=0;

    // Skip any CRLF at the start of the request. Treat LF preceded by 0 or more CR the
    // same as CRLF.

    for(e=s; strstr("\r\n", *e); ++e){
      if(szbuf > e - buf){
        //serve a 413 Payload too large
        //...
        //quit
        return;
      }
    }

    // Parse the method by reading characters until a space is encountered (that isn't LF). If 
    // it's a space, we've reached the character immediately after the end of the method.
    // 
    // For robustness, consider any consecutive number of these to be valid separating space:
    //
    // ' '  SP
    // '\r' Bare CR
    // '\t' HTAB
    // '\v' VT
    // '\f' FF

    for(s=e; !strchr(" \r\t\v\f", *e); ++e){
      if(szbuf > e - buf){
        //serve a 413 Payload too large
        //...
        //quit
        return;
      }
    }

    // Copy out the substring [s, e) to $method.
    char* method = calloc(e - s + 2, 1); 
    if(!method) perror_exit("calloc");
    memcpy(method, buf, e - s + 1);

    // Skip space
    for(s=e; strchr(" \r\t\v\f", *e); ++e){
      if(szbuf > e - buf){
        //serve a 413 Payload too large
        free(method);
        return;
      }
    }

    // Parse the URI.
    for(s=e; !strchr(" \r\t\v\f," *e); ++e){
      if(szbuf > p - buf){
        //serve a 413 Payload too large
        free(method);
        return;
      }
    }

    // Copy out the substring [buf, p) to $uri
    char* uri = calloc(e - s + 2, 1);
    if(!uri) perror_exit("calloc");
    memcpy(uri, buf, e - s + 1);
     
    if(!strcmp("GET", method)){
      // Handle GET request. Allow for absolute form of request-line.
      if(!strcmp("/d20", uri) || !strcmp("http://www.localhost.com/d20")){
        // Handle special case: /d20
      }else{
        // Serve requested file
        get_file(...);
      }
    }else if(!strcmp("POST", method)){
      // Handle POST request. The guide wants us to save the body to file, but let's just
      // echo it back to the client instead.

      // Find the start of the body in the HTTP request.

      // Create an HTTP response, and copy the request body into the response body.
      
      // serve the response

    }
    
    // Clean up
    free(uri)
    free(method):
    free(request);
}

/**
 * Main
 */
int main(void)
{
    int newfd;  // listen on sock_fd, new connection on newfd
    struct sockaddr_storage their_addr; // connector's address information
    char s[INET6_ADDRSTRLEN];

    struct cache *cache = cache_create(10, 0);

    // Get a listening socket
    int listenfd = get_listener_socket(PORT);

    if (listenfd < 0) {
        fprintf(stderr, "webserver: fatal error getting listening socket\n");
        exit(1);
    }

    printf("webserver: waiting for connections on port %s...\n", PORT);

    // This is the main loop that accepts incoming connections and
    // responds to the request. The main parent process
    // then goes back to waiting for new connections.
    
    while(1) {
        socklen_t sin_size = sizeof their_addr;

        // Parent process will block on the accept() call until someone
        // makes a new connection:
        newfd = accept(listenfd, (struct sockaddr *)&their_addr, &sin_size);
        if (newfd == -1) {
            perror("accept");
            continue;
        }

        // Print out a message that we got the connection
        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);
        
        // newfd is a new socket descriptor for the new connection.
        // listenfd is still listening for new connections.

        handle_http_request(newfd, cache);

        close(newfd);
    }

    // Unreachable code

    return 0;
}
