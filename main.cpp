#include <iostream>
#include <cpr/cpr.h>

using namespace std;



void printHeaders(map<string, string>* headers){
    for(auto pair : *headers){
        cout<<"\t - "<<pair.first<<" : "<<pair.second<<endl;
    }
    cout<<"-----------------------"<<endl;
}

string makeURLRequestFromHeaders(std::map<std::string, std::string>* headers)
{
    if (headers->empty()) return "";

    std::string result = "?";
    {
        for (auto it : *headers)
        {
            result += it.first + "=" + it.second + "&";
        }
    }
    if (result[result.length() - 1] == '&')
        result.resize(result.length()-1);

    return result;
}


void sendRequest(string request, map<string, string>* headers)
{
    string urlWithGet = "http://httpbin.org/post";
    if(request == "post")
    {
        cpr::Payload payload({});
        for(auto it : *headers)
        {
            cpr::Pair p = cpr::Pair(it.first.c_str(), it.second.c_str());
            payload.Add(p);
        }

        cpr::Response r = cpr::Post(cpr::Url(urlWithGet), payload);
        cout << "POST request is sent.\n";
        cout << "Server reply:\n";
        cout << r.text << "\n";
    }
    else if(request == "get")
    {
        string urlWithParameters = urlWithGet + makeURLRequestFromHeaders(headers);
        cpr::Response r = cpr::Get(cpr::Url(urlWithParameters));
        std::cout << "GET request is sent: " + urlWithParameters + "\n";
        std::cout << "Server reply:\n";
        std::cout << r.text << "\n";
    }
}

void addHeaders(map<string, string>* headers, string first, string second)
{
    auto pair = make_pair(first, second);
    auto it = headers->find(first);
    if(it == headers->end())
    {
        headers->insert(pair);
    }
    else
    {
        it->second = second;
    }

    cout << "Pair " << first << " : " << second << " is added to map.\n";


}

void showHelp()
{
    std::cout << "Please enter the input.\n";
    std::cout << "\t - \'[header]\' \'[value]\' ... with space delimiter to set / reset header.\n";
    std::cout << "\t - \'get\' - to make get request with named headers.\n";
    std::cout << "\t - \'post\' - to make post request with named headers.\n";
    std::cout << "\t - \'headers\' - to print actual headers & values.\n";
    std::cout << "\t - \'clear\' - to remove actual headers & values.\n";
    std::cout << "\t - \'help\' - to see this message again.\n";
    std::cout << "\t - \'exit\' - FINISH the program.\n";
}


int main() {

    map<std::string, std::string> headers;
    string input;
    string headerName;
    string headerValue;
    bool isFirst = true;
    bool isActive = true;
    showHelp();
    string command;
    while (isActive)
    {
        cin>>input;
        if(input == "get")
        {
            sendRequest("get", &headers);
        }
        else if (input == "post")
        {
            sendRequest("post", &headers);
        }
        else if(input == "clear")
        {
            cout << "Clearing headers...\n";
            headers.clear();
            cout << "Headers are cleared.\n";
        }
        else if(input == "help")
        {
            showHelp();
        }
        else if(input == "headers")
        {
            printHeaders(&headers);
        }
        else if(input == "exit")
        {
            isActive = false;
        }
        else
        {
            if(isFirst)
            {
                headerName = input;
                isFirst = false;
            }
            else
            {
                headerValue = input;
                auto pair = make_pair(headerName, headerValue);
                addHeaders(&headers, headerName, headerValue);
                isFirst = true;
            }
        }
    }

    return 0;
}
