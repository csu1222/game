#pragma once


/*
-------------------------
	RecvBuffer 선언부
-------------------------
*/

class RecvBuffer
{
	// 데이터 복사비용을 줄이기 위한 방법
	// 인자로 받은 버퍼 사이즈를 BUFFER_COUNT 만큼곱한 크기로 잡습니다. 
	enum { BUFFER_COUNT = 10 };
public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();

	void			Clean();
	bool			OnRead(int32 numOfBytes);
	bool			OnWrite(int32 numOfBytes);

	BYTE*			ReadPos() { return &_buffer[_readPos]; }
	BYTE*			WritePos() { return &_buffer[_writePos]; }
	int32			DataSize() { return _writePos - _readPos; }
	int32			FreeSize() { return _capacity - _writePos; }

private:
	/* 멤버 변수
	_bufferSize와 _buffer는 말그대로 버퍼자체와 그 길이를 뜻합니다. 
	_readPos와 _writePos가 무엇을 뜻이냐 하면 커서와 같은 역할을합니다. 
	맨처음 아무런 데이터를 받지 않았을때는 _readPos와 _wirtePos가 _buffer 
	맨앞에 같이 위치하고 있을것입니다. 
	이후 Recv로 데이터를 _buffer에 차례대로 받아오면서 그만큼 _writePos도 같이
	이동하게 되는 것이고 _readPos는 맨앞 그대로 있으면서 그 사이에 있는 데이터들이
	사실상 지금까지 유효한 데이터 범위라고 볼 수 있는겁니다.
	그래서 쓰는쪽에서는 _writePos에 이어서 쓰면 되고 읽는쪽에서는 _readPos부터
	읽어 나가면 됩니다. 

	읽고 쓰기를 진행하다보면 한도 끝까지 사용하게 될겁니다. 
	오래되고 정석적인 방법은 순환 버퍼를 사용하는것입니다. 
	즉, 커서가 버퍼 끝에 다다르면 버퍼의 처음과 끝이 이어져 있는것처럼 
	커서를 다시 버퍼의 맨앞부터 읽도록 하면서 계속 사용할 수 있도록 하는것이 
	순환버퍼입니다. 
	또 다른 방법은 처음부터 버퍼사이즈를 넉넉하게 만들어놓고 
	데이터를 받다가 언젠가 _readPos와 _writePos가 곂치게 되면 한번에 보낼만한 
	데이터는 다 받았다는 뜻이기 때문에 _readPos, _wirtePos 둘다 버퍼의 맨 앞으로
	이동시킵니다. 그러면 다시 버퍼를 처음부터 사용할 수 있게 됩니다. 
	하지만 만약 넉넉하게 잡은 버퍼 마저도 넘는 크기의 데이터 였다면 
	버퍼의 끝에 _writePos가 닿게 되고 _readPos는 조금 뒤에 위치 할것입니다. 
	이 두 커서 사이의 데이터를 버퍼의 맨앞으로 옮기고 다시 떨어져 있던 거리만큼
	커서의 거리를 유지해 버퍼 앞으로 가져다 놓으면 됩니다.
	이 방법을 사용하면 만약 버퍼를 다 사용하기 전에 데이터를 다 받는다면 
	복사 비용없이 커서들만 이동하면되고 만약 버퍼를 넘었다면 커서 사이의 만큼
	비교적 작은 데이터만 복사이동할 수 있습니다. 
	*/
	int32			_bufferSize = 0;
	int32			_readPos = 0;
	int32			_writePos = 0;
	Vector<BYTE>	_buffer;
	// 데이터 복사 비용을 줄이기 위해 널널하게 잡는 실제 버퍼 크기
	int32			_capacity = 0;
};

