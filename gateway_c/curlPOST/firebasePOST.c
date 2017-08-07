#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#define DEST_URL "https://iot-project-1e8b6.firebaseio.com/items.json"
//#define AUTH_KEY_LOCATION "./authentification_firebase.key"


int main(int argc, char *argv[])
{
		char json_to_send[255];
		CURL *curl;
		FILE *fd=fopen("test.json","r");
		fgets(json_to_send, 255, fd);
		fclose(fd);
		printf("%s\n", json_to_send);
		struct curl_slist *headers = NULL;
		headers=curl_slist_append(headers, "Accept: application/json");
		headers=curl_slist_append(headers, "Content-Type: application/json");
		headers=curl_slist_append(headers, "charsets: utf-8");
		CURLcode res;
		curl_global_init(CURL_GLOBAL_DEFAULT);
		curl = curl_easy_init();
		if(curl){
				curl_easy_setopt(curl, CURLOPT_URL, DEST_URL );
				curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
				curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
				curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_to_send);
				res = curl_easy_perform(curl);
				if (res != CURLE_OK) {
						fprintf(stderr, "curl_easy_perform() failded: %s\n", curl_easy_strerror(res));
						curl_easy_cleanup(curl);
				}
		}
		curl_global_cleanup();
		return 0;
}
