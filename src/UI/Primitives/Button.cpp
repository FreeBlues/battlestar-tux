/* Button.cpp
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


#include <math.h>

#include "Button.h"
#include "ButtonClickEvent.h"

Button::Button( GUI* gui, std::string s, W_Alignment h ) : Widget( gui ) {
	buttonText = s;
	hAlign = h;

	float llx = 0.0;
	float lly = 0.0;
	float llz = 0.0;
	float urx = 0.0;
	float ury = 0.0;
	float urz = 0.0;
	float descender = font->Descender();
	float ascender = font->Ascender();
	font->BBox( buttonText.c_str(), llx, lly, llz, urx, ury, urz );

	textWidth = urx - llx;
	preferredSize[0] = size[0] = ceilf( urx - llx + W_HORIZ_PAD * 2 );
	preferredSize[1] = size[1] = ceilf( ascender + (-descender) + W_VERTI_PAD );
}


Button::~Button() {}


void Button::Draw() {
	// Translate to object position.
	glTranslatef( pos[0], pos[1], 0.0 );

	// Draw background.
	if( hover && clicked )
		glColor4fv( W_BG_CLICKED );
	else if( hover )
		glColor4fv( W_BG_HOVER );
	else
		glColor4fv( W_BG );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBegin( GL_POLYGON );
		glVertex3f( 0.0, 0.0, 0.0 );
		glVertex3f( size[0] - W_EDGE_OFFSET, 0.0, 0.0 );
		glVertex3f( size[0], W_EDGE_OFFSET, 0.0 );
		glVertex3f( size[0], size[1], 0.0 );
		glVertex3f( W_EDGE_OFFSET, size[1], 0.0 );
		glVertex3f( 0.0, size[1] - W_EDGE_OFFSET, 0.0 );
	glEnd();

	// Draw text.
	glPushMatrix();
		switch( hAlign ) {
			case HORIZ_LEFT:
				glTranslatef( W_HORIZ_PAD, -font->Descender(), 0.0 );
				break;

			case HORIZ_CENTER:
				glTranslatef( (size[0] - textWidth)/2, -font->Descender(), 0.0 );
				break;

			case HORIZ_RIGHT:
			default:
				glTranslatef( size[0] - textWidth - W_HORIZ_PAD, -font->Descender(), 0.0 );
				break;
		}

		if( hover && clicked )
			glColor4fv( W_FG_CLICKED );
		else if( hover )
			glColor4fv( W_FG_HOVER );
		else
			glColor4fv( W_FG );
		font->Render( buttonText.c_str() );
	glPopMatrix();

	// Draw edge.
	if( hover && clicked )
		glColor4fv( W_EDGE_CLICKED );
	else if( hover )
		glColor4fv( W_EDGE_HOVER );
	else
		glColor4fv( W_EDGE );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBegin( GL_LINE_LOOP );
		glVertex3f( 0.0, 0.0, 0.0 );
		glVertex3f( size[0] - W_EDGE_OFFSET, 0.0, 0.0 );
		glVertex3f( size[0], W_EDGE_OFFSET, 0.0 );
		glVertex3f( size[0], size[1], 0.0 );
		glVertex3f( W_EDGE_OFFSET, size[1], 0.0 );
		glVertex3f( 0.0, size[1] - W_EDGE_OFFSET, 0.0 );
	glEnd();
}


void Button::ClickEvent() {
	ButtonClickEvent* event = new ButtonClickEvent( buttonText );
	GenerateEvent( event );
	delete event;
}