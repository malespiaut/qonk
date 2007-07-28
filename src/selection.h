// Copyright 2005 by Anthony Liekens anthony@liekens.net

#ifndef SELECTION_H
#define SELECTION_H

#include "coordinate.h"

class Selection {
	Coordinate c1, c2;
	enum SelectionStates { NOT_SELECTING, SELECTING };
	SelectionStates state;

public:
	Selection();

        void start(int, int);
        void update(int, int);
        void end();

	void render();

	double getMinX() const;
	double getMaxX() const;

	double getMinY() const;
	double getMaxY() const;

  bool isEmpty() const;

  bool isSelecting() const;
};

#endif
