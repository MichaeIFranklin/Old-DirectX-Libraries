#include "Multiuser.h"

// init Externals 

// sockets
WSAServer SkServer;
WSAClient SkClient;

// Client Variables
bool Kicked;
bool ServerOn;

// Client\Server Variables
int IsServer;
int PlayerNum;
std::vector<bool> PlayerList;
int PlayerLimit;

// Define Prototypes

 void MUSetup()
 {
	 // setup sockets
	 SkServer.WSAInit(false);
	 SkClient.WSAInit(false);

	 // setup Client\Server Variables
	 IsServer = 0;
	 PlayerLimit = 4; // max player limit (change to 0 to allow unlimited players)
 }

 void MUSend(char SendBuffer[])
 {
	 if (IsServer == 2)
	 {
		for(int i = 0;i < SkServer.GetTotalClients();i++)
		{
			SkServer.SendData(i+1,SendBuffer,sizeof(SendBuffer));
		}
	 }
	 else
	 {
		 SkClient.SendData(SendBuffer,sizeof(SendBuffer));
	 }
 }

 bool MUServerConnect(HWND HandleToWindow,int port)
 {
	 bool Connected;
	 Connected = SkServer.Connect(HandleToWindow,port);
	 IsServer = 2;
	  PlayerList.push_back(true); // Comment Out if you want Server to NOT be a player
	  PlayerNum = 1; // Comment Out if you want Server to NOT be a player

	  return Connected;
 }
 bool MUClientConnect(HWND HandleToWindow,CHAR ip[],int port)
 {
	 return SkClient.Connect(HandleToWindow,ip,port);
 }

 void MURead(WPARAM FDReadsWparam)
 {
	 char RecvChar[80];
	 char SendChar[80];
	 if(IsServer == 2) // if server
	 {
		 SkServer.RecvData(FDReadsWparam,RecvChar,sizeof(RecvChar));
		 
		 //read loop /////
		 bool ReadEnd = false; // true if end of message
		 while (!ReadEnd)
		 {
			 ///player connection and disconnection //////
			 if((RecvChar[0] == '0' && RecvChar[2] == '1') || RecvChar[0] == 'p') // if client is asking to join
			 {
				 int PlayersIn = 0;
				 for (int i = 0;i < PlayerList.size();i++) // go through currently joined player list
				 {
					 if (PlayerList[i]) // if player i (e.g. player 1) is connected
					 {
						 PlayersIn++; // add one to amount of players in
					 }
				 }
				 if (PlayersIn < PlayerLimit || PlayerLimit == 0)
				 { // if max player limit not reached
					 int PlayerToken = 0; // init connecting player's number to 0
					 for(int i = 0;i < PlayerList.size();i++) // go through connected players list
					 {
						 if (!PlayerList[i]) // if slot i (e.g. slot 1) is open for players
						 {
							 PlayerToken = i; // set connecting player's number to this slot
							 PlayerList[i] = true; // set this slot to closed to players
							 i = PlayerList.size(); // exit for loop
						 }
					 }
					 if (PlayerToken == 0) // if no slots are open, but max player limit not reached
					 {
						 PlayerList.push_back(true); // add new player slot to list
						 PlayerToken = PlayerList.size()-1; // set connecting player's number to newly made slot
					 }
					 PlayerToken++; // add one to player number (slot 1 is really 0 to program)
					 sprintf(SendChar,"0:1:%d~",PlayerToken); // create send string
					 SkServer.SendData(FDReadsWparam,SendChar,sizeof(SendChar)); // send player number to connecting player
					 for (int i = 0;i < PlayerList.size();i++)
					 {
						 sprintf(SendChar,"0:2:%i:%b~",i,PlayerList[i]); // create send string
						 SkServer.SendData(FDReadsWparam,SendChar,sizeof(SendChar)); // send player list element data to connecting player
					 }
					 sprintf(SendChar,"0:3:%d~",PlayerToken); // create send string
					 // send that player has connected to all players
					 for(int i = 0;i < SkServer.GetTotalClients();i++)
					 {
						 SkServer.SendData(i+1,SendChar,sizeof(SendChar));
					 } 
				 }
				 else
				 { // if player limit reached
					 SkServer.SendData(FDReadsWparam,"0:1:-1",sizeof("0:1:-1")); // send -1 as player number to connecting player (to tell player server is full)
				 }
			 }
			 else if(RecvChar[0] == '0' && RecvChar[2] == '2') // if player has sent they have left
			 {
				 int DropPlayer;
				 sscanf(RecvChar,"%*c%*c%*c%*c%d",&DropPlayer); // decrypt sent message to see discounting player's number
				 PlayerList[DropPlayer-1] = false; // set disconnecting player's slot to open for players
				 sprintf(SendChar,"0:4:%d~",DropPlayer); // create send string
				 // send that player has disconnected to all players
				 for(int i = 0;i < SkServer.GetTotalClients();i++)
				 {
					 SkServer.SendData(i+1,SendChar,sizeof(SendChar));
				 }  
			 }
			 ///end of player connection and disconnection //////

			 // Determine if at end of message ////////
			 int CopyPoint = 0; // point to start copying message remainder
			 for (int i = 0;i < sizeof(RecvChar);i++) // step through msg one character at a time
			 {
				 if (i == sizeof(RecvChar) - 1) // if we are at end of message
				 {
					 ReadEnd = true; // break from read loop
				 }
				 else if (RecvChar[i] == '~') // if segment end is reached
				 {
					 if (RecvChar[i+1] == 0) // if we are at end of message
					 {
						 ReadEnd = true; // break from read loop
					 }
					 else // if not at end of message
					 {
						 i++; // add one to incremented
						 CopyPoint = i; // set CopyPoint to this part of message
						 i = sizeof(RecvChar); // break from for loop
					 }
				 }
			 }
			 if (CopyPoint != 0) // if there is still message left
			 {
				 char TempChar[80]; // hold message for copying
				 // transfer remaining message to TempChar
				 for (int i = CopyPoint;i < sizeof(RecvChar);i++)
				 {
					 TempChar[i - CopyPoint] = RecvChar[i]; 
				 }
				 // clear RecvChar
				 for (int i = 0;i < sizeof(RecvChar);i++)
				 {
					 RecvChar[i] = 0; 
				 }
				 // copy TempChar back to RecvChar
				 for (int i = 0;i < sizeof(TempChar);i++)
				 {
					 RecvChar[i] = TempChar[i]; 
				 }
			 }
			 //end of Determine if at end of message ////////
		 };
		 //end of read loop /////
	 }
	 if(IsServer != 2) // if client
	 {
		 SkClient.RecvData(RecvChar,sizeof(RecvChar));

		 //read loop /////
		 bool ReadEnd = false; // true if end of message
		 while (!ReadEnd)
		 {
			 ///player connection and disconnection //////
			 if(RecvChar[0] == 'p') // if pinged
			 {
				 SkClient.SendData("0:1",1); // send player number request
			 }
			 else if(RecvChar[0] == '0' && RecvChar[2] == '1') // if server sent player number
			 {
				 sscanf(RecvChar,"%*c%*c%*c%*c%d",&PlayerNum); // decrypt message and set player number
				 if (PlayerNum == -1) // if player number is -1
				 { // set kicked to true and disconnect
					 Kicked = true; 
					 SkClient.ShutDown();
				 }
				 else
				 { // if player number is not -1
					 ServerOn = true; // set to client and set serveron to true
					 IsServer = 1;
				 }
			 }
			 else if(RecvChar[0] == '0' && RecvChar[2] == '2') // if server sent player list data
			 {
				 int Element; // holds player list element
				 bool Data; // holds element data
				 sscanf(RecvChar,"%*c%*c%*c%*c%d%*c%i",&Element,&Data); // decrypt message and set variables
				 if (Element == PlayerList.size()) // sent element exceeds client player list size
				 {
					 PlayerList.push_back(Data); // add element to player list and input sent data
				 }
				 else
				 {
					 PlayerList[Element] = Data; // set sent element to sent data
				 }
			 }
			 else if(RecvChar[0] == '0' && RecvChar[2] == '3') // if server sent player has connected
			 {
				 int ConnectedPlayer; // holds player number
				 sscanf(RecvChar,"%*c%*c%*c%*c%d",&ConnectedPlayer); // decrypt message and set variable
				 if (ConnectedPlayer - 1 == PlayerList.size()) // if sent player number exceeds client player list size
				 {
					 PlayerList.push_back(true); // add element to player list and set to true
				 }
				 else
				 {
					 PlayerList[ConnectedPlayer - 1] = true; // set sent player slot to true
				 }
			 }
			 else if(RecvChar[0] == '0' && RecvChar[2] == '4') // if server sent player has disconnected
			 {
				 int DisconnectedPlayer; // holds player number
				 sscanf(RecvChar,"%*c%*c%*c%*c%d",&DisconnectedPlayer); // decrypt message and set variable
				 PlayerList[DisconnectedPlayer - 1] = false; // set sent player slot to false
			 }
			 ///end of player connection and disconnection //////

			 // Determine if at end of message ////////
			 int CopyPoint = 0; // point to start copying message remainder
			 for (int i = 0;i < sizeof(RecvChar);i++) // step through msg one character at a time
			 {
				 if (i == sizeof(RecvChar) - 1) // if we are at end of message
				 {
					  ReadEnd = true; // break from read loop
				 }
				 else if (RecvChar[i] == '~') // if segment end is reached
				 {
					 if (RecvChar[i+1] == 0) // if we are at end of message
					 {
						 ReadEnd = true; // break from read loop
					 }
					 else // if not at end of message
					 {
						 i++; // add one to incremented
						 CopyPoint = i; // set CopyPoint to this part of message
						 i = sizeof(RecvChar); // break from for loop
					 }
				 }
			 }
			 if (CopyPoint != 0) // if there is still message left
			 {
				 char TempChar[80]; // hold message for copying
				 // transfer remaining message to TempChar
				 for (int i = CopyPoint;i < sizeof(RecvChar);i++)
				 {
					 TempChar[i - CopyPoint] = RecvChar[i]; 
				 }
				 // clear RecvChar
				 for (int i = 0;i < sizeof(RecvChar);i++)
				 {
					 RecvChar[i] = 0; 
				 }
				 // copy TempChar back to RecvChar
				 for (int i = 0;i < sizeof(TempChar);i++)
				 {
					 RecvChar[i] = TempChar[i]; 
				 }
			 }
			 //end of Determine if at end of message ////////
		 };
		 //end of read loop /////
	 }
 }
 void MUClose(WPARAM FDClosesWparam)
 {
	 if(IsServer == 2)
		 SkServer.ClientClose(FDClosesWparam);

	 if(IsServer == 1)
	 {
		 IsServer = -1;
		 ServerOn = false;
		 SkClient.ShutDown();
	 }
 }
 void MUAccept(WPARAM FDAcceptsWparam)
 {
	 SkServer.Accept(FDAcceptsWparam);
 }

  void MUShutdown()
  {
	  if(IsServer == 1 && ServerOn)
	  {
		  char Send[10];
		  sprintf(Send,"0:2:%d~",PlayerNum);
		  SkClient.SendData(Send,sizeof(Send));
	  }
	  IsServer = 0;
	  SkServer.ShutDown();
	  SkClient.ShutDown();
  }