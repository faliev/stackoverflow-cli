#include "stackoverflow-cli.h"

#include <curl/curl.h>

static size_t requestCallback (char *ptr, size_t size, size_t nmemb, void *usr) {
    size_t realsize = size * nmemb;
    responseObject *response = (responseObject *)usr;

    if (realsize != 0) {
        response->data = realloc(response->data, response->size + realsize + 1);

        if (response->data == NULL) {
            printf("Not enough memory!");
            exit(EXIT_FAILURE);
        }

        memcpy(&response->data[response->size], ptr, realsize);
        response->size += realsize;
        response->data[response->size] = 0;
    } else {
        response->data = NULL;
        response->size = 0;
    }

    return realsize;
}

void makeWebRequest (const char *url, responseObject *response) {
    CURL *curlHandle = NULL;

    curl_global_init(CURL_GLOBAL_ALL);

    curlHandle = curl_easy_init();

    memset(response, 0, sizeof(responseObject));

    curl_easy_setopt(curlHandle, CURLOPT_URL, url);
    curl_easy_setopt(curlHandle, CURLOPT_ACCEPT_ENCODING, "\0");
    curl_easy_setopt(curlHandle, CURLOPT_WRITEFUNCTION, requestCallback);
    curl_easy_setopt(curlHandle, CURLOPT_WRITEDATA, (void *)response);
    curl_easy_perform(curlHandle);
    curl_easy_cleanup(curlHandle);
}

void freeWebResponse (responseObject *response) {
    if (response == NULL)
        return;

    if (response->data != NULL)
        free(response->data);

    response->size = 0;
}
