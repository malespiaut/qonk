// Copyright 2005 by Anthony Liekens anthony@liekens.net
#include "coordinate.h"
#include "selection.h"
#include "canvas.h"

Selection::Selection()
{
	state = NOT_SELECTING;
}

void
Selection::start(int x, int y)
{
  state = SELECTING;

  c1.setXMapped(x);
  c1.setYMapped(y);
  
  c2.setXMapped(x);
  c2.setYMapped(y);
}

void
Selection::update(int x, int y)
{
  // Updates lower bound if currently selecting
  if (state == SELECTING)
  {
    c2.setXMapped(x);
    c2.setYMapped(y);
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

