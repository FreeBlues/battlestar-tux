/* Fighter.h
 *
 * Copyright 2005-2008 Eliot Eshelman
 * battlestartux@6by9.net
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


#ifndef FIGHTER_H_
#define FIGHTER_H_

#include <SDL_opengl.h>
#include "CollisionShapes/btCompoundShape.h"

#include "CoreCell.h"
#include "Game.h"
#include "HexCell.h"
#include "HexCellList.h"
#include "Object.h"
#include "Vector.h"

class WeaponSystem;


// Distinguish between 'evil' and 'good' fighters.
enum FighterAlignment {
	HEROS_FIGHTER,		// Fighter flown by player
	ENEMY_FIGHTER		// Enemy fighter
};


// A fighter aircraft.
// All the physical parts are actually handled by
// the cells.  This is just a logical grouping.
class Fighter : public Object {
	public:
				Fighter( FighterAlignment );
				virtual ~Fighter();

				// Update fighter systems.
				void Update( int );

				void Draw();

				// Start or stop weapons fire.
				void FirePrimary( bool );
				void FireSecondary( bool );

				float getHealth();
				float getFullHealth();

				// Connect a new ship component onto this fighter at specified position.
				// If that position is already taken, function will return false.
				bool attachCell( HexCell*, ivec2 & );

				// One of the ship's components is no longer a part of the ship.
				// Usually this happens when a cell becomes disconnected and
				// floats away.  The cell itself remains intact.
				void detachCell( HexCell* );

				// One of the ship's components has been destroyed.
				void destroyCell( HexCell* );

				FighterAlignment getAlignment() { return align; }

				inline bool hasComponents() { return true; }
				inline ObjectList* getComponents() { return allCells; }

				inline WeaponSystem* getWeaponSystem() { return weaponSystem; }

	protected:
				// Fighter's collision shape depends on what cells are attached.
				btCompoundShape* m_collisionShape;
				btCollisionShape* getCollisionShape() { return m_collisionShape; }
				void addCellShape( HexCell* );

				// Ship components can get power here.
				// Returns the amount of power obtained.
				float getPower( float );

				// If a component doesn't use all the power it has
				// reserved, it must be returned to the ship.
				void returnPower( float );

				// Propulsion cells will be used to move ship.
				void setPropulsion( const vec3 & );

				// Propulsion cells will be used to rotate the ship.
				void setRotation( const float );

				WeaponSystem* weaponSystem;

				CoreCell* coreCell;

				// It's tempting to make different lists for each type of cell.
				// Unfortunately, objects can only be members of one list at a
				// time.  There shouldn't be too many cells and too much overhead.
				HexCellList* allCells;

				FighterAlignment align;

	private:
				Fighter( const Fighter & );
				const Fighter & operator= ( const Fighter & );


	friend class ArmorCell;
	friend class CoreCell;
	friend class FighterController;
	friend class GenerationCell;
	friend class PropulsionCell;
	friend class StorageCell;
};


#endif /*FIGHTER_H_*/
