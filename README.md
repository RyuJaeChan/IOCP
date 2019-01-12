# IOCP
IOCP template code by C++



## STACK

window10

visual studio 2017





## ERROR HANDLING

### 1. GetQueuedCompletionStatus()가 반응 없을때

클라이언트 접속 후 accept까지 처리하고 WSARecv로 등록을 했음에도 ```GetQueuedCompletionStatus()```로 완료 알림을 받을 수 없었다. 원인은 ```OVERLAPPED```를 상속받아 생성한 클래스의 초기화가 이루어지지 않아서였다. ```OVERLAPPED```의 멤버들을 모두 0으로 초기화해야되는데 이를 처리하지 않았다.

```cpp
class IoData : public OVERLAPPED
{
public:
	IoData() :OVERLAPPED({ 0 }) //이렇게 0으로 초기화해야한다.
    {
    
    }
	~IoData();

	WSABUF wsaBuf;
};
```

