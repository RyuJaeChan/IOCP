#include "TCPSocket.h"


SocketAddress::SocketAddress() : addrLen(sizeof(SOCKADDR_IN))
{

}

SocketAddress::SocketAddress(SOCKADDR_IN sockAddr) : sockAddr(sockAddr), addrLen(sizeof(SOCKADDR_IN))
{

}

SocketAddress::~SocketAddress()
{

}

SocketAddress SocketAddress::CreateTCPSocket(UINT16 portNum)
{
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sockAddr.sin_port = htons(portNum);

	return SocketAddress(sockAddr);
}

SocketAddress SocketAddress::CreateTCPSocket(std::string host, UINT16 portNum)
{
	SOCKADDR_IN sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(portNum);
	InetPton(AF_INET, host.c_str(), &sockAddr.sin_addr);

	return SocketAddress(sockAddr);
}


int SocketUtil::GetLastSocketError()
{
	int ret = ::GetLastError();
	fprintf(stderr, "Socket error(error code : %d)\n", ret);
	return ret;
}


TCPSocket::TCPSocket()
{
}

TCPSocket::TCPSocket(SOCKET sock, SocketAddress sockAddr) : sock(sock), sockAddr(sockAddr)
{
}

TCPSocket::~TCPSocket()
{
	::closesocket(sock);
}

SOCKET TCPSocket::GetSocket()
{
	return sock;
}

void TCPSocket::SetSocket(SOCKET socket)
{
	sock = socket;
}

SocketAddress TCPSocket::GetSocketAddress()
{
	return sockAddr;
}

int TCPSocket::Init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		SocketUtil::GetLastSocketError();
		return SOCKET_FAIL;
	}

	return SOCKET_SUCCESS;
}

int TCPSocket::CreateSocket()
{
	sock = ::socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		SocketUtil::GetLastSocketError();
		return SOCKET_FAIL;
	}

	return SOCKET_SUCCESS;
}

int TCPSocket::Bind(UINT16 portNum)
{
	sockAddr = SocketAddress::CreateTCPSocket(portNum);
	if (::bind(sock, (SOCKADDR*)&sockAddr, sockAddr.addrLen) == SOCKET_ERROR)
	{
		SocketUtil::GetLastSocketError();
		return SOCKET_FAIL;
	}

	return SOCKET_SUCCESS;
}

int TCPSocket::Listen(int backlog)
{
	if (::listen(sock, backlog) == SOCKET_ERROR)
	{
		SocketUtil::GetLastSocketError();
		return SOCKET_FAIL;
	}

	return SOCKET_SUCCESS;
}

TCPSocket* TCPSocket::Accept()
{
	SocketAddress clientAddr;
	SOCKET clientSock = ::accept(sock, (SOCKADDR*)&clientAddr, &clientAddr.addrLen);

	if (clientSock == INVALID_SOCKET)
	{
		SocketUtil::GetLastSocketError();
		return nullptr;
	}

	return new TCPSocket(clientSock, clientAddr);
}

int TCPSocket::Connect()
{
	if (::connect(sock, (SOCKADDR*)&sockAddr, sockAddr.addrLen) == SOCKET_ERROR)
	{
		SocketUtil::GetLastSocketError();
		return SOCKET_FAIL;
	}

	return SOCKET_SUCCESS;
}

void TCPSocket::CleanUp()
{
	::WSACleanup();
}
