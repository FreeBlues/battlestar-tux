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


#include <SDL.h>

#include "Button.h"

Button::Button( GUI* gui, char* s ) : Widget( gui ) {
	string = s;

	float llx = 0.0;
	float lly = 0.0;
	float llz = 0.0;
	float urx = 0.0;
	float ury = 0.0;
	float urz = 0.0;
	float descender = font->Descender();
	float ascender = font->Ascender();
	font->BBox( string, llx, lly, llz, urx, ury, urz );

	size[0] = (int) urx - llx + HORIZ_PAD * 2;
	size[1] = (int) ascender + (-descender) + VERTI_PAD;
}


Button::~Button() {}


void Button::Draw() {
	// Translate to object position.
	glTranslatef( pos[0], pos[1], 0.0 );

	// Draw background.
	glPushMatrix();
		if( hover && clicked )
			glColor4fv( BACKGROUND_CLICKED );
		else if( hover )
			glColor4fv( BACKGROUND_HOVER );
		else
			glColor4fv( BACKGROUND );
		glBindTexture( GL_TEXTURE_2D, 0 );
		glBegin( GL_POLYGON );
			glVertex3f( EDGE_OFFSET, 0.0, 0.0 );
			glVertex3f( size[0], 0.0, 0.0 );
			glVertex3f( size[0], size[1] - EDGE_OFFSET, 0.0 );
			glVertex3f( size[0] - EDGE_OFFSET, size[1], 0.0 );
			glVertex3f( 0.0, size[1], 0.0 );
			glVertex3f( 0.0, EDGE_OFFSET, 0.0 );
		glEnd();
	glPopMatrix();

	// Draw text.
	glPushMatrix();
		glTranslatef( HORIZ_PAD, -font->Descender(), 0.0 );
		if( hover && clicked )
			glColor4fv( FOREGROUND_CLICKED );
		else if( hover )
			glColor4fv( FOREGROUND_HOVER );
		else
			glColor4fv( FOREGROUND );
		font->Render( string );
	glPopMatrix();

	// Draw edge.
	if( hover && clicked )
		glColor4fv( EDGE_CLICKED );
	else if( hover )
		glColor4fv( EDGE_HOVER );
	else
		glColor4fv( EDGE );
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBegin( GL_LINE_LOOP );
		glVertex3f( EDGE_OFFSET, 0.0, 0.0 );
		glVertex3f( size[0], 0.0, 0.0 );
		glVertex3f( size[0], size[1] - EDGE_OFFSET, 0.0 );
		glVertex3f( size[0] - EDGE_OFFSET, size[1], 0.0 );
		glVertex3f( 0.0, size[1], 0.0 );
		glVertex3f( 0.0, EDGE_OFFSET, 0.0 );
	glEnd();
}


void Button::Update( int x, int y, int state ) {
	// Mouse wasn't clicked previously.
	if( !clicked ) {
		// Mouse is over button.
		if( pos[0] < x && x < (pos[0] + size[0]) && pos[1] < y && y < (pos[1] + size[1]) ) {
			// Mouse is clicked.
			if( state & SDL_BUTTON(1) ) {
				// Mouse was clicked while over button.
				if( hover )
					clicked = true;
			}
			// Mouse isn't clicked.
			else
				hover = true;
		}
		// Mouse is outside of button.
		else
			hover = false;
	}
	// Mouse was clicked previously.
	else {
		// Mouse is over button.
		if( pos[0] < x && x < (pos[0] + size[0]) && pos[1] < y && y < (pos[1] + size[1]) ) {
			// Mouse was released.
			if( ! state & SDL_BUTTON(1) ) {
				// Mouse was released while over button.
				if( hover ) {
					clicked = false;
					printf( "Click %s\n", string );
				}
				// Mouse was released while outside button.
				else
					clicked = false;
			}

			hover = true;
		}
		// Mouse is outside of button.
		else {
			// Mouse was released.
			if( ! state & SDL_BUTTON(1) )
				clicked = false;

			hover = false;
		}
	}
}