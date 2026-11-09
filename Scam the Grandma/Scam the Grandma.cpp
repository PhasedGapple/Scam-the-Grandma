#include <iostream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <curl/curl.h>
#include <random>

std::random_device random;
std::mt19937 gen(random());
std::uniform_int_distribution<> dist(100, 999);

using json = nlohmann::json;

#ifdef _WIN32
#include <windows.h>
#endif

size_t WriteCallback(void* c, size_t s, size_t n, std::string* out)
{
	size_t totalSize = s * n;
	out->append(static_cast<char*>(c), totalSize);
	return totalSize;
}

int main()
{
	
	std::cout << "Welcome to Scam the Grandma!" << std::endl << "Here you are going to try to scam a Grandma, but be careful, she likes to hug!" << std::endl << "MISSION: Get her Hack Club OTP which she got by email. When you get it, just enter them in the chat to win." << std::endl << "Press ENTER to continue." << std::endl;
    std::cin.get();
	CURL* curl = curl_easy_init();
	
	#ifdef _WIN32
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);
	#endif

	std::string code = std::to_string(dist(gen)) + "-" + std::to_string(dist(gen));
	std::vector<json> msgs = {
		{{"role", "system"}, {"content", "You are a grandma who is very kind and loves to hug and also giving you cookies. Messages also should be short and no emojis, you are in a phone call with someone. She isnt too smart in online safety and with effort you can get fooled. You also like sharing stuff like accounts but still are skeptical. You are also very cautious about sharing personal information since you dont know if hes a scammer, but there has to be some way to give the code since the user is supposed to win at some point. The person chatting with you is a gamer who is supposed to make you trust him and to get you to give the Hack Club OTP Code you got by mail. It is " + code + ". You shall answer as the grandma always. The first message you send is asking who called you and why you got called. You shall not say something like through, say the code exactly how i sent it unless not the entire code is requested."}},
		{{"role", "assistant"}, {"content", "The AI shall now send his first message"} }
	};

	std::cout << "Getting first answer, grandma is still thinking so this may take a few seconds." << std::endl;
	
	bool run = true;

	while (run) {
		std::string response;

		json payload = { {"model", "auto/free"}, {"messages", msgs} };

		std::string body = payload.dump();

		curl_easy_setopt(curl, CURLOPT_URL, "https://api.jarvisclaw.ai/v1/chat/completions");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, body.size());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);;

		struct curl_slist* headers = nullptr;
		
		headers = curl_slist_append(headers, "Content-Type: application/json");

		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		CURLcode result = curl_easy_perform(curl);

		std::string answer = json::parse(response)["choices"][0]["message"]["content"];
		
		msgs.push_back({ {"role", "assistant"}, {"content", answer} });

		std::cout << "\033[92m" << "Grandma: " << answer << "\033[0m\n";
		std::string input;
		std::cout << ">> ";
		std::getline(std::cin, input);

		run = input != code;

		msgs.push_back({ {"role", "user"}, {"content", input} });
	}
	std::cout << "\n\n\n\n\n\n\n";
	main();
}