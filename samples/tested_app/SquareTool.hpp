/*
 * SquareTool.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Oct 21, 2014
 */
#ifndef SQUARETOOL_HPP_
#define SQUARETOOL_HPP_

#include "IGraphic.hpp"

class SquareTool {
	int _startX;
	int _startY;
	int _size;
public:
	void setStart(int x, int y) {
		_startX = x;
		_startY = y;
	}

	void setSize(int size) {
		_size = size;
	}

	void DrawOn(IGraphics& g) {
		g.DrawLine(_startX, _startY, _startX + _size, _startY);
		g.DrawLine(_startX + _size, _startY, _startX + _size, _startY + _size);
		g.DrawLine(_startX + _size, _startY + _size, _startX, _startY + _size);
		g.DrawLine(_startX, _startY + _size, _startX, _startY);
	}
};

#endif /* SQUARETOOL_HPP_ */
