// Copyright 2005 by Anthony Liekens anthony@liekens.net
#include "coordinate.h"
#include "selection.h"
#include "canvas.h"

Selection::Selection()
{
	state = NOT_SELECTING;
}

void
Selection::start(Coordinate &c)
{
  state = SELECTING;

  c1 = c;
  
  c2 = c;
}

void
Selection::update(Coordinate &c)
{
  // Updates lower bound if currently selecting
  if (state == SELECTING)
  {
    c2 = c;
  }
}

void
Selection::end()
{
  state = NOT_SELECTING;
}

void 
Selection::render() {
	if( state == SELECTING ) {
                Canvas::drawMouseSelection(c1, c2);
	}
}

double 
Selection::getMinX() const { 
  return ( c1.getX() < c2.getX() ? c1.getX() : c2.getX() );
}

double 
Selection::getMaxX() const { 
  return ( c1.getX() > c2.getX() ? c1.getX() : c2.getX() ); 
}

double 
Selection::getMinY() const { 
  return ( c1.getY() < c2.getY() ? c1.getY() : c2.getY() ); 
}

double 
Selection::getMaxY() const { 
  return ( c1.getY() > c2.getY() ? c1.getY() : c2.getY() ); 
}

bool
Selection::isEmpty() const
{
  return c1 == c2;
}

bool
Selection::isSelecting() const
{
  return state == SELECTING;
}

