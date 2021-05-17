#include "Connection.h"

char e_strWSAError[1024];

void WSAClient::WSAInit(bool ShowMessageBoxes)
{
	int iResult;

	m_bShowMessageBoxes = ShowMessageBoxes;
	m_ptrResult = NULL;
	m_ptrPtr = NULL;
	m_sckClientConnectSocket = INVALID_SOCKET;
	iResult = WSAStartup(MAKEWORD(2,0),&m_WSAData);
	if(iResult != 0)
	{
		sprintf(e_strWSAError,"Winsock client startup failed. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes)
			MessageBox(0,e_strWSAError,"Client Error:",MB_ICONERROR);
	}
}

bool WSAClient::Connect(HWND hwndHandle, char ip[], int port)
{
	HRESULT hr;
	int iResult = 0;
	bool bError = true;
	m_iConnectionPort = port;
	m_pcstrAddress = ip;

	closesocket(m_sckClientConnectSocket);
	WSACleanup();

	m_sckClientConnectSocket = INVALID_SOCKET;

	WSAInit(m_bShowMessageBoxes);

	m_sckClientConnectSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_sckClientConnectSocket == INVALID_SOCKET && WSAGetLastError()!= WSAEWOULDBLOCK)
	{
		sprintf(e_strWSAError,"Client Connect Error. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes)
			MessageBox(0,e_strWSAError,"Client Error:",MB_ICONERROR);
		WSACleanup();
		bError = false;
	}

	iResult = WSAAsyncSelect(m_sckClientConnectSocket, hwndHandle, WM_SOCKET,(FD_CLOSE | FD_READ));

	if(iResult !=0 && WSAGetLastError()!= WSAEWOULDBLOCK)
	{
		sprintf(e_strWSAError,"Client Connect Error. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes)
			MessageBox(0,e_strWSAError,"Client Error:",MB_ICONERROR);
		bError = false;
	}
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons (m_iConnectionPort);
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = inet_addr(m_pcstrAddress);
	
	connect(m_sckClientConnectSocket,(LPSOCKADDR)(&SockAddr),sizeof(SockAddr));
	Sleep(100);
	hr = send(m_sckClientConnectSocket,"ping~",5,0);
	if (FAILED(hr) && WSAGetLastError()!= WSAEWOULDBLOCK) 
	{
		sprintf(e_strWSAError,"Client Connect Error. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes)
			MessageBox(0,e_strWSAError,"Client Error:",MB_ICONERROR);
		bError = false;
		closesocket(m_sckClientConnectSocket);
	}
	return bError;
}

void WSAClient::SendData(char strSendBuffer[], int Size)
{
	int iResult = 0;
	char strSendData[1024];
	ZeroMemory(strSendData,sizeof(strSendData));
	for(int iIterator = 0;iIterator < Size;iIterator++)
	{
		strSendData[iIterator] = strSendBuffer[iIterator];
	}
	iResult = send(m_sckClientConnectSocket,strSendData,(int)strlen(strSendData),0);
	if(iResult == SOCKET_ERROR && WSAGetLastError()!= WSAEWOULDBLOCK)
	{
		sprintf(e_strWSAError,"Could not send data to host. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes )
			MessageBox(0,e_strWSAError,"Client Error:",MB_ICONERROR);
	}
	else
		sprintf(e_strWSAError,"Socket OK",WSAGetLastError());
}

void WSAClient::RecvData(char strRecvBuffer[], int Size)
{
	char strRecvData[1024];
	ZeroMemory(strRecvData,sizeof(strRecvData));
	recv(m_sckClientConnectSocket, (char*)strRecvData, 1024, 0);
	for (int iIterator = 0;iIterator < Size;iIterator++)
	{
		strRecvBuffer[iIterator] = strRecvData[iIterator];
	}
}

void WSAClient::ShutDown()
{
	closesocket(m_sckClientConnectSocket);
}

int WSAServerClientSocket::m_iClientCount = 0;


void WSAServer::WSAInit(bool displayMessageBoxes)
{
	int iResult = 0;
	m_ptrResult = NULL;
	m_ptrPtr = NULL;
	m_sckServerListenSocket = INVALID_SOCKET;
	iResult = WSAStartup(MAKEWORD(2,0),&m_WSAData);
	if(iResult != 0 && WSAGetLastError()!= WSAEWOULDBLOCK)
	{
		sprintf(e_strWSAError,"Winsock server startup failed. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes)
			MessageBox(0,e_strWSAError,"Server Error:",MB_ICONERROR);
	}
}

char* WSAServer::GetServerAddress()
{
	return ServerAddress;
}

bool WSAServer::Connect(HWND hwndHandle, int iPort)
{
	int iResult = 0;
	bool bError = true;
	m_iConnectionPort = iPort;

	
	
	WSACleanup();

	WSAInit(m_bShowMessageBoxes);
	m_saServerAddress.sin_family = AF_INET;
	m_saServerAddress.sin_port = htons(m_iConnectionPort);
	m_saServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);

	m_sckServerListenSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_sckServerListenSocket == INVALID_SOCKET && WSAGetLastError()!= WSAEWOULDBLOCK)
	{
		sprintf(e_strWSAError,"Server connection error. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes)
			MessageBox(0,e_strWSAError,"Server Error:",MB_ICONERROR);
		WSACleanup();
		bError = false;
	}
	iResult = WSAAsyncSelect(m_sckServerListenSocket,hwndHandle,WM_SOCKET,(FD_CLOSE | FD_READ | FD_ACCEPT));
	if(iResult != 0 && WSAGetLastError()!= WSAEWOULDBLOCK)
	{
		sprintf(e_strWSAError,"Server Connect Error. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes)
			MessageBox(0,e_strWSAError,"Server Error:",MB_ICONERROR);
		WSACleanup();
		bError = false;
	}
	iResult = bind(m_sckServerListenSocket,(struct sockaddr*)&m_saServerAddress,sizeof(sockaddr));
	if(iResult == SOCKET_ERROR && WSAGetLastError()!= WSAEWOULDBLOCK)
	{
		sprintf(e_strWSAError,"Server could not bind socket. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes)
			MessageBox(0,e_strWSAError,"Server Error:",MB_ICONERROR);
		closesocket(m_sckServerListenSocket);
		WSACleanup();
		bError = false;
	}
	if(listen(m_sckServerListenSocket,0) == SOCKET_ERROR && WSAGetLastError()!= WSAEWOULDBLOCK)
	{
		sprintf(e_strWSAError,"Server could not open listening socket. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes)
			MessageBox(0,e_strWSAError,"Server Error:",MB_ICONERROR);
		closesocket(m_sckServerListenSocket);
		WSACleanup();
		bError = false;
	}

	// if no errors get server's IP
	if (bError)
	{
		char CompName[80];
		gethostname(CompName, sizeof(CompName));

		struct hostent *IPList = gethostbyname(CompName);
		struct in_addr IP;
		memcpy(&IP, IPList->h_addr_list[0], sizeof(struct in_addr));
		sprintf(ServerAddress,"%s",inet_ntoa(IP));
	}
	return bError;
}

void WSAServer::SendData(int iSocketNum, char strSendBuffer[], int Size)
{
	int iResult = 0;
	char strSendData[1024];
	ZeroMemory(strSendData,sizeof(strSendData));
	for(int iIterator = 0;iIterator < Size;iIterator++)
	{
		strSendData[iIterator] = strSendBuffer[iIterator];
	}

	iResult = send(m_clsServerClientSocket[iSocketNum-1].m_sckServerClientCommSocket,strSendData,(int)strlen(strSendData),0);
	if(iResult == SOCKET_ERROR && WSAGetLastError()!= WSAEWOULDBLOCK)
	{
		sprintf(e_strWSAError,"Could not send data to client. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes)
			MessageBox(0,e_strWSAError,"Server Error:",MB_ICONERROR);
	}
}
void WSAServer::SendData(WPARAM wparam, char strSendBuffer[], int Size)
{
	int iResult = 0;
	char strSendData[1024];
	ZeroMemory(strSendData,sizeof(strSendData));
	for(int iIterator = 0;iIterator < Size;iIterator++)
	{
		strSendData[iIterator] = strSendBuffer[iIterator];
	}

	for (int iIterator = 0;iIterator < m_clsServerClientSocket[0].m_iClientCount;iIterator++)
	{
		if (wparam == m_clsServerClientSocket[iIterator].m_sckServerClientCommSocket)
		{
			iResult = send(wparam,strSendData,(int)strlen(strSendData),0);
			if(iResult == SOCKET_ERROR && WSAGetLastError()!= WSAEWOULDBLOCK)
			{
				sprintf(e_strWSAError,"Could not send data to client. (Error Code: %d)",WSAGetLastError());
				if (m_bShowMessageBoxes)
					MessageBox(0,e_strWSAError,"Server Error:",MB_ICONERROR);
			}
		}
	}
}

void WSAServer::RecvData(WPARAM wparam,char strRecvBuffer[], int Size)
{
	char strRecvData[1024];
	ZeroMemory(strRecvData,sizeof(strRecvData));
	ZeroMemory(strRecvBuffer,sizeof(strRecvBuffer));
	recv(wparam, (char*)strRecvData, 1024, 0);
	sprintf(e_strWSAError,"Could not receive data from client. (Error Code: %d)",WSAGetLastError());
	if (m_bShowMessageBoxes&& WSAGetLastError()!= WSAEWOULDBLOCK)
		MessageBox(0,e_strWSAError,"Server Error:",MB_ICONERROR);

	for (int iIterator = 0;iIterator < Size;iIterator++)
	{
		strRecvBuffer[iIterator] = strRecvData[iIterator];
	}

}

int WSAServer::ClientClose(WPARAM wparam)
{
	int ClientID;
	for (int iIterator = 0;iIterator < m_clsServerClientSocket.size();iIterator++)
	{
		if(wparam == m_clsServerClientSocket[iIterator].m_sckServerClientCommSocket)
		{
			m_clsServerClientSocket[0].m_iClientCount--;
			closesocket(m_clsServerClientSocket[iIterator].m_sckServerClientCommSocket);
			m_clsServerClientSocket.erase(m_clsServerClientSocket.begin() + (iIterator));
			ClientID = iIterator+1;
		}
	}
	return ClientID;
}

void WSAServer::ShutDown()
{
	for(int iIterator = 0;iIterator < m_clsServerClientSocket.size();iIterator++)
	{
		m_clsServerClientSocket[0].m_iClientCount--;
		closesocket(m_clsServerClientSocket[iIterator].m_sckServerClientCommSocket);
		m_clsServerClientSocket.pop_back();
		iIterator--;
	}
	m_clsServerClientSocket.clear();
	closesocket(m_sckServerListenSocket);
	
}

bool WSAServer::Accept(WPARAM wparam)
{
	HRESULT hr;
	int iClientSize = sizeof(sockaddr_in);
	WSAServerClientSocket clsTempServerClientSocket;
	clsTempServerClientSocket.m_sckServerClientCommSocket = INVALID_SOCKET;

	clsTempServerClientSocket.m_sckServerClientCommSocket = accept(wparam,(struct sockaddr*)&clsTempServerClientSocket.m_ClientAddress,&iClientSize);
	clsTempServerClientSocket.m_ClientAddress.m_iSegmentOne = clsTempServerClientSocket.m_saClientAddress.sin_addr.S_un.S_un_b.s_b1;
	clsTempServerClientSocket.m_ClientAddress.m_iSegmentTwo = clsTempServerClientSocket.m_saClientAddress.sin_addr.S_un.S_un_b.s_b2;
	clsTempServerClientSocket.m_ClientAddress.m_iSegmentThree = clsTempServerClientSocket.m_saClientAddress.sin_addr.S_un.S_un_b.s_b3;
	clsTempServerClientSocket.m_ClientAddress.m_iSegmentFour = clsTempServerClientSocket.m_saClientAddress.sin_addr.S_un.S_un_b.s_b4;
	hr = send(clsTempServerClientSocket.m_sckServerClientCommSocket,"ping~",5,0);
	if (FAILED(hr) && WSAGetLastError()!= WSAEWOULDBLOCK) 
	{
		sprintf(e_strWSAError,"Failed to accept connection from client. (Error Code: %d)",WSAGetLastError());
		if (m_bShowMessageBoxes)
			MessageBox(0,e_strWSAError,"Server Error:",MB_ICONERROR);
		closesocket(m_sckServerListenSocket);
		WSACleanup();
		return false;
	}
	m_clsServerClientSocket.push_back(clsTempServerClientSocket);
	m_clsServerClientSocket[0].m_iClientCount++;
	return true;
}