#pragma once

class Board;

class Player
{
public:
	void		Init(Board* board);
	void		Update(int32 deltaTick);

	void		SetPos(Pos pos) { _pos = pos; };
	Pos			Getpos() { return _pos; };

private:
	Pos			_pos = {};
	int32		_dir = Dir::DIR_UP;
	Board*		_board = nullptr;
};

