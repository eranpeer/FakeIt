/*
 * IGraphic.hpp
 * Copyright (c) 2014 Eran Pe'er.
 *
 * This program is made available under the terms of the MIT License.
 * 
 * Created on Oct 21, 2014
 */
#ifndef IGRAPHIC_HPP_
#define IGRAPHIC_HPP_

struct IGraphics {
	virtual ~IGraphics() = default;
	virtual void DrawLine(int fromX, int fromY, int toX, int toY) = 0;
};

#endif /* IGRAPHIC_HPP_ */
