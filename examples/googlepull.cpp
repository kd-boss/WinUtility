/*
    Simple example demonstrating how to use my SslClientSocket class.
    Downloads and saves the html from google's homepage as out.html.

    release build - compile with:
        g++ googlepull.cpp -s -O3 -mwindows -municode -luser32 -lSecur32
   -lcrypt32 -lws2_32 -o googlepull.exe
    debug build - compile with :
        g++ googlepull.cpp -g -mwindows -municode -luser32 -lSecur32 -lcrypt32
   -lws2_32 -o googlepull.exe

   also works without -municode for those who don't like to use unicode.
*/
#define WINVER 0x0600
#define _WIN32_WINNT 0x0600

#define SECURITY_WIN32
#include <fstream>
#include <utility\SSLClientSocket.h>

WSAInitHandler hndler;
SslClientSocket sck;
std::fstream fs;

bool contains(std::string a, std::string b)
{
    return a.find(b) != std::string::npos;
}

void OnError(WSAException e)
{
    std::wcout << e.what() << std::endl;
}

void OnRecieve(std::vector<unsigned char> Data)
{
    try
    {
        auto dat = std::string(Data.begin(), Data.end());

        if (fs.is_open())
        {
            if (contains(dat,"<!doctype html>"))
			{
				std::string odat = dat.substr(dat.find("<!doctype html>"));
				fs << odat;
				std::cout << dat;
			}
            else if (contains(dat, "</html>")) //then we're done receiving data, we got the whole page. 
            {
                std::string odat = dat.substr(0, dat.find("</html>") + 7);
                fs << odat;
                std::cout << dat << std::endl;
                fs.close();
                sck.Close();
            }
            else
            {
                fs << dat;
                std::cout << dat;
            }
        }
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }
}

void OnConnect()
{
    try
    {
        fs.open("out.html", std::ios_base::out);
        std::string get = "GET /?gws_rd=ssl HTTP/1.1\r\n"
                          "Host: www.google.com\r\n"
                          "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; "
                          "rv:32.0) Gecko/20100101 Firefox/32.0\r\n"
                          "Accept: text/html\r\n"
                          "Accept-Language: en-US,en;q=0.5\r\n"
                          "Accept-Encoding:  text/html\r\n"
                          "Connection: keep-alive\r\n"
                          "\r\n";
        std::vector<unsigned char> buff(get.begin(), get.end());
        sck.Send(buff);
    }
    catch (std::exception &e)
    {
        std::cout << e.what();
    }
}

void OnClose()
{
	try
	{
		std::cout << std::endl << "The out file is saved and closed, Exiting the application." << std::endl;
	}
	catch(std::exception &e)
	{
		std::cout << e.what();
	}
	PostQuitMessage(0); //RETURNS void, no error codes. 
		
}

#ifndef UNICODE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmd, INT nShow) //ascii entry point
{
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmd, INT nShow) //unicode entry point
{
#endif
    try
    {
        sck.m_OnRecieve = OnRecieve;
        sck.m_OnConnect = OnConnect;
        sck.m_OnClose = OnClose;
        sck.m_OnError = OnError;
        std::tstring server = TEXT("google.com");
        sck.Connect(server, 443);
        MSG msg;
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return 0;
    }
    catch (WSAException &e)
    {
        std::wcout << e.what() << std::endl;
        return 0;
    }
}