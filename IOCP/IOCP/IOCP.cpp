#include "IOCP.h"


SocketInfo::SocketInfo(TCPSocket sock) : sock(sock)
{
}


IoData::IoData(IoDataMode mode) : wsaBuf({sizeof(JCPacket), (char*)&packet}), mode(mode)
{
}

IoData::~IoData()
{
}


IOCP::IOCP() : tcpSocket(std::make_shared<TCPSocket>())
{
}

IOCP::~IOCP()
{
}

void IOCP::AcceptWork()
{
	acceptWorking = true;
	while(acceptWorking)
	{
		TCPSocket* clientSocket = tcpSocket->Accept();
		if (clientSocket == nullptr)
		{
			SocketUtil::GetLastSocketError();
			continue;
		}
		fprintf(stdout, "client connected...\n");
		SocketInfo* socketInfo = new SocketInfo(*clientSocket);

		HANDLE port = CreateIoCompletionPort(
			(HANDLE)clientSocket->GetSocket(),
			comPort,
			(DWORD)socketInfo,
			0);

		if (port != comPort)
		{
			fprintf(stderr, "Create Client IoCompletionPort Error...\n");
			continue;
		}

		IoData* ioData = new IoData(RECV);

		DWORD recvBytes, flags = 0;
		WSARecv(
			clientSocket->GetSocket(),
			&ioData->wsaBuf,
			1,
			&recvBytes,
			&flags,
			ioData,
			NULL);
	}
}

void IOCP::IoCompletionWork()
{
	IoCompletionWorking = true;
	while (IoCompletionWorking)
	{
		DWORD bytesTrans;
		SocketInfo* socketInfo = new SocketInfo();
		IoData* ioData = new IoData(RECV);


		GetQueuedCompletionStatus(
			comPort,
			&bytesTrans,
			(LPDWORD)&socketInfo,
			(LPOVERLAPPED*)&ioData,
			INFINITE);


		fprintf(stdout, "after evt recv\n");
		TCPSocket clientSocket = socketInfo->sock;

		if (bytesTrans == 0)    //close socket
		{
			closesocket(clientSocket.GetSocket());
			delete socketInfo;
			delete ioData;

			OnCloseSocket();
			continue;
		}

		if (ioData->mode == SEND)
		{
			//Onsend
			delete ioData;
			continue;
		}
		else if (ioData->mode == RECV)
		{
			OnRecvPacket(socketInfo, ioData->packet);
			DWORD flags = 0;
			WSARecv(clientSocket.GetSocket(),
				&ioData->wsaBuf,
				1,
				NULL,
				&flags,
				ioData,
				NULL);
		}



	}


}

void IOCP::InitIocp()
{
	comPort = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL,
		0,
		0);

	//set work thread, num of thread is (process cnt * 2)
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	for (int i = 0; i < sysInfo.dwNumberOfProcessors; i++)
	{
		std::thread workThread(&IOCP::IoCompletionWork, this);
		workThread.detach();
	}
}

bool IOCP::RunServer(UINT16 portNum)
{
	if (!tcpSocket->Init())
	{
		return false;
	}

	InitIocp();

	tcpSocket->SetSocket(WSASocketW(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED));


	if (!tcpSocket->Bind(portNum))
	{
		return false;
	}

	if (!tcpSocket->Listen(30))
	{
		return false;
	}

	std::thread acpt(&IOCP::AcceptWork, this);
	acpt.detach();

	return true;
}

void IOCP::Send(JCPacket packet)
{
	IoData* ioData = new IoData(SEND);
	ioData->packet = packet;

	WSASend(tcpSocket->GetSocket(),
		&ioData->wsaBuf,
		1,
		NULL,
		0,
		ioData,
		NULL);

}

void IOCP::Send(SOCKET destSock, JCPacket packet)
{
	IoData* ioData = new IoData(SEND);
	ioData->packet = packet;

	WSASend(destSock,
		&ioData->wsaBuf,
		1,
		NULL,
		0,
		ioData,
		NULL);
}

