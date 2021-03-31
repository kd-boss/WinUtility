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

//syntatic sugar. 
bool contains(std::string a, std::string b)
{
		return a.find(b) != std::string::npos;
}

/*This is the best way to be notified of socket errors. C++ can't throw from a window procedure as it's in a different thread context than the main program. 
  expections work only within the a single thread context, not across thread boundries. That once worked, but only on 32bit verions of Windows. 
  As we no longer use 32 bit windows, this was the best solution I could think of, and in hindsight, it allows users to have their socket
  error handeling all in one convienent spot. 
*/
 
void OnError(WSAException e) 
{
	try
	{
		std::wcout << e.what() << std::endl;
	}
	catch(std::exception ex) //can operator<< throw? Yes. Will it throw? Probably never when inserting C style strings, but better to be safe than sorry. 
	{
		std::cout << ex.what() << std::endl;
	}
}

void OnRecieve(std::vector<unsigned char> Data)
{
    try
    {
        auto dat = std::string(Data.begin(), Data.end());

        if (fs.is_open())
        {
            if (contains(dat, "<!doctype html>")) //strip out the HTTP header. 
            {
                std::string odat = dat.substr(dat.find("<!doctype html>"));
                fs << odat;
                std::cout << odat;
                return;
            }
            else if (contains(dat, "</html>")) //then we're done receiving data, we got the whole page. 
            {
                std::string odat = dat.substr(0, dat.find("</html>") + 7); 
                fs << odat;
                std::cout << odat << std::endl;
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
    std::cout << std::endl << "The out file is saved and closed, Exiting the application." << std::endl;
    PostQuitMessage(0);
}

#ifndef UNICODE
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmd, INT nShow)
{
#else
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmd, INT nShow)
{
#endif
    try
    {
		//assign the socket 'event's you wish to be notified of with your functions using the following signatures: 
		//void OnConnect() occurs when the connection is established. 
		//void OnClose()   occours when the socket is closed. This can be initatied by you, or the server you're connected to.
		//void OnRecieve(std::vector<unsigned char>)  occours when the server has sent you data. 
		//void onError(WSAException)  occurs when there's an exception with the socket api, events such as loss of network or socket time out's. 
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
    catch (std::exception &e)
    {
        std::wcout << e.what() << std::endl;
        return 0;
    }
}