#ifndef Multiuser_h
#define Multiuser_h

#include "Connection.h"
#include <vector>

// Prototypes //////////////

// call this in setup
 void MUSetup();

 bool MUServerConnect(HWND HandleToWindow,int port);
 bool MUClientConnect(HWND HandleToWindow,CHAR ip[],int port);

 void MUSend(char SendBuffer[]);
 void MURead(WPARAM FDReadsWparam);
 void MUClose(WPARAM FDClosesWparam);
 void MUAccept(WPARAM FDAcceptsWparam);

 void MUShutdown();

// end of Prototypes //////////////

// Externals ///////////////////

// Needed Variables

// sockets
extern WSAServer SkServer;
extern WSAClient SkClient;

// Client Variables
extern bool Kicked;
extern bool ServerOn;

// Server Variables
extern std::vector<bool> PlayerList;

// Client\Server Variables
extern int IsServer;
extern int PlayerNum;

// end of Needed Variables

// Project Specific Variables (specify all
// variables that will be effected by multiuser
// here)



// end of Project Specific Variables

// end of Externals ///////////////

#endif