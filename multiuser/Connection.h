#ifndef Connection_h__
#define Connection_h__

#pragma comment(lib,"Ws2_32.lib")


#include <WinSock2.h>
#include <cstdio>
#include <vector>
#define WM_SOCKET 15627

extern char e_strWSAError[1024];

struct ipaddrass
{
	int m_iSegmentOne, m_iSegmentTwo, m_iSegmentThree, m_iSegmentFour;
};

class WSAClient
{
private:
	WSADATA m_WSAData;
	addrinfo *m_ptrResult, *m_ptrPtr, m_hints;
	SOCKET m_sckClientConnectSocket;
	sockaddr_in m_saClientAddress;
	PCSTR m_pcstrAddress;
	int m_iClientID;
	int m_iConnectionPort;
	bool m_bShowMessageBoxes;
public:
	void WSAInit(bool ShowMessageBoxes);
	bool Connect(HWND hwndHandle,char ip[], int port);
	void RecvData(char RecvBuffer[], int BufferSize);
	void SendData(char SendBuffer[], int BufferSize);
	void ShutDown();
};

class WSAServerClientSocket
{
public:
	static int m_iClientCount;
	ipaddrass m_ClientAddress;
	SOCKET m_sckServerClientCommSocket;
	sockaddr_in m_saClientAddress;

	void WSAInit(bool);
	void ShutDown();
};

class WSAServer
{
private:
	
	WSADATA m_WSAData;
	addrinfo *m_ptrResult, *m_ptrPtr, m_hints;
	SOCKET m_sckServerListenSocket;
	std::vector<WSAServerClientSocket> m_clsServerClientSocket;
	sockaddr_in m_saServerAddress;
	PCSTR m_pcstrAddress;
	int m_iConnectionPort;
	bool m_bShowMessageBoxes;
	char ServerAddress[100];
public:
	
	inline int GetTotalClients()
	{
		if (m_clsServerClientSocket.size() == 0)
		{
			return 0;
		}
		return m_clsServerClientSocket[0].m_iClientCount;
	}

	char* GetServerAddress();
	void WSAInit(bool ShowMessageBoxes);
	bool Connect(HWND hwndHandle, int Port);
	void RecvData(WPARAM FD_READsWParam,char RecvBuffer[],int BufferSize);
	void SendData(int SocketNum,char SendBuffer[],int BufferSize);
	void SendData(WPARAM FD_READsWParam,char SendBuffer[],int BufferSize);
	bool Accept(WPARAM FD_ACCEPTsWParam);
	int ClientClose(WPARAM FD_CLOSEsWParam);
	void ShutDown();
};

#endif
