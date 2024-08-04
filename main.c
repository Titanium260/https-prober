#include <curl/curl.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void) {
    CURL *curl;
    CURLcode res;
    struct timespec ts;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {

    char usrUrl[65536];
    printf("Enter the URL to probe : ");
    scanf("%s", usrUrl);
    int i = 0;

    while(1) {
        i += 1;

        curl_easy_setopt(curl, CURLOPT_URL, usrUrl);
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
        curl_easy_setopt(curl, CURLOPT_PROXY, "socks5://127.0.0.1:9050");
        curl_easy_setopt(curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);

        res = curl_easy_perform(curl);
        int http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        if (http_code == 200) {
            printf("Status code: 200 success!\n");
            break;
        }

        printf("Attempt %ld, ", i);
        printf("status code: %ld\n", http_code);

        system("systemctl restart tor");

        // Wait for 10 seconds
        ts.tv_sec = 5;
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);
    }

    } else {
        printf("curl_easy_init failed");
        curl_global_cleanup();
        return 1;
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}
