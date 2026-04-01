#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    output->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string askLLM(const std::string& question) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (!curl) {
        return "Error: Could not initialize curl";
    }

    
    json requestBody = {
        {"model", "phi-3.5-mini-instruct"},
        {"messages", json::array({
            {{"role", "user"}, {"content", question}}
        })},
        {"temperature", 0.7}
    };

    std::string jsonStr = requestBody.dump();

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, "Authorization: Bearer lm-studio");

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:1234/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonStr.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        return "Error: " + std::string(curl_easy_strerror(res));
    }

  
    try {
        auto jsonResponse = json::parse(response);
        return jsonResponse["choices"][0]["message"]["content"];
    } catch (...) {
        return "Error parsing response: " + response;
    }
}

int main() {
    std::cout << "================================" << std::endl;
    std::cout << "   Local LLM Chat (Phi-3.5)    " << std::endl;
    std::cout << "   Type 'exit' to quit          " << std::endl;
    std::cout << "================================" << std::endl;

    std::string question;

    while (true) {
        std::cout << "\nYou: ";
        std::getline(std::cin, question);

        if (question == "exit") {
            std::cout << "Bye!" << std::endl;
            break;
        }

        if (question.empty()) continue;

        std::cout << "\nPhi: " << askLLM(question) << std::endl;
        std::cout << "--------------------------------" << std::endl;
    }

    return 0;
}