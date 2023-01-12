#pragma once

class Board;

class Player
{
	enum
	{
		MOVE_TICK = 100
	};

public:
	void			Init(Board* board);
	void			Update(int32 deltaTick);

	void			SetPos(Pos pos) { _pos = pos; };
	Pos				Getpos() { return _pos; };

	bool			CanGo(Pos pos);

private:
	Pos				_pos = {};
	int32			_dir = DIR_UP;
	Board*			_board = nullptr;

	vector<Pos>		_path;
	uint32			_pathIndex = 0;
	uint32			_sumTick = 0;
};

