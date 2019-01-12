#include "IOCP.h"


SocketInfo::SocketInfo(TCPSocket sock) : sock(sock)
{
}


IoData::IoData(IoDataMode mode) : wsaBuf({sizeof(JCPacket), buff}), mode(mode)
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
		//SocketInfo* socketInfo = new SocketInfo(*clientSocket);


		//동적할당 해서 메모리에 있어야 이 후 recv/send하고 getqueue했을 때 정보를 가져올 수 있다.
		LPSOCKET_INFO socketInfo = (LPSOCKET_INFO)calloc(1, sizeof(SOCKET_INFO));
		socketInfo->clientSocket = clientSocket->GetSocket();

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

		//IoData* ioData = new IoData(RECV);
		LPIO_DATA ioData = (LPIO_DATA)calloc(1, sizeof(IO_DATA));
		ioData->wsaBuf.len = BUF_SIZE;
		ioData->wsaBuf.buf = ioData->buffer;	//??
		ioData->mode = RECV;

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
		//SocketInfo* socketInfo = new SocketInfo();
		//IoData* ioData = new IoData(RECV);

		LPSOCKET_INFO socketInfo;
		LPIO_DATA ioData;

		GetQueuedCompletionStatus(
			comPort,
			&bytesTrans,
			(LPDWORD)&socketInfo,
			(LPOVERLAPPED*)&ioData,
			INFINITE);


		fprintf(stdout, "after evt recv : %s\n", ioData->wsaBuf.buf);
		//TCPSocket clientSocket = socketInfo->sock;
		SOCKET clientSocket = socketInfo->clientSocket;

		if (bytesTrans == 0)    //close socket
		{
			closesocket(clientSocket);
			delete socketInfo;
			delete ioData;

			OnCloseSocket();
			continue;
		}

		if (ioData->mode == SEND)
		{
			//Onsend
			free(ioData);
			continue;
		}
		else if (ioData->mode == RECV)
		{
			OnRecvPacket(socketInfo, ioData->wsaBuf.buf);

			DWORD flags = 0;
			WSARecv(clientSocket,
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
	//memcpy(ioData->buff, packet, 128);

	WSASend(tcpSocket->GetSocket(),
		&ioData->wsaBuf,
		1,
		NULL,
		0,
		ioData,
		NULL);

}

void IOCP::Send(SOCKET destSock, char* packet)
{
	//IoData* ioData = new IoData(SEND);
	//memcpy(ioData->buff, packet, 128);
	printf("Send Called : %s\n", packet);
	LPIO_DATA ioData = (LPIO_DATA)calloc(1, sizeof(IO_DATA));
	ioData->wsaBuf.len = BUF_SIZE;
	ioData->wsaBuf.buf = ioData->buffer;	//??
	ioData->mode = SEND;

	memcpy(ioData->buffer, packet, BUF_SIZE);

	WSASend(destSock,
		&ioData->wsaBuf,
		1,
		NULL,
		0,
		ioData,
		NULL);
}

