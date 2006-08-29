/* VerticalContainer.cpp
 *
 * Copyright 2006 Eliot Eshelman
 * eliot@6by9.net
 *
 *
 *  This file is part of Battlestar Tux.
 *
 *  Battlestar Tux is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; version 2 of the License.
 *
 *  Battlestar Tux is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Battlestar Tux; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */


#include "VerticalContainer.h"
#include "Widget.h"

VerticalContainer::VerticalContainer( GUI* g, bool resize, W_Alignment h ) : Container( g, true ) {
	resizeWidgets = resize;

	hAlign = h;

	preferredSize[0] = size[0] = 10;
	preferredSize[1] = size[1] = 10;
}

VerticalContainer::~VerticalContainer() {}


void VerticalContainer::Draw() {
	// Draw all elements in container.
	Widget* cur = (Widget*) rootObj;

	while( cur ) {
		glLoadIdentity();
		cur->Draw();
		cur = (Widget*) cur->getNext();
	}
}


void VerticalContainer::Update( int x, int y, int state ) {
	// Update all elements in container.
	Widget* cur = (Widget*) rootObj;

	while( cur ) {
		cur->Update( x, y, state );
		cur = (Widget*) cur->getNext();
	}
}


void VerticalContainer::ReevaluateElements() {
	preferredSize[0] = 0;
	preferredSize[1] = - C_WIDGET_PAD;
	int offset[2] = { 0, 0 };
	int maxWidgetWidth = 0;
	int containerWidth = 0;
	int* wSize;

	Widget* cur = (Widget*) rootObj;
	// Loop through widgets - determining container size.
	while( cur ) {
		wSize = cur->getPreferredSize();

		if( maxWidgetWidth < wSize[0] ) {
			maxWidgetWidth = wSize[0];
			preferredSize[0] = wSize[0];
		}
		preferredSize[1] += wSize[1] + C_WIDGET_PAD;

		cur = (Widget*) cur->getNext();
	}

	containerWidth = preferredSize[0];
	offset[0] = 0;
	offset[1] = preferredSize[1];

	// Set size of container.
	if( size[0] < preferredSize[0] )
		size[0] = preferredSize[0];
	if( size[1] < preferredSize[1] )
		size[1] = preferredSize[1];

	// Set position of container.
	setContainerPos();

	cur = (Widget*) rootObj;
	// Loop through widgets - setting size and position.
	while( cur ) {
		wSize = cur->getPreferredSize();
		offset[1] -= wSize[1];

		// Horizontally align widgets.
		switch( hAlign ) {
			case HORIZ_LEFT:
				cur->setPos( offset[0] + pos[0], offset[1] + pos[1] );
				break;

			case HORIZ_CENTER:
				cur->setPos( offset[0] + pos[0] + (containerWidth - cur->getSize()[0])/2, offset[1] + pos[1] );
				break;

			case HORIZ_RIGHT:
			default:
				cur->setPos( offset[0] + pos[0] + containerWidth - cur->getSize()[0], offset[1] + pos[1] );
				break;
		}

		if( resizeWidgets )
			cur->setSize( maxWidgetWidth, cur->getSize()[1] );

		cur = (Widget*) cur->getNext();
		offset[1] -= C_WIDGET_PAD;
	}
}